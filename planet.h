#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QTimer>
#include <memory>

class Player;

class Planet : public QObject
{
    Q_OBJECT
public:
    explicit Planet(QObject *parent = nullptr);
    explicit Planet(char planetName, int numProduced, int numShips, int planetRatio, QObject *parent = nullptr);

    char planetName() const;
    int numberShipsProduced() const;
    int numberShips() const;
    int planetRatio() const;
    bool isNull() const;
    void setOwner(std::shared_ptr<Player> p);
    int ticksTillNextProduction() const;
    std::shared_ptr<Player> owner() const;

signals:
    void shipsProduced();

public slots:
    void gameStarted();
    void gamePaused();
    void tick();

private:
    char m_planetName = 0;
    int m_numberShipsProduced = 0;
    int m_numberShips = 0;
    int m_planetRatio = 0;
    bool m_isNull = true;
    int m_prodTurns;
    std::shared_ptr<Player> m_owner;
};

#endif // PLANET_H
