#include "fleet.h"

static const double FLEETSPEED = 0.100001f;

Fleet::Fleet(std::shared_ptr<Player> owner,
             std::shared_ptr<Planet> from,
             std::shared_ptr<Planet> to,
             int numShips,
             int ratio,
             double distance,
             QObject *parent)
    : QObject{parent},
      m_owner(owner),
      m_from(from),
      m_to(to),
      m_numShips(numShips),
      m_ratio(ratio),
      m_distance(distance)
{
}

void Fleet::tick(){
    m_distance -= FLEETSPEED;

    if(m_distance <= 0){
        // the fleet has arrived at its destination
        emit arrived();
    }
}

bool Fleet::hasArrived(){
    if(m_distance <= 0){
        return true;
    }

    return false;
}
