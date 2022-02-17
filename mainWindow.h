#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QObject>
#include <QTimer>
#include <QTextEdit>
#include <QComboBox>
#include "Board.h"
#include "AlgorithmThread.h"
#include "PaintableObject.h"
class MainWindow:public QWidget{
    Q_OBJECT
public:
    MainWindow();
    QLabel *timeText_ = nullptr;
    QPushButton *giveUp_= nullptr;
    QPushButton *confirm_ = nullptr;
    QPushButton *start_ = nullptr;
    QPushButton *restart_ = nullptr;
    QLCDNumber *timeNum_ = nullptr;
    QTextEdit *textRegion_ = nullptr;
    QTimer* timer = nullptr;
    Board* board_ = nullptr;
    TurnAlert* turnAlert_ = nullptr;
    Commentary* comment_ = nullptr;
    Thread* t = nullptr;
    QLabel* timeSelectText_ = nullptr;
    QComboBox* timeSelect_ = nullptr;
    std::string timeOption_[5]= {"30秒","60秒","90秒","120秒","300秒"};
protected:
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
private:
    bool started_ = false;
    void GameOver();
    void machineDecide();
    Type checkGame();
    void restart();
    void start();
    int turn_ = 0;
    int timeLimit_ = 0;
    int remainTime_ = 0;
public slots:
    void recieve(Type res,int x,int y);
private slots:
    void displayTime();
    void handleConfirm();
    void handleGiveUp();
    void test(int ind);

};

#endif // MAINWINDOW_H
