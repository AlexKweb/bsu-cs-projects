#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QString> // added v2
#include "figure.h"

class PaintScene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(int typeFigure
               READ typeFigure WRITE setTypeFigure
               NOTIFY typeFigureChanged)

public:
    explicit PaintScene(QObject *parent = 0);
    ~PaintScene();

    int typeFigure() const;
    void setTypeFigure(const int type);
    void fillSelectedFigures(const QColor &color); // added v2
    void clearFigures(); // added v2
    bool saveToFile(const QString &filePath) const; // added v2
    bool loadFromFile(const QString &filePath); // added v2

    enum FigureTypes {
        SquareType,
        RombType,
        TriangleType
    };

signals:
    void typeFigureChanged();

private:
    Figure *tempFigure; // added v2
    int m_typeFigure;
    Figure *createFigure(int type, const QPointF &point) const; // added v2
    QList<Figure *> figures() const; // added v2

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QList<Figure *> selectedFigures() const; // added v2

};

#endif // PAINTSCENE_H
