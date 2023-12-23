#include "ui/mainwindow.h"

#include <QApplication>

#include "igxgame.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IGXGame gm;
    MainWindow w;
    gm.startGame();
    w.setIGXGame(&gm);
    w.show();
    return a.exec();
}
