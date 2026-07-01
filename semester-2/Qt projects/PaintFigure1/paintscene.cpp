#include "paintscene.h"
#include <QFile>
#include <QTextStream>
#include "romb.h"
#include "triangle.h"
#include "square.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent)
{
    tempFigure = nullptr;
    m_typeFigure = SquareType;
}

PaintScene::~PaintScene()
{

}

int PaintScene::typeFigure() const
{
    return m_typeFigure;
}

void PaintScene::setTypeFigure(const int type)
{
    m_typeFigure = type;
}

void PaintScene::fillSelectedFigures(const QColor &color)
{
    const QList<Figure *> figures = selectedFigures();
    for (Figure *figure : figures) {
        figure->setColor(color);
    }
}

void PaintScene::clearFigures()
{
    tempFigure = nullptr;
    clear();
}

void PaintScene::undoLastFigure()
{
    const QList<Figure *> allFigures = figures();
    if (allFigures.isEmpty()) {
        return;
    }

    removeItem(allFigures.last());
    delete allFigures.last();
}

bool PaintScene::saveToFile(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    const QList<Figure *> allFigures = figures();
    out << allFigures.size() << '\n';

    for (const Figure *figure : allFigures) {
        out << *figure;
    }

    return out.status() == QTextStream::Ok;
}

bool PaintScene::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    int count = 0;
    in >> count;
    if (in.status() != QTextStream::Ok) {
        return false;
    }

    clearFigures();

    for (int i = 0; i < count; ++i) {
        FigureData data;
        in >> data;
        if (in.status() != QTextStream::Ok) {
            clearFigures();
            return false;
        }

        Figure *figure = createFigure(data.type, data.start);
        if (!figure) {
            clearFigures();
            return false;
        }

        figure->applyData(data);
        addItem(figure);
    }

    return true;
}

Figure *PaintScene::createFigure(int type, const QPointF &point) const
{
    Figure *figure = nullptr;

    switch (type) {
    case SquareType:
        figure = new Square(point);
        break;
    case RombType:
        figure = new Romb(point);
        break;
    case TriangleType:
        figure = new Triangle(point);
        break;
    default:
        return nullptr;
    }

    figure->setType(type);
    return figure;
}

QList<Figure *> PaintScene::figures() const
{
    QList<Figure *> result;
    const QList<QGraphicsItem *> allItems = items(Qt::AscendingOrder);

    for (QGraphicsItem *item : allItems) {
        if (Figure *figure = dynamic_cast<Figure *>(item)) {
            result.append(figure);
        }
    }

    return result;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!tempFigure) {
        QGraphicsScene::mouseMoveEvent(event);
        return;
    }

    tempFigure->setEndPoint(event->scenePos());
    this->update(QRectF(0,0,this->width(), this->height()));
    QGraphicsScene::mouseMoveEvent(event);
}
void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    tempFigure = createFigure(m_typeFigure, event->scenePos());
    if (!tempFigure) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    this->addItem(tempFigure);
    QGraphicsScene::mousePressEvent(event);
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (tempFigure && event->button() == Qt::LeftButton) {
        tempFigure->setEndPoint(event->scenePos());
        tempFigure = nullptr;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

QList<Figure *> PaintScene::selectedFigures() const
{
    QList<Figure *> figures;
    const QList<QGraphicsItem *> items = selectedItems();

    for (QGraphicsItem *item : items) {
        if (Figure *figure = dynamic_cast<Figure *>(item)) {
            figures.append(figure);
        }
    }

    return figures;
}
