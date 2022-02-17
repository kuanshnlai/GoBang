

#include <QApplication>
#include "mainWindow.h"
#include "Board.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Game g;
//    g.show();
    MainWindow window;
    window.show();
    return a.exec();
}
