#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>

namespace Ui {
class addUser;
}

class addUser : public QDialog
{
    Q_OBJECT

public:
    explicit addUser(QWidget *parent = nullptr);
    ~addUser();

private slots:
    void on_btnSure_clicked();
    void on_btnCancel_clicked();
public slots:
    void setLineID(QString tempstr);

signals:
    void myClose();
    void addUserData(QStringList str);

private:
    Ui::addUser *ui;
};

#endif // ADDUSER_H
