#ifndef BOARD_H
#define BOARD_H
#include <QWidget>
#include <QPoint>
#include <vector>
#include <QObject>
#include <set>

#include "Stone.h"
enum Dir{
    Top,
    RightTop,
    Right,
    RightDown,
    Down,
    LeftDown,
    Left,
    LeftTop
};
class Board:public QWidget{
    Q_OBJECT
public:

    Board(QWidget* parent=nullptr);
    void clear();
    void addStone(Type t,int x,int y);
    bool confirmPending(Type type);
    bool isValid(int x,int y);
    bool isValid(QPoint pos,int x,int y);
    bool isFirstValid(int x,int y);
    Type avail_[15][15];
    Type checkGame();
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
private:
    QPoint pendingStonePos(QPoint* p);
    void drawPendingStone(QPoint pos,Type t);
    void draw(QPoint pos);
    void updatePending(QPoint pos);
    Dir allDir[8]={Top,RightTop,Right,RightDown,Down,LeftDown,Left,LeftTop};
    std::pair<int,int>getDiff(Dir d)const;
    int enableOffsetX_ = 20;
    int enableOffsetY_ = 20;
    int gapWidth = 40;
    int gapHeight = 40;
    bool first = true;

    int test = 0;
    Stone* pending_ = nullptr;
    Stone* error_ = nullptr;
    int pendingX_ = -1;
    int pendingY_ = -1;
    std::vector<Stone> white_;
    std::vector<Stone> black_;
    std::set<std::pair<int,int>> nextPossible_;
    std::vector<std::pair<int,int>> line_;

};
#endif // BOARD_H
