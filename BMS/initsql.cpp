#include "initsql.h"

initSql::initSql(){}

//连接数据库
bool initSql::connectDatabase()
{
    qDebug()<<QSqlDatabase::drivers();//打印可用的数据库
    //QMYSQL
    //QSQLITE
    //主要使用这两个数据库

#if 0
    QSqlDatabase db =QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setPort(3306);
    db.setDatabaseName("qt");
#else
    QSqlDatabase db =QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/LJH2022Project/Qt/BMS/book.db");
#endif

    bool ret = db.open();
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=db.lastError().text();
    }
    return ret;
}

//返回sql错误信息
QString initSql::sqlError()
{
    return strError;
}


/*
 *书籍接口
 */

//增加书籍
bool initSql::addBook(Book book)
{
    QSqlQuery query;
    query.prepare("insert into book (b_number, b_name, b_author,b_price,b_press,b_PublicationDate) "
                  "values (:b_number, :b_name, :b_author,:b_price, :b_press, :b_PublicationDate)");
    query.bindValue(":b_number", book.b_number);
    query.bindValue(":b_name", book.b_name);
    query.bindValue(":b_author", book.b_author);
    query.bindValue(":b_price", book.b_price);
    query.bindValue(":b_press", book.b_press);
    query.bindValue(":b_PublicationDate", book.b_PublicationDate);
    bool ret=query.exec();
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }

    return ret;
}

//删除书籍通过书籍编号
bool initSql::delBook(int number)
{
    QSqlQuery query;
    query.prepare(QString("delete from book where b_number = %1").arg(number));
    bool ret=query.exec();
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return ret;
}

//清空所有书籍
//delete from book where 1=1
bool initSql::clearAllBook()
{
    QSqlQuery query;
    query.prepare(QString("delete from book where 1=1"));
    bool ret=query.exec();
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return ret;
}

//修改书籍信息过书籍编号
//ID设为主键自增，不能修改
//update [table]set column = ? where column = ?
bool initSql::modifyBook(Book book)
{
    QSqlQuery query;
    query.prepare(QString("update book set (b_name, b_author,b_price,b_press,b_PublicationDate)"
                          "=('%1', '%2',%3, '%4', '%5') where b_number=%6 ")
                  .arg(book.b_name)
                  .arg(book.b_author)
                  .arg(book.b_price)
                  .arg(book.b_press)
                  .arg(book.b_PublicationDate)
                  .arg(book.b_number));
    bool ret=query.exec();//query.exec()能返回执行数据库是否成。
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return ret;
}

QList<Book> initSql::searchAllBook()
{
    //QSqlQuery query(QString("select * from book where b_name = '%1'").arg(bookName));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from book "));
    bool ret=query.exec();
    QList<Book> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Book book;
    while (query.next()) {
        book.b_number = query.value(0).toInt();
        book.b_name = query.value(1).toString();
        book.b_author = query.value(2).toString();
        book.b_price = query.value(3).toInt();
        book.b_press = query.value(4).toString();
        book.b_PublicationDate = query.value(5).toString();
         //qDebug() << r_number<<"\t"<<r_name<<"\t"<<r_author<<"\t"<<r_price<<"\t"<<r_press<<"\t"<<r_PublicationDate;
        restlist.push_back(book);
    }

    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}

//查询书籍信息通过书名(默认通过ID升序)
QList<Book> initSql::searchBookByName(QString bookName)
{
    //QSqlQuery query(QString("select * from book where b_name = '%1'").arg(bookName));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from book where b_name = '%1'").arg(bookName));
    bool ret=query.exec();
    QList<Book> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Book book;
    while (query.next()) {
        book.b_number = query.value(0).toInt();
        book.b_name = query.value(1).toString();
        book.b_author = query.value(2).toString();
        book.b_price = query.value(3).toInt();
        book.b_press = query.value(4).toString();
        book.b_PublicationDate = query.value(5).toString();
         //qDebug() << r_number<<"\t"<<r_name<<"\t"<<r_author<<"\t"<<r_price<<"\t"<<r_press<<"\t"<<r_PublicationDate;
        restlist.push_back(book);
    }

    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}
//查询书籍信息通过书名(分页显示,默认通过ID升序)
//bookName：书名
//pageNo:   页码
//pageSize: 每页显示几条
QList<Book> initSql::searchBookByName(QString bookName, int pageNo, int pageSize)
{
    //limit(pageNo - 1) * pageSize, pageSize
//    QSqlQuery query(QString("select * from book where b_name = '%1'  order by b_number asc limit %2,%3;")
//                    .arg(bookName)
//                    .arg((pageNo - 1) * pageSize)
//                    .arg(pageSize));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from book where b_name = '%1'  order by b_number asc limit %2,%3;")
                  .arg(bookName)
                  .arg((pageNo - 1) * pageSize)
                  .arg(pageSize));
    bool ret=query.exec();
    QList<Book> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Book book;
    while (query.next()) {
        book.b_number = query.value(0).toInt();
        book.b_name = query.value(1).toString();
        book.b_author = query.value(2).toString();
        book.b_price = query.value(3).toInt();
        book.b_press = query.value(4).toString();
        book.b_PublicationDate = query.value(5).toString();
         //qDebug() << r_number<<"\t"<<r_name<<"\t"<<r_author<<"\t"<<r_price<<"\t"<<r_press<<"\t"<<r_PublicationDate;
        restlist.push_back(book);
    }
    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}
//查询书籍信息通过作者名(默认通过ID升序)
QList<Book> initSql::searchBookByAuthorNmae(QString authorName)
{
    //QSqlQuery query(QString("select * from book where b_author = '%1'").arg(authorName));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from book where b_author = '%1'").arg(authorName));
    bool ret=query.exec();
    QList<Book> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Book book;
    while (query.next()) {
        book.b_number = query.value(0).toInt();
        book.b_name = query.value(1).toString();
        book.b_author = query.value(2).toString();
        book.b_price = query.value(3).toInt();
        book.b_press = query.value(4).toString();
        book.b_PublicationDate = query.value(5).toString();
         //qDebug() << r_number<<"\t"<<r_name<<"\t"<<r_author<<"\t"<<r_price<<"\t"<<r_press<<"\t"<<r_PublicationDate;
        restlist.push_back(book);
    }
    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}
//查询书籍信息通过作者名(分页显示,默认通过ID升序)
//authorName    作者名
//pageNo:       页码
//pageSize:     每页显示几条
QList<Book> initSql::searchBookByAuthorNmae(QString authorName, int pageNo, int pageSize)
{
    //limit(pageNo - 1) * pageSize, pageSize
//    QSqlQuery query(QString("select * from book where b_author = '%1' order by b_number asc limit %2,%3;")
//                    .arg(authorName)
//                    .arg((pageNo - 1) * pageSize)
//                    .arg(pageSize));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from book where b_author = '%1' order by b_number asc limit %2,%3;")
                  .arg(authorName)
                  .arg((pageNo - 1) * pageSize)
                  .arg(pageSize));
    bool ret=query.exec();
    QList<Book> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Book book;
    while (query.next()) {
        book.b_number = query.value(0).toInt();
        book.b_name = query.value(1).toString();
        book.b_author = query.value(2).toString();
        book.b_price = query.value(3).toInt();
        book.b_press = query.value(4).toString();
        book.b_PublicationDate = query.value(5).toString();
         //qDebug() << r_number<<"\t"<<r_name<<"\t"<<r_author<<"\t"<<r_price<<"\t"<<r_press<<"\t"<<r_PublicationDate;
        restlist.push_back(book);
    }
    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}
//查询所有书籍数量
//select count(1) sum from book
int initSql::searchBookSum()
{
    //QSqlQuery query(QString("select count(1) sum from book"));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select count(1) sum from book"));
    bool ret=query.exec();
    QString count;
    while (query.next()) {
        count = query.value(0).toString();
    }
    int m_sum =count.toInt();
    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return m_sum;
}

/*
 *用户接口
 */

//添加用户
bool initSql::addUser(Users user)
{
    QSqlQuery query;
    query.prepare("insert into user (u_number, u_name, u_passWord,u_phone,u_accessLevel) "
                  "values (:u_number, :u_name, :u_passWord,:u_phone,:u_accessLevel)");
    query.bindValue(":u_number", user.u_number);
    query.bindValue(":u_name", user.u_name);
    query.bindValue(":u_passWord", user.u_passWord);
    query.bindValue(":u_phone", user.u_phone);
    query.bindValue(":u_accessLevel", user.u_accessLevel);

    bool ret=query.exec();
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }

    return ret;
}

//删除用户通过编号
bool initSql::delUser(int number)
{
    QSqlQuery query;
    query.prepare(QString("delete from user where u_number = %1").arg(number));
    bool ret=query.exec();
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return ret;
}

//删除所有用户
bool initSql::delAllUser()
{
    QSqlQuery query;
    query.prepare(QString("delete from user where 1=1"));
    bool ret=query.exec();
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return ret;
}

//修改用户信息
bool initSql::modfiyUser(Users user)
{
    QSqlQuery query;
    query.prepare(QString("update user set (u_name,u_passWord,u_phone,u_accessLevel)"
                          "=('%1', '%2','%3','%4') where u_number = %5 ")
                  .arg(user.u_name)
                  .arg(user.u_passWord)
                  .arg(user.u_phone)
                  .arg(user.u_accessLevel)
                  .arg(user.u_number));

    bool ret=query.exec();//query.exec()能返回执行数据库是否成。
    //记录错误信息
    if(ret == true)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return ret;
}

//查询用户通过名字
QList<Users> initSql::searcUser(QString uName)
{
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from user where u_name = '%1'").arg(uName));
    bool ret=query.exec();
    QList<Users> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Users users;
    while (query.next()) {
        users.u_number = query.value(0).toInt();
        users.u_name = query.value(1).toString();
        users.u_passWord = query.value(2).toString();
        users.u_phone = query.value(3).toString();
        users.u_accessLevel = query.value(4).toString();
         //qDebug() << r_number<<"\t"<<r_name<<"\t"<<r_author<<"\t"<<r_price<<"\t"<<r_press<<"\t"<<r_PublicationDate;
        restlist.push_back(users);
    }

    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}

QList<Users> initSql::searcAllUser()
{
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select * from user "));
    bool ret=query.exec();
    QList<Users> restlist;//QList可以一直插入，那么取数据的时候6个一组
    Users users;
    while (query.next()) {
        users.u_number = query.value(0).toInt();
        users.u_name = query.value(1).toString();
        users.u_passWord = query.value(2).toString();
        users.u_phone = query.value(3).toString();
        users.u_accessLevel = query.value(4).toString();
        restlist.push_back(users);
    }

    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return restlist;
}

//查询所有用户数量
int initSql::searchUserSum()
{
    //QSqlQuery query(QString("select count(1) sum from book"));
    //这里明明可以一步完成，但是分开写，是为了更好的判断返回值
    QSqlQuery query;
    query.prepare(QString("select count(1) sum from user"));
    bool ret=query.exec();
    QString count;
    while (query.next()) {
        count = query.value(0).toString();
    }
    int m_sum =count.toInt();
    if(true == ret)
    {
        strError=QString("sql语句执行成功");
    }else
    {
        strError=query.lastError().text();
    }
    return m_sum;
}

//关闭数据库
void initSql::closeDatabase()
{
    QSqlDatabase db =QSqlDatabase::database();
    db.close();
}


