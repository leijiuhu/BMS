#include "adduser.h"
#include "ui_adduser.h"

addUser::addUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addUser)
{
    ui->setupUi(this);
    setWindowFlags (Qt::FramelessWindowHint);
    setModal(true);
}

addUser::~addUser()
{
    delete ui;
}

void addUser::on_btnSure_clicked()
{
    QStringList str;
    str<<ui->lineID->text()
      <<ui->lineName->text()
      <<ui->linePassword->text()
      <<ui->linePhone->text()
      <<ui->lineLevel->text();
    emit addUserData(str);
}

void addUser::on_btnCancel_clicked()
{
    emit myClose();
}

void addUser::setLineID(QString tempstr)
{
    ui->lineID->setText(tempstr);
}
