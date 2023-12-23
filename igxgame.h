#ifndef IGXGAME_H
#define IGXGAME_H

#include <QObject>
#include <QVector>
#include <memory>

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

signals:
private:
    int m_numPlanets;
    GameBoard m_gameBoard;
};

#endif // IGXGAME_H
