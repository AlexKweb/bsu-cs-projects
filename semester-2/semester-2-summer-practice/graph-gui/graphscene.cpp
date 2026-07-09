#include "graphscene.h"

#include <QBrush>
#include <QColor>
#include <QCursor>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QLineF>
#include <QPen>
#include <QTimer>

#include <algorithm>
#include <cmath>

namespace {
constexpr qreal PI = 3.14159265358979323846;
constexpr qreal VertexRadius = 24.0;
constexpr qreal SceneMargin = 70.0;
constexpr qreal WeightPixels = 10.0;
constexpr qreal CarSpeedPixels = 6.0;
const QColor SceneBackgroundColor("#d6d6d6");
const QColor EdgeColor("#202020");
const QColor PathColor("#d00000");
const QColor VertexBorderColor("#202020");
const QColor VertexFillColor("#f5f5f5");
const QColor PathVertexFillColor("#ffd84d");
const QColor TextColor("#111111");
const QColor WeightTextColor("#003f8f");
const QColor CarBodyColor("#2070d0");
const QColor CarWindowColor("#f0b000");
const QColor CarWheelColor("#05070a");
}

GraphScene::GraphScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(SceneBackgroundColor);
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(16);
    connect(m_animationTimer, &QTimer::timeout, this, &GraphScene::advanceCarAnimation);
}

void GraphScene::drawGraph(Graph& graph, const QVector<int>& highlightedPath, bool resetLayout)
{
    const bool keepPositions = !resetLayout
                               && m_userMovedVertices
                               && m_graph == &graph
                               && m_positions.size() == graph.vertexCount();
    stopCarAnimation();
    clear();
    m_graph = &graph;
    m_highlightedPath = highlightedPath;
    m_edges.clear();
    m_edgeLines.clear();
    m_edgeTexts.clear();
    m_vertexItems.clear();
    m_vertexTexts.clear();
    m_carBody = nullptr;
    m_frontWheel = nullptr;
    m_backWheel = nullptr;
    m_draggedVertex = -1;
    if (resetLayout) {
        m_userMovedVertices = false;
    }

    const int vertexCount = graph.vertexCount();
    if (vertexCount <= 0) {
        return;
    }

    if (!keepPositions) {
        m_positions = calculateVertexPositions(graph);
    }

    updateAllWeightsFromGeometry();
    createGraphItems();
    updateGraphItems();
}

void GraphScene::startCarAnimation(const QVector<int>& path)
{
    stopCarAnimation();

    if (!m_graph || path.isEmpty()) {
        return;
    }

    m_animationPath = path;
    m_animationSegment = 0;
    m_animationProgress = 0.0;
    createCarItems();
    updateCarPosition();

    if (m_animationPath.size() > 1) {
        m_animationTimer->start();
    } else {
        emit animationFinished();
    }
}

void GraphScene::stopCarAnimation()
{
    if (m_animationTimer) {
        m_animationTimer->stop();
    }

    m_animationPath.clear();
    m_animationSegment = 0;
    m_animationProgress = 0.0;
    removeCarItems();
}

bool GraphScene::isAnimating() const
{
    return m_animationTimer && m_animationTimer->isActive();
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    const int vertex = vertexAt(event->scenePos());
    if (vertex >= 0) {
        stopCarAnimation();
        m_draggedVertex = vertex;
        m_dragOffset = m_positions[vertex] - event->scenePos();
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_draggedVertex >= 0 && m_draggedVertex < m_positions.size()) {
        m_positions[m_draggedVertex] = limitedVertexPosition(event->scenePos() + m_dragOffset);
        m_userMovedVertices = true;
        m_highlightedPath.clear();
        updateIncidentWeights(m_draggedVertex);
        updateGraphItems();
        event->accept();
        return;
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_draggedVertex >= 0) {
        m_draggedVertex = -1;
        emit graphWeightsChanged();
        event->accept();
        return;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphScene::createGraphItems()
{
    if (!m_graph) {
        return;
    }

    const int vertexCount = m_graph->vertexCount();

    // Рёбра создаются первыми, чтобы вершины перекрывали концы линий.
    for (int first = 0; first < vertexCount; ++first) {
        for (int second = first + 1; second < vertexCount; ++second) {
            const int edgeWeight = m_graph->weight(first, second);
            if (edgeWeight == Graph::INF) {
                continue;
            }

            const bool inPath = edgeBelongsToPath(first, second, m_highlightedPath);
            const QPen edgePen(inPath ? PathColor : EdgeColor, inPath ? 5 : 2);
            QGraphicsLineItem* edgeItem = addLine(QLineF(), edgePen);
            edgeItem->setZValue(inPath ? 1.0 : 0.0);

            QGraphicsTextItem* weightText = addText(QString::number(edgeWeight), weightFont());
            weightText->setDefaultTextColor(WeightTextColor);
            weightText->setZValue(3.0);

            m_edges.append(qMakePair(first, second));
            m_edgeLines.append(edgeItem);
            m_edgeTexts.append(weightText);
        }
    }

    for (int vertex = 0; vertex < vertexCount; ++vertex) {
        const bool inPath = m_highlightedPath.contains(vertex);
        const QPen vertexPen(inPath ? PathColor : VertexBorderColor, inPath ? 4 : 2);
        const QBrush vertexBrush(inPath ? PathVertexFillColor : VertexFillColor);
        QGraphicsEllipseItem* vertexItem = addEllipse(QRectF(), vertexPen, vertexBrush);
        vertexItem->setZValue(4.0);
        vertexItem->setCursor(QCursor(Qt::OpenHandCursor));

        QGraphicsTextItem* numberText = addText(QString::number(vertex + 1), vertexFont());
        numberText->setDefaultTextColor(TextColor);
        numberText->setZValue(5.0);

        m_vertexItems.append(vertexItem);
        m_vertexTexts.append(numberText);
    }
}

void GraphScene::updateGraphItems()
{
    if (!m_graph) {
        return;
    }

    for (int edgeIndex = 0; edgeIndex < m_edges.size(); ++edgeIndex) {
        const int first = m_edges[edgeIndex].first;
        const int second = m_edges[edgeIndex].second;

        QPointF startPoint = m_positions[first];
        QPointF endPoint = m_positions[second];
        QLineF centerLine(startPoint, endPoint);
        if (centerLine.length() > 0.0) {
            const qreal dx = centerLine.dx() / centerLine.length();
            const qreal dy = centerLine.dy() / centerLine.length();
            const qreal radius = vertexRadius();
            startPoint += QPointF(dx * radius, dy * radius);
            endPoint -= QPointF(dx * radius, dy * radius);
        }

        const bool inPath = edgeBelongsToPath(first, second, m_highlightedPath);
        m_edgeLines[edgeIndex]->setLine(QLineF(startPoint, endPoint));
        m_edgeLines[edgeIndex]->setPen(QPen(inPath ? PathColor : EdgeColor, inPath ? 5 : 2));
        m_edgeLines[edgeIndex]->setZValue(inPath ? 1.0 : 0.0);

        QGraphicsTextItem* weightText = m_edgeTexts[edgeIndex];
        weightText->setPlainText(QString::number(m_graph->weight(first, second)));
        const QRectF textRect = weightText->boundingRect();
        const QPointF middle = (startPoint + endPoint) / 2.0;
        weightText->setPos(middle.x() - textRect.width() / 2.0,
                           middle.y() - textRect.height() / 2.0);
    }

    for (int vertex = 0; vertex < m_positions.size(); ++vertex) {
        const bool inPath = m_highlightedPath.contains(vertex);
        const QPointF center = m_positions[vertex];
        const qreal radius = vertexRadius();
        const QRectF ellipseRect(center.x() - radius,
                                 center.y() - radius,
                                 radius * 2.0,
                                 radius * 2.0);

        m_vertexItems[vertex]->setRect(ellipseRect);
        m_vertexItems[vertex]->setPen(QPen(inPath ? PathColor : VertexBorderColor, inPath ? 4 : 2));
        m_vertexItems[vertex]->setBrush(QBrush(inPath ? PathVertexFillColor : VertexFillColor));

        const QRectF textRect = m_vertexTexts[vertex]->boundingRect();
        m_vertexTexts[vertex]->setPos(center.x() - textRect.width() / 2.0,
                                      center.y() - textRect.height() / 2.0);
    }
}

void GraphScene::updateIncidentWeights(int vertex)
{
    if (!m_graph) {
        return;
    }

    for (const QPair<int, int>& edge : m_edges) {
        if (edge.first != vertex && edge.second != vertex) {
            continue;
        }

        const qreal distance = QLineF(m_positions[edge.first], m_positions[edge.second]).length();
        m_graph->setWeight(edge.first, edge.second, weightFromDistance(distance));
    }
}

void GraphScene::updateAllWeightsFromGeometry()
{
    if (!m_graph) {
        return;
    }

    const int vertexCount = m_graph->vertexCount();
    if (m_positions.size() != vertexCount) {
        return;
    }

    for (int first = 0; first < vertexCount; ++first) {
        for (int second = first + 1; second < vertexCount; ++second) {
            if (m_graph->weight(first, second) == Graph::INF) {
                continue;
            }

            const qreal distance = QLineF(m_positions[first], m_positions[second]).length();
            m_graph->setWeight(first, second, weightFromDistance(distance));
        }
    }
}

void GraphScene::createCarItems()
{
    removeCarItems();

    m_carBody = addRect(-14, -8, 28, 16, QPen(Qt::NoPen), QBrush(CarBodyColor));
    m_carBody->setZValue(20.0);
    m_carBody->setTransformOriginPoint(0, 0);

    QGraphicsRectItem* window = new QGraphicsRectItem(-4, -12, 9, 7, m_carBody);
    window->setPen(QPen(Qt::NoPen));
    window->setBrush(QBrush(CarWindowColor));

    m_frontWheel = new QGraphicsEllipseItem(5, 5, 8, 8, m_carBody);
    m_frontWheel->setPen(QPen(Qt::NoPen));
    m_frontWheel->setBrush(QBrush(CarWheelColor));

    m_backWheel = new QGraphicsEllipseItem(-13, 5, 8, 8, m_carBody);
    m_backWheel->setPen(QPen(Qt::NoPen));
    m_backWheel->setBrush(QBrush(CarWheelColor));
}

void GraphScene::updateCarPosition()
{
    if (!m_carBody || m_animationPath.isEmpty() || m_animationPath.first() < 0
        || m_animationPath.first() >= m_positions.size()) {
        return;
    }

    if (m_animationPath.size() == 1 || m_animationSegment >= m_animationPath.size() - 1) {
        const int lastVertex = m_animationPath.last();
        if (lastVertex >= 0 && lastVertex < m_positions.size()) {
            m_carBody->setPos(m_positions[lastVertex]);
        }
        m_carBody->setRotation(0);
        return;
    }

    const int from = m_animationPath[m_animationSegment];
    const int to = m_animationPath[m_animationSegment + 1];
    if (from < 0 || from >= m_positions.size() || to < 0 || to >= m_positions.size()) {
        return;
    }

    const QPointF start = m_positions[from];
    const QPointF finish = m_positions[to];
    const QPointF delta = finish - start;
    const qreal length = std::hypot(delta.x(), delta.y());
    if (length <= 0.0) {
        m_carBody->setPos(finish);
        return;
    }

    const qreal ratio = std::clamp(m_animationProgress / length, 0.0, 1.0);
    const QPointF position = start + delta * ratio;
    const qreal angleDegrees = std::atan2(delta.y(), delta.x()) * 180.0 / PI;

    m_carBody->setPos(position);
    m_carBody->setRotation(angleDegrees);
}

void GraphScene::advanceCarAnimation()
{
    if (m_animationPath.size() < 2 || m_animationSegment >= m_animationPath.size() - 1) {
        if (m_animationTimer) {
            m_animationTimer->stop();
        }
        emit animationFinished();
        return;
    }

    const int from = m_animationPath[m_animationSegment];
    const int to = m_animationPath[m_animationSegment + 1];
    if (from < 0 || from >= m_positions.size() || to < 0 || to >= m_positions.size()) {
        stopCarAnimation();
        emit animationFinished();
        return;
    }

    const qreal segmentLength = QLineF(m_positions[from], m_positions[to]).length();
    if (segmentLength <= 0.0) {
        ++m_animationSegment;
        m_animationProgress = 0.0;
        updateCarPosition();
        return;
    }

    m_animationProgress += CarSpeedPixels;

    if (m_animationProgress >= segmentLength) {
        ++m_animationSegment;
        m_animationProgress = 0.0;

        if (m_animationSegment >= m_animationPath.size() - 1) {
            updateCarPosition();
            if (m_animationTimer) {
                m_animationTimer->stop();
            }
            emit animationFinished();
            return;
        }
    }

    updateCarPosition();
}

void GraphScene::removeCarItems()
{
    if (m_carBody) {
        removeItem(m_carBody);
        delete m_carBody;
    }

    m_carBody = nullptr;
    m_frontWheel = nullptr;
    m_backWheel = nullptr;
}

int GraphScene::weightFromDistance(qreal distance) const
{
    return std::max(1, int(std::round(distance / WeightPixels)));
}

qreal GraphScene::vertexRadius() const
{
    if (!m_graph) {
        return VertexRadius;
    }

    if (m_graph->vertexCount() >= 35) {
        return 13.0;
    }

    if (m_graph->vertexCount() >= 25) {
        return 16.0;
    }

    if (m_graph->vertexCount() >= 16) {
        return 19.0;
    }

    return VertexRadius;
}

QFont GraphScene::vertexFont() const
{
    const int count = m_graph ? m_graph->vertexCount() : 0;
    if (count >= 35) {
        return QFont("Arial", 8, QFont::Bold);
    }

    if (count >= 25) {
        return QFont("Arial", 9, QFont::Bold);
    }

    return QFont("Arial", 12, QFont::Bold);
}

QFont GraphScene::weightFont() const
{
    const int count = m_graph ? m_graph->vertexCount() : 0;
    if (count >= 35) {
        return QFont("Arial", 7, QFont::Bold);
    }

    if (count >= 25) {
        return QFont("Arial", 8, QFont::Bold);
    }

    return QFont("Arial", 11, QFont::Bold);
}

int GraphScene::vertexAt(const QPointF& scenePosition) const
{
    const qreal radius = vertexRadius();
    for (int vertex = m_positions.size() - 1; vertex >= 0; --vertex) {
        if (QLineF(scenePosition, m_positions[vertex]).length() <= radius) {
            return vertex;
        }
    }

    return -1;
}

QPointF GraphScene::limitedVertexPosition(const QPointF& position) const
{
    const QRectF currentRect = sceneRect();
    const qreal minX = currentRect.left() + SceneMargin;
    const qreal maxX = currentRect.right() - SceneMargin;
    const qreal minY = currentRect.top() + SceneMargin;
    const qreal maxY = currentRect.bottom() - SceneMargin;

    return QPointF(std::clamp(position.x(), minX, maxX),
                   std::clamp(position.y(), minY, maxY));
}

QVector<QPointF> GraphScene::calculateVertexPositions(const Graph& graph) const
{
    const int vertexCount = graph.vertexCount();
    QVector<QPointF> positions;
    positions.reserve(vertexCount);

    const QRectF currentRect = sceneRect();
    const QPointF center = currentRect.center();
    const qreal minimalSize = std::min(currentRect.width(), currentRect.height());
    const qreal radius = std::max<qreal>(0.0, minimalSize / 2.0 - SceneMargin);

    if (vertexCount == 1) {
        positions.append(center);
        return positions;
    }

    for (int vertex = 0; vertex < vertexCount; ++vertex) {
        const qreal angle = -PI / 2.0 + 2.0 * PI * vertex / vertexCount;
        const qreal x = center.x() + radius * std::cos(angle);
        const qreal y = center.y() + radius * std::sin(angle);
        positions.append(QPointF(x, y));
    }

    return positions;
}

bool GraphScene::edgeBelongsToPath(int firstVertex, int secondVertex, const QVector<int>& path) const
{
    for (int index = 0; index + 1 < path.size(); ++index) {
        const int pathFirst = path[index];
        const int pathSecond = path[index + 1];
        if ((pathFirst == firstVertex && pathSecond == secondVertex)
            || (pathFirst == secondVertex && pathSecond == firstVertex)) {
            return true;
        }
    }

    return false;
}
