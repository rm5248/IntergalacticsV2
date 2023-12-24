#ifndef IGXGAME_H
#define IGXGAME_H

#include <QObject>
#include <QVector>
#include <memory>
#include <QTimer>

#include "planet.h"

class Fleet;

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
    void fleetSpawned(std::shared_ptr<Planet> planet, std::shared_ptr<Fleet> f);

private:
    double distanceBetweenPlanets(std::shared_ptr<Planet> from, std::shared_ptr<Planet> to);

private slots:
    void fleetArrived();

private:
    int m_numPlanets;
    GameBoard m_gameBoard;
    QTimer m_tickTimer;
    QVector<std::shared_ptr<Player>> m_players;
    std::shared_ptr<Player> m_me;
    QVector<std::shared_ptr<Fleet>> m_fleets;
};

#endif // IGXGAME_H
