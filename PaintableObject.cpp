#include "PaintableObject.h"
#include <QPainter>
#include <QPen>
TurnAlert::TurnAlert(QWidget* parent){
    this->setParent(parent);
}
void TurnAlert::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen;

    int width = 150;
    int height = 60;
    QRect mainRect = QRect(0,0,width,height);
//    painter.setBrush(QColor(131, 99, 56, 255));
//    painter.setPen(QColor(131, 99, 56, 255));
//    painter.drawRect(mainRect);
    QRect fontRect = QRect(0,0,90,60);
    QFont font = painter.font();
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    font.setPixelSize(25);
    painter.setFont(font);
    painter.drawText(fontRect,Qt::AlignCenter,tr(text.c_str()));
    if(turn_%2 == 0){
        pen.setColor(QColor(0,0,0,255));
    }
    else{
        pen.setColor(QColor(255,255,255,255));
    }
    pen.setWidth(15);
    painter.setPen(pen);

    QRect circleRect = QRect(100,20,15,15);
    painter.drawEllipse(circleRect);
}
Commentary::Commentary(QWidget* parent){
    this->setParent(parent);
}
void Commentary::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen;
    QRect blackRect(15,15,15,15);
    QRect whiteRect(15,50,15,15);
    QRect pendingRect(15,85,15,15);
    QRect errorRect(15,120,15,15);

    pen.setWidth(15);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawEllipse(blackRect);

    pen.setWidth(15);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawEllipse(whiteRect);

    pen.setWidth(15);
    pen.setColor(QColor(192,192,192,255));
    painter.setPen(pen);
    painter.drawEllipse(pendingRect);

    pen.setWidth(15);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    painter.drawEllipse(errorRect);

    QRect blackTextRect(40,5,150,30);
    QRect whiteTextRect(40,40,150,30);
    QRect pendingTextRect(40,75,150,30);
    QRect errorTextRect(40,110,150,30);
    QFont font = painter.font();
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    font.setPixelSize(25);
    painter.setFont(font);
    painter.drawText(blackTextRect,Qt::AlignCenter,tr("黑方(玩家)"));
    painter.drawText(whiteTextRect,Qt::AlignCenter,tr("白方(電腦)"));
    painter.drawText(pendingTextRect,Qt::AlignCenter,tr("未確認走步"));
    painter.drawText(errorTextRect,Qt::AlignCenter,tr("不合法走步"));
}
