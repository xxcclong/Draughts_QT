#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QString>
#include <qtxml/QDomDocument>
#include <QSignalMapper>

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

private slots:
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket[4];
    int clientnum = 0;
};

#endif // MAINWINDOW_H
