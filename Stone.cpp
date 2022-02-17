#include "Stone.h"
int Stone::nums_ = 0;
Stone::Stone(int x,int y,int rowInd,int colInd,Type t){
    //設定棋子的顏色
    x_ = x;
    y_ = y;
    type_ = t;
    id_ = nums_;
    rowInd_ = rowInd;
    colInd_ = colInd;
    switch(type_){
    case Type::black:
        stoneColor_ = QColor(0,0,0);
        textColor_ = QColor(255,255,255);
        nums_++;
        //如果生成的是白棋則num++
        break;
    case Type::white:
        stoneColor_ = QColor(255,255,255);
        textColor_ = QColor(0,0,0);
        nums_++;
        //如果生成的是白棋則num++
        break;
    case Type::pending:
        stoneColor_ = QColor(192,192,192,255);
        textColor_ = QColor(0,0,0);
        break;
    case Type::error:
        stoneColor_ = QColor(255,0,0,255);
        textColor_ = QColor(0,0,0);
        break;
    }
}
