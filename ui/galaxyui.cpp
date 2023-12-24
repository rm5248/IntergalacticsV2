#include "galaxyui.h"
#include "ui_galaxyui.h"
#include "planetui.h"
#include "igxgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include <QSlider>
#include <QLayout>

static const int SLIDER_WIDTH = 20;
static const int SLIDER_HEIGHT = 60;

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

    m_moveState = FleetMoveState::NothingSelected;

    setMouseTracking(true);

    setMinimumWidth(500 + SLIDER_WIDTH);
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

//        qDebug() << from << "->" << to;

        // TODO figure out why the java code works and this doesn't
//        connectPlanets(evt);

        QBrush brush(QColor(245,245,65));
        QPen pen;
        pen.setWidth(1);
        pen.setBrush(brush);
        painter.setPen(pen);
        painter.drawLine(from, to);
    }

    painter.end();
    drawSlider();
}

void GalaxyUI::mousePressEvent(QMouseEvent* evt){
    qDebug() << "mouse press";

    if(evt->button() == Qt::RightButton){
        m_moveState = FleetMoveState::NothingSelected;
        m_fromPlanet = nullptr;
        m_toPlanet = nullptr;
        update();
        return;
    }

    if(m_moveState == FleetMoveState::PlanetsSelected){
        // Spawn a fleet and move it!

        qDebug() << "move fleet " << m_shipsToSend;
        emit doneSelectingShips();
        m_moveState = FleetMoveState::NothingSelected;
        m_fromPlanet = nullptr;
        m_toPlanet = nullptr;

        update();
        return;
    }

    QWidget* widget = childAt(evt->pos());
    PlanetUI* planet = qobject_cast<PlanetUI*>(widget);
    if(planet){
        if(!m_fromPlanet){
            if((planet->planet()->owner() == m_game->me())){
                // We can only send ships from planets that we own
                m_fromPlanet = planet;
            }
        }else if(planet != m_fromPlanet){
            m_toPlanet = planet;
            m_moveState = FleetMoveState::PlanetsSelected;
        }

        update();
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
    // Note: width needs to take into account the width of the slider
    int size = std::min(width(), height());

    QRect geomoery = geometry();
    geomoery.setWidth(size + SLIDER_WIDTH);
    geomoery.setHeight(size);

    setGeometry(geomoery);

    calculatePlanetLocations();
    update();
}

void GalaxyUI::calculatePlanetLocations(){
    const int gridXSize = (width() - SLIDER_WIDTH) / 16;
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

void GalaxyUI::drawSlider(){
    QPainter painter;
    painter.begin(this);

    // Always draw the line the slider slides on
    QBrush lineBrush(QColor(255, 255, 255));
    QPen pen;
    pen.setWidth(1);
    pen.setBrush(lineBrush);
    painter.setPen(pen);
    painter.drawLine(width() - (SLIDER_WIDTH / 2), 0,
                     width() - (SLIDER_WIDTH / 2), height());

    if(m_moveState != FleetMoveState::PlanetsSelected){
        return;
    }

    // Draw the slider depending on where the mouse is
    QPoint sliderCenterPoint(width() - (SLIDER_WIDTH / 2), m_mouseLocation.y());
    QPoint sliderTopLeft(sliderCenterPoint.x() - (SLIDER_WIDTH / 2), sliderCenterPoint.y() - (SLIDER_HEIGHT / 2));
    QPoint sliderBottomRight(sliderCenterPoint.x() + (SLIDER_WIDTH / 2), sliderCenterPoint.y() + (SLIDER_HEIGHT / 2));

    painter.fillRect(QRect(sliderTopLeft, sliderBottomRight), lineBrush);
}

void GalaxyUI::mouseMoveEvent(QMouseEvent* evt){
    m_mouseLocation = evt->pos();

    double shipsPercentage = 1.0 - ((double)m_mouseLocation.y() / (double)height());
    if(shipsPercentage > .95){
        shipsPercentage = 1.0;
    }

    if(m_moveState == FleetMoveState::PlanetsSelected){
        m_shipsToSend = m_fromPlanet->planet()->numberShips() * shipsPercentage;
        emit selectingShips(m_shipsToSend);
        update();
    }
}

void GalaxyUI::tick(){
    update();
}
