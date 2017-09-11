#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TCP Server");
    memset(status,0,sizeof(status));
    memset(pair,0,sizeof(pair));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initServer()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8989);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void MainWindow::acceptConnection()
{
    for(int i=0;i<=7;++i)
    {

        if(status[i] == 0)
        {
            clientnum = i;
            break;
        }
    }

    status[clientnum] = 1;
    this->readWriteSocket[clientnum] =this->listenSocket->nextPendingConnection();
    qDebug()<<clientnum;
    QSignalMapper *m = new QSignalMapper(this);

    QObject::connect(this->readWriteSocket[clientnum],SIGNAL(readyRead()),m,SLOT(map()));
    m->setMapping(this->readWriteSocket[clientnum],clientnum);
    connect(m, SIGNAL(mapped(int)), this, SLOT(recvMessage(int)));

    //QSignalMapper *p = new QSignalMapper(this);
    //QObject::connect(this->readWriteSocket[clientnum],SIGNAL(disconnect()),p,SLOT(map()));
    //p->setMapping(this->readWriteSocket[clientnum],clientnum);
    //connect(p, SIGNAL(mapped(int)), this, SLOT(disconnecteed(int)));


}

void MainWindow::disconnecteed(int i)
{
    qDebug()<<"diaconnencoe";
    delete readWriteSocket[i];
    status[i] = 0;
    if(pair[i]!=-1)
    {
        pair[pair[i]] = -1;
        status[pair[i]] = 1;
        pair[i] = -1;
    }
}

void MainWindow::recvMessage(int i)
{
    if(status[i] == 1)
    {
        QString info;
        info = this->readWriteSocket[i]->readAll();
        qDebug()<<info;
        qDebug()<<i<<"clientnum";
        for(int j = 0;j<8;++j)
        {
            if(j!=i&&status[j] == 1)
            {
                pair[i] = j;
                pair[j] = i;
                status[j] = status[i] = 2;
                QString ss = "white";
                QString jj = "black";
                this->readWriteSocket[i]->write(ss.toStdString().c_str(),strlen(ss.toStdString().c_str()));
                this->readWriteSocket[j]->write(jj.toStdString().c_str(),strlen(jj.toStdString().c_str()));
                break;
            }
        }
        return;
    }
    if(status[i] == 2)
    {
        QString info;
        info = this->readWriteSocket[i]->readAll();

        QByteArray *array =new QByteArray;
        array->clear();
        array->append(info);
        qDebug()<<info;
        this->readWriteSocket[pair[i]]->write(array->data());
        if(info == "quit")
        {
            pair[pair[i]] = -1;
            status[pair[i]] = 1;
            status[i] = 1;
            pair[i] = -1;
        }
        return;
    }
    qDebug()<<status[i];
}

void MainWindow::on_sendButton_clicked()
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(ui->inputEdit->toPlainText());
    for(int i=0;i<=7;++i)
    {
        if(status[i] == 2)
            this->readWriteSocket[i]->write(array->data());
        ui->infoEdit->append(QString::number(i)+"   ");
    }
    ui->inputEdit->clear();
}
