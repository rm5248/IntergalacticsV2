#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "igxgame.h"
#include "galaxyui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIGXGame(IGXGame* game){
    m_game = game;
    ui->galaxyUI->setIGXGame(m_game);
}
