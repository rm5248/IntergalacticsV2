#include "planet.h"

Planet::Planet(QObject *parent)
    : QObject{parent}
{

}

Planet::Planet(char planetName, int numProduced, int numShips, int planetRatio, QObject *parent)
    : QObject{parent}{
    m_planetName = planetName;
    m_numberShipsProduced = numProduced;
    m_numberShips = numShips;
    m_planetRatio = planetRatio;
    m_isNull = false;
}

char Planet::planetName() const{
    return m_planetName;
}

int Planet::numberShipsProduced() const{
    return m_numberShipsProduced;
}

int Planet::numberShips() const{
    return m_numberShips;
}

int Planet::planetRatio() const{
    return m_planetRatio;
}

bool Planet::isNull() const {
    return m_isNull;
}
