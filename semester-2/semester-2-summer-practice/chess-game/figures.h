#ifndef FIGURES_H
#define FIGURES_H

#include <QChar>
#include <QVector>
#include <QPair>

enum class Color { White, Black };
enum class Type { Pawn, Rook, Knight, Bishop, Queen, King };

class Board;

Color opposite(Color c);

class Figure {
public:
    Figure(Color color, Type type, int row, int col);
    virtual ~Figure() = default;

    Color getColor() const { return color; }
    Type getType() const { return type; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    void setPosition(int r, int c) { row = r; col = c; }

    QChar symbol() const;
    virtual bool isValidMove(int toRow, int toCol, const Board &board) const = 0;
    virtual bool attacksSquare(int toRow, int toCol, const Board &board) const = 0;
    virtual Figure *clone() const = 0;

protected:
    bool isPathClear(int toRow, int toCol, const Board &board) const;
    bool isOnBoard(int r, int c) const;
    bool hasEnemy(int toRow, int toCol, const Board &board) const;

    Color color;
    Type type;
    int row, col;
};

class Pawn : public Figure {
public:
    using Figure::Figure;
    bool isValidMove(int toRow, int toCol, const Board &board) const override;
    bool attacksSquare(int toRow, int toCol, const Board &board) const override;
    Pawn *clone() const override { return new Pawn(*this); }
};

class Rook : public Figure {
public:
    using Figure::Figure;
    bool isValidMove(int toRow, int toCol, const Board &board) const override;
    bool attacksSquare(int toRow, int toCol, const Board &board) const override;
    Rook *clone() const override { return new Rook(*this); }
};

class Knight : public Figure {
public:
    using Figure::Figure;
    bool isValidMove(int toRow, int toCol, const Board &board) const override;
    bool attacksSquare(int toRow, int toCol, const Board &board) const override;
    Knight *clone() const override { return new Knight(*this); }
};

class Bishop : public Figure {
public:
    using Figure::Figure;
    bool isValidMove(int toRow, int toCol, const Board &board) const override;
    bool attacksSquare(int toRow, int toCol, const Board &board) const override;
    Bishop *clone() const override { return new Bishop(*this); }
};

class Queen : public Figure {
public:
    using Figure::Figure;
    bool isValidMove(int toRow, int toCol, const Board &board) const override;
    bool attacksSquare(int toRow, int toCol, const Board &board) const override;
    Queen *clone() const override { return new Queen(*this); }
};

class King : public Figure {
public:
    using Figure::Figure;
    bool isValidMove(int toRow, int toCol, const Board &board) const override;
    bool attacksSquare(int toRow, int toCol, const Board &board) const override;
    King *clone() const override { return new King(*this); }
};

#endif
