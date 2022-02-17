#ifndef STONE_H
#define STONE_H
#include <QColor>
#include <QPen>
enum Type{black,white,pending,empty,error};

class Stone{
    public:
       Stone(int x,int y,int rowInd,int colInd,Type t);

       QColor getStoneColor()const{return stoneColor_;};
       QColor getTextColor()const{return textColor_;};
       Type getType()const{return type_;};
       int x()const{return x_;};
       int y()const{return y_;};
       int id()const{return id_;};
       int rowInd()const{return rowInd_;};
       int colInd()const{return colInd_;};
       static void clear(){nums_ = 0;};
    private:
       int x_;
       int y_;
       int id_;
       int colInd_;
       int rowInd_;
       static int nums_;
       Type type_;
       QColor stoneColor_;
       QColor textColor_;
       QPen pen_;
};

#endif // STONE_H
