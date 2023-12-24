#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class IGXGame;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setIGXGame(IGXGame* game);

private slots:
    void selectingShips(int numShips);
    void doneSelectingShips();

private:
    Ui::MainWindow *ui;
    IGXGame* m_game;
};
#endif // MAINWINDOW_H
