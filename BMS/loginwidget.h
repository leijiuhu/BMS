#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "initsql.h"
#include <QTimer>
#include "homepage.h"



QT_BEGIN_NAMESPACE
namespace Ui { class loginWidget; }
QT_END_NAMESPACE

class loginWidget : public QWidget
{
    Q_OBJECT

public:
    loginWidget(QWidget *parent = nullptr);
    ~loginWidget();

private slots:
    void on_radioButton_clicked();
    void on_btnExit_clicked();
    void on_btnLogin_clicked();
signals:
    void initData();
private:
    Ui::loginWidget *ui;
    bool passwordEnable =true;//是否显示密码,默认不显示。
    initSql *m_initSql;//数据库

    QTimer *conSql;//连接数据库的定时器
    homePage * m_homePage;//主页
};
#endif // LOGINWIDGET_H
