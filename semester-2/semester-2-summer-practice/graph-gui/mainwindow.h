#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph.h"
#include "graphscene.h"

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QResizeEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void loadGraph();
    void runDijkstra();
    void startAnimation();
    void stopAnimation();

private:
    Ui::MainWindow *ui;
    Graph m_graph;
    GraphScene* m_scene = nullptr;
    QVector<int> m_currentPath;

    void updateSceneSize();
    void redrawGraph();
    void updateAnimationButtons(bool isAnimating);
    void applyStyle();
};
#endif // MAINWINDOW_H
