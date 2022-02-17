#include "simulationBoard.h"
#include <iostream>
SimulationBoard::SimulationBoard(const Board& board){
    //弱化版Board 主要用來讓machine可以模擬
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            avail_[i][j] = board.avail_[i][j];
            if(avail_[i][j] == Type::black){
                black_.push_back(std::make_pair(i,j));
            }
            else if(avail_[i][j] == Type::white){
                white_.push_back(std::make_pair(i,j));
            }
        }
    }

    std::cout<<"copy init------------"<<std::endl;
    update();
    std::cout<<"copy init finish-----"<<std::endl;
}
void SimulationBoard::Init(const Board& board){
   black_.clear();
   white_.clear();
   for(int i=0;i<15;i++){
       for(int j=0;j<15;j++){
           avail_[i][j] = board.avail_[i][j];
           if(avail_[i][j] == Type::black){
               black_.push_back(std::make_pair(i,j));
           }
           else if(avail_[i][j] == Type::white){
               white_.push_back(std::make_pair(i,j));
           }
       }
   }
   update();
}
void SimulationBoard::update(){
    //每次呼叫更新下一次可能的位置
    //可能位置為目前所有棋的8個方向距離一的位置
    //因為可能會重複所以使用std::set來儲存
    nextPossible.clear();
    for(auto it:black_){
        int i = it.first;
        int j = it.second;
        for(auto d:allDir){
            auto diff = getDiff(d);
            int di = diff.first;
            int dj = diff.second;
            bool res = isValid(i+di,j+dj);

            if(res){
//                std::cout<<i+di<<" "<<j+dj<<"is valid"<<std::endl;
                std::pair<int,int> p = std::make_pair(i+di,j+dj);
                nextPossible.insert(p);
            }
            else{
//                std::cout<<i+di<<" "<<j+dj<<"is not valid"<<std::endl;
            }
        }
    }
    for(auto it:white_){
        int i = it.first;
        int j = it.second;
        for(auto d:allDir){
            auto diff = getDiff(d);
            int di = diff.first;
            int dj = diff.second;
            bool res = isValid(i+di,j+dj);

            if(res){
//                std::cout<<i+di<<" "<<j+dj<<"is valid"<<std::endl;
                std::pair<int,int> p = std::make_pair(i+di,j+dj);
                nextPossible.insert(p);
            }
            else{
//                std::cout<<i+di<<" "<<j+dj<<"is not valid"<<std::endl;
            }
        }
    }
//    std::cout<<"Show update debug==============="<<std::endl;
//    for(auto it:nextPossible){
//        std::cout<<"("<<it.first<<","<<it.second<<")"<<std::endl;
//    }
//    std::cout<<"Show update debug Finish==============="<<std::endl;

}
std::pair<int,int> SimulationBoard::getDiff(Dir d)const{
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
bool SimulationBoard::isValid(int x,int y){
    if(x<0||y<0||x>=15||y>=15){
//        std::cout<<"Out of boundary "<<x<<" "<<y<<std::endl;
        return false;
    }
    if(avail_[x][y] != Type::black && avail_[x][y]!=Type::white){
        return true;
    }
    return false;
}
void SimulationBoard::addStone(Type t,int x,int y){
    //添加棋函數 主要為判斷該位置合不合法，如果合法將該位置設成傳進來的棋類t(黑棋或白棋)
    bool valid = isValid(x,y);
    if(!valid){
        std::cout<<"Not valid move"<<"("<<x<<" ,"<<y<<")"<<std::endl;
        return ;
    }
    else if(t != Type::white&&t!=Type::black){
        std::cout<<"Stone is not valid"<<std::endl;

    }
    else{
        avail_[x][y] = t;
        if(t == Type::white){
            white_.push_back(std::make_pair(x,y));
        }
        else{
            black_.push_back(std::make_pair(x,y));
        }
        update();//每次有棋添加都呼叫update
        //std::cout<<"Add stone success"<<std::endl;
        return;
    }

}
void SimulationBoard::resume(int x,int y){
    //將位置x y的棋清空
    //且狀態要回復到沒有x y沒有棋的狀態(nextPossible要回復)
    if(x<0||y<0||x>=15||y>=15){
        std::cout<<"Resume fail boundary error ("<<x<<","<<y<<")"<<std::endl;
    }
    //do remove work
    if(avail_[x][y]==Type::black){//要刪除的棋是黑棋
        std::pair<int,int> target = std::make_pair(x,y);
        auto it = find(black_.begin(),black_.end(),target);
        if(it != black_.end()){
            black_.erase(it);
        }
        else{
            std::cout<<"Can't find "<< "("<<x<<" ,"<<y<<")this element in black"<<std::endl;
        }
    }
    else if(avail_[x][y]==Type::white){//要刪除的棋是白棋
        std::pair<int,int> target = std::make_pair(x,y);
        auto it = find(white_.begin(),white_.end(),target);
        if(it != white_.end()){
            white_.erase(it);
        }
        else{
            std::cout<<"Can't find "<< "("<<x<<" ,"<<y<<")this element in black"<<std::endl;
        }
    }
    else{//該位置沒有棋
       return;
    }
    avail_[x][y] = Type::empty;
    update();
}
int SimulationBoard::get_score(int x,int y)const{
    //判斷棋子分數
    int scoreList[6]={60,70,1250,1300,5000,10000000};//活二 死三 活三 死四 活四 五個棋連線
    int score = 0;
    Type cur = avail_[x][y];
    for(auto d:allDir){
        int tmpX = x;
        int tmpY = y;
        auto diff = this->getDiff(d);
        int dx = diff.first;
        int dy = diff.second;
        int countConnect = 0;
        int jumpCount = 0;
        int tmpScore = 0;
        bool block[] = {false,false};//判斷兩邊有沒有被阻檔
        bool jump = false;//判斷跳棋(中間空一格)
        for(int i=0;i<5;i++){
            if(tmpX<0||tmpY<0||tmpX>=15||tmpY>=15){
                block[0] = true;
                break;
            }
            if(!jump && avail_[tmpX][tmpY] == cur){
                ++countConnect;
            }
            else if(!jump && (avail_[tmpX][tmpY]==Type::empty||avail_[tmpX][tmpY]==Type::pending)){
               jump = true;
            }
            else if(jump && avail_[tmpX][tmpY] == cur){
                ++jumpCount;
            }
            else if(jump && avail_[tmpX][tmpY] != cur){
                break;
            }
            else if(avail_[tmpX][tmpY] != cur && avail_[tmpX][tmpY]!=Type::empty&&avail_[tmpX][tmpY]!=Type::pending){
                block[0] = true;
                break;
            }
            tmpX += dx;
            tmpY += dy;
        }
        tmpX = x - dx;
        tmpY = y - dy;
        if((tmpX>15||tmpY>15||tmpX<0||tmpY<0)||(avail_[tmpX][tmpY] != cur && avail_[tmpX][tmpY]!=Type::empty && avail_[tmpX][tmpY]!=Type::pending) ){
           //如果反方向位置不合法或者有對方的棋則判斷為阻擋
            block[1] = true;
        }
        int total = jumpCount + countConnect;//判斷盤面有連己的潛力
                //cout<<"Jump  = "<<jumpCount<<"Connect = "<<countConnect<<endl;
        bool live = (!block[0]&&!block[1]); //若兩邊都沒有被擋則為活棋
        bool dead = ((!block[1]&&block[0])||(!block[0]&&block[1]));//一邊被擋則為死棋(死三 死四)
        if(!live && !dead){//非活棋也非死棋則兩邊都被擋則此棋沒有潛力
           tmpScore = 0;
        }
        //live two
        else if((total== 2) && live){
           tmpScore = scoreList[0];
        }
       //dead three
        else if((total == 3) && !live &&dead){
           tmpScore = scoreList[1];
        }
        //live three
        else if((total==3) && live){
           tmpScore = scoreList[2];
        }
        //dead four
        else if((total == 4)&&dead){
           tmpScore = scoreList[3];
        }
                //live four
        else if((total == 4)&&live){
           tmpScore = scoreList[4];
        }
                //five
        else if(total == 5){
           tmpScore = scoreList[5];
        }
        else{
           tmpScore = 0;
        }
        score = std::max(score,tmpScore);
    }
    return score;
}

int SimulationBoard::critic()const{
    //判斷盤面分數
    int whiteScore = 0;
    int blackScore = 0;
    for(auto& it:black_){
        blackScore = std::max(blackScore,get_score(it.first,it.second));//所有黑棋取最大的分數
    }
    for(auto& it:white_){
        whiteScore = std::max(whiteScore,get_score(it.first,it.second));//所有白棋取最大的分數
    }
    return whiteScore-(blackScore*1.2); //因本系統電腦只能是白方(待改進)，回傳分數為白棋分數-黑棋分數*1.2 代表防守較為重要
}
