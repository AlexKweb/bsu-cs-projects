#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QString>
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
    void fillSelectedFigures(const QColor &color);
    void clearFigures();
    void undoLastFigure();
    bool saveToFile(const QString &filePath) const;
    bool loadFromFile(const QString &filePath);

    enum FigureTypes {
        SquareType,
        RombType,
        TriangleType
    };

signals:
    void typeFigureChanged();

private:
    Figure *tempFigure;
    int m_typeFigure;
    Figure *createFigure(int type, const QPointF &point) const;
    QList<Figure *> figures() const;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QList<Figure *> selectedFigures() const;

};

#endif
