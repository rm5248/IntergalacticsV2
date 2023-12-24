#include "igxgame.h"
#include "fleet.h"
#include <QRandomGenerator>
#include <QDebug>

IGXGame::IGXGame(QObject *parent)
    : QObject{parent}
{
    m_numPlanets = 10;
}

void IGXGame::setNumberPlanets(int numPlanets){
    m_numPlanets = numPlanets;
}

void IGXGame::startGame(){
    const char* valid_planet_names = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            m_gameBoard[x][y] = std::make_shared<Planet>();
        }
    }

    int currentPlayer = 0;
    for(int x = 0; x < m_numPlanets; x++){
        quint32 value = QRandomGenerator::global()->generate();
        int xLoc = QRandomGenerator::global()->generate() % 16;
        int yLoc = QRandomGenerator::global()->generate() % 16;

        if(!m_gameBoard[xLoc][yLoc]->isNull()){
            // Planet already exists here, try again
            x--;
            continue;
        }

        char name = valid_planet_names[ value % strlen(valid_planet_names) ];
        std::shared_ptr<Planet> p;
        if(currentPlayer >= m_players.size()){
            // Make an un-owned planet
            p = std::make_shared<Planet>(name, QRandomGenerator::global()->generate() % 10, 0, QRandomGenerator::global()->generate() % 60);
        }else{
            // Make the home base for this player
            p = std::make_shared<Planet>(name, 10, 10, 10);
            p->setOwner(m_players[currentPlayer]);
            currentPlayer++;
        }

        m_gameBoard[xLoc][yLoc] = p;
        qDebug() << "planet at " << xLoc << "," << yLoc;

        p->gameStarted();

        connect(&m_tickTimer, &QTimer::timeout,
                p.get(), &Planet::tick);
    }

    m_tickTimer.start(2500);
}

GameBoard IGXGame::board() const{
    return m_gameBoard;
}

void IGXGame::addPlayer(std::shared_ptr<Player> p){
    m_players.append(p);
}

void IGXGame::setMe(std::shared_ptr<Player> m){
    m_me = m;
}

std::shared_ptr<Player> IGXGame::me() const{
    return m_me;
}

void IGXGame::moveShips(std::shared_ptr<Planet> from, std::shared_ptr<Planet> to, int numShips){
    double distance = distanceBetweenPlanets(from, to);
    std::shared_ptr<Fleet> f = std::make_shared<Fleet>( from->owner(), from, to, numShips, from->planetRatio(), distance);

    m_fleets.append(f);

    connect(&m_tickTimer, &QTimer::timeout,
            f.get(), &Fleet::tick);
    connect(f.get(), &Fleet::arrived,
            this, &IGXGame::fleetArrived);

    emit fleetSpawned(from, f);
}

double IGXGame::distanceBetweenPlanets(std::shared_ptr<Planet> from, std::shared_ptr<Planet> to){
    int p1X;
    int p1Y;
    int p2X;
    int p2Y;
    double distance;

    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            if(m_gameBoard[x][y] == from){
                p1X = x;
                p1Y = y;
            }else if(m_gameBoard[x][y] == to){
                p2X = x;
                p2Y = y;
            }
        }
    }

    distance = std::sqrt(std::pow(p2X - p1X, 2) + std::pow(p2Y - p1Y, 2));

    return distance;
}

void IGXGame::fleetArrived(){
    // Tell the
}
