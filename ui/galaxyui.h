#ifndef GALAXYUI_H
#define GALAXYUI_H

#include <QWidget>

class PlanetUI;
class IGXGame;

namespace Ui {
class GalaxyUI;
}

typedef std::array<std::array<PlanetUI*, 16>, 16> GameBoardUI;

class GalaxyUI : public QWidget
{
    Q_OBJECT

public:
    explicit GalaxyUI(QWidget *parent = nullptr);
    ~GalaxyUI();

    void setIGXGame(IGXGame* game);

    void paintEvent(QPaintEvent* evt);
    void mousePressEvent(QMouseEvent* evt);
    void resizeEvent(QResizeEvent* evt);

private:
    void connectPlanets(QPaintEvent* evt);
    void calculatePlanetLocations();

private:
    Ui::GalaxyUI *ui;
    PlanetUI* m_fromPlanet = nullptr;
    PlanetUI* m_toPlanet = nullptr;
    IGXGame* m_game;
    GameBoardUI m_boardUI;
};

#endif // GALAXYUI_H
