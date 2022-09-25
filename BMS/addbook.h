#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>

namespace Ui {
class addBook;
}

class addBook : public QDialog
{
    Q_OBJECT

public:
    explicit addBook(QWidget *parent = nullptr);
    ~addBook();

private slots:
    void on_btnSure_clicked();
    void on_btnCancel_clicked();
public slots:
    void setLineID(QString tempstr);
signals:
    void myClose();
    void addBookData(QStringList str);

private:
    Ui::addBook *ui;
};

#endif // ADDBOOK_H
