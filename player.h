#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QColor>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    void setColor(QColor c);
    QColor color();

signals:
private:
    QString m_playerName;
    QColor m_color;

};

#endif // PLAYER_H
