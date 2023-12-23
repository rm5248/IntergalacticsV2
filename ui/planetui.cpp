#include "planetui.h"
#include "ui_planetui.h"
#include "planet.h"

#include <QPainter>

PlanetUI::PlanetUI(std::shared_ptr<Planet> planet, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanetUI),
    m_planet(planet)
{
    ui->setupUi(this);
}

PlanetUI::~PlanetUI()
{
    delete ui;
}

void PlanetUI::paintEvent(QPaintEvent* evt){
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.translate(width() / 2, height() / 2);

    QBrush brush(QColor(128,128,128));

    QPoint center(width() / 2, height() / 2);

    painter.setBrush(brush);
    painter.drawEllipse(center, (int)(width() / 2 * .66), (int)(height() / 2 * .66) );

    QBrush brush2(QColor(0, 255, 255));
    QPen planetPen;
    planetPen.setBrush(brush2);
    painter.setPen(planetPen);
    QString name;
    name.push_back(m_planet->planetName());
    painter.drawText(0, 15, name);

    QString numShips = QString::number(m_planet->numberShips());
    QString numShipsProduced = QString::number(m_planet->numberShipsProduced());

    painter.drawText(0, 30, numShipsProduced);
    painter.drawText(15, 15, numShips);

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
