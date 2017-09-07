#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TCP Server");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initServer()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void MainWindow::acceptConnection()
{
    this->readWriteSocket[clientnum] =this->listenSocket->nextPendingConnection();
    QSignalMapper *m = new QSignalMapper(this);

    QObject::connect(this->readWriteSocket[clientnum],SIGNAL(readyRead()),m,SLOT(map()));
    m->setMapping(this->readWriteSocket[clientnum],clientnum);
    connect(m, SIGNAL(mapped(int)), this, SLOT(recvMessage(int)));

    ++clientnum;

}
void MainWindow::recvMessage(int i)
{
    QString info;
    info = this->readWriteSocket[i]->readAll();
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(info);
    qDebug()<<info;
    if(i%2)// i is odd
        this->readWriteSocket[i-1]->write(array->data());
    else
        this->readWriteSocket[i+1]->write(array->data());
}

void MainWindow::on_sendButton_clicked()
{
    /*QByteArray *array =new QByteArray;
    array->clear();
    array->append(ui->inputEdit->toPlainText());
    this->readWriteSocket->write(array->data());
    ui->infoEdit->append("Server: "+ui->inputEdit->toPlainText());
//    ui->inputEdit->clear();*/
}
