#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <array>
#include <memory>
#include <QObject>
#include <QVector>
#include <QPair>
#include <vector>

#include "figures.h"

struct UndoInfo {
    std::array<std::array<std::unique_ptr<Figure>, 8>, 8> squaresBefore;
    Color turnBefore = Color::White;
    bool whiteKingMovedBefore = false;
    bool blackKingMovedBefore = false;
    bool whiteKingsideRookMovedBefore = false;
    bool whiteQueensideRookMovedBefore = false;
    bool blackKingsideRookMovedBefore = false;
    bool blackQueensideRookMovedBefore = false;
    int enPassantRowBefore = -1;
    int enPassantColBefore = -1;
};

class Board : public QObject {
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    ~Board() override;

    void setup();
    void clear();

    bool moveFigure(int fromRow, int fromCol, int toRow, int toCol,
                    Type promotionType = Type::Queen);
    void undoMove();
    bool canUndo() const { return !undoStack.empty(); }

    QVector<QPair<int,int>> getValidMoves(int row, int col) const;

    const Figure *getFigure(int row, int col) const;
    Color getCurrentTurn() const { return currentTurn; }

    bool saveToFile(const QString &path) const;
    bool loadFromFile(const QString &path);

    bool isInCheck(Color color) const;
    bool isSquareAttacked(int row, int col, Color byColor) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    bool canCastle(Color color, bool kingside) const;
    bool isEnPassantMove(int fromRow, int fromCol, int toRow, int toCol) const;

signals:
    void check(Color color);
    void checkmate(Color color);
    void stalemate(Color color);

private:
    bool hasAnyValidMove(Color color) const;
    bool wouldBeInCheck(int fromRow, int fromCol, int toRow, int toCol,
                        Type promotionType) const;
    void saveUndoSnapshot();
    void restoreSnapshot(UndoInfo &info);
    void cloneInto(std::array<std::array<std::unique_ptr<Figure>, 8>, 8> &target) const;
    void applyMoveUnchecked(int fromRow, int fromCol, int toRow, int toCol,
                            Type promotionType);

    std::array<std::array<std::unique_ptr<Figure>, 8>, 8> squares;
    Color currentTurn = Color::White;
    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteKingsideRookMoved = false;
    bool whiteQueensideRookMoved = false;
    bool blackKingsideRookMoved = false;
    bool blackQueensideRookMoved = false;
    int enPassantRow = -1;
    int enPassantCol = -1;
    std::vector<UndoInfo> undoStack;
};

#endif
