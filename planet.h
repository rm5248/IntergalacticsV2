#ifndef PLANET_H
#define PLANET_H

#include <QObject>

class Planet : public QObject
{
    Q_OBJECT
public:
    explicit Planet(QObject *parent = nullptr);
    explicit Planet(char planetName, int numProduced, int numShips, int planetRatio, QObject *parent = nullptr);

    char planetName() const;
    int numberShipsProduced() const;
    int numberShips() const;
    int planetRatio() const;
    bool isNull() const;

signals:

private:
    char m_planetName = 0;
    int m_numberShipsProduced = 0;
    int m_timeNextShipsProduced; // TBD
    int m_numberShips = 0;
    int m_planetRatio = 0;
    bool m_isNull = true;
};

#endif // PLANET_H
