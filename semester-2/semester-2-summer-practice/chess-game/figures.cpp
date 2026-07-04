#include "figures.h"
#include "chessboard.h"

Figure::Figure(Color color, Type type, int row, int col)
    : color(color), type(type), row(row), col(col) {}

Color opposite(Color c) {
    return c == Color::White ? Color::Black : Color::White;
}

bool Figure::isOnBoard(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

bool Figure::hasEnemy(int toRow, int toCol, const Board &board) const {
    const Figure *f = board.getFigure(toRow, toCol);
    return f && f->getColor() != color;
}

bool Figure::isPathClear(int toRow, int toCol, const Board &board) const {
    int dr = (toRow > row) ? 1 : (toRow < row) ? -1 : 0;
    int dc = (toCol > col) ? 1 : (toCol < col) ? -1 : 0;
    int r = row + dr;
    int c = col + dc;
    while (r != toRow || c != toCol) {
        if (board.getFigure(r, c))
            return false;
        r += dr;
        c += dc;
    }
    return true;
}

QChar Figure::symbol() const {
    static const char16_t wp[] = { 0x2659, 0x2656, 0x2658, 0x2657, 0x2655, 0x2654 };
    static const char16_t bp[] = { 0x265F, 0x265C, 0x265E, 0x265D, 0x265B, 0x265A };
    int idx = static_cast<int>(type);
    return QChar(color == Color::White ? wp[idx] : bp[idx]);
}

// ─── Pawn ──────────────────────────────────────────────────────────────────

bool Pawn::isValidMove(int toRow, int toCol, const Board &board) const {
    int dir = (color == Color::White) ? -1 : 1;
    int startRow = (color == Color::White) ? 6 : 1;
    int dr = toRow - row;
    int dc = qAbs(toCol - col);

    if (dc == 0) {
        if (dr == dir && !board.getFigure(toRow, toCol))
            return true;
        if (dr == 2 * dir && row == startRow && !board.getFigure(toRow, toCol)
            && !board.getFigure(row + dir, col))
            return true;
    }
    if (dc == 1 && dr == dir) {
        if (hasEnemy(toRow, toCol, board))
            return true;
    }
    return false;
}

// ─── Rook ──────────────────────────────────────────────────────────────────

bool Rook::isValidMove(int toRow, int toCol, const Board &board) const {
    if (!isOnBoard(toRow, toCol)) return false;
    if (row == toRow && col == toCol) return false;
    bool straight = (row == toRow || col == toCol);
    if (!straight) return false;
    if (!isPathClear(toRow, toCol, board)) return false;
    const Figure *target = board.getFigure(toRow, toCol);
    return !target || target->getColor() != color;
}

// ─── Knight ────────────────────────────────────────────────────────────────

bool Knight::isValidMove(int toRow, int toCol, const Board &board) const {
    if (!isOnBoard(toRow, toCol)) return false;
    if (row == toRow && col == toCol) return false;
    int dr = qAbs(toRow - row);
    int dc = qAbs(toCol - col);
    if (!((dr == 1 && dc == 2) || (dr == 2 && dc == 1)))
        return false;
    const Figure *target = board.getFigure(toRow, toCol);
    return !target || target->getColor() != color;
}

// ─── Bishop ────────────────────────────────────────────────────────────────

bool Bishop::isValidMove(int toRow, int toCol, const Board &board) const {
    if (!isOnBoard(toRow, toCol)) return false;
    if (row == toRow && col == toCol) return false;
    if (qAbs(toRow - row) != qAbs(toCol - col)) return false;
    if (!isPathClear(toRow, toCol, board)) return false;
    const Figure *target = board.getFigure(toRow, toCol);
    return !target || target->getColor() != color;
}

// ─── Queen ─────────────────────────────────────────────────────────────────

bool Queen::isValidMove(int toRow, int toCol, const Board &board) const {
    if (!isOnBoard(toRow, toCol)) return false;
    if (row == toRow && col == toCol) return false;
    bool straight = (row == toRow || col == toCol);
    bool diagonal = (qAbs(toRow - row) == qAbs(toCol - col));
    if (!straight && !diagonal) return false;
    if (!isPathClear(toRow, toCol, board)) return false;
    const Figure *target = board.getFigure(toRow, toCol);
    return !target || target->getColor() != color;
}

// ─── King ──────────────────────────────────────────────────────────────────

bool King::isValidMove(int toRow, int toCol, const Board &board) const {
    if (!isOnBoard(toRow, toCol)) return false;
    if (row == toRow && col == toCol) return false;
    if (qAbs(toRow - row) > 1 || qAbs(toCol - col) > 1) return false;
    const Figure *target = board.getFigure(toRow, toCol);
    if (target && target->getColor() == color) return false;
    return !board.isSquareAttacked(toRow, toCol, opposite(color));
}
