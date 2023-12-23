#include "galaxyui.h"
#include "ui_galaxyui.h"
#include "planetui.h"
#include "igxgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPen>

GalaxyUI::GalaxyUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GalaxyUI)
{
    ui->setupUi(this);

    for(int x = 0; x < m_boardUI.size(); x++){
        for(int y = 0; y < m_boardUI[0].size(); y++){
            m_boardUI[x][y] = nullptr;
        }
    }
}

GalaxyUI::~GalaxyUI()
{
    delete ui;
}

void GalaxyUI::connectPlanets(QPaintEvent* evt){
    QPainter painter;
    if(!painter.begin(this)){
        qDebug() << "cant paint";
    }

    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(QColor(245,245,65));
    QPen pen;
    pen.setWidth(1);
    pen.setBrush(brush);
    painter.setPen(pen);

    int planetSize = 2;

    int deltaX = m_toPlanet->x() - m_fromPlanet->x();
    int deltaY = m_toPlanet->y() - m_fromPlanet->y();
    float dX = deltaX;
    float dY = deltaY;
    int fromX, fromY, toX, toY;
    int intersectPoint;
    int planetLength = planetSize - 1;
    if (std::abs(deltaY) > std::abs(deltaX)) {
        if (deltaY < 0) {
            fromY = planetSize * m_fromPlanet->y() - 1;
            toY = planetSize * m_toPlanet->y() + planetSize;
            deltaY = -deltaY;
        } else {
            fromY = planetSize * m_fromPlanet->y() + planetSize;
            toY = planetSize * m_toPlanet->y() - 1;
        }
        intersectPoint = planetLength * (planetSize + planetSize * deltaX / deltaY) / 2 / planetSize;
        fromX = planetSize * m_fromPlanet->x() + intersectPoint;
        toX = planetSize * m_toPlanet->x() + planetSize - 1 - intersectPoint;
    } else {
        if (deltaX < 0) {
            fromX = planetSize * m_fromPlanet->x() - 1;
            toX = planetSize * m_toPlanet->x() + planetSize;
            deltaX = -deltaX;
        } else {
            fromX = planetSize * m_fromPlanet->x() + planetSize;
            toX = planetSize * m_toPlanet->x() - 1;
        }
        intersectPoint = planetLength * (planetSize + planetSize * deltaY / deltaX) / 2 / planetSize;
        fromY = planetSize * m_fromPlanet->y() + intersectPoint;
        toY = planetSize * m_toPlanet->y() + planetSize - 1 - intersectPoint;
    }
    qDebug() << toX << "," << toY << "->" << fromX << "," << fromY;
    painter.drawLine(toX, toY, fromX, fromY);
    // Do up arrow
    float arrowLeftX = dY - dX;
    float arrowLeftY = -dX - dY;
    float arrowRightX = -dY - dX;
    float arrowRightY = dX - dY;
    float arrowLength = (float) std::sqrt(arrowRightX * arrowRightX + arrowRightY * arrowRightY);
    arrowLeftX *= planetSize / arrowLength / 3;
    arrowLeftY *= planetSize / arrowLength / 3;
    arrowRightX *= planetSize / arrowLength / 3;
    arrowRightY *= planetSize / arrowLength / 3;
    painter.drawLine(toX, toY, (toX + (int) arrowLeftX), (toY + (int) arrowLeftY));
    painter.drawLine(toX, toY, (toX + (int) arrowRightX), (toY + (int) arrowRightY));
    painter.end();
}

void GalaxyUI::paintEvent(QPaintEvent* evt){
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(QColor(0,0,0));
    painter.fillRect(0, 0, width(), height(), brush);

    if(m_fromPlanet){
        QBrush brush(QColor(255,0,0));
        QPen pen;
        pen.setWidth(1);
        pen.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(m_fromPlanet->x(), m_fromPlanet->y(), m_fromPlanet->width(), m_fromPlanet->height() );
    }

    if(m_toPlanet){
        QBrush brush(QColor(240,240,240));
        QPen pen;
        pen.setWidth(1);
        pen.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(m_toPlanet->x(), m_toPlanet->y(), m_toPlanet->width(), m_toPlanet->height() );
    }

    if(m_fromPlanet && m_toPlanet){
//        // Draw the line between them
        QPoint from( m_fromPlanet->x() + m_fromPlanet->width() / 2,
                     m_fromPlanet->y() + m_fromPlanet->height() / 2);
        QPoint to( m_toPlanet->x() + m_toPlanet->width() / 2,
                     m_toPlanet->y() + m_toPlanet->height() / 2);

        qDebug() << from << "->" << to;

        // TODO figure out why the java code works and this doesn't
//        connectPlanets(evt);

        QBrush brush(QColor(245,245,65));
        QPen pen;
        pen.setWidth(1);
        pen.setBrush(brush);
        painter.setPen(pen);
        painter.drawLine(from, to);
    }

}

void GalaxyUI::mousePressEvent(QMouseEvent* evt){
    qDebug() << "mouse press";

    QWidget* widget = childAt(evt->pos());
    PlanetUI* planet = qobject_cast<PlanetUI*>(widget);
    if(planet){
        qDebug() << "has planet!";
        if(!m_fromPlanet){
            m_fromPlanet = planet;
        }else{
            m_toPlanet = planet;
        }

        update();
    }else{
        qDebug() << "nothing underneath";
    }
}

void GalaxyUI::setIGXGame(IGXGame* game){
    m_game = game;

    GameBoard board = m_game->board();
    for(int x = 0; x < board.size(); x++){
        for(int y = 0; y < board[0].size(); y++){
            if(m_boardUI[x][y]){
                m_boardUI[x][y]->deleteLater();
                m_boardUI[x][y] = nullptr;
            }

            std::shared_ptr<Planet> currPlanet = board[x][y];
            if(currPlanet->isNull()){
                continue;
            }

            // Create our UI element
            m_boardUI[x][y] = new PlanetUI(currPlanet, this);
        }
    }
}

void GalaxyUI::resizeEvent(QResizeEvent* evt){
    // Make sure that we are square, otherwise it looks weird.
    int size = std::min(width(), height());

    QRect geomoery = geometry();
    geomoery.setWidth(size);
    geomoery.setHeight(size);

    setGeometry(geomoery);

    calculatePlanetLocations();
    update();
}

void GalaxyUI::calculatePlanetLocations(){
    const int gridXSize = width() / 16;
    const int gridYSize = height() / 16;

    // Figure out where the planets should be based on the UI size
    for(int x = 0; x < m_boardUI.size(); x++){
        for(int y = 0; y < m_boardUI[0].size(); y++){
            PlanetUI* uiElem = m_boardUI[x][y];
            int xLocCenter = (gridXSize * x) + (gridXSize / 2);
            int yLocCenter = (gridYSize * y) + (gridYSize / 2);

            if(!uiElem){
                continue;
            }

            uiElem->setMaximumSize(gridXSize, gridYSize);
            uiElem->setMinimumSize(gridXSize, gridYSize);
            uiElem->move(xLocCenter - (gridXSize / 2), yLocCenter - (gridYSize / 2));
        }
    }
}
