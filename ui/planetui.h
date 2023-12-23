#ifndef PLANETUI_H
#define PLANETUI_H

#include <QWidget>
#include <memory>

class Planet;

namespace Ui {
class PlanetUI;
}

class PlanetUI : public QWidget
{
    Q_OBJECT

public:
    explicit PlanetUI(std::shared_ptr<Planet> planet, QWidget *parent = nullptr);
    ~PlanetUI();

    void paintEvent(QPaintEvent* evt);

private:
    void paintRatio();

private:
    Ui::PlanetUI *ui;
    std::shared_ptr<Planet> m_planet;
};

#endif // PLANETUI_H
