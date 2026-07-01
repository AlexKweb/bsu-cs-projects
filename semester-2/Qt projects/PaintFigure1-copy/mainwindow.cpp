#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication> // added v2
#include <QMessageBox> // added v2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentColor = Qt::red; // added v2

    scene = new PaintScene(); // added v2
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(); // added v2
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer); // added v2
    timer->start(100); // added v2
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::storageFilePath() const // added v2
{
    return QCoreApplication::applicationDirPath() + "/figures.txt"; // added v2
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
    m_currentColor = Qt::red; // added v2
}

void MainWindow::on_pushButton_5_clicked()
{
    m_currentColor = Qt::green; // added v2
}

void MainWindow::on_pushButton_6_clicked()
{
    m_currentColor = Qt::blue; // added v2
}

void MainWindow::on_pushButton_7_clicked()
{
    scene->fillSelectedFigures(m_currentColor); // added v2
}

void MainWindow::on_pushButton_8_clicked()
{
    if (!scene->saveToFile(storageFilePath())) { // added v2
        return;
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    if (!scene->loadFromFile(storageFilePath())) { // added v2
        return;
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    scene->clearFigures(); // added v2
}
