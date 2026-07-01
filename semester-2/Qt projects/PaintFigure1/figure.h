#ifndef FIGURE_H
#define FIGURE_H

#include <QGraphicsItem>
#include <QObject>
#include <QPen>
#include <QTextStream>

struct FigureData {
    int type;
    QPointF start;
    QPointF end;
    QColor penColor;
    QColor fillColor;
    int penStyle;
};

QTextStream& operator>>(QTextStream& in, FigureData &rhs);


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
    void setPen(const QPen &pen);
    void setColor(const QColor &color);

    void setType(int type);
    int type() const;
    FigureData toData() const;
    void applyData(const FigureData &data);

    friend QTextStream& operator<<(QTextStream& out, const Figure &rhs);
    friend QTextStream& operator>>(QTextStream& in, FigureData &rhs);

protected:
    QPen pen() const;
    QColor getColor() const;

signals:
    void pointChanged();

private:
    int m_type;
    QPointF m_startPoint;
    QPointF m_endPoint;
    QPen m_pen;
    QColor m_color;

    QRectF boundingRect() const;

public slots:
    void updateRomb();
};

#endif
