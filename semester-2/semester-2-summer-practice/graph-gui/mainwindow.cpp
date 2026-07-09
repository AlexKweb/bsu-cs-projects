#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QStringList>
#include <QTimer>

#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    applyStyle();

    m_scene = new GraphScene(this);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadGraph);
    connect(ui->dijkstraButton, &QPushButton::clicked, this, &MainWindow::runDijkstra);
    connect(ui->animateButton, &QPushButton::clicked, this, &MainWindow::startAnimation);
    connect(ui->stopAnimationButton, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(m_scene, &GraphScene::graphWeightsChanged, this, [this]() {
        m_currentPath.clear();
        updateAnimationButtons(false);
        ui->resultLabel->setText("Веса рёбер изменены после перемещения вершины. Запустите поиск кратчайшего пути снова.");
    });
    connect(m_scene, &GraphScene::animationFinished, this, [this]() {
        updateAnimationButtons(false);
    });

    ui->startSpinBox->setRange(1, 1);
    ui->finishSpinBox->setRange(1, 1);
    ui->startSpinBox->setEnabled(false);
    ui->finishSpinBox->setEnabled(false);
    ui->dijkstraButton->setEnabled(false);
    ui->animateButton->setEnabled(false);
    ui->stopAnimationButton->setEnabled(false);

    QTimer::singleShot(0, this, [this]() {
        updateSceneSize();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    stopAnimation();
    updateSceneSize();
    redrawGraph();
}

void MainWindow::loadGraph()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          "Загрузить граф",
                                                          QString(),
                                                          "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) {
        return;
    }

    stopAnimation();
    if (!m_graph.loadFromFile(fileName)) {
        QMessageBox::warning(this, "Ошибка загрузки графа", m_graph.lastError());
        return;
    }

    m_currentPath.clear();

    const int vertexCount = m_graph.vertexCount();
    ui->startSpinBox->setRange(1, vertexCount);
    ui->finishSpinBox->setRange(1, vertexCount);
    ui->startSpinBox->setValue(1);
    ui->finishSpinBox->setValue(vertexCount > 1 ? vertexCount : 1);
    ui->startSpinBox->setEnabled(true);
    ui->finishSpinBox->setEnabled(true);
    ui->dijkstraButton->setEnabled(true);
    updateAnimationButtons(false);

    updateSceneSize();
    m_scene->drawGraph(m_graph, m_currentPath, true);

    ui->resultLabel->setText(QString("Граф загружен. Вершин: %1. Рёбер: %2.")
                                 .arg(m_graph.vertexCount())
                                 .arg(m_graph.edgeCount())
                             + "\nВеса рассчитаны автоматически по длинам рёбер.");
}

void MainWindow::runDijkstra()
{
    stopAnimation();
    if (m_graph.vertexCount() == 0) {
        ui->resultLabel->setText("Сначала загрузите граф из файла.");
        return;
    }

    const int startVertex = ui->startSpinBox->value() - 1;
    const int finishVertex = ui->finishSpinBox->value() - 1;

    QVector<int> parents;
    const QVector<int> distances = m_graph.dijkstra(startVertex, parents);

    if (finishVertex < 0 || finishVertex >= distances.size() || distances[finishVertex] == Graph::INF) {
        m_currentPath.clear();
        updateAnimationButtons(false);
        ui->resultLabel->setText(QString("Путь между вершинами %1 и %2 отсутствует.")
                                     .arg(startVertex + 1)
                                     .arg(finishVertex + 1));
        redrawGraph();
        return;
    }

    const QVector<int> path = m_graph.buildPath(startVertex, finishVertex, parents);
    if (path.isEmpty()) {
        m_currentPath.clear();
        updateAnimationButtons(false);
        ui->resultLabel->setText("Не удалось восстановить кратчайший путь.");
        redrawGraph();
        return;
    }

    m_currentPath = path;
    updateAnimationButtons(true);

    QStringList userPath;
    for (int vertex : m_currentPath) {
        userPath.append(QString::number(vertex + 1));
    }

    ui->resultLabel->setText(QString("Кратчайшее расстояние: %1\nПуть: %2")
                                 .arg(distances[finishVertex])
                                 .arg(userPath.join(" → ")));
    redrawGraph();
}

void MainWindow::startAnimation()
{
    if (m_currentPath.isEmpty()) {
        ui->resultLabel->setText("Сначала найдите кратчайший путь.");
        updateAnimationButtons(false);
        return;
    }

    m_scene->startCarAnimation(m_currentPath);
    updateAnimationButtons(m_scene->isAnimating());
}

void MainWindow::stopAnimation()
{
    if (m_scene) {
        m_scene->stopCarAnimation();
    }
    updateAnimationButtons(false);
}

void MainWindow::updateSceneSize()
{
    if (!m_scene || !ui->graphicsView->viewport()) {
        return;
    }

    const QRect viewportRect = ui->graphicsView->viewport()->rect();
    const int width = std::max(1, viewportRect.width());
    const int height = std::max(1, viewportRect.height());
    m_scene->setSceneRect(0, 0, width, height);
}

void MainWindow::redrawGraph()
{
    if (m_scene && m_graph.vertexCount() > 0) {
        m_scene->drawGraph(m_graph, m_currentPath);
    }
}

void MainWindow::updateAnimationButtons(bool isAnimating)
{
    const bool hasPath = !m_currentPath.isEmpty();
    ui->animateButton->setEnabled(hasPath && !isAnimating);
    ui->stopAnimationButton->setEnabled(isAnimating);
}

void MainWindow::applyStyle()
{
    setStyleSheet(QString());
}
