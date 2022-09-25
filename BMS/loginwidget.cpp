#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QDebug>
#include <QMessageBox>

loginWidget::loginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginWidget)
{
    ui->setupUi(this);


    //默认密码不可见
    ui->lineEditPassword->setEchoMode(QLineEdit::EchoMode::Password);

    //初始化样式
    //上面背景：40,102,169
    //下面背景：255,255,255



    this->setAttribute(Qt::WA_StyledBackground,true);
    //因为this是QWidge，且是顶级窗口，要让它的样式表语句生效必须加上上面一句
    this->setStyleSheet("QWidget#loginWidget{border-image:url(:/images/bk2.png)}");

    ui->w_title->setStyleSheet(QString("background-color:rgb(40,102,169);color:rgb(255,255,255);"));
    ui->w_login->setStyleSheet(QString("background-color:rgb(255,255,255)"));
    ui->lineEditUserName->setStyleSheet("QLineEdit#lineEditUserName{border: 2px solid #bdc3c7;border-radius:6px;}");
    ui->lineEditPassword->setStyleSheet("QLineEdit#lineEditPassword{border: 2px solid #bdc3c7;border-radius:6px;}");
    ui->btnLogin->setStyleSheet(QString("background-color:rgb(40,102,169);color:rgb(255,255,255);border-radius:6px;"));
    ui->btnExit->setStyleSheet(QString("background-color:rgb(40,102,169);color:rgb(255,255,255);border-radius:6px;"));

    QIcon icon(":/images/BMS.ico");
    setWindowIcon(icon);
    setWindowTitle("图书管理系统");

    //设置定时器程序运行1秒后连接数据库。
    QTimer *conSql =new QTimer(this);
    conSql->start(1000);


    connect(conSql,&QTimer::timeout,[=]
    {

        conSql->stop();//停止定时器

        m_homePage= new homePage();//创建主页对象
        connect(this,&loginWidget::initData,m_homePage,&homePage::initSlots);

        m_initSql = new initSql();//创建m_initSql对象
        bool connectRet = m_initSql->connectDatabase();//连接数据库

        if(false == connectRet)
        {
            qDebug()<<"连接数据库失败";
            QMessageBox::warning(nullptr,QString("警告"),"连接数据库失败");
        }else
        {
            qDebug()<<"连接数据库成功";
            //QMessageBox::information(nullptr,QString("提示"),"连接数据库成功");
        }
        qDebug()<<m_initSql->sqlError();




        //测试数据库sql代码
#if 0
    //测试代码添加书籍
    Book book;
    book.b_number=10000;
    book.b_name="西游记";
    book.b_author="罗贯中";
    book.b_price=388;
    book.b_press="清华大学出版社";
    book.b_PublicationDate="2008-5-21";
    bool addret=m_initSql->addBook(book);


    if(addret == true)
    {
        qDebug()<<"添加书籍成功";
    }else
    {
        qDebug()<<"添加书籍失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试删除书籍过书籍编号
    //问题记录1:删除前是否先查询书籍是否存在，否则数据不存在也会提示删除成功。
    //思路：删除时应该是在列表中选择当前行后再删除，所以数据肯定存在
    bool delret=m_initSql->delBook(1111);
    if(delret == true)
    {
        qDebug()<<"删除书籍成功";
    }else
    {
        qDebug()<<"删除书籍失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试修改书籍信息过书籍编号
    Book modifyBook;
    modifyBook.b_number=3333;
    modifyBook.b_name="红楼梦";
    modifyBook.b_author="施耐庵";
    modifyBook.b_price=38;
    modifyBook.b_press="清华大学出版社";
    modifyBook.b_PublicationDate="2006-5-21";
    bool modifyret=m_initSql->modifyBook(modifyBook);
    if(modifyret == true)
    {
        qDebug()<<"修改书籍成功";
    }else
    {
        qDebug()<<"修改书籍失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //清空所有书籍
    //返回值功能未测试
    bool clearRet=m_initSql->clearAllBook();
    if(clearRet == true)
    {
        qDebug()<<"清除所有书籍成功";
    }else
    {
        qDebug()<<"清除所有书籍失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

    //问题记录2：所有的查询语句返回的是查询成功的信息，那么查询失败或者为空信息怎么返回，是否需要提示。(已解决)
#if 0
    //查询所有书籍通过名字
    QList<Book> selectByNameRet =m_initSql->searchBookByName("西游记");
    if(selectByNameRet.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Book>::iterator selectList1;
        for (selectList1 = selectByNameRet.begin(); selectList1 != selectByNameRet.end(); ++selectList1)
            qDebug()<< (*selectList1).b_number <<"\t"
                    << (*selectList1).b_name <<"\t"
                    << (*selectList1).b_author <<"\t"
                    << (*selectList1).b_price <<"\t"
                    << (*selectList1).b_press <<"\t"
                    << (*selectList1).b_PublicationDate <<"\t";
    }
    qDebug()<<m_initSql->sqlError();

#endif


#if 0
    //分页查询所有书籍通过书籍名字
    QList<Book> selectByNameRet2 =m_initSql->searchBookByName("西游记",1,5);

    if(selectByNameRet2.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Book>::iterator selectList3;
        for (selectList3 = selectByNameRet2.begin(); selectList3 != selectByNameRet2.end(); ++selectList3)
            qDebug()<< (*selectList3).b_number <<"\t"
                    << (*selectList3).b_name <<"\t"
                    << (*selectList3).b_author <<"\t"
                    << (*selectList3).b_price <<"\t"
                    << (*selectList3).b_press <<"\t"
                    << (*selectList3).b_PublicationDate <<"\t";
    }
    qDebug()<<m_initSql->sqlError();
#endif


#if 0
    //查询所有书籍通过作者名字
    QList<Book> selectByAuthorRet =m_initSql->searchBookByAuthorNmae("施耐庵");

    if(selectByAuthorRet.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Book>::iterator selectList2;
        for (selectList2 = selectByAuthorRet.begin(); selectList2 != selectByAuthorRet.end(); ++selectList2)
            qDebug()<< (*selectList2).b_number <<"\t"
                    << (*selectList2).b_name <<"\t"
                    << (*selectList2).b_author <<"\t"
                    << (*selectList2).b_price <<"\t"
                    << (*selectList2).b_press <<"\t"
                    << (*selectList2).b_PublicationDate <<"\t";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //分页查询所有书籍通过书籍名字
    QList<Book> selectByAuthorRet2 =m_initSql->searchBookByAuthorNmae("罗贯中",2,6);

    if(selectByAuthorRet2.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Book>::iterator selectList4;
        for (selectList4 = selectByAuthorRet2.begin(); selectList4 != selectByAuthorRet2.end(); ++selectList4)
            qDebug()<< (*selectList4).b_number <<"\t"
                    << (*selectList4).b_name <<"\t"
                    << (*selectList4).b_author <<"\t"
                    << (*selectList4).b_price <<"\t"
                    << (*selectList4).b_press <<"\t"
                    << (*selectList4).b_PublicationDate <<"\t";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //查询所有书籍数量
    int countRet=m_initSql->searchBookSum();
    qDebug()<<m_initSql->sqlError();
    qDebug() <<"书籍数量"<< countRet;

#endif

    /*
     *测试用户接口
     */

#if 0
    //测试添加用户
    Users user;
    user.u_number =2;
    user.u_name =QString("李四");
    user.u_passWord =QString("123456789");
    user.u_phone =QString("12345678901");
    user.u_accessLevel=QString("普通用户");
    bool uAddret=m_initSql->addUser(user);
    if(true == uAddret)
    {
        qDebug() <<"添加用户成功";
    }
    else
    {
        qDebug() <<"添加用户失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试删除用户过用户编号
    //问题记录1:删除前是否先查询用户是否存在，否则数据不存在也会提示删除成功。
    //思路：删除时应该是在列表中选择当前行后再删除，所以数据肯定存在
    bool uDelRet=m_initSql->delUser(3);
    if(uDelRet == true)
    {
        qDebug()<<"删除用户成功";
    }else
    {
        qDebug()<<"删除用户失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试删除所有用户
    bool uDelAllret=m_initSql->delAllUser();
    if(uDelAllret == true)
    {
        qDebug()<<"删除所有用户成功";
    }else
    {
        qDebug()<<"删除所有用户失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试修改用户
    Users users;
    users.u_number =1;
    users.u_name =QString("张三");
    users.u_phone =QString("11011011011");
    users.u_passWord =QString("987654321");
    users.u_accessLevel=QString("管理员");
    bool uModRet=m_initSql->modfiyUser(users);
    if(uModRet == true)
    {
        qDebug()<<"修改用户成功";
    }else
    {
        qDebug()<<"修改用户失败";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试查询用户
    QList<Users> SelectUsersRet=m_initSql->searcUser("张三");
    if(SelectUsersRet.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Users>::iterator selectUserList;
        for (selectUserList = SelectUsersRet.begin(); selectUserList != SelectUsersRet.end(); ++selectUserList)
            qDebug()<< (*selectUserList).u_number <<"\t"
                    << (*selectUserList).u_name <<"\t"
                    << (*selectUserList).u_passWord <<"\t"
                    << (*selectUserList).u_phone <<"\t"
                    << (*selectUserList).u_accessLevel <<"\t";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //测试查询所有用户
    QList<Users> SelectAllUsersRet=m_initSql->searcAllUser();
    if(SelectAllUsersRet.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Users>::iterator selectAllUserList;
        for (selectAllUserList = SelectAllUsersRet.begin(); selectAllUserList != SelectAllUsersRet.end(); ++selectAllUserList)
            qDebug()<< (*selectAllUserList).u_number <<"\t"
                    << (*selectAllUserList).u_name <<"\t"
                    << (*selectAllUserList).u_passWord <<"\t"
                    << (*selectAllUserList).u_phone <<"\t"
                    << (*selectAllUserList).u_accessLevel <<"\t";
    }
    qDebug()<<m_initSql->sqlError();
#endif

#if 0
    //查询所有用户数量
    int usercountRet=m_initSql->searchUserSum();
    qDebug()<<m_initSql->sqlError();
    qDebug() <<"用户数量"<< usercountRet;
#endif

    //m_initSql->closeDatabase();//关闭数据库

    });
}

loginWidget::~loginWidget()
{
    delete m_initSql;
    m_initSql =NULL;

    delete m_homePage;
    m_homePage =NULL;

    delete ui;
}

//设置密码是否可见
void loginWidget::on_radioButton_clicked()
{
    if(passwordEnable == false)//不可见
    {
        passwordEnable=true;
        ui->lineEditPassword->setEchoMode(QLineEdit::EchoMode::Password);
    }
    else//可见
    {
        passwordEnable=false;
        ui->lineEditPassword->setEchoMode(QLineEdit::EchoMode::Normal);
    }
}

//退出
void loginWidget::on_btnExit_clicked()
{
    this->close();
}

//登录
void loginWidget::on_btnLogin_clicked()
{
    QString userName=ui->lineEditUserName->text();//获取用户名
    QString passWord=ui->lineEditPassword->text();//获取密码
    if(userName.isEmpty() || passWord.isEmpty())//判读用户名、密码是否为空
    {
        QMessageBox::warning(nullptr,"警告",QString("用户名或密码不能为空！"));
        return;
    }
    bool loginFalg = false;

    QList<Users> SelectUsersRet=m_initSql->searcAllUser();
    if(SelectUsersRet.isEmpty())
    {
        qDebug()<<"查询数据为空";
    }else
    {
        //通过迭代器遍历QList链表内容
        QList<Users>::iterator selectUserList;
        for (selectUserList = SelectUsersRet.begin(); selectUserList != SelectUsersRet.end(); ++selectUserList)
        //如果链表里包含输入的用户名和密码loginFalg为真，登录成功。

        if(((*selectUserList).u_name == userName) &&((*selectUserList).u_passWord ==passWord))
        {
            loginFalg = true;
        }
    }
    qDebug()<<m_initSql->sqlError();
    if(loginFalg)
    {
        qDebug()<<"登录成功";
        this->hide();
        m_homePage->longinName=userName;
        m_homePage->show();
        emit initData();
    }else
    {
        //查询
        QMessageBox::warning(nullptr,QString("警告"),QString("用户名或密码错误"));
        return;
    }
}
