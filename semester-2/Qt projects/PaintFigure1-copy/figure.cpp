#include "figure.h"
#include <QPainter>

Figure::Figure(QPointF point, QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    m_type = 0; // added v2
    this->setStartPoint(mapFromScene(point));
    this->setEndPoint(mapFromScene(point));
    m_pen = QPen(Qt::white, 2); // added v2
    m_brush = QBrush(Qt::NoBrush); // added v2
    setFlag(QGraphicsItem::ItemIsSelectable, true); // added v2
    connect(this, &Figure::pointChanged, this, &Figure::updateRomb); // added v2
}

Figure::~Figure()
{

}

QRectF Figure::boundingRect() const
{
    return QRectF((endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x()) - 5,
                  (endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y()) - 5,
                  qAbs(endPoint().x() - startPoint().x()) + 10,
                  qAbs(endPoint().y() - startPoint().y()) + 10);
}

void Figure::updateRomb()
{
    this->update((endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x()) - 5,
                 (endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y()) - 5,
                 qAbs(endPoint().x() - startPoint().x()) + 10,
                 qAbs(endPoint().y() - startPoint().y()) + 10);
}

void Figure::setStartPoint(const QPointF point)
{
    m_startPoint = mapFromScene(point);
    emit pointChanged();
}

void Figure::setEndPoint(const QPointF point)
{
    m_endPoint = mapFromScene(point);
    emit pointChanged();
}

QPointF Figure::startPoint() const
{
    return m_startPoint;
}

QPointF Figure::endPoint() const
{
    return m_endPoint;
}

void Figure::setPen(const QPen &pen)
{
    m_pen = pen; // added v2
    update(); // added v2
}

void Figure::setBrush(const QBrush &brush)
{
    m_brush = brush; // added v2
    update(); // added v2
}

QPen Figure::pen() const
{
    return m_pen; // added v2
}

QBrush Figure::brush() const
{
    return m_brush; // added v2
}

void Figure::setType(int type) // added v2
{
    m_type = type;
}

int Figure::type() const // added v2
{
    return m_type;
}

FigureData Figure::toData() const // added v2
{
    FigureData data; // added v2
    data.type = m_type; // added v2
    data.start = mapToScene(m_startPoint); // added v2
    data.end = mapToScene(m_endPoint); // added v2
    data.penColor = m_pen.color(); // added v2
    data.brushColor = m_brush.color(); // added v2
    data.penStyle = static_cast<int>(m_pen.style()); // added v2
    data.brushStyle = static_cast<int>(m_brush.style()); // added v2
    return data;
}

void Figure::applyData(const FigureData &data) // added v2
{
    setType(data.type); // added v2
    setStartPoint(data.start); // added v2
    setEndPoint(data.end); // added v2

    QPen loadedPen(data.penColor, 2); // added v2
    loadedPen.setStyle(static_cast<Qt::PenStyle>(data.penStyle)); // added v2
    setPen(loadedPen); // added v2

    QBrush loadedBrush(data.brushColor); // added v2
    loadedBrush.setStyle(static_cast<Qt::BrushStyle>(data.brushStyle)); // added v2
    setBrush(loadedBrush); // added v2
}

QTextStream& operator<<(QTextStream& out, const Figure &rhs) // added v2
{
    const FigureData data = rhs.toData(); // added v2
    out << data.type << ' ' // added v2
        << data.start.x() << ' ' << data.start.y() << ' ' // added v2
        << data.end.x() << ' ' << data.end.y() << ' ' // added v2
        << data.penColor.name() << ' ' // added v2
        << data.penStyle << ' ' // added v2
        << data.brushColor.name() << ' ' // added v2
        << data.brushStyle << '\n'; // added v2
    return out;
}

QTextStream& operator>>(QTextStream& in, FigureData &rhs) // added v2
{
    QString penColorName; // added v2
    QString brushColorName; // added v2

    in >> rhs.type // added v2
       >> rhs.start.rx() >> rhs.start.ry() // added v2
       >> rhs.end.rx() >> rhs.end.ry() // added v2
       >> penColorName // added v2
       >> rhs.penStyle // added v2
       >> brushColorName // added v2
       >> rhs.brushStyle; // added v2

    rhs.penColor = QColor(penColorName); // added v2
    rhs.brushColor = QColor(brushColorName); // added v2
    return in;
}
