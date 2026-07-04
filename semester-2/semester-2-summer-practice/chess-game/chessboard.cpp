#include "chessboard.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

Board::Board(QObject *parent) : QObject(parent) {
    clear();
}

Board::~Board() = default;

void Board::clear() {
    for (auto &row : squares)
        for (auto &cell : row)
            cell.reset();
    currentTurn = Color::White;
}

static std::unique_ptr<Figure> makeFigure(Color c, Type t, int r, int col) {
    switch (t) {
    case Type::Pawn:   return std::make_unique<Pawn>(c, t, r, col);
    case Type::Rook:   return std::make_unique<Rook>(c, t, r, col);
    case Type::Knight: return std::make_unique<Knight>(c, t, r, col);
    case Type::Bishop: return std::make_unique<Bishop>(c, t, r, col);
    case Type::Queen:  return std::make_unique<Queen>(c, t, r, col);
    case Type::King:   return std::make_unique<King>(c, t, r, col);
    }
    return nullptr;
}

void Board::setup() {
    clear();
    // Black back rank (row 0)
    Type backRank[] = {Type::Rook, Type::Knight, Type::Bishop, Type::Queen,
                       Type::King, Type::Bishop, Type::Knight, Type::Rook};
    for (int c = 0; c < 8; ++c) {
        squares[0][c] = makeFigure(Color::Black, backRank[c], 0, c);
        squares[1][c] = makeFigure(Color::Black, Type::Pawn, 1, c);
        squares[6][c] = makeFigure(Color::White, Type::Pawn, 6, c);
        squares[7][c] = makeFigure(Color::White, backRank[c], 7, c);
    }
    currentTurn = Color::White;
}

const Figure *Board::getFigure(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7)
        return nullptr;
    return squares[row][col].get();
}

bool Board::isValidDestination(int row, int col, Color pieceColor) const {
    const Figure *f = getFigure(row, col);
    return !f || f->getColor() != pieceColor;
}

bool Board::wouldBeInCheck(int fromRow, int fromCol, int toRow, int toCol) const {
    // Simulate move on a copy
    Board copy;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (squares[r][c])
                copy.squares[r][c].reset(squares[r][c]->clone());

    Figure *mover = copy.squares[fromRow][fromCol].get();
    Color moverColor = mover->getColor();
    copy.squares[toRow][toCol] = std::move(copy.squares[fromRow][fromCol]);
    mover->setPosition(toRow, toCol);

    return copy.isInCheck(moverColor);
}

bool Board::moveFigure(int fromRow, int fromCol, int toRow, int toCol) {
    Figure *f = squares[fromRow][fromCol].get();
    if (!f) return false;
    if (f->getColor() != currentTurn) return false;
    if (!f->isValidMove(toRow, toCol, *this)) return false;
    if (wouldBeInCheck(fromRow, fromCol, toRow, toCol)) return false;

    // Execute move
    squares[toRow][toCol] = std::move(squares[fromRow][fromCol]);
    f = squares[toRow][toCol].get();
    f->setPosition(toRow, toCol);

    // Switch turn
    currentTurn = opposite(currentTurn);

    // Check game state
    if (isInCheck(currentTurn)) {
        if (isCheckmate(currentTurn)) {
            emit checkmate(currentTurn);
        } else {
            emit check(currentTurn);
        }
    } else if (isStalemate(currentTurn)) {
        emit stalemate(currentTurn);
    }

    return true;
}

QVector<QPair<int,int>> Board::getValidMoves(int row, int col) const {
    QVector<QPair<int,int>> moves;
    const Figure *f = getFigure(row, col);
    if (!f) return moves;
    if (f->getColor() != currentTurn) return moves;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (f->isValidMove(r, c, *this) && !wouldBeInCheck(row, col, r, c))
                moves.append({r, c});
        }
    }
    return moves;
}

bool Board::isSquareAttacked(int row, int col, Color byColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Figure *f = getFigure(r, c);
            if (f && f->getColor() == byColor) {
                // Temporarily create a copy of this figure to check move validity
                // without self-check filtering (the other figure doesn't care about
                // leaving its own king in check to attack a square)
                if (f->isValidMove(row, col, *this))
                    return true;
            }
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {
    // Find king
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (const Figure *f = getFigure(r, c))
                if (f->getType() == Type::King && f->getColor() == color)
                    return isSquareAttacked(r, c, opposite(color));
    return false;
}

bool Board::isCheckmate(Color color) const {
    if (!isInCheck(color)) return false;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (const Figure *f = getFigure(r, c))
                if (f->getColor() == color && !getValidMoves(r, c).isEmpty())
                    return false;
    return true;
}

bool Board::isStalemate(Color color) const {
    if (isInCheck(color)) return false;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (const Figure *f = getFigure(r, c))
                if (f->getColor() == color && !getValidMoves(r, c).isEmpty())
                    return false;
    return true;
}

// ─── Save / Load ────────────────────────────────────────────────────────────

static QString coord(int row, int col) {
    return QString("%1%2").arg(QChar('A' + col)).arg(8 - row);
}

static bool parseCoord(const QString &s, int &row, int &col) {
    if (s.length() != 2) return false;
    QChar letter = s[0].toUpper();
    QChar digit  = s[1];
    if (letter < 'A' || letter > 'H') return false;
    if (digit < '1'  || digit > '8')  return false;
    col = letter.toLatin1() - 'A';
    row = 8 - digit.digitValue();
    return true;
}

static void findFigures(const std::array<std::array<std::unique_ptr<Figure>, 8>, 8> &sq,
                        Color color, Type types[], int n,
                        QString result[])
{
    // For each type, collect all matching figures, sort by column
    for (int ti = 0; ti < n; ++ti) {
        QVector<QPair<int,int>> found;
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                if (auto *f = sq[r][c].get())
                    if (f->getType() == types[ti] && f->getColor() == color)
                        found.append({r, c});

        std::sort(found.begin(), found.end(), [](auto &a, auto &b) {
            return a.second < b.second; // sort by column
        });

        if (ti == 0 && n == 8) { // RNBQKBNR — first R at token 0, last R at token 7
            // Rooks: token 0 and 7
            // Knights: token 1 and 6
            // Bishops: token 2 and 5
            // Queen: token 3, King: token 4
            int firstIdx = (ti == 0) ? 0 : (ti == 1) ? 1 : 2;
            int secondIdx = (ti == 0) ? 7 : (ti == 1) ? 6 : 5;
            result[firstIdx] = found.size() > 0 ? coord(found[0].first, found[0].second) : "00";
            result[secondIdx] = found.size() > 1 ? coord(found[1].first, found[1].second) : "00";
        } else {
            if (found.isEmpty()) {
                result[ti] = "00";
            } else {
                result[ti] = coord(found[0].first, found[0].second);
            }
        }
    }
}

bool Board::saveToFile(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);

    out << (currentTurn == Color::White ? 'w' : 'b') << "\n";

    // White pawns — scan all rows for pawn in each column
    for (int c = 0; c < 8; ++c) {
        QString found = "00";
        for (int r = 0; r < 8; ++r)
            if (auto *f = squares[r][c].get())
                if (f->getType() == Type::Pawn && f->getColor() == Color::White)
                    { found = coord(r, c); break; }
        out << found << ' ';
    }
    out << "\n";

    // White figures R N B Q K B N R
    Type bRank[] = {Type::Rook, Type::Knight, Type::Bishop, Type::Queen,
                    Type::King, Type::Bishop, Type::Knight, Type::Rook};
    QString wFig[8];
    findFigures(squares, Color::White, bRank, 8, wFig);
    for (int i = 0; i < 8; ++i)
        out << wFig[i] << ' ';
    out << "\n";

    // Black pawns
    for (int c = 0; c < 8; ++c) {
        QString found = "00";
        for (int r = 0; r < 8; ++r)
            if (auto *f = squares[r][c].get())
                if (f->getType() == Type::Pawn && f->getColor() == Color::Black)
                    { found = coord(r, c); break; }
        out << found << ' ';
    }
    out << "\n";

    // Black figures R N B Q K B N R
    QString bFig[8];
    findFigures(squares, Color::Black, bRank, 8, bFig);
    for (int i = 0; i < 8; ++i)
        out << bFig[i] << ' ';
    out << "\n";

    return true;
}

bool Board::loadFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QStringList lines = QString::fromUtf8(file.readAll()).split('\n', Qt::SkipEmptyParts);
    file.close();

    qDebug() << "lines:" << lines;

    if (lines.size() < 5) return false;
    // lines[0] might have \r stripped — good

    clear();

    QString turnLine = lines[0].trimmed();
    if (turnLine.isEmpty()) return false;
    currentTurn = (turnLine == "w") ? Color::White : Color::Black;

    Color colors[] = {Color::White, Color::White, Color::Black, Color::Black};
    Type figTypes[] = {Type::Rook, Type::Knight, Type::Bishop, Type::Queen,
                       Type::King, Type::Bishop, Type::Knight, Type::Rook};

    for (int section = 0; section < 4; ++section) {
        Color col = colors[section];
        QStringList tokens = lines[section + 1].split(' ', Qt::SkipEmptyParts);
        if (tokens.size() != 8) return false;

        bool isPawnSection = (section == 0 || section == 2);

        if (isPawnSection) {
            for (int i = 0; i < 8; ++i) {
                if (tokens[i] == "00") continue;
                int r, c;
                if (!parseCoord(tokens[i], r, c)) return false;
                squares[r][c] = makeFigure(col, Type::Pawn, r, c);
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                if (tokens[i] == "00") continue;
                int r, c;
                if (!parseCoord(tokens[i], r, c)) return false;
                squares[r][c] = makeFigure(col, figTypes[i], r, c);
            }
        }
    }

    return true;
}
