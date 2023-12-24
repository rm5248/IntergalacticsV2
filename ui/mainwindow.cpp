#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "igxgame.h"
#include "galaxyui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->galaxyUI, &GalaxyUI::selectingShips,
            this, &MainWindow::selectingShips);
    connect(ui->galaxyUI, &GalaxyUI::doneSelectingShips,
            this, &MainWindow::doneSelectingShips);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIGXGame(IGXGame* game){
    m_game = game;
    ui->galaxyUI->setIGXGame(m_game);
}

void MainWindow::selectingShips(int numShips){
    QString str = QString("Ships: %1").arg(numShips);
    ui->shipSelectionLabel->setText(str);
}

void MainWindow::doneSelectingShips(){
    ui->shipSelectionLabel->clear();
}
