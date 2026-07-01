#ifndef PIECE_H
#define PIECE_H

#include <QChar>
#include <optional>

enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };
enum class Color { White, Black };

struct Piece {
    PieceType type;
    Color color;

    bool operator==(const Piece& other) const {
        return type == other.type && color == other.color;
    }
    bool operator!=(const Piece& other) const {
        return !(*this == other);
    }
};

inline QChar pieceToUnicode(PieceType type, Color color) {
    if (color == Color::White) {
        switch (type) {
            case PieceType::King:   return QChar(0x2654);
            case PieceType::Queen:  return QChar(0x2655);
            case PieceType::Rook:   return QChar(0x2656);
            case PieceType::Bishop: return QChar(0x2657);
            case PieceType::Knight: return QChar(0x2658);
            case PieceType::Pawn:   return QChar(0x2659);
        }
    } else {
        switch (type) {
            case PieceType::King:   return QChar(0x265A);
            case PieceType::Queen:  return QChar(0x265B);
            case PieceType::Rook:   return QChar(0x265C);
            case PieceType::Bishop: return QChar(0x265D);
            case PieceType::Knight: return QChar(0x265E);
            case PieceType::Pawn:   return QChar(0x265F);
        }
    }
    return QChar();
}

inline QChar pieceToFen(Piece p) {
    switch (p.type) {
        case PieceType::King:   return p.color == Color::White ? 'K' : 'k';
        case PieceType::Queen:  return p.color == Color::White ? 'Q' : 'q';
        case PieceType::Rook:   return p.color == Color::White ? 'R' : 'r';
        case PieceType::Bishop: return p.color == Color::White ? 'B' : 'b';
        case PieceType::Knight: return p.color == Color::White ? 'N' : 'n';
        case PieceType::Pawn:   return p.color == Color::White ? 'P' : 'p';
    }
    return '?';
}

inline PieceType fenToPiece(QChar ch) {
    switch (ch.toLower().toLatin1()) {
        case 'k': return PieceType::King;
        case 'q': return PieceType::Queen;
        case 'r': return PieceType::Rook;
        case 'b': return PieceType::Bishop;
        case 'n': return PieceType::Knight;
        case 'p': return PieceType::Pawn;
        default:  return PieceType::Pawn;
    }
}

inline Color otherColor(Color c) {
    return c == Color::White ? Color::Black : Color::White;
}

#endif // PIECE_H
