#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QScrollArea>
#include <QtNetwork>
#include <QString>
#include <QTimer>
#include "ui_mainwindow.h"
#include "checkergame.h"
#include "checkerstate.h"
#include "optionsdialog.h"
#include "dialog.h"
class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void peace();
    void lose();
private slots:
    void startNewGame();
    void setState(CheckersState *);
    void endGame();
    //void option();
    void send();
    //void about();
    //void help();
    void gameEnded(uint8 status);
    //void saveSettings();
    //void loadSettings();
    void timeChanged();

    void on_actionNewGame_triggered();
    void megoing();
    void yougoing();
    void on_actionOption_triggered();
    void m_y();
    void y_m();

private:
    Dialog* dia;
    CheckersGame * game;
    OptionsDialog *optionsdialog;
    int myColor;
    QString greeting;
    QTcpSocket* tcp;
    QLabel * timeLabel;
    QTimer * timeme;
    QTimer * timeyou;
    QLabel * whiteIcon;
    QLabel * whiteLabel;
    QLabel * blackIcon;
    QLabel * blackLabel;
    QString ip;
    QString port;
    int numme = 300;
    int numyou = 300;
};

#endif // MAINWINDOW_H
