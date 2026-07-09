#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include <QVector>

#include "graph.h"

class QGraphicsEllipseItem;
class QGraphicsLineItem;
class QGraphicsRectItem;
class QGraphicsSceneMouseEvent;
class QGraphicsTextItem;
class QTimer;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphScene(QObject* parent = nullptr);

    void drawGraph(Graph& graph, const QVector<int>& highlightedPath = {}, bool resetLayout = false);
    void startCarAnimation(const QVector<int>& path);
    void stopCarAnimation();
    bool isAnimating() const;

signals:
    void graphWeightsChanged();
    void animationFinished();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QVector<QPointF> calculateVertexPositions(const Graph& graph) const;
    void createGraphItems();
    void updateGraphItems();
    void updateIncidentWeights(int vertex);
    void updateAllWeightsFromGeometry();
    void createCarItems();
    void updateCarPosition();
    void advanceCarAnimation();
    void removeCarItems();
    int weightFromDistance(qreal distance) const;
    qreal vertexRadius() const;
    QFont vertexFont() const;
    QFont weightFont() const;
    int vertexAt(const QPointF& scenePosition) const;
    QPointF limitedVertexPosition(const QPointF& position) const;
    bool edgeBelongsToPath(int firstVertex, int secondVertex, const QVector<int>& path) const;

    Graph* m_graph = nullptr;
    QVector<int> m_highlightedPath;
    QVector<QPointF> m_positions;
    QVector<QPair<int, int>> m_edges;
    QVector<QGraphicsLineItem*> m_edgeLines;
    QVector<QGraphicsTextItem*> m_edgeTexts;
    QVector<QGraphicsEllipseItem*> m_vertexItems;
    QVector<QGraphicsTextItem*> m_vertexTexts;
    QTimer* m_animationTimer = nullptr;
    QGraphicsRectItem* m_carBody = nullptr;
    QGraphicsEllipseItem* m_frontWheel = nullptr;
    QGraphicsEllipseItem* m_backWheel = nullptr;
    QVector<int> m_animationPath;
    int m_animationSegment = 0;
    qreal m_animationProgress = 0.0;
    int m_draggedVertex = -1;
    QPointF m_dragOffset;
    bool m_userMovedVertices = false;
};

#endif // GRAPHSCENE_H
