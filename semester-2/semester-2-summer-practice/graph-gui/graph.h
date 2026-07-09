#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QVector>

class Graph
{
public:
    static constexpr int INF = 1000000000;

    Graph();

    bool loadFromFile(const QString& fileName);
    void clear();

    int vertexCount() const;
    int edgeCount() const;
    int weight(int from, int to) const;
    void setWeight(int from, int to, int newWeight);

    const QVector<QVector<int>>& matrix() const;

    QVector<int> dijkstra(int startVertex, QVector<int>& parents) const;
    QVector<int> buildPath(int startVertex, int finishVertex, const QVector<int>& parents) const;

    QString lastError() const;

private:
    int m_vertexCount = 0;
    int m_edgeCount = 0;
    QVector<QVector<int>> m_matrix;
    QString m_lastError;
};

#endif // GRAPH_H
