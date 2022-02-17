#include <QWidget>
#include "mainWindow.h"
#include <iostream>
#include <sstream>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QLCDNumber>
#include <QTimer>
#include <QMessageBox>
#include "AlgorithmThread.h"
#include <QPainter>

MainWindow::MainWindow(){
    //主視窗 ui 的建構
    //主要為添加元件和設定元件初值
     QPalette palette;
     this->setWindowTitle("AI_Project");
     this->setFixedSize(1024,800);
     this->setGeometry(0,0,1024,800);
     palette.setColor(QPalette::Window, Qt::white);
     this->setAutoFillBackground(true);
     this->setPalette(palette);
     timeText_ = new QLabel("剩餘時間: ",this);
     timeText_->setGeometry(80,10,100,50);
     timeNum_ = new QLCDNumber(this);
     timeNum_->setGeometry(180,10,50,50);
     turnAlert_ = new TurnAlert(this);
     turnAlert_->setGeometry(300,10,150,60);

     giveUp_ = new QPushButton("投降",this);
     giveUp_->setGeometry(700,150,70,30);
     confirm_ = new QPushButton("確認",this);
     confirm_->setGeometry(700,190,70,30);
     confirm_->setEnabled(false);
     start_ = new QPushButton("開始遊戲",this);
     start_->setGeometry(700,100,70,30);
     timeSelect_ = new QComboBox(this);
     timeSelect_->setGeometry(700,70,70,20);

     timeSelect_->insertItem(0,tr(timeOption_[0].c_str()));
     timeSelect_->insertItem(1,tr(timeOption_[1].c_str()));
     timeSelect_->insertItem(2,tr(timeOption_[2].c_str()));
     timeSelect_->insertItem(3,tr(timeOption_[3].c_str()));
     timeSelect_->insertItem(4,tr(timeOption_[4].c_str()));
     std::stringstream ss;
     ss<<timeOption_[timeSelect_->currentIndex()];
     ss>>timeLimit_;
     remainTime_ = timeLimit_;
//     std::cout<<"T = "<<timeLimit_<<std::endl;
     QObject::connect(timeSelect_,&QComboBox::currentIndexChanged,this,&MainWindow::test);
     timeNum_->setPalette(Qt::red);
     timeNum_->display(remainTime_);
     timeSelectText_ = new QLabel("時間選擇",this);
     timeSelectText_->setGeometry(650,70,50,20);

     board_ = new Board(this);
     board_->setGeometry(60,100,630,630);
     comment_ = new Commentary(this);
     comment_->setGeometry(700,250,300,150);
     textRegion_ = new QTextEdit(tr("hello\n"),this);
     textRegion_->setGeometry(700,450,300,150);
     textRegion_->setReadOnly(true);
     QObject::connect(start_,&QPushButton::pressed,this,&MainWindow::restart);
     QObject::connect(confirm_,&QPushButton::pressed,this,&MainWindow::handleConfirm);
     QObject::connect(giveUp_,&QPushButton::pressed,this,&MainWindow::handleGiveUp);
     timer = new QTimer(this);

     QObject::connect(timer,&QTimer::timeout,this,&MainWindow::displayTime);
     //timeout函數綁到display函數，timer時間設定為1秒
     //意思為每秒都會timeout然後更新時間並顯示
     t = new Thread();//此thread是電腦進行模擬所需要的thread
//     t->InitThread();

     QObject::connect(t,&Thread::send,this,&MainWindow::recieve);
     //為了將模擬結果傳回主執行緒需要將t執行緒的send函數綁到recieve函數上
}
void MainWindow::paintEvent(QPaintEvent* event){
    //畫背景
    QPainter painter(this);
    QPen pen;
    QRect mainRect(0,0,1024,800);
    painter.setBrush(QColor(112, 145, 177, 255));
    painter.setPen(QColor(112, 145, 177, 255));
    painter.drawRect(mainRect);
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    std::cout<<"mouse click"<<std::endl;
}
void MainWindow::displayTime(){
    //更新並顯示時間
    if(remainTime_<=0){//如果沒時間則遊戲結束
        std::cout<<"Time's up"<<std::endl;
        GameOver();
        return;
    }
    remainTime_-- ;
    timeNum_->display(remainTime_);
}
void MainWindow::handleConfirm(){
    //處理確認事件，功能為將待確認棋確認
    std::cout<<"confirm click"<<std::endl;
    bool res = false;
    Type win;
    if(turn_%2 == 0){
        res = board_->confirmPending(Type::black);
        board_->update();
    }
    else{
        std::cout<<"hhhhh"<<std::endl;
    }
    if(!res){//位置不合法則提示使用者
        textRegion_->moveCursor(QTextCursor::End);
        textRegion_->insertPlainText("第一步必須下在棋盤的最外圍兩層\n");
        textRegion_->moveCursor(QTextCursor::End);
    }
    if(res){
        remainTime_ = timeLimit_;
        timeNum_->display(remainTime_);
        turn_++;
        turnAlert_->setTurn(turn_);
        win = checkGame();//每次下完棋都會檢查是不是有人贏
        if(win == Type::black || win == Type::white){
            GameOver();
            return;
        }
        machineDecide();//玩家下完後換機器(白方)下

    }
}
void MainWindow::machineDecide(){
    t->setBoard(board_);//重設棋盤
    t->start();//開始執行緒
}
void MainWindow::handleGiveUp(){
    //處理投降事件
    std::cout<<"GiveUp click"<<std::endl;
    GameOver();
}
Type MainWindow::checkGame(){
    return board_->checkGame();
}
void MainWindow::test(int ind){
//    std::cout<<"test "<<ind<<std::endl;
    //處理設定時間的事件
    std::stringstream ss;
    ss<<timeOption_[ind];
    ss>>timeLimit_;
    remainTime_ = timeLimit_;
    timeNum_->setPalette(Qt::red);
    timeNum_->display(remainTime_);
}
void MainWindow::restart(){
    //重新開始遊戲的事件
    timeSelect_->setEnabled(false);
    remainTime_ = timeLimit_;
    turn_ = 0;
    board_->clear();
    timer->start(1000);
    started_ = true;
    start_->setEnabled(false);
    confirm_->setEnabled(true);
    textRegion_->clear();
    textRegion_->moveCursor(QTextCursor::End);
    textRegion_->insertPlainText("hello");
    textRegion_->moveCursor(QTextCursor::End);
    turnAlert_->setTurn(0);
}
void MainWindow::GameOver(){
    QString s;
    if(turn_%2 == 0){
        s = "White win!!";
        textRegion_->moveCursor(QTextCursor::End);
        textRegion_->insertPlainText(s);
        textRegion_->moveCursor(QTextCursor::End);
    }
    else{
        s = "Black win!!";
        textRegion_->moveCursor(QTextCursor::End);
        textRegion_->insertPlainText(s);
        textRegion_->moveCursor(QTextCursor::End);
    }
    timer->stop();
    QMessageBox messageBox;
    messageBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Reset);
    messageBox.setText(s);
    int ret = messageBox.exec();
    switch(ret){
        case QMessageBox::Ok:
            std::cout<<"Ok press"<<std::endl;
            confirm_->setEnabled(false);
            start_->setEnabled(true);
            timeSelect_->setEnabled(true);
            break;
        case QMessageBox::Reset:
            std::cout<<"Reset press"<<std::endl;
            restart();
            break;
    }
}
void MainWindow::recieve(Type res,int x,int y){
    //用來接收機器回傳的位置
    std::cout<<"In main thread"<<std::endl;
//    std::cout<<res<<std::endl;
//    std::cout<<res.id()<<std::endl;


    board_->addStone(res,x,y);
    turn_++;
    Type win = checkGame();
    if(win == Type::black || win == Type::white){
        GameOver();
        return;
    }
    turnAlert_->setTurn(turn_);
    remainTime_ = timeLimit_;
    timeNum_->display(remainTime_);


}
