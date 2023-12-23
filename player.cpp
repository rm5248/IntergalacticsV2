#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{

}

void Player::setColor(QColor c){
    m_color = c;
}

QColor Player::color(){
    return m_color;
}
