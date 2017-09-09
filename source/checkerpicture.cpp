#include "checkerpicture.h"
#include <QDebug>

CheckersPicture::CheckersPicture(QWidget *parent) : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Light,QColor(0x4c,0x7a,0x79, 200));
    setPalette(pal);
    setBackgroundRole(QPalette::Light);
    setAutoFillBackground(true);
    Red = QImage(":/icons/red.png");

    Redking = QImage(":/icons/redwang.png");
    White = QImage(":/icons/bai.png");

    Whiteking = QImage(":/icons/baiwang.png");

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setAttribute(Qt::WA_StaticContents);

    zoom = 16;
    curstate = NULL;
    n = 8;
    hourglass=QImage(":/icons/10bf64872ad6acc0fdd7b51ee2f40edb-2.png");
    setMinimumSize(zoom*(n+1),zoom*(n+1));
    showHourglass = true;

    thinking = false;
}

CheckersPicture::~CheckersPicture()
{

}

/*返回现在的状态*/
CheckersState * CheckersPicture::state() {
    return curstate;
}

/*设置电脑棋子颜色*/
void CheckersPicture::setComputerColor(uint8 color) {
    this->color = color;
}

/*设置当前状态*/
void CheckersPicture::setState(CheckersState * state) {
    if(state) {
        curstate = state;
        n = state->size();
        v.clear();
        repaint();
    } else {
        clear();
    }
}

/*设置棋盘大小*/
void CheckersPicture::setSize(int n) {
    this->n = n;
    update();
}

/*清除点集*/
void CheckersPicture::deleteVector() {
    if(v.size()) {
        v.clear();
        update();
    }
}

/*设置点集*/
void CheckersPicture::setVector(std::vector <point> & v) {
    this->v = v;
    update();
}

/*清除点集和当前状态*/
void CheckersPicture::clear()
{
    curstate = NULL;
    v.clear();
    update();
}

/*开始思考*/
void CheckersPicture::startThinking()
{
    thinking = true;
    repaint();
}

/*停止思考*/
void CheckersPicture::stopThinking()
{
    thinking = false;
    repaint();
}

/*左击鼠标事件*/
void CheckersPicture::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        qreal i = (event->pos().x() - p.x()) * (n)/side;
        qreal j = (double)n - (event->pos().y() - p.y()) * (n)/side - 1;
        if (color==BLACK)
            emit mouseClicked((int)i,(int)j);
        else
            emit mouseClicked(n -1 - (int)i, n - 1 - (int)j);
    }
}

/*绘制*/
void CheckersPicture::paintEvent(QPaintEvent *event)
{
    n = 10;
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setViewport(p.x(),p.y(),side,side);
    painter.setWindow(0, 0, zoom*(n), zoom*(n));

    QColor dark(0xcc,0xcc,0xcc,200);

    QColor endColor(0x78,0xff,0x21,200);            /*终点颜色*/
    QColor startColor(0xea,0x76,0x0f,200);              /*起点颜色*/
    QColor capturedColor(0xed,0x50,0x62,200);       /*捕获颜色*/
    QColor normalColor(0xd6,0xb8,0x2c,200);         /*正常色*/

    //QColor white(0xff, 0xff, 0xff, 220);
    QColor light(223, 240, 216);
    QColor deep(48, 168, 96);

    /*绘制棋盘*/
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            QRect rect = pixelRect(i, j);
            if( !((i+j%2)%2) ) {
                    painter.fillRect(rect, deep);
            } else {
                painter.fillRect(rect, light);
            }
        }
    }

    int ix,jx;

    if(v.size()) {
        int type;
        for(unsigned i=0; i< v.size(); i++) {
            color==WHITE ? jx = n-1-v.at(i).y : jx = v.at(i).y;
            color==WHITE ? ix = n-1-v.at(i).x : ix = v.at(i).x;
            QRect rect = pixelRect(ix, jx);
            type = v.at(i).type;
            if(type == MOVEDFROM)
                painter.fillRect(rect, startColor);
            else if(type == MOVEDTO || type == TOKING)
                painter.fillRect(rect, endColor);
            else if(type == MOVEDTHROUGH)
                painter.fillRect(rect, normalColor);
            else if(type == DELETED)
                painter.fillRect(rect, capturedColor);
        }
    }

    int s = zoom*0.4;
    int sd = zoom*0.3;
    /*画白棋*/
    if(curstate){   /*如果棋局状态发生了改变*/
        painter.setPen(Qt::white);
        painter.setBrush(QBrush(Qt::white));
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {

                color==WHITE ? jx = j+1 : jx = n-j;
                color==WHITE? ix = n-i : ix = i+1;
                if(curstate->at(i,j)==WHITE)
                {
                    //qDebug()<<zoom*(ix-0.5)-s<<' '<<zoom*(jx-0.5)-s<<' '<<s;
                    //painter.drawPixmap(QRect(zoom*(ix-0.5)-s,zoom*(jx-0.5)-s,2*s,2*s),QPixmap::fromImage(White));
                    //White = White.scaled(2*s, 2*s, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    //painter.drawImage(zoom*(ix-0.5)-s,zoom*(jx-0.5)-s,White);
                    //QImage fitimgpic=imgpic.scaled(400,560, Qt::KeepAspectRatioByExpanding);
                    painter.drawEllipse(QPoint(zoom*(ix-0.5),zoom*(jx-0.5)),s,s);
                }
                if(curstate->at(i,j)==WHITEKING) {
                    painter.drawEllipse(QPoint(zoom*(ix-0.5),zoom*(jx-0.5)),s,s);
                    painter.drawEllipse(QPoint(zoom*(ix-0.5),zoom*(jx-0.5)),sd,sd);
                }
            }
        }
        /*画黑棋*/
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                color==WHITE ? jx = j+1 : jx = n-j;
                color==WHITE ? ix = n-i : ix = i+1;
                if(curstate->at(i,j)==BLACK)
                    painter.drawEllipse(QPoint(zoom*(ix-0.5),zoom*(jx-0.5)),s,s);
                if(curstate->at(i,j)==BLACKKING) {
                    painter.drawEllipse(QPoint(zoom*(ix-0.5),zoom*(jx-0.5)),s,s);
                    painter.setPen(QPen(Qt::white,zoom*0.1));
                    painter.drawEllipse(QPoint(zoom*(ix-0.5),zoom*(jx-0.5)),sd,sd);
                    painter.setPen(QPen(Qt::black,zoom*0.1));
                }
            }
        }
    }
    if (thinking && showHourglass)
    {
        painter.setWindow(painter.viewport());
        painter.drawImage((width() - hourglass.width()+40) / 2, (height() - hourglass.height()) / 2, hourglass);
    }
}

/*拖动改变大小*/
void CheckersPicture::resizeEvent (QResizeEvent * event) {
    if(event->oldSize()!=event->size()) {
        update();
        side = qMin(width(), height());
        p = QPoint((width() - side) / 2, (height() - side) / 2);
        hourglass = QImage(":/icons/10bf64872ad6acc0fdd7b51ee2f40edb-2.png");
    } else {
        event->ignore();
    }
}

/*返回一个矩形*/
QRect CheckersPicture::pixelRect(int i, int j) const
{
    return QRect(zoom * (i-0.5) + zoom*0.5, zoom*(n-1.0) - zoom * j, zoom, zoom);
}



