#include "AlgorithmThread.h"
#include <iostream>
#include <climits>
const int NINF = INT_MIN;
const int INF = INT_MAX;
void Thread::InitThread(){
    //board_ = nullptr;
}

void Thread::run(){
    std::cout<<"I run"<<std::endl;

    int depth = 3;  // alpha beta搜尋深度3為剛好的層數若再多搜尋速度會明顯下降許多
    std::pair<int,int> pos = alphaBeta(depth);//執行alpha beta search 深度為3層
    int x = pos.first;
    int y = pos.second;
    bool res = board_->isValid(x,y);//如果傳回來的位置不合法則random取位置
    while(!res){
        x = rand()%15;
        y = rand()%15;
        res = board_->isValid(x,y);
    }
//    std::cout<<res_.x()<<" ,"<<res_.y()<<std::endl;
    emit send(Type::white,x,y);
}
int Thread::simulation(SimulationBoard fakeBoard, int depth, int maxDepth, int alpha, int beta){
    //std::cout<<"Enter simulation"<<std::endl;
    //進行節點模擬幫盤面打分數
    bool update = false;
    int score = NINF;
    if(depth == maxDepth){
        return fakeBoard.critic();
    }
    else{
        Type player = (depth%2==0)?Type::black:Type::white;
        auto allPoss = fakeBoard.nextPossible;
        for(auto it:allPoss){//所有可能當作下一步位置
            fakeBoard.addStone(player,it.first,it.second);
            if(depth%2==0){ //深度為偶數代表玩家(黑方)
                int tmpScore = simulation(fakeBoard,depth+1,maxDepth,alpha,beta);
                if(!update){ //若節點還沒被更新過則直接取模擬的分數
                    score = tmpScore;
                    update = true;
                }
                else{
                    score = std::min(tmpScore,score);//使用min函數來更新score
                }
                beta = std::min(beta,score);//更新beta
                if(beta<=alpha){//beta<=alpha則剪枝
                    return score;
                }
            }
            else{//深度為偶數代表電腦(白方)
                int tmpScore = simulation(fakeBoard,depth+1,maxDepth,alpha,beta);
                if(!update){//若節點還沒被更新過則直接取模擬的分數
                    score = tmpScore;
                    update = true;
                }
                else{
                    score = std::max(tmpScore,score);//使用max函數來更新score
                }
                alpha = std::max(alpha,score);//更新alpha
                if(beta<=alpha){//beta<=alpha則剪枝
                    return score;
                }
            }
            fakeBoard.resume(it.first,it.second);//回復盤面以便進行下次模擬
        }
    }
    return score;
}
std::pair<int,int> Thread::alphaBeta(int depth){
    std::pair<int,int> res(-1,-1);
    std::cout<<"In alpha beta"<<std::endl;
//    for(int i=0;i<16;i++){
//        for(int j=0;j<16;j++){
//            if(board_->avail_[i][j] == Type::black){
//                std::cout<<"1 ";
//            }
//            else if(board_->avail_[i][j] == Type::white){
//                std::cout<<"2 ";
//            }
//            else if(board_->avail_[i][j] == Type::empty){
//                std::cout<<"0 ";
//            }
//            else{
//                std::cout<<"-1 ";
//            }
//        }
//        std::cout<<std::endl;
//    }
    SimulationBoard fakeBoard(*board_);//生成一個假的棋盤方便模擬
    std::vector<int> score(fakeBoard.nextPossible.size(),0);//幫下一步可能的所有位置打分數
    int alpha = NINF,beta = INF;
    int cnt = 0;
    int maxScore = NINF;
    std::cout<<"Simulation Debug info"<<std::endl;
    for(auto &it : fakeBoard.nextPossible){
        std::cout<<"("<<it.first<<" ,"<<it.second<<")"<<std::endl;
    }
    std::cout<<"Simulation Debug info Finish"<<std::endl;
    std::set<std::pair<int,int>> allPoss = fakeBoard.nextPossible;
    for(auto& it : allPoss){
        std::cout<<"Simulation "<<cnt<<" move"<<"("<< it.first<<" ,"<<it.second<<")"<<std::endl;
        int x = it.first;
        int y = it.second;
        fakeBoard.addStone(Type::white,x,y);//將棋下在目前迭代到的位置
        score[cnt] = simulation(fakeBoard,0,depth-1,alpha,beta);//進行模擬
        fakeBoard.resume(x,y);//回復棋盤
        if(score[cnt]>maxScore){//如果分數比目前最大分數高則更新
            maxScore = score[cnt];
            res.first = x;
            res.second = y;
        }
        else if(score[cnt] == maxScore){//如果分數等於目前最大分數則直接判斷將棋下在哪個位置分數高
            fakeBoard.addStone(Type::white,res.first,res.second);
            int nowScore = fakeBoard.critic();
            fakeBoard.resume(res.first,res.second);

            fakeBoard.addStone(Type::white,it.first,it.second);
            int tmpScore = fakeBoard.critic();
            fakeBoard.resume(it.first,it.second);
            if(tmpScore>nowScore){
                res.first = it.first;
                res.second = it.second;
            }
        }
        ++cnt;
    }
    std::cout<<"All score"<<std::endl;
    for(size_t i = 0;i<score.size();i++){
        std::cout<<"Score "<<i<<" = "<<score[i]<<std::endl;
    }
    if(res.first == -1 && res.second == -1){
      std::cout<<"invalid"<<std::endl;
    }
    return res;
}
