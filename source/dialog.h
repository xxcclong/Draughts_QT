#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QtNetwork>
#include <QMessageBox>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString ip;QString port;

private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H
