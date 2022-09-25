#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include "initsql.h"
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "addbook.h"
#include "adduser.h"

namespace Ui {
class homePage;
}

class homePage : public QWidget
{
    Q_OBJECT

public:
    explicit homePage(QWidget *parent = nullptr);
    ~homePage();
public slots:
    void initSlots();
    void treeItemClickedSlots(QTreeWidgetItem *item, int column);
    void showdata();
    void setTableState();
public:
    QString longinName;

private slots:


    void on_btn_query_clicked();
    void on_btn_add_clicked();
    void on_btn_del_clicked();
    void on_btn_modify_clicked();

    void addBookDataSlots(QStringList data);
    void addUserDataSlots(QStringList data);

    void on_tableView_clicked(const QModelIndex &index);
signals:
    void setID(QString str);
private:
    Ui::homePage *ui;
    initSql * m_sql;
    QStandardItemModel *m_model;
    QString treeStr1;
    QString treeStr2;
    QString mcurrentStr;
    addBook* maddBook;
    QStringList mdata;
    addUser *maddUser;
    int mTabCurrentRow;
    int mAddModFalg ;//添加，修改标志
};

#endif // HOMEPAGE_H
