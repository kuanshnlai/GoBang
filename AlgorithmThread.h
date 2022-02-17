#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H
#include <QThread>
#include <string>
#include "Board.h"
#include "Stone.h"
#include "simulationBoard.h"
class Thread: public QThread{
    Q_OBJECT
public:
   void InitThread();
   void setBoard(Board* board){board_ = board;};
protected:
   void run();
private:

//   int critic(SimulationBoard fakeBoard);
   Board* board_ = nullptr;
   std::pair<int,int>alphaBeta(int depth);
   int simulation(SimulationBoard fakeBoard,int depth,int maxDepth,int alpha,int beta);
signals:
   void send(Type  res,int x,int y);
};

#endif // ALGORITHMTHREAD_H
