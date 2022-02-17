#ifndef PAINTABLEOBJECT_H
#define PAINTABLEOBJECT_H
#include <QWidget>
#include <string>
class TurnAlert: public QWidget{
public:
    TurnAlert(QWidget* parent);
    void setTurn(int turn){turn_ = turn;text = "Turn "+std::to_string(turn_);update();};
protected:
    void paintEvent(QPaintEvent* event);
private:
    int turn_ = 0;
    std::string text = "Turn "+std::to_string(turn_);
};
class Commentary: public QWidget{
public:
    Commentary(QWidget* parent);
protected:
    void paintEvent(QPaintEvent* event);
};

#endif // PAINTABLEOBJECT_H
