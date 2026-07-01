#include "figure.h"
#include <QPainter>

Figure::Figure(QPointF point, QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    m_type = 0;
    this->setStartPoint(mapFromScene(point));
    this->setEndPoint(mapFromScene(point));
    m_pen = QPen(Qt::white, 2);
    m_color = Qt::transparent;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    connect(this, &Figure::pointChanged, this, &Figure::updateRomb);
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
    m_pen = pen;
    update();
}

void Figure::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QPen Figure::pen() const
{
    return m_pen;
}

QColor Figure::getColor() const
{
    return m_color;
}

void Figure::setType(int type)
{
    m_type = type;
}

int Figure::type() const
{
    return m_type;
}

FigureData Figure::toData() const
{
    FigureData data;
    data.type = m_type;
    data.start = mapToScene(m_startPoint);
    data.end = mapToScene(m_endPoint);
    data.penColor = m_pen.color();
    data.fillColor = m_color;
    data.penStyle = static_cast<int>(m_pen.style());
    return data;
}

void Figure::applyData(const FigureData &data)
{
    setType(data.type);
    setStartPoint(data.start);
    setEndPoint(data.end);

    QPen loadedPen(data.penColor, 2);
    loadedPen.setStyle(static_cast<Qt::PenStyle>(data.penStyle));
    setPen(loadedPen);
    setColor(data.fillColor);
}

QTextStream& operator<<(QTextStream& out, const Figure &rhs)
{
    const FigureData data = rhs.toData();
    out << data.type << ' '
        << data.start.x() << ' ' << data.start.y() << ' '
        << data.end.x() << ' ' << data.end.y() << ' '
        << data.penColor.name() << ' '
        << data.penStyle << ' '
        << data.fillColor.rgba() << '\n';
    return out;
}

QTextStream& operator>>(QTextStream& in, FigureData &rhs)
{
    QString penColorName;
    QString fillColorName;

    in >> rhs.type
       >> rhs.start.rx() >> rhs.start.ry()
       >> rhs.end.rx() >> rhs.end.ry()
       >> penColorName
       >> rhs.penStyle
       >> fillColorName;

    bool ok = false;
    const QRgb fillRgba = fillColorName.toUInt(&ok);

    rhs.penColor = QColor(penColorName);
    rhs.fillColor = ok ? QColor::fromRgba(fillRgba) : QColor(fillColorName);
    return in;
}
