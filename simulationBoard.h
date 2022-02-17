#ifndef SIMULATIONBOARD_H
#define SIMULATIONBOARD_H
#include "Stone.h"
#include <vector>
#include <set>
#include "Board.h"

class SimulationBoard{
public:
    SimulationBoard(const Board& board);
    Type avail_[15][15]={};
    std::vector<std::pair<int,int>> white_;
    std::vector<std::pair<int,int>> black_;
    std::set<std::pair<int,int>>nextPossible;
    bool isValid(int x,int y);
    void addStone(Type t,int x,int y);
    void resume(int x,int y);
    void update();
    int get_score(int x,int y)const;
    int critic()const;
    const int possibleNum_ = 1;
    Dir allDir[8]={Top,RightTop,Right,RightDown,Down,LeftDown,Left,LeftTop};
    std::pair<int,int>getDiff(Dir d)const;
    void Init(const Board& board);

};
#endif // SIMULATIONBOARD_H
