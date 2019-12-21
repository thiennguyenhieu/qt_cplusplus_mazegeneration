#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widgetMaze->generateMazeData();
    ui->widgetMaze->setParent(this);
    ui->cmbSolveType->insertItem(0, "DFS", MazeHandling::SOLVEMAZE_DFS);
    ui->cmbSolveType->insertItem(1, "BFS", MazeHandling::SOLVEMAZE_BFS);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateExecuteTime(int val)
{
    QString strExecuteTime = QString::number(val) + " microsecs";
    ui->lblTimeVal->setText(strExecuteTime);
}

void MainWindow::on_btnGenMaze_clicked()
{
    ui->widgetMaze->generateMazeData();
    ui->lblTimeVal->setText("_ _ _");
}

void MainWindow::on_btnSolve_clicked()
{
    ui->widgetMaze->solveMazeData();
}

void MainWindow::on_cmbSolveType_currentIndexChanged(int index)
{
    (void)index;
    ui->widgetMaze->setSolveMazeType(static_cast<MazeHandling::eSOLVEMAZE_TYPE>(ui->cmbSolveType->currentData().toInt()));
}
