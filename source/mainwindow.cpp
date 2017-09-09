#include "mainwindow.h"
#include <QStackedLayout>
#include <QLabel>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    dia = new Dialog;

    if(dia->exec()==0)
        close();
    ip = dia->ip;
    port = dia->port;
    setupUi(this);
    whiteIcon = new QLabel;
    statusbar->addWidget(whiteIcon);


    whiteLabel = new QLabel;
    statusbar->addWidget(whiteLabel);

    blackIcon = new QLabel;
    statusbar->addWidget(blackIcon);


    blackLabel = new QLabel;
    statusbar->addWidget(blackLabel);

    QWidget * spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    statusbar->addWidget(spacer,1);

    timeLabel = new QLabel;
    timeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    statusbar->addWidget(timeLabel);
    timeLabel->setStyleSheet("color: black;");

    timeme = new QTimer;
    timeyou = new QTimer;
    timeme -> setInterval(1000);
    timeyou -> setInterval(1000);
    connect(timeme,SIGNAL(timeout()),this,SLOT(megoing()));
    connect(timeyou,SIGNAL(timeout()),this,SLOT(yougoing()));

    tcp = new QTcpSocket;
    connect(tcp,SIGNAL(connected()),this,SLOT(send()));
    tcp->connectToHost(QHostAddress(ip),port.toInt());


    game = new CheckersGame(tcp,picture);
    //optionsdialog=new OptionsDialog(this);

    //connect(actionNewGame, SIGNAL(triggered()), this, SLOT(startNewGame()));
    //connect(actionOption, SIGNAL(triggered()), this, SLOT(option()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    //connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    //connect(actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    //connect(optionsdialog,SIGNAL(accepted()),this,SLOT(saveSettings()));
    //connect(optionsdialog,SIGNAL(accepted()),this,SLOT(startNewGame()));


    connect(picture, SIGNAL(mouseClicked(int, int)), game, SLOT(setClicked(int, int)));
    connect(game, SIGNAL(stateChanged(CheckersState *)), picture, SLOT(setState(CheckersState *)));
    connect(game, SIGNAL(stateChanged(CheckersState *)), SLOT(setState(CheckersState *)));
    connect(game, SIGNAL(vectorChanged(std::vector <point> &)), picture, SLOT(setVector(std::vector <point> &)));
    connect(game, SIGNAL(vectorDeleted()), picture, SLOT(deleteVector()));
    connect(this,SIGNAL(peace()),game,SLOT(peace()));
    connect(this,SIGNAL(lose()),game,SLOT(lose()));
    connect(game, SIGNAL(gameEnded(uint8)), SLOT(gameEnded(uint8)));
    connect(game, SIGNAL(startThinking()), picture, SLOT(startThinking()));
    connect(game, SIGNAL(startThinking()), this, SLOT(m_y()));
    connect(game, SIGNAL(stopThinking()), this, SLOT(y_m()));
    connect(game, SIGNAL(stopThinking()), picture, SLOT(stopThinking()));

    startNewGame();
}

MainWindow::~MainWindow()
{
    delete game;
}


void MainWindow::send()
{
    QString qstr = "fook";
    tcp->write(qstr.toStdString().c_str(),strlen(qstr.toStdString().c_str()));
}

/*开始新的游戏*/
void MainWindow::startNewGame()
{
    //loadSettings();

    //QSettings settings("IoT1301", "Checkers");
    //int color = myColor = settings.value("color",WHITE).toInt();
    //int level = settings.value("level",EASY).toInt();
    //int show=settings.value("showHourglass").toBool();
    //if (color == WHITE)
      //  color = BLACK;
    //else
      //  color = WHITE;
    picture->setComputerColor(BLACK);
    picture->setShowHourglass(1);
    game->setMaxLevel(3);
    game->startNewGame(BLACK);

}

/*更新棋子计数器*/
void MainWindow::setState(CheckersState * state)
{
    if (state->counts().size())
    {
        int whiteCount = state->counts().at(0) + state->counts().at(1);
        int blackCount = state->counts().at(4) + state->counts().at(5);
        whiteLabel->setText(QString("<b><font color=red>%1</font></b>").arg(whiteCount));
        blackLabel->setText(QString("<b><font color=red>%1</font></b>").arg(blackCount));
    }
}

/*程序结束*/
void MainWindow::endGame()
{
    game->endGame();
    picture->stopThinking();
    close();
}

/*游戏结束*/
void MainWindow::gameEnded(uint8 status)
{
    QString str;
    if(status == myColor)
        str=QString("you win!!!");
    else if(status == 0)
    {
        str=QString("tied game");
    }
    else str=QString("you lose");
    int r=QMessageBox::information(this,tr("result"),str,QMessageBox::Yes|QMessageBox::No);
    if(r==QMessageBox::Yes)
        endGame();
    else
        endGame();
}


void MainWindow::timeChanged()
{
    timeLabel->setText(QTime::currentTime().toString("HH:mm:ss"));
}

void MainWindow::on_actionNewGame_triggered()
{
    emit peace();
}

void MainWindow::on_actionOption_triggered()
{
    emit lose();
}


void MainWindow::megoing()
{
    --numme;
    timeLabel->setText(QString::number(numme)+"   "+QString::number(numyou));
    if(numme<=0)
        emit lose();
}

void MainWindow::yougoing()
{
    --numyou;
    timeLabel->setText(QString::number(numme)+"   "+QString::number(numyou));

}


void MainWindow::y_m()
{
    timeme->start();
    timeyou->stop();
}
void MainWindow::m_y()
{
    timeme->stop();
    timeyou->start();
}
