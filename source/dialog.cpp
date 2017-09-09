#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString localip;


        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

           for (int i = 0; i < ipAddressesList.size(); ++i) {

                if (ipAddressesList.at(i) != QHostAddress::LocalHost &&

                    ipAddressesList.at(i).toIPv4Address()) {

                    localip = ipAddressesList.at(i).toString();

                    break;

              }

           }


    ui->label->setText("Your IP is \n"+localip);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    if(ui->lineEdit->text() == ""||ui->lineEdit_2->text() == "")
    {
        QMessageBox::information(NULL, "SUDOtree", "Please input server ip and port.", QMessageBox::Yes , QMessageBox::Yes);
        return;
    }
    ip = ui->lineEdit->text();
    port = ui->lineEdit_2->text();
    accept();
}

void Dialog::on_pushButton_2_clicked()
{
    reject();
}
