#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QString>
#include <qtxml/QDomDocument>
#include <QSignalMapper>
#include <string>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initServer();

public slots:
    void acceptConnection();
    void recvMessage(int);
    void disconnecteed(int);

private slots:
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    int status[8];
    int pair[8];
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket[8];
    int clientnum ;
};

#endif // MAINWINDOW_H
