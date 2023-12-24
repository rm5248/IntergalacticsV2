#ifndef FLEET_H
#define FLEET_H

#include <QObject>
#include <memory>

class Player;
class Planet;

class Fleet : public QObject
{
    Q_OBJECT
public:
    explicit Fleet(std::shared_ptr<Player> owner,
                   std::shared_ptr<Planet> from,
                   std::shared_ptr<Planet> to,
                   int numShips,
                   int ratio,
                   double distance,
                   QObject *parent = nullptr);

    bool hasArrived();

public slots:
    void tick();

signals:
    void arrived();

private:
    std::shared_ptr<Player> m_owner;
    std::shared_ptr<Planet> m_from;
    std::shared_ptr<Planet> m_to;
    int m_numShips;
    int m_ratio;
    double m_distance;
};

#endif // FLEET_H
