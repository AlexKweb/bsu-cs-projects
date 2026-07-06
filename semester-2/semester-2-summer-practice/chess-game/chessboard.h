#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <array>
#include <memory>
#include <QObject>
#include <QVector>
#include <QPair>

#include "figures.h"

struct UndoInfo {
    int fromRow = -1, fromCol = -1, toRow = -1, toCol = -1;
    std::unique_ptr<Figure> captured;
    Color turnBefore = Color::White;
};

class Board : public QObject {
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    ~Board() override;

    void setup();
    void clear();

    bool moveFigure(int fromRow, int fromCol, int toRow, int toCol);
    void undoMove();
    bool canUndo() const { return undoInfo.fromRow != -1; }

    QVector<QPair<int,int>> getValidMoves(int row, int col) const;

    const Figure *getFigure(int row, int col) const;
    Color getCurrentTurn() const { return currentTurn; }

    bool saveToFile(const QString &path) const;
    bool loadFromFile(const QString &path);

    bool isInCheck(Color color) const;
    bool isSquareAttacked(int row, int col, Color byColor) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;

signals:
    void check(Color color);
    void checkmate(Color color);
    void stalemate(Color color);

private:
    bool isValidDestination(int row, int col, Color pieceColor) const;
    bool wouldBeInCheck(int fromRow, int fromCol, int toRow, int toCol) const;

    std::array<std::array<std::unique_ptr<Figure>, 8>, 8> squares;
    Color currentTurn = Color::White;
    UndoInfo undoInfo;
};

#endif
