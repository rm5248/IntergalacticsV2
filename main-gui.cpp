#include "ui/mainwindow.h"

#include <QApplication>

#include "igxgame.h"
#include "player.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IGXGame gm;
    std::shared_ptr<Player> player1 = std::make_shared<Player>();
    std::shared_ptr<Player> player2 = std::make_shared<Player>();
    player1->setColor(QColor(255, 0, 0));
    player2->setColor(QColor(0, 255, 0));
    gm.addPlayer(player1);
    gm.addPlayer(player2);
    gm.setMe(player1);

    MainWindow w;
    gm.startGame();
    w.setIGXGame(&gm);
    w.show();
    return a.exec();
}
