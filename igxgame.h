#ifndef IGXGAME_H
#define IGXGAME_H

#include <QObject>
#include <QVector>
#include <memory>
#include <QTimer>

#include "planet.h"

typedef std::array<std::array<std::shared_ptr<Planet>, 16>, 16> GameBoard;

class IGXGame : public QObject
{
    Q_OBJECT
public:
    explicit IGXGame(QObject *parent = nullptr);

    void setNumberPlanets(int numPlanets);

    void startGame();

    GameBoard board() const;

    void addPlayer(std::shared_ptr<Player> p);

    void setMe(std::shared_ptr<Player> m);

    std::shared_ptr<Player> me() const;

    void moveShips(std::shared_ptr<Planet> from, std::shared_ptr<Planet> to, int numShips);

signals:
private:
    int m_numPlanets;
    GameBoard m_gameBoard;
    QTimer m_tickTimer;
    QVector<std::shared_ptr<Player>> m_players;
    std::shared_ptr<Player> m_me;
};

#endif // IGXGAME_H
