#include "chessboard.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <cmath>

Board::Board(QObject *parent) : QObject(parent) {
    clear();
}

Board::~Board() = default;

static bool onBoard(int row, int col)
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
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

void Board::clear() {
    for (auto &row : squares)
        for (auto &cell : row)
            cell.reset();
    currentTurn = Color::White;
    whiteKingMoved = blackKingMoved = false;
    whiteKingsideRookMoved = whiteQueensideRookMoved = false;
    blackKingsideRookMoved = blackQueensideRookMoved = false;
    enPassantRow = -1;
    enPassantCol = -1;
    undoStack.clear();
}

void Board::setup() {
    clear();
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
    if (!onBoard(row, col))
        return nullptr;
    return squares[row][col].get();
}

void Board::cloneInto(std::array<std::array<std::unique_ptr<Figure>, 8>, 8> &target) const
{
    for (auto &row : target)
        for (auto &cell : row)
            cell.reset();

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (squares[r][c])
                target[r][c].reset(squares[r][c]->clone());
}

void Board::saveUndoSnapshot()
{
    UndoInfo info;
    cloneInto(info.squaresBefore);
    info.turnBefore = currentTurn;
    info.whiteKingMovedBefore = whiteKingMoved;
    info.blackKingMovedBefore = blackKingMoved;
    info.whiteKingsideRookMovedBefore = whiteKingsideRookMoved;
    info.whiteQueensideRookMovedBefore = whiteQueensideRookMoved;
    info.blackKingsideRookMovedBefore = blackKingsideRookMoved;
    info.blackQueensideRookMovedBefore = blackQueensideRookMoved;
    info.enPassantRowBefore = enPassantRow;
    info.enPassantColBefore = enPassantCol;
    undoStack.push_back(std::move(info));
}

void Board::restoreSnapshot(UndoInfo &info)
{
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            squares[r][c] = std::move(info.squaresBefore[r][c]);

    currentTurn = info.turnBefore;
    whiteKingMoved = info.whiteKingMovedBefore;
    blackKingMoved = info.blackKingMovedBefore;
    whiteKingsideRookMoved = info.whiteKingsideRookMovedBefore;
    whiteQueensideRookMoved = info.whiteQueensideRookMovedBefore;
    blackKingsideRookMoved = info.blackKingsideRookMovedBefore;
    blackQueensideRookMoved = info.blackQueensideRookMovedBefore;
    enPassantRow = info.enPassantRowBefore;
    enPassantCol = info.enPassantColBefore;
}

bool Board::isEnPassantMove(int fromRow, int fromCol, int toRow, int toCol) const
{
    const Figure *f = getFigure(fromRow, fromCol);
    if (!f || f->getType() != Type::Pawn)
        return false;
    return toRow == enPassantRow && toCol == enPassantCol
           && std::abs(toCol - fromCol) == 1
           && getFigure(toRow, toCol) == nullptr;
}

bool Board::canCastle(Color color, bool kingside) const
{
    const int row = (color == Color::White) ? 7 : 0;
    const int rookCol = kingside ? 7 : 0;
    const int step = kingside ? 1 : -1;
    const int kingToCol = kingside ? 6 : 2;

    if (isInCheck(color))
        return false;

    if (color == Color::White) {
        if (whiteKingMoved || (kingside ? whiteKingsideRookMoved : whiteQueensideRookMoved))
            return false;
    } else {
        if (blackKingMoved || (kingside ? blackKingsideRookMoved : blackQueensideRookMoved))
            return false;
    }

    const Figure *king = getFigure(row, 4);
    const Figure *rook = getFigure(row, rookCol);
    if (!king || !rook || king->getType() != Type::King || rook->getType() != Type::Rook)
        return false;
    if (king->getColor() != color || rook->getColor() != color)
        return false;

    const int firstBetween = kingside ? 5 : 1;
    const int lastBetween = kingside ? 6 : 3;
    for (int c = firstBetween; c <= lastBetween; ++c)
        if (getFigure(row, c))
            return false;

    const Color enemy = opposite(color);
    for (int c = 4 + step; c != kingToCol + step; c += step)
        if (isSquareAttacked(row, c, enemy))
            return false;

    return true;
}

void Board::applyMoveUnchecked(int fromRow, int fromCol, int toRow, int toCol,
                               Type promotionType)
{
    Figure *moving = squares[fromRow][fromCol].get();
    if (!moving)
        return;

    const Color movingColor = moving->getColor();
    const Type movingType = moving->getType();
    const bool castling = movingType == Type::King && fromCol == 4 && std::abs(toCol - fromCol) == 2;
    const bool enPassant = isEnPassantMove(fromRow, fromCol, toRow, toCol);

    if (movingType == Type::King) {
        if (movingColor == Color::White)
            whiteKingMoved = true;
        else
            blackKingMoved = true;
    }

    if (movingType == Type::Rook) {
        if (fromRow == 7 && fromCol == 0) whiteQueensideRookMoved = true;
        if (fromRow == 7 && fromCol == 7) whiteKingsideRookMoved = true;
        if (fromRow == 0 && fromCol == 0) blackQueensideRookMoved = true;
        if (fromRow == 0 && fromCol == 7) blackKingsideRookMoved = true;
    }

    if (const Figure *captured = getFigure(toRow, toCol)) {
        if (captured->getType() == Type::Rook) {
            if (toRow == 7 && toCol == 0) whiteQueensideRookMoved = true;
            if (toRow == 7 && toCol == 7) whiteKingsideRookMoved = true;
            if (toRow == 0 && toCol == 0) blackQueensideRookMoved = true;
            if (toRow == 0 && toCol == 7) blackKingsideRookMoved = true;
        }
    }

    if (enPassant)
        squares[fromRow][toCol].reset();

    enPassantRow = -1;
    enPassantCol = -1;
    if (movingType == Type::Pawn && std::abs(toRow - fromRow) == 2) {
        enPassantRow = (fromRow + toRow) / 2;
        enPassantCol = fromCol;
    }

    squares[toRow][toCol] = std::move(squares[fromRow][fromCol]);
    squares[toRow][toCol]->setPosition(toRow, toCol);

    if (castling) {
        const int rookFromCol = toCol > fromCol ? 7 : 0;
        const int rookToCol = toCol > fromCol ? 5 : 3;
        squares[toRow][rookToCol] = std::move(squares[toRow][rookFromCol]);
        if (squares[toRow][rookToCol])
            squares[toRow][rookToCol]->setPosition(toRow, rookToCol);
    }

    if (movingType == Type::Pawn && (toRow == 0 || toRow == 7)) {
        if (promotionType == Type::Pawn || promotionType == Type::King)
            promotionType = Type::Queen;
        squares[toRow][toCol] = makeFigure(movingColor, promotionType, toRow, toCol);
    }
}

bool Board::wouldBeInCheck(int fromRow, int fromCol, int toRow, int toCol,
                           Type promotionType) const
{
    Board copy;
    cloneInto(copy.squares);
    copy.currentTurn = currentTurn;
    copy.whiteKingMoved = whiteKingMoved;
    copy.blackKingMoved = blackKingMoved;
    copy.whiteKingsideRookMoved = whiteKingsideRookMoved;
    copy.whiteQueensideRookMoved = whiteQueensideRookMoved;
    copy.blackKingsideRookMoved = blackKingsideRookMoved;
    copy.blackQueensideRookMoved = blackQueensideRookMoved;
    copy.enPassantRow = enPassantRow;
    copy.enPassantCol = enPassantCol;

    const Figure *mover = copy.getFigure(fromRow, fromCol);
    if (!mover)
        return true;
    const Color moverColor = mover->getColor();
    copy.applyMoveUnchecked(fromRow, fromCol, toRow, toCol, promotionType);
    return copy.isInCheck(moverColor);
}

bool Board::moveFigure(int fromRow, int fromCol, int toRow, int toCol,
                       Type promotionType)
{
    if (!onBoard(fromRow, fromCol) || !onBoard(toRow, toCol))
        return false;

    Figure *f = squares[fromRow][fromCol].get();
    if (!f) return false;
    if (f->getColor() != currentTurn) return false;
    if (!f->isValidMove(toRow, toCol, *this)) return false;
    if (wouldBeInCheck(fromRow, fromCol, toRow, toCol, promotionType)) return false;

    saveUndoSnapshot();
    applyMoveUnchecked(fromRow, fromCol, toRow, toCol, promotionType);
    currentTurn = opposite(currentTurn);

    if (isInCheck(currentTurn)) {
        if (isCheckmate(currentTurn))
            emit checkmate(currentTurn);
        else
            emit check(currentTurn);
    } else if (isStalemate(currentTurn)) {
        emit stalemate(currentTurn);
    }

    return true;
}

void Board::undoMove()
{
    if (undoStack.empty())
        return;

    UndoInfo info = std::move(undoStack.back());
    undoStack.pop_back();
    restoreSnapshot(info);
}

QVector<QPair<int,int>> Board::getValidMoves(int row, int col) const {
    QVector<QPair<int,int>> moves;
    const Figure *f = getFigure(row, col);
    if (!f) return moves;
    if (f->getColor() != currentTurn) return moves;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (f->isValidMove(r, c, *this) && !wouldBeInCheck(row, col, r, c, Type::Queen))
                moves.append({r, c});
        }
    }
    return moves;
}

bool Board::isSquareAttacked(int row, int col, Color byColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Figure *f = getFigure(r, c);
            if (f && f->getColor() == byColor && f->attacksSquare(row, col, *this))
                return true;
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (const Figure *f = getFigure(r, c))
                if (f->getType() == Type::King && f->getColor() == color)
                    return isSquareAttacked(r, c, opposite(color));
    return false;
}

bool Board::hasAnyValidMove(Color color) const
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Figure *f = getFigure(r, c);
            if (!f || f->getColor() != color)
                continue;
            for (int tr = 0; tr < 8; ++tr)
                for (int tc = 0; tc < 8; ++tc)
                    if (f->isValidMove(tr, tc, *this)
                        && !wouldBeInCheck(r, c, tr, tc, Type::Queen))
                        return true;
        }
    }
    return false;
}

bool Board::isCheckmate(Color color) const {
    return isInCheck(color) && !hasAnyValidMove(color);
}

bool Board::isStalemate(Color color) const {
    return !isInCheck(color) && !hasAnyValidMove(color);
}

// Save / Load

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

static QChar pieceToChar(const Figure *f)
{
    if (!f)
        return '.';

    QChar ch;
    switch (f->getType()) {
    case Type::Pawn: ch = 'p'; break;
    case Type::Rook: ch = 'r'; break;
    case Type::Knight: ch = 'n'; break;
    case Type::Bishop: ch = 'b'; break;
    case Type::Queen: ch = 'q'; break;
    case Type::King: ch = 'k'; break;
    }
    return f->getColor() == Color::White ? ch.toUpper() : ch;
}

static bool charToPiece(QChar ch, Color &color, Type &type)
{
    if (ch == '.')
        return false;

    color = ch.isUpper() ? Color::White : Color::Black;
    switch (ch.toLower().toLatin1()) {
    case 'p': type = Type::Pawn; return true;
    case 'r': type = Type::Rook; return true;
    case 'n': type = Type::Knight; return true;
    case 'b': type = Type::Bishop; return true;
    case 'q': type = Type::Queen; return true;
    case 'k': type = Type::King; return true;
    default: return false;
    }
}

bool Board::saveToFile(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);

    out << "chess-game-v2\n";
    out << (currentTurn == Color::White ? 'w' : 'b') << "\n";
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c)
            out << pieceToChar(squares[r][c].get());
        out << "\n";
    }
    out << (whiteKingMoved ? '1' : '0') << ' '
        << (blackKingMoved ? '1' : '0') << ' '
        << (whiteKingsideRookMoved ? '1' : '0') << ' '
        << (whiteQueensideRookMoved ? '1' : '0') << ' '
        << (blackKingsideRookMoved ? '1' : '0') << ' '
        << (blackQueensideRookMoved ? '1' : '0') << "\n";
    out << (enPassantRow == -1 ? QString("00") : coord(enPassantRow, enPassantCol)) << "\n";
    return true;
}

bool Board::loadFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QStringList lines = QString::fromUtf8(file.readAll()).split('\n', Qt::SkipEmptyParts);
    file.close();

    if (lines.size() < 5) return false;

    clear();

    if (lines[0].trimmed() == "chess-game-v2") {
        if (lines.size() < 12) return false;

        QString turnLine = lines[1].trimmed();
        currentTurn = (turnLine == "b") ? Color::Black : Color::White;

        for (int r = 0; r < 8; ++r) {
            QString line = lines[r + 2].trimmed();
            if (line.size() != 8) return false;
            for (int c = 0; c < 8; ++c) {
                Color color;
                Type type;
                if (charToPiece(line[c], color, type))
                    squares[r][c] = makeFigure(color, type, r, c);
                else if (line[c] != '.')
                    return false;
            }
        }

        QStringList flags = lines[10].split(' ', Qt::SkipEmptyParts);
        if (flags.size() == 6) {
            whiteKingMoved = flags[0] == "1";
            blackKingMoved = flags[1] == "1";
            whiteKingsideRookMoved = flags[2] == "1";
            whiteQueensideRookMoved = flags[3] == "1";
            blackKingsideRookMoved = flags[4] == "1";
            blackQueensideRookMoved = flags[5] == "1";
        }

        QString ep = lines[11].trimmed();
        if (ep != "00")
            parseCoord(ep, enPassantRow, enPassantCol);

        undoStack.clear();
        return true;
    }

    QString turnLine = lines[0].trimmed();
    if (turnLine.isEmpty()) return false;
    currentTurn = (turnLine == "b") ? Color::Black : Color::White;

    Color colors[] = {Color::White, Color::White, Color::Black, Color::Black};
    Type figTypes[] = {Type::Rook, Type::Knight, Type::Bishop, Type::Queen,
                       Type::King, Type::Bishop, Type::Knight, Type::Rook};

    for (int section = 0; section < 4; ++section) {
        Color col = colors[section];
        QStringList tokens = lines[section + 1].split(' ', Qt::SkipEmptyParts);
        if (tokens.size() != 8) return false;

        const bool isPawnSection = (section == 0 || section == 2);
        for (int i = 0; i < 8; ++i) {
            if (tokens[i] == "00") continue;
            int r, c;
            if (!parseCoord(tokens[i], r, c)) return false;
            squares[r][c] = makeFigure(col, isPawnSection ? Type::Pawn : figTypes[i], r, c);
        }
    }

    if (lines.size() >= 6) {
        QStringList flags = lines[5].split(' ', Qt::SkipEmptyParts);
        if (flags.size() == 6) {
            whiteKingMoved = flags[0] == "1";
            blackKingMoved = flags[1] == "1";
            whiteKingsideRookMoved = flags[2] == "1";
            whiteQueensideRookMoved = flags[3] == "1";
            blackKingsideRookMoved = flags[4] == "1";
            blackQueensideRookMoved = flags[5] == "1";
        }
    }

    if (lines.size() >= 7) {
        QString ep = lines[6].trimmed();
        if (ep != "00")
            parseCoord(ep, enPassantRow, enPassantCol);
    }

    undoStack.clear();
    return true;
}
