#include "igxgame.h"
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
        std::shared_ptr<Planet> p = std::make_shared<Planet>(name, QRandomGenerator::global()->generate() % 10, 0, QRandomGenerator::global()->generate() % 60);

        m_gameBoard[xLoc][yLoc] = p;
        qDebug() << "planet at " << xLoc << "," << yLoc;
    }
}

GameBoard IGXGame::board() const{
    return m_gameBoard;
}
