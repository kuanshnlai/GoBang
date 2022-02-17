#include <iostream>
#include <QPainter>
#include "Board.h"
#include <QPen>
#include <QMouseEvent>
#include <QRect>
#include <string>
#include <QBrush>


Board::Board(QWidget* parent){
    this->setParent(parent);
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            avail_[i][j] = Type::empty;
        }
    }
    this->setParent(parent);
    test = 100;
}
void Board::paintEvent(QPaintEvent* event){

    //畫棋盤的事件，每次更新會呼叫此函數
    QPainter painter(this);
    QPen pen;
    int offsetX = 15;
    int offsetY = 15;
    int width = 575;
    int height = 575;
    int boardWidth = 590;
    int boardHeight = 590;
    //畫棋盤背景
    QRect mainBoard = QRect(0,0,boardWidth,boardHeight);
    painter.setBrush(QColor(131, 99, 56, 255));
    painter.setPen(QColor(131, 99, 56, 255));
    painter.drawRect(mainBoard);


    painter.setBrush(Qt::NoBrush);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    painter.setPen(pen);


    //畫棋盤線
    for(int i=0;i<15;++i){
        painter.drawLine(offsetX,15,offsetX,height);
        offsetX += gapWidth;
    }
    for(int i=0;i<15;++i){
        painter.drawLine(15,offsetY,width,offsetY);
        offsetY += gapHeight;
    }

    QFont font = painter.font();
    font.setPixelSize(15);
    painter.setFont(font);
    //畫白棋
    for(auto &it:white_){
        int x = it.x() - 7.5+15;
        int y = it.y() - 7.5+15;
        QRect rect = QRect(x,y,15,15);
        pen.setWidth(15);
        pen.setColor(it.getStoneColor());
        painter.setPen(pen);
        painter.drawEllipse(rect);
        pen.setColor(it.getTextColor());
        painter.setPen(pen);
        painter.drawText(rect,Qt::AlignCenter,tr(std::to_string(it.id()).c_str()));
    }
    //畫黑棋
    for(auto &it:black_){
        int x = it.x() - 7.5+15;
        int y = it.y() - 7.5+15;
        QRect rect = QRect(x,y,15,15);
        pen.setWidth(15);
        pen.setColor(it.getStoneColor());
        painter.setPen(pen);
        painter.drawEllipse(rect);
        pen.setColor(it.getTextColor());
        painter.setPen(pen);
        painter.drawText(rect,Qt::AlignCenter,tr(std::to_string(it.id()).c_str()));
    }
    //如果有待確認的棋則畫它
    if(pending_!=nullptr){
        int x = pending_->x()- 7.5+15;
        int y = pending_->y()- 7.5+15;
        QRect rect = QRect(x,y,15,15);
        pen.setWidth(15);
        pen.setColor(pending_->getStoneColor());
        painter.setPen(pen);
        painter.drawEllipse(rect);
        pen.setColor(pending_->getTextColor());
        painter.setPen(pen);
        painter.drawText(rect,Qt::AlignCenter,tr(std::to_string(pending_->id()).c_str()));
     }
    //如果有錯誤的棋則畫它
    if(error_!=nullptr){
        int x = error_->x()- 7.5+15;
        int y = error_->y()- 7.5+15;
        QRect rect = QRect(x,y,15,15);
        pen.setWidth(15);
        pen.setColor(error_->getStoneColor());
        painter.setPen(pen);
        painter.drawEllipse(rect);
        pen.setColor(error_->getTextColor());
        painter.setPen(pen);
        painter.drawText(rect,Qt::AlignCenter,tr(std::to_string(error_->id()).c_str()));
    }
    //若有連線則畫
    if(!line_.empty()){
        pen.setWidth(8);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        std::pair<int,int> f = line_[0];
        std::pair<int,int> e = line_[line_.size()-1];
        painter.drawLine(f.first,f.second,e.first,e.second);
    }
}
void Board::mousePressEvent(QMouseEvent *event){
    //std::cout<<event->x()<<" ,"<<event->y()<<std::endl;
    //滑鼠點擊事件
    draw(event->pos());

}
void Board::draw(QPoint pos){

    std::cout<<"add_point"<<std::endl;
    updatePending(pos);//滑鼠點擊的話呼叫此函數來更新待確認棋的位置
    if(pending_!=nullptr){
        std::cout<<"add at "<<pending_->x()<<" "<<pending_->y()<<std::endl;
     }
    update();//呼叫update來重新繪製棋盤
}
bool Board::isFirstValid(int x,int y){
    //判斷第一手是不是合法
    if(!isValid(x,y)){
        return false;
    }
    else{
        if(x==0||x==1||y==0||y==1||x==13||x==14||y==13||y==14){
            return true;
        }
    }
    return false;
}
void Board::clear(){
    //把棋盤清空初始化
    nextPossible_.clear();
    white_.clear();
    black_.clear();
    pending_ = nullptr;
    pendingX_ = -1;
    pendingY_ = -1;
    first = true;
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            avail_[i][j] = Type::empty;
        }
    }
    line_.clear();
    update();
    Stone::clear();//將生成的棋數量設為0
}
bool Board::confirmPending(Type type){
    //將待確認棋確認，變黑棋或者白棋
    if(pending_==nullptr||pendingX_==-1||pendingY_==-1){
        std::cout<<"Pending empty"<<std::endl;
        return false;
    }
    if(first){
        bool res = isFirstValid(pendingX_,pendingY_);
        if(res){
            addStone(type,pendingX_,pendingY_);
            pending_ = nullptr;
            first = false;
            if(error_ != nullptr){
                delete error_;
            }
            error_ = nullptr;
            return true;
        }
        else{
            std::cout<<"First Step Invalid"<<std::endl;
            if(error_!=nullptr){
                delete error_;
            }
            error_ = new Stone(pending_->x(),pending_->y(),pendingX_,pendingY_,Type::error);
            return false;
        }
    }
//    Stone s = Stone(pending_->x(),pending_->y(),pendingX_,pendingY_,type);
    switch(type){
        case Type::black:
            std::cout<<"change"<<std::endl;
            addStone(type,pendingX_,pendingY_);
            pending_ = nullptr;
            return true;
        case Type::white:
            std::cout<<"change"<<std::endl;
            addStone(type,pendingX_,pendingY_);
            pending_ = nullptr;
            return true;
        case Type::pending:
            std::cout<<"Confirm fail"<<std::endl;
            return false;
        case Type::empty:
            std::cout<<"Confirm fail"<<std::endl;
            return false;
    }
    return false;
}
void Board::updatePending(QPoint pos){
    //更新待確認其位置函數
    if(pending_!=nullptr){
        delete pending_;
        pending_ = nullptr;
        pendingX_ = -1;
        pendingY_ = -1;
    }

    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){

            if(isValid(pos,i,j)){//位置有效(沒其他棋子且位置合法)則更新
                  int x = i * gapWidth;
                  int y = j * gapHeight;
                  std::cout<<"stone "<<std::endl;
                  pendingX_ = i;
                  pendingY_ = j;
                  pending_ = new Stone(x,y,i,j,Type::pending);
                  return;
            }
        }
    }
    pendingX_ = -1;
    pendingY_ = -1;
    pending_ = nullptr;//否則將待確認棋清空
}
void Board::addStone(Type t,int x,int y){
    //添加棋的函數
    int tmpx = x*gapWidth;
    int tmpy = y*gapHeight;
    Stone s = Stone(tmpx,tmpy,x,y,t);
    switch(t){
        case Type::pending:
            std::cout<<"pending cannot add\n";
            break;
        case Type::black:
            std::cout<<"add stone at ("<<x<<" ,"<<y<<")"<<std::endl;
            black_.push_back(s);
            avail_[x][y] = Type::black;
            break;
        case Type::white:
            std::cout<<"add stone at ("<<x<<" ,"<<y<<")"<<std::endl;
            white_.push_back(s);
            avail_[x][y] = Type::white;
            break;
        case Type::empty:
            std::cout<<"empty cannot add\n";
            break;
    }
    //新增可能的位置
    for(auto d:allDir){
        int dx = getDiff(d).first;
        int dy = getDiff(d).second;
        if(isValid(x+dx,y+dy)){
            std::pair<int,int> p = std::make_pair(x+dx,y+dy);
            nextPossible_.insert(p);
        }
    }
//    std::cout<<"Debug-----------"<<std::endl;
//    for(auto &it : nextPossible_){
//        std::cout<<"("<<it.first<<" ,"<<it.second<<")"<<std::endl;
//    }
//    std::cout<<"Debig Finish--------"<<std::endl;
    update();

}
std::pair<int,int> Board::getDiff(Dir d)const{
    //取得所有方向的位移向量dx dy
    int dx = 0;
    int dy = 0;
    switch(d){
        case Dir::Top:
            dx = 0;
            dy = -1;
            break;
        case Dir::RightTop:
            dx = 1;
            dy = -1;
            break;
        case Dir::Right:
            dx = 1;
            dy = 0;
            break;
        case Dir::RightDown:
            dx = 1;
            dy = 1;
            break;
        case Dir::Down:
            dx = 0;
            dy = 1;
            break;
        case Dir::LeftDown:
            dx = -1;
            dy = 1;
            break;
        case Dir::Left:
            dx = -1;
            dy = 0;
            break;
        case Dir::LeftTop:
            dx = -1;
            dy = -1;
            break;
    }
    return std::make_pair(dx,dy);
}
bool Board::isValid(int x,int y){
    //判斷位置是否合法 for 機器模擬 可以直接用 x y 當作avail的index
    if(x<0||y<0||x>=15||y>=15){
        return false;
    }
    if(avail_[x][y]!=Type::black&&avail_[x][y]!=Type::white){
        return true;
    }
    return false;
}
bool Board::isValid(QPoint pos,int i, int j){
    //判斷位置是否合法 for使用者點擊
    int x = i*gapWidth;
    int y = j*gapHeight;
    return ((pos.x() <= x+enableOffsetX_ &&pos.y() <= y+enableOffsetY_)&&(pos.x() >= x-enableOffsetX_ &&pos.y() >= y-enableOffsetY_))&&(isValid(i,j));
}
Type Board::checkGame(){
    //判斷是否結束遊戲(有人連線)回傳值為勝方，若沒人勝利則回傳empty
    for(auto &it:white_){

        for(auto d:allDir){
            int i = it.rowInd();
            int j = it.colInd();
            Type cur = avail_[i][j];
            int di = getDiff(d).first;
            int dj = getDiff(d).second;
            bool connect = true;
            for(int t=0;t<5;t++){
                if(i<0||j<0||i>=15||j>=15){
                    line_.clear();
                    connect = false;
                    break;
                }
                if(avail_[i][j]!=cur){
                    line_.clear();
                    connect = false;
                    break;
                }
                line_.push_back(std::make_pair(i*gapWidth+15,j*gapHeight+15));
                i+=di;
                j+=dj;
            }
            if(connect){
                update();
                return cur;
            }
        }
    }
    for(auto &it:black_){
        for(auto d:allDir){
            int i = it.rowInd();
            int j = it.colInd();
            Type cur = avail_[i][j];
            int di = getDiff(d).first;
            int dj = getDiff(d).second;
            bool connect = true;
            for(int t=0;t<5;t++){
                if(i<0||j<0||i>=15||j>=15){
                    line_.clear();
                    connect = false;
                    break;
                }
                if(avail_[i][j]!=cur){
                    line_.clear();
                    connect = false;
                    break;
                }
                line_.push_back(std::make_pair(i*gapWidth+15,j*gapHeight+15));
                i+=di;
                j+=dj;
            }
            if(connect){
                update();
                return cur;
            }
        }
    }
    line_.clear();
    return Type::empty;
}
