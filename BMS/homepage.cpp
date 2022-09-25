#include "homepage.h"
#include "ui_homepage.h"
#include <QMessageBox>

const int book_col=6;//书列数
const int user_col=5;//用户列数


homePage::homePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homePage)
{
    ui->setupUi(this);
    QIcon icon(":/images/BMS.ico");
    setWindowIcon(icon);
    setWindowTitle("图书管理系统");

    m_sql = new initSql();
    m_model= new QStandardItemModel(this);
    treeStr1 = QString("书籍信息");
    treeStr2 = QString("用户信息");

    maddBook = new addBook();
    connect(maddBook,&addBook::myClose,this,[=]{maddBook->hide();});
    connect(maddBook,&addBook::addBookData,this,&homePage::addBookDataSlots);
    connect(this,&homePage::setID,maddBook,&addBook::setLineID);

    maddUser =new addUser();
    connect(maddUser,&addUser::myClose,this,[=]{maddUser->hide();});
    connect(maddUser,&addUser::addUserData,this,&homePage::addUserDataSlots);
    connect(this,&homePage::setID,maddUser,&addUser::setLineID);

}

homePage::~homePage()
{
    m_sql->closeDatabase();//关闭数据库

    //删除指针(new开辟的空间没有指定父对象)
    delete maddBook;maddBook =NULL;
    delete maddUser;maddUser = NULL;
    delete m_sql;m_sql = NULL;
    delete ui;
}

void homePage::initSlots()
{
    //设置用户名
    ui->label_realname ->setText(longinName);

   //添加顶层节点
   QTreeWidgetItem *topItem1 = new QTreeWidgetItem(ui->treeWidget);
   topItem1->setText(0,treeStr1);

   QTreeWidgetItem *topItem2 = new QTreeWidgetItem(ui->treeWidget);
   topItem2->setText(0,treeStr2);

   ui->treeWidget->addTopLevelItem(topItem1);
   ui->treeWidget->addTopLevelItem(topItem2);

   connect(ui->treeWidget,&QTreeWidget::itemClicked,this,&homePage::treeItemClickedSlots);


}

//设置表格公共属性(因为table每次使用后都没清空)
void homePage::setTableState()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setModel(m_model);
    //QTableView设为不可编辑：
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //QTableView设置整行选中：
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个

}
//显示所有数据
void homePage::showdata()
{
    if(mcurrentStr == treeStr1){
        //查询所有书籍
        QList<Book> bookList =m_sql->searchAllBook();
        if(bookList.isEmpty())
        {
            qDebug()<<"查询数据为空";
        }else
        {

            m_model->setColumnCount(book_col);
            QStringList laberlist;
            laberlist<<"编号"<<"名字"<<"作者"<<"价格"<<"出版社"<<"出版日期";
            m_model->setHorizontalHeaderLabels(laberlist);

            //通过迭代器遍历QList链表内容
            QList<Book>::iterator selectList4;

            int book_row=0;
            for (selectList4 = bookList.begin(); selectList4 != bookList.end(); ++selectList4)
            {
                QStandardItem *item1 = new QStandardItem(QString::number((*selectList4).b_number));
                QStandardItem *item2 = new QStandardItem((*selectList4).b_name);
                QStandardItem *item3 = new QStandardItem((*selectList4).b_author);
                QStandardItem *item4 = new QStandardItem(QString::number((*selectList4).b_price));
                QStandardItem *item5 = new QStandardItem((*selectList4).b_press);
                QStandardItem *item6 = new QStandardItem((*selectList4).b_PublicationDate);

                m_model->setItem(book_row,0,item1);
                m_model->setItem(book_row,1,item2);
                m_model->setItem(book_row,2,item3);
                m_model->setItem(book_row,3,item4);
                m_model->setItem(book_row,4,item5);
                m_model->setItem(book_row,5,item6);

                book_row++;
            }
        }
    }else if(mcurrentStr == treeStr2){
        //查询所有用户
        QList<Users> SelectAllUsersRet=m_sql->searcAllUser();
        if(SelectAllUsersRet.isEmpty())
        {
            qDebug()<<"查询数据为空";
        }else
        {

            m_model->setColumnCount(user_col);
            QStringList laberlist;
            laberlist<<"编号"<<"名字"<<"密码"<<"电话"<<"权限";
            m_model->setHorizontalHeaderLabels(laberlist);
            //通过迭代器遍历QList链表内容
            QList<Users>::iterator selectAllUserList;
            int book_row=0;
            for (selectAllUserList = SelectAllUsersRet.begin(); selectAllUserList != SelectAllUsersRet.end(); ++selectAllUserList)
               {
                QStandardItem *item1 = new QStandardItem(QString::number((*selectAllUserList).u_number));
                QStandardItem *item2 = new QStandardItem((*selectAllUserList).u_name);
                QStandardItem *item3 = new QStandardItem((*selectAllUserList).u_passWord);
                QStandardItem *item4 = new QStandardItem((*selectAllUserList).u_phone);
                QStandardItem *item5 = new QStandardItem((*selectAllUserList).u_accessLevel);

                m_model->setItem(book_row,0,item1);
                m_model->setItem(book_row,1,item2);
                m_model->setItem(book_row,2,item3);
                m_model->setItem(book_row,3,item4);
                m_model->setItem(book_row,4,item5);


                book_row++;

                qDebug()<< (*selectAllUserList).u_number <<"\t"
                        << (*selectAllUserList).u_name <<"\t"
                        << (*selectAllUserList).u_passWord <<"\t"
                        << (*selectAllUserList).u_phone <<"\t"
                        << (*selectAllUserList).u_accessLevel <<"\t";
            }
        }
    }
    qDebug()<<m_sql->sqlError();
    setTableState();
}
void homePage::treeItemClickedSlots(QTreeWidgetItem *item, int column)
{
    mTabCurrentRow =-1;//tba选中行清空
    m_model->clear();//使用之前清空。
    mcurrentStr=item->text(column);
    showdata();
}

//功能区
//区分显示的是哪个页面？
//查询(分页查询目前先不做？查询只做通过书名和用户名？)
void homePage::on_btn_query_clicked()
{
    //获取当前文本名字
    QString strName=ui->lineEdit->text();
    //qDebug()<<strName;
    if(strName.isEmpty())return;
    const int book_col=6;//书列数
    const int user_col=5;//用户列数
    m_model->clear();//使用之前清空。
    if(mcurrentStr == treeStr1){
        //查询所有书籍
        QList<Book> bookList =m_sql->searchBookByName(strName);
        if(bookList.isEmpty())
        {
            qDebug()<<"查询数据为空";
        }else
        {

            m_model->setColumnCount(book_col);
            QStringList laberlist;
            laberlist<<"编号"<<"名字"<<"作者"<<"价格"<<"出版社"<<"出版日期";
            m_model->setHorizontalHeaderLabels(laberlist);

            //通过迭代器遍历QList链表内容
            QList<Book>::iterator selectList4;

            int book_row=0;
            for (selectList4 = bookList.begin(); selectList4 != bookList.end(); ++selectList4)
            {
                QStandardItem *item1 = new QStandardItem(QString::number((*selectList4).b_number));
                QStandardItem *item2 = new QStandardItem((*selectList4).b_name);
                QStandardItem *item3 = new QStandardItem((*selectList4).b_author);
                QStandardItem *item4 = new QStandardItem(QString::number((*selectList4).b_price));
                QStandardItem *item5 = new QStandardItem((*selectList4).b_press);
                QStandardItem *item6 = new QStandardItem((*selectList4).b_PublicationDate);

                m_model->setItem(book_row,0,item1);
                m_model->setItem(book_row,1,item2);
                m_model->setItem(book_row,2,item3);
                m_model->setItem(book_row,3,item4);
                m_model->setItem(book_row,4,item5);
                m_model->setItem(book_row,5,item6);

                book_row++;
            }
        }
    }else if(mcurrentStr == treeStr2){
        //查询所有用户
        QList<Users> SelectAllUsersRet=m_sql->searcUser(strName);
        if(SelectAllUsersRet.isEmpty())
        {
            qDebug()<<"查询数据为空";
        }else
        {

            m_model->setColumnCount(user_col);
            QStringList laberlist;
            laberlist<<"编号"<<"名字"<<"密码"<<"电话"<<"权限";
            m_model->setHorizontalHeaderLabels(laberlist);
            //通过迭代器遍历QList链表内容
            QList<Users>::iterator selectAllUserList;
            int book_row=0;
            for (selectAllUserList = SelectAllUsersRet.begin(); selectAllUserList != SelectAllUsersRet.end(); ++selectAllUserList)
               {
                QStandardItem *item1 = new QStandardItem(QString::number((*selectAllUserList).u_number));
                QStandardItem *item2 = new QStandardItem((*selectAllUserList).u_name);
                QStandardItem *item3 = new QStandardItem((*selectAllUserList).u_passWord);
                QStandardItem *item4 = new QStandardItem((*selectAllUserList).u_phone);
                QStandardItem *item5 = new QStandardItem((*selectAllUserList).u_accessLevel);

                m_model->setItem(book_row,0,item1);
                m_model->setItem(book_row,1,item2);
                m_model->setItem(book_row,2,item3);
                m_model->setItem(book_row,3,item4);
                m_model->setItem(book_row,4,item5);


                book_row++;

                qDebug()<< (*selectAllUserList).u_number <<"\t"
                        << (*selectAllUserList).u_name <<"\t"
                        << (*selectAllUserList).u_passWord <<"\t"
                        << (*selectAllUserList).u_phone <<"\t"
                        << (*selectAllUserList).u_accessLevel <<"\t";
            }
        }
    }
    qDebug()<<m_sql->sqlError();
    setTableState();
}

//获取数据
void homePage::addBookDataSlots(QStringList data)
{
    if(mAddModFalg == 0)//添加，修改标志(0:添加,1:修改)
    {
        //添加书籍
        Book book;
        book.b_number=data.at(0).toInt();
        book.b_name=data.at(1);
        book.b_author=data.at(2);
        book.b_price=data.at(3).toInt();
        book.b_press=data.at(4);
        book.b_PublicationDate=data.at(5);
        bool addret=m_sql->addBook(book);
        if(addret == true)
        {
            //qDebug()<<"添加书籍成功";
            QMessageBox::warning(nullptr,QString("警告"),QString("添加书籍成功"));
            maddBook->hide();

            //显示所有数据
            m_model->clear();//使用之前清空。
            showdata();
        }else
        {
            //qDebug()<<"添加书籍失败";
            QMessageBox::warning(nullptr,QString("警告"),QString("添加书籍失败"));
        }
        qDebug()<<m_sql->sqlError();
    }else if(mAddModFalg == 1)//添加，修改标志(0:添加,1:修改))
    {
        //测试修改书籍信息过书籍编号
        Book modifyBook;
        modifyBook.b_number=mTabCurrentRow;
        modifyBook.b_name=data.at(1);
        modifyBook.b_author=data.at(2);
        modifyBook.b_price=data.at(3).toInt();
        modifyBook.b_press=data.at(4);
        modifyBook.b_PublicationDate=data.at(5);
        bool modifyret=m_sql->modifyBook(modifyBook);
        if(modifyret == true)
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("修改书籍成功"));
            maddBook->hide();

            //显示所有数据
            m_model->clear();//使用之前清空。
            showdata();
            mTabCurrentRow =-1;//tba选中行清空
        }else
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("修改书籍失败"));
        }
        qDebug()<<m_sql->sqlError();
    }
}

void homePage::addUserDataSlots(QStringList data)
{
    //qDebug()<<data;
    if(mAddModFalg == 0)//添加，修改标志(0:添加,1:修改)
    {
        //添加用户
        Users user;
        user.u_number=data.at(0).toInt();
        user.u_name=data.at(1);
        user.u_passWord=data.at(2);
        user.u_phone=data.at(3);
        user.u_accessLevel=data.at(4);

        bool addret=m_sql->addUser(user);
        if(addret == true)
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("添加用户成功"));
            maddUser->hide();

            //显示所有数据
            m_model->clear();//使用之前清空。
            showdata();
        }else
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("添加用户失败"));
        }
        qDebug()<<m_sql->sqlError();
    }else if(mAddModFalg == 1)//添加，修改标志(0:添加,1:修改))
    {
        //修改用户
        Users users;
        users.u_number =mTabCurrentRow;
        users.u_name =data.at(1);;
        users.u_passWord =data.at(2);;
        users.u_phone =data.at(3);;
        users.u_accessLevel=data.at(4);;
        bool uModRet=m_sql->modfiyUser(users);
        if(uModRet == true)
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("修改用户成功"));
            maddUser->hide();

            //显示所有数据
            m_model->clear();//使用之前清空。
            showdata();
            mTabCurrentRow =-1;//tba选中行清空
        }else
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("修改用户失败"));
        }
        qDebug()<<m_sql->sqlError();
    }
}

//增
void homePage::on_btn_add_clicked()
{
    mAddModFalg=0;
    emit setID("");
    if(mcurrentStr == treeStr1){
        maddBook->show();
    }else if(mcurrentStr == treeStr2){
        maddUser->show();
    }
}

//单击选中一行才可以删除和修改
void homePage::on_tableView_clicked(const QModelIndex &index)
{
    //获取当前选中行信息
    QList<QModelIndex> list =  ui->tableView->selectionModel()->selectedIndexes();

//    if (!ui->tableView->selectionModel()->selectedIndexes().isEmpty())
//         {
//          for (int i = 0; i < 6; i++)
//                {
//                   qDebug() << list[i].data();
//                }
//         }
    mTabCurrentRow=list[0].data().toInt();//获取当前选中行ID
    qDebug()<<mTabCurrentRow;
}


//删
void homePage::on_btn_del_clicked()
{
    if(mTabCurrentRow ==-1)
    {
        QMessageBox::warning(nullptr,QString("警告"),QString("当前选中行不能为空"));
        return;
    }
    if(mcurrentStr == treeStr1){
        //测试删除书籍过书籍编号
        //问题记录1:删除前是否先查询书籍是否存在，否则数据不存在也会提示删除成功。
        //思路：删除时应该是在列表中选择当前行后再删除，所以数据肯定存在
        bool delret=m_sql->delBook(mTabCurrentRow);
        if(delret == true)
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("删除书籍成功"));

            //显示所有数据
            m_model->clear();//使用之前清空。
            showdata();
        }else
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("删除书籍失败"));
        }
        qDebug()<<m_sql->sqlError();
    }else if(mcurrentStr == treeStr2){
        //测试删除用户过用户编号
        //问题记录1:删除前是否先查询用户是否存在，否则数据不存在也会提示删除成功。
        //思路：删除时应该是在列表中选择当前行后再删除，所以数据肯定存在
        bool uDelRet=m_sql->delUser(mTabCurrentRow);
        if(uDelRet == true)
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("删除用户成功"));

            //显示所有数据
            m_model->clear();//使用之前清空。
            showdata();
        }else
        {
            QMessageBox::warning(nullptr,QString("警告"),QString("删除用户失败"));
        }
        qDebug()<<m_sql->sqlError();
    }
}
//改
void homePage::on_btn_modify_clicked()
{
    if(mTabCurrentRow ==-1)
    {
        QMessageBox::warning(nullptr,QString("警告"),QString("当前选中行不能为空"));
        return;
    }
    mAddModFalg=1;
    emit setID(QString::number(mTabCurrentRow));
    if(mcurrentStr == treeStr1){
        maddBook->show();
    }else if(mcurrentStr == treeStr2){
        maddUser->show();
    }
}



