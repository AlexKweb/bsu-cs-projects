#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentColor = Qt::red;

    scene = new PaintScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::storageFilePath() const
{
    return "figures.txt";
}

void MainWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QMainWindow::resizeEvent(event);
}

void MainWindow::on_pushButton_clicked()
{
    scene->setTypeFigure(PaintScene::RombType);
}

void MainWindow::on_pushButton_2_clicked()
{
    scene->setTypeFigure(PaintScene::SquareType);
}

void MainWindow::on_pushButton_3_clicked()
{
    scene->setTypeFigure(PaintScene::TriangleType);
}

void MainWindow::on_pushButton_4_clicked()
{
    m_currentColor = Qt::red;
}

void MainWindow::on_pushButton_5_clicked()
{
    m_currentColor = Qt::green;
}

void MainWindow::on_pushButton_6_clicked()
{
    m_currentColor = Qt::blue;
}

void MainWindow::on_pushButton_7_clicked()
{
    scene->fillSelectedFigures(m_currentColor);
}

void MainWindow::on_pushButton_8_clicked()
{
    scene->saveToFile(storageFilePath());
}

void MainWindow::on_pushButton_9_clicked()
{
    scene->loadFromFile(storageFilePath());
}

void MainWindow::on_pushButton_10_clicked()
{
    scene->clearFigures();
}

void MainWindow::on_pushButton_11_clicked()
{
    scene->undoLastFigure();
}
