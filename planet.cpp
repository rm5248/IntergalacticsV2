#include "planet.h"

static const int TICKS_PER_TURN = 20;

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
    m_prodTurns = TICKS_PER_TURN;
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

void Planet::gamePaused(){

}

void Planet::gameStarted(){

}

void Planet::tick(){
    if(m_owner){
        m_prodTurns--;
        if(m_prodTurns == 0){
            m_numberShips += m_numberShipsProduced;
            m_prodTurns = TICKS_PER_TURN;
            emit shipsProduced();
        }
    }
}

void Planet::setOwner(std::shared_ptr<Player> p){
    m_owner = p;
    m_prodTurns = TICKS_PER_TURN;
}

int Planet::ticksTillNextProduction() const{
    return m_prodTurns;
}

std::shared_ptr<Player> Planet::owner() const{
    return m_owner;
}
