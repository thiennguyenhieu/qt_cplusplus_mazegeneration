#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widgetMaze->generateMazeData();
    ui->cmbGenType->insertItem(0, "DFS", MazeHandling::GENMAZE_DFS);
    ui->cmbSolveType->insertItem(0, "DFS", MazeHandling::SOLVEMAZE_DFS);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGenMaze_clicked()
{
    ui->widgetMaze->generateMazeData();
}

void MainWindow::on_btnSolve_clicked()
{
    ui->widgetMaze->solveMazeData();
}

void MainWindow::on_cmbGenType_currentIndexChanged(int index)
{
    (void)index;
    ui->widgetMaze->setGenMazeType(static_cast<MazeHandling::eGENMAZE_TYPE>(ui->cmbGenType->currentData().toInt()));
}

void MainWindow::on_cmbSolveType_currentIndexChanged(int index)
{
    (void)index;
    ui->widgetMaze->setSolveMazeType(static_cast<MazeHandling::eSOLVEMAZE_TYPE>(ui->cmbSolveType->currentData().toInt()));
}
