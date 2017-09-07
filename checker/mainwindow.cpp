#include "mainwindow.h"
#include <QStackedLayout>
#include <QLabel>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    whiteIcon = new QLabel;
    statusbar->addWidget(whiteIcon);
    whiteIcon->setPixmap(QPixmap(":/icons/whitelabel.png"));

    whiteLabel = new QLabel;
    statusbar->addWidget(whiteLabel);

    blackIcon = new QLabel;
    statusbar->addWidget(blackIcon);
    blackIcon->setPixmap(QPixmap(":/icons/blacklabel.png"));

    blackLabel = new QLabel;
    statusbar->addWidget(blackLabel);

    QWidget * spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    statusbar->addWidget(spacer,1);

    timeLabel = new QLabel;
    timeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    statusbar->addWidget(timeLabel);
    timeLabel->setStyleSheet("color: black;");

    time = new QTimer;
    connect(time, SIGNAL(timeout()), SLOT(timeChanged()));
    time->start(1000);
    timeChanged();

    game = new CheckersGame;
    optionsdialog=new OptionsDialog(this);

    connect(actionNewGame, SIGNAL(triggered()), this, SLOT(startNewGame()));
    connect(actionOption, SIGNAL(triggered()), this, SLOT(option()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(optionsdialog,SIGNAL(accepted()),this,SLOT(saveSettings()));
    connect(optionsdialog,SIGNAL(accepted()),this,SLOT(startNewGame()));


    connect(picture, SIGNAL(mouseClicked(int, int)), game, SLOT(setClicked(int, int)));
    connect(game, SIGNAL(stateChanged(CheckersState *)), picture, SLOT(setState(CheckersState *)));
    connect(game, SIGNAL(stateChanged(CheckersState *)), SLOT(setState(CheckersState *)));
    connect(game, SIGNAL(vectorChanged(std::vector <point> &)), picture, SLOT(setVector(std::vector <point> &)));
    connect(game, SIGNAL(vectorDeleted()), picture, SLOT(deleteVector()));

    connect(game, SIGNAL(gameEnded(uint8)), SLOT(gameEnded(uint8)));
    connect(game, SIGNAL(startThinking()), picture, SLOT(startThinking()));
    connect(game, SIGNAL(stopThinking()), picture, SLOT(stopThinking()));

    startNewGame();
}

MainWindow::~MainWindow()
{
    delete game;
}

/*开始新的游戏*/
void MainWindow::startNewGame()
{
    loadSettings();

    QSettings settings("IoT1301", "Checkers");
    int color = myColor = settings.value("color",WHITE).toInt();
    int level = settings.value("level",EASY).toInt();
    int show=settings.value("showHourglass").toBool();
    if (color == WHITE)
        color = BLACK;
    else
        color = WHITE;
    picture->setComputerColor(WHITE);
    picture->setShowHourglass(show);
    game->setMaxLevel(level);
    game->startNewGame(WHITE);

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
        str=QString("恭喜，你赢了！\n再来一局？");
    else
        str=QString("很遗憾，你输了！\n再来一局？");
    int r=QMessageBox::information(this,tr("结果"),str,QMessageBox::Yes|QMessageBox::No);
    if(r==QMessageBox::Yes)
        startNewGame();
    else
        endGame();
}

/*打开选项对话框*/
void MainWindow::option()
{
    if (!optionsdialog) {
        optionsdialog= new OptionsDialog(this);
    }
    optionsdialog->show();
    optionsdialog->raise();
    optionsdialog->activateWindow();
}

/*保存设置*/
void MainWindow::saveSettings()
{
    QSettings settings("IoT1301", "Checkers");
    int color, level;

    if( optionsdialog->colorcomboBox->currentIndex() == 0)
        color = WHITE;
    else
        color = BLACK;

    if(optionsdialog->levelcomboBox->currentIndex()==0)
        level=EASY;
    else if(optionsdialog->levelcomboBox->currentIndex()==1)
        level=MEDIUM;
    else
        level=DIFFICULT;

    settings.setValue("color",color);
    settings.setValue("level",level);
    settings.setValue("showHourglass", optionsdialog->showcheckBox->isChecked());
}

/*载入设置*/
void MainWindow::loadSettings()
{
    QSettings settings("IoT1301", "Checkers");

    int color = settings.value("color",WHITE).toInt();
    if(color != WHITE && color !=BLACK)
        color = WHITE;
    int level = settings.value("level",EASY).toInt();
    if( level < EASY || level > DIFFICULT)
        level = EASY;

    if( color == BLACK )
        optionsdialog->colorcomboBox->setCurrentIndex(1);
    else
        optionsdialog->colorcomboBox->setCurrentIndex(0);
    if(level==MEDIUM)
        optionsdialog->levelcomboBox->setCurrentIndex(1);
    else if(level==DIFFICULT)
        optionsdialog->levelcomboBox->setCurrentIndex(2);
    else
        optionsdialog->levelcomboBox->setCurrentIndex(0);

    bool showHourglass= settings.value("showHourglass", true).toBool();
    optionsdialog->showcheckBox->setChecked(showHourglass);
}

void MainWindow::timeChanged()
{
    timeLabel->setText(QTime::currentTime().toString("HH:mm:ss"));
}

void MainWindow::about() {
    QMessageBox::about(this, trUtf8("关于国际跳棋"),
                       trUtf8(
                           "<h3 align=center>关于国际跳棋</h3>"
                           "<P>开发者"
                           "<P align=center>黄可钊"
                           "<P align=center>- HUST, IOT1301, 2017 -"));
}

void MainWindow::help(){
    QMessageBox *messageRecord = new QMessageBox(this);
    messageRecord->setStyleSheet("background-color:white");
    messageRecord->setWindowTitle("国际跳棋规则");
    QString str = QString("<h3>棋盘</h3>");

    QLabel *label = new QLabel(str);
    label->setWordWrap(true);
    QScrollArea *area = new QScrollArea();
    area->setMinimumSize(530, 550);
    area->setWidget(label);
    area->setWidgetResizable(true);
    area->setStyleSheet("border:0");
    area->setBackgroundRole(QPalette::Midlight);
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(area);
    (dynamic_cast<QGridLayout *>(messageRecord->layout()))->addLayout(grid, 0, 0);
    QPushButton *buttonOK = messageRecord->addButton("确 认", QMessageBox::AcceptRole);
    buttonOK->setStyleSheet("QPushButton{width:65px solid; height: 22px; border-radius:4px; "
                            "background-color:qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 255), "
                            "stop:0.5 rgba(236, 236, 236, 255));}"
                            "QPushButton:hover{ background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9,"
                            " stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255));border-radius:5px;"
                            " border: 1px solid #3C80B1}"
                            "QPushButton:pressed{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, "
                            "stop:0.494318 rgba(134, 198, 233, 255), stop:0.5 rgba(206, 234, 248, 255));border-radius:5px;"
                            "border: 1px solid #5F92B2;  }");
    messageRecord->exec();
}
