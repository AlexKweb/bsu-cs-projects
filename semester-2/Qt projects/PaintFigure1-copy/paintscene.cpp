#include "paintscene.h"
#include <QFile> // added v2
#include <QTextStream> // added v2
#include "romb.h"
#include "triangle.h"
#include "square.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent)
{
    tempFigure = nullptr; // added v2
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

void PaintScene::fillSelectedFigures(const QColor &color) // added v2
{
    const QList<Figure *> figures = selectedFigures(); // added v2
    for (Figure *figure : figures) {
        figure->setBrush(QBrush(color)); // added v2
    }
}

void PaintScene::clearFigures() // added v2
{
    tempFigure = nullptr; // added v2
    clear(); // added v2
}

bool PaintScene::saveToFile(const QString &filePath) const // added v2
{
    QFile file(filePath); // added v2
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file); // added v2
    const QList<Figure *> allFigures = figures(); // added v2
    out << allFigures.size() << '\n'; // added v2

    for (const Figure *figure : allFigures) {
        out << *figure; // added v2
    }

    return out.status() == QTextStream::Ok; // added v2
}

bool PaintScene::loadFromFile(const QString &filePath) // added v2
{
    QFile file(filePath); // added v2
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file); // added v2
    int count = 0; // added v2
    in >> count; // added v2
    if (in.status() != QTextStream::Ok) {
        return false;
    }

    clearFigures(); // added v2

    for (int i = 0; i < count; ++i) {
        FigureData data; // added v2
        in >> data; // added v2
        if (in.status() != QTextStream::Ok) {
            clearFigures(); // added v2
            return false;
        }

        Figure *figure = createFigure(data.type, data.start); // added v2
        if (!figure) {
            clearFigures(); // added v2
            return false;
        }

        figure->applyData(data); // added v2
        addItem(figure); // added v2
    }

    return true;
}

Figure *PaintScene::createFigure(int type, const QPointF &point) const // added v2
{
    Figure *figure = nullptr; // added v2

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

    figure->setType(type); // added v2
    return figure;
}

QList<Figure *> PaintScene::figures() const // added v2
{
    QList<Figure *> result; // added v2
    const QList<QGraphicsItem *> allItems = items(Qt::AscendingOrder); // added v2

    for (QGraphicsItem *item : allItems) {
        if (Figure *figure = dynamic_cast<Figure *>(item)) {
            result.append(figure); // added v2
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
    QGraphicsScene::mouseMoveEvent(event); // added v2
}
void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    tempFigure = createFigure(m_typeFigure, event->scenePos()); // added v2
    if (!tempFigure) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    this->addItem(tempFigure); // added v2
    QGraphicsScene::mousePressEvent(event);
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (tempFigure && event->button() == Qt::LeftButton) {
        tempFigure->setEndPoint(event->scenePos());
        tempFigure = nullptr; // added v2
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

QList<Figure *> PaintScene::selectedFigures() const // added v2
{
    QList<Figure *> figures; // added v2
    const QList<QGraphicsItem *> items = selectedItems(); // added v2

    for (QGraphicsItem *item : items) {
        if (Figure *figure = dynamic_cast<Figure *>(item)) {
            figures.append(figure); // added v2
        }
    }

    return figures; // added v2
}
