#include "addbook.h"
#include "ui_addbook.h"

addBook::addBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBook)
{
    ui->setupUi(this);
    setWindowFlags (Qt::FramelessWindowHint);
    setModal(true);
}

addBook::~addBook()
{
    delete ui;
}
//确定
void addBook::on_btnSure_clicked()
{
    QStringList str;
    str<<ui->lineID->text()
      <<ui->lineName->text()
      <<ui->lineAuthon->text()
      <<ui->linePrice->text()
      <<ui->linePress->text()
      <<ui->linePressTime->text();
    emit addBookData(str);
}
//取消
void addBook::on_btnCancel_clicked()
{
    emit myClose();
}

void addBook::setLineID(QString tempstr)
{
    ui->lineID->setText(tempstr);
}
