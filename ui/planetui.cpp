#include "planetui.h"
#include "ui_planetui.h"
#include "planet.h"
#include "player.h"

#include <QPainter>

PlanetUI::PlanetUI(std::shared_ptr<Planet> planet, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanetUI),
    m_planet(planet)
{
    ui->setupUi(this);

    connect(planet.get(), &Planet::shipsProduced,
            this, &PlanetUI::updatePlanet);
}

PlanetUI::~PlanetUI()
{
    delete ui;
}

void PlanetUI::paintEvent(QPaintEvent* evt){
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint center(width() / 2, height() / 2);
    double radius = height() / 2 * .7;

    QBrush planet_color_brush;
    if(!m_planet->owner()){
        QRadialGradient gradient(center, radius);
        gradient.setColorAt(0, QColor(128,128,128));
        QColor outsideColor = QColor(128,128,128);
        outsideColor.setRed(outsideColor.red() - 80);
        outsideColor.setGreen(outsideColor.green() - 80);
        outsideColor.setBlue(outsideColor.blue() - 80);
        gradient.setColorAt(1, outsideColor);
        planet_color_brush = QBrush(gradient);
    }else{
        QRadialGradient gradient(center, radius);
        gradient.setColorAt(0, m_planet->owner()->color());
        QColor outsideColor = m_planet->owner()->color();
        int r = outsideColor.red() - 80;
        int g = outsideColor.green() - 80;
        int b = outsideColor.blue() - 80;
        outsideColor.setRed(r < 0 ? 0 : r);
        outsideColor.setGreen(g < 0 ? 0 : g);
        outsideColor.setBlue(b < 0 ? 0 : b);
        gradient.setColorAt(1, outsideColor);
        planet_color_brush = QBrush(gradient);
    }

    painter.setBrush(planet_color_brush);
    painter.drawEllipse(center, (int)radius, (int)radius );

    // Draw the planet name in the owner's color
    QFont planet_info_font = painter.font();
    planet_info_font.setPointSize(10);
    painter.setFont(planet_info_font);
    QBrush planet_name_brush;
    if(!m_planet->owner()){
        planet_name_brush = QBrush(QColor(0xc8, 0xc8, 0xc8));
    }else{
        planet_name_brush = QBrush(QColor(m_planet->owner()->color()));
    }
    QPen planetPen;
    planetPen.setBrush(planet_name_brush);
    painter.setPen(planetPen);
    QString name;
    name.push_back(m_planet->planetName());
    painter.drawText(0, 15, name);

    // If this planet is owned, print number of ships and number of ships produced
    if(m_planet->owner()){
        QString numShips = QString::number(m_planet->numberShips());
        QString numShipsProduced = QString::number(m_planet->numberShipsProduced());

        painter.drawText(0, 30, numShipsProduced);
        painter.drawText(15, 15, numShips);
    }

    painter.end();

    paintRatio();
}

void PlanetUI::paintRatio(){
    QPainter painter;
    if(!painter.begin(this)){
        qDebug() << "can't paint";
    }

    // Special ratio max
    const static int SPECIALRATIOMAX = 60;
    // Thresholds and colours on ratio chart
    const static int LOWRATIO = SPECIALRATIOMAX / 2;
    const static int MIDRATIO = 5 * SPECIALRATIOMAX / 6;
    const static int HIGHRATIO = SPECIALRATIOMAX;
    const static QColor HIGHCOLOUR = QColor(255, 0, 0);
    const static QColor MIDCOLOUR = QColor(255, 255, 0);
    const static QColor LOWCOLOUR = QColor(0, 255, 0);

    // The ratio is a bar on the right side of the planet.  The colors are on top of each other:
    // RED
    // YELLOW
    // GREEN
    int planetRatio = m_planet->planetRatio();

    // How high does this graph need to be?
    int graphHeightInPx = ((double)planetRatio / (double)HIGHRATIO) * height();
    int maxHeightInPx = height() - 4;
    int midRatioLocation = height() - (maxHeightInPx * ((double)MIDRATIO / (double)SPECIALRATIOMAX));
    int bottomRatioLocation = (height() - 4) * ((double)LOWRATIO / (double)SPECIALRATIOMAX);

    QPoint topLeftGraph(width() - 6, 2 + (height() - graphHeightInPx));
    QPoint bottomRightGraph(width() - 2, height() - 2);

//    qDebug() << "wid: " << width() << " height: " << height() << "TL: " << topLeftGraph << " BR: " << bottomRightGraph;

    // draw the red portion, followed by the yellow, and then the green
    painter.fillRect(topLeftGraph.x(), topLeftGraph.y(), 4, graphHeightInPx, HIGHCOLOUR);
    painter.fillRect(topLeftGraph.x(), std::max(midRatioLocation, topLeftGraph.y()), 4, graphHeightInPx, MIDCOLOUR);
    painter.fillRect(topLeftGraph.x(), std::max(bottomRatioLocation, topLeftGraph.y()), 4, graphHeightInPx, LOWCOLOUR);
}

void PlanetUI::updatePlanet(){
    update();
}

std::shared_ptr<Planet> PlanetUI::planet() const{
    return m_planet;
}
