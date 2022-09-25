#ifndef INITSQL_H
#define INITSQL_H

#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

//书
struct Book
{
    int b_number;//编号
    QString b_name;//书名
    QString b_author;//作者
    int b_price;//价格
    QString b_press;//出版社
    QString b_PublicationDate;//出版时间
};

//用户
struct Users
{
    int u_number;//编号
    QString u_name;//姓名
    QString u_passWord;//密码
    QString u_phone;//电话
    QString u_accessLevel;//权限等级
};

class initSql
{

public:
   initSql();

    //连接数据库
    bool connectDatabase();

    //返回sql错误信息
    //默认noError
    QString sqlError();

    /*
     *书籍接口
     */

    //增加书籍
    bool addBook(Book book);

    //删除书籍通过书籍编号
    bool delBook(int number);

    //清空所有书籍
    bool clearAllBook();

    //修改书籍信息
    bool modifyBook(Book book);

    //查询所有书籍信息
    QList<Book> searchAllBook();

    //查询书籍信息通过书名(正常显示,通过ID)
    //QList<Book>比QStringList更加方便类型方面确定了是book类型的链表
    QList<Book> searchBookByName(QString bookName);

    //查询书籍信息通过书名(分页显示)
    //bookName：书名
    //pageNo:   页码
    //pageSize: 每页显示几条
    QList<Book> searchBookByName(QString bookName,int pageNo,int pageSize);

    //查询书籍信息通过作者名
    QList<Book> searchBookByAuthorNmae(QString authorName);

    //查询书籍信息通过作者名
    //bookName：书名
    //pageNo:   页码
    //pageSize: 每页显示几条
    QList<Book> searchBookByAuthorNmae(QString authorName,int pageNo ,int pageSize);

    //查询所有书籍数量
    int searchBookSum();

    /*
     *用户接口
     */

    //用户权限分为两种：管理员，普通用户。只有管理员才可以对用户增删改查
    //普通用户不能进入用户管理模块，只能进入书籍模块。

    //添加用户
    bool addUser(Users user);

    //删除用户通过编号
    bool delUser(int number);

    //删除所有用户
    bool delAllUser();

    //修改用户
    bool modfiyUser(Users user);

    //查询用户通过名字
    QList<Users> searcUser(QString uName);

    //查询所有用户
    QList<Users> searcAllUser();

    //查询所有用户数量
    int searchUserSum();

    //关闭数据库
    void closeDatabase();



private:
    QString strError;
};
#endif // INITSQL_H
