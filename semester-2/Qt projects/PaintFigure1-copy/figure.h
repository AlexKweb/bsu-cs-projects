#ifndef FIGURE_H
#define FIGURE_H

#include <QGraphicsItem>
#include <QBrush>
#include <QObject>
#include <QPen>
#include <QTextStream> // added v2

struct FigureData {
    int type;
    QPointF start;
    QPointF end;
    QColor penColor;
    QColor brushColor;
    int penStyle; // added v2
    int brushStyle; // added v2
};

QTextStream& operator>>(QTextStream& in, FigureData &rhs); // added v2


class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF startPoint
               READ startPoint WRITE setStartPoint
               NOTIFY pointChanged)
    Q_PROPERTY(QPointF endPoint
               READ endPoint WRITE setEndPoint
               NOTIFY pointChanged)
public:
    explicit Figure(QPointF point, QObject *parent = 0);
    ~Figure();

    QPointF startPoint() const;
    QPointF endPoint() const;

    void setStartPoint(const QPointF point);
    void setEndPoint(const QPointF point);
    void setPen(const QPen &pen); // added v2
    void setBrush(const QBrush &brush); // added v2

    void setType(int type); // added v2
    int type() const; // added v2
    FigureData toData() const; // added v2
    void applyData(const FigureData &data); // added v2

    friend QTextStream& operator<<(QTextStream& out, const Figure &rhs); // added v2
    friend QTextStream& operator>>(QTextStream& in, FigureData &rhs); // added v2

protected:
    QPen pen() const; // added v2
    QBrush brush() const; // added v2

signals:
    void pointChanged();

private:
    int m_type; // added v2
    QPointF m_startPoint;
    QPointF m_endPoint;
    QPen m_pen; // added v2
    QBrush m_brush; // added v2

    QRectF boundingRect() const;

public slots:
    void updateRomb();
};

#endif // FIGURE_H
