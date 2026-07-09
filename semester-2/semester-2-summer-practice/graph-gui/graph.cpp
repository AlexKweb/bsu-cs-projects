#include "graph.h"

#include <QFile>
#include <QTextStream>

#include <algorithm>
#include <limits>

Graph::Graph() = default;

bool Graph::loadFromFile(const QString& fileName)
{
    clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QString("Не удалось открыть файл: %1").arg(file.errorString());
        return false;
    }

    QTextStream input(&file);

    int vertexCount = 0;
    int declaredEdgeCount = 0;
    if (!(input >> vertexCount >> declaredEdgeCount)) {
        m_lastError = "Не удалось прочитать количество вершин и рёбер.";
        return false;
    }

    if (vertexCount <= 0) {
        m_lastError = "Количество вершин должно быть больше 0.";
        return false;
    }

    if (declaredEdgeCount < 0) {
        m_lastError = "Количество рёбер не должно быть отрицательным.";
        return false;
    }

    m_vertexCount = vertexCount;
    m_matrix = QVector<QVector<int>>(m_vertexCount, QVector<int>(m_vertexCount, INF));
    for (int vertex = 0; vertex < m_vertexCount; ++vertex) {
        m_matrix[vertex][vertex] = 0;
    }

    int uniqueEdgeCount = 0;
    for (int edgeIndex = 0; edgeIndex < declaredEdgeCount; ++edgeIndex) {
        int from = 0;
        int to = 0;

        if (!(input >> from >> to)) {
            const QString error = QString("Не удалось прочитать ребро номер %1.").arg(edgeIndex + 1);
            clear();
            m_lastError = error;
            return false;
        }

        if (from < 1 || from > m_vertexCount || to < 1 || to > m_vertexCount) {
            const QString error = QString("Номер вершины в ребре %1 вне диапазона 1..%2.")
                                      .arg(edgeIndex + 1)
                                      .arg(m_vertexCount);
            clear();
            m_lastError = error;
            return false;
        }

        if (from == to) {
            const QString error = QString("Петля в ребре %1 недопустима.").arg(edgeIndex + 1);
            clear();
            m_lastError = error;
            return false;
        }

        const int fromIndex = from - 1;
        const int toIndex = to - 1;
        if (m_matrix[fromIndex][toIndex] == INF) {
            ++uniqueEdgeCount;
        }

        m_matrix[fromIndex][toIndex] = 1;
        m_matrix[toIndex][fromIndex] = 1;
    }

    m_edgeCount = uniqueEdgeCount;
    m_lastError.clear();
    return true;
}

void Graph::clear()
{
    m_vertexCount = 0;
    m_edgeCount = 0;
    m_matrix.clear();
    m_lastError.clear();
}

int Graph::vertexCount() const
{
    return m_vertexCount;
}

int Graph::edgeCount() const
{
    return m_edgeCount;
}

int Graph::weight(int from, int to) const
{
    if (from < 0 || from >= m_vertexCount || to < 0 || to >= m_vertexCount) {
        return INF;
    }

    return m_matrix[from][to];
}

void Graph::setWeight(int from, int to, int newWeight)
{
    if (from < 0 || from >= m_vertexCount || to < 0 || to >= m_vertexCount || from == to) {
        return;
    }

    if (newWeight < 0 || m_matrix[from][to] == INF) {
        return;
    }

    m_matrix[from][to] = newWeight;
    m_matrix[to][from] = newWeight;
}

const QVector<QVector<int>>& Graph::matrix() const
{
    return m_matrix;
}

QVector<int> Graph::dijkstra(int startVertex, QVector<int>& parents) const
{
    QVector<int> distances(m_vertexCount, INF);
    QVector<bool> visited(m_vertexCount, false);
    parents = QVector<int>(m_vertexCount, -1);

    if (startVertex < 0 || startVertex >= m_vertexCount) {
        return distances;
    }

    distances[startVertex] = 0;

    for (int step = 0; step < m_vertexCount; ++step) {
        int currentVertex = -1;
        int bestDistance = INF;

        for (int vertex = 0; vertex < m_vertexCount; ++vertex) {
            if (!visited[vertex] && distances[vertex] < bestDistance) {
                bestDistance = distances[vertex];
                currentVertex = vertex;
            }
        }

        if (currentVertex == -1 || distances[currentVertex] == INF) {
            break;
        }

        visited[currentVertex] = true;

        for (int neighbour = 0; neighbour < m_vertexCount; ++neighbour) {
            const int edgeWeight = weight(currentVertex, neighbour);
            if (visited[neighbour] || neighbour == currentVertex || edgeWeight == INF || edgeWeight < 0) {
                continue;
            }

            if (distances[currentVertex] > INF - edgeWeight) {
                continue;
            }

            const int newDistance = distances[currentVertex] + edgeWeight;
            if (newDistance < distances[neighbour]) {
                distances[neighbour] = newDistance;
                parents[neighbour] = currentVertex;
            }
        }
    }

    return distances;
}

QVector<int> Graph::buildPath(int startVertex, int finishVertex, const QVector<int>& parents) const
{
    if (startVertex < 0 || startVertex >= m_vertexCount || finishVertex < 0 || finishVertex >= m_vertexCount) {
        return {};
    }

    if (startVertex == finishVertex) {
        return {startVertex};
    }

    if (parents.size() != m_vertexCount) {
        return {};
    }

    QVector<int> path;
    int currentVertex = finishVertex;

    for (int step = 0; step < m_vertexCount; ++step) {
        path.append(currentVertex);
        if (currentVertex == startVertex) {
            std::reverse(path.begin(), path.end());
            return path;
        }

        currentVertex = parents[currentVertex];
        if (currentVertex < 0 || currentVertex >= m_vertexCount) {
            return {};
        }
    }

    return {};
}

QString Graph::lastError() const
{
    return m_lastError;
}
