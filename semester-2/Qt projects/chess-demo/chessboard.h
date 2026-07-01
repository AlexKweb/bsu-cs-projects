#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QString>
#include <QVector>
#include <array>
#include <optional>
#include "piece.h"

using BoardType = std::array<std::array<std::optional<Piece>, 8>, 8>;

class ChessBoard {
public:
    enum CastlingSide { Kingside, Queenside };

    struct Move {
        int fromRow = 0, fromCol = 0;
        int toRow = 0, toCol = 0;
        PieceType piece = PieceType::Pawn;
        Color color = Color::White;
        bool isPromotion = false;
        PieceType promotion = PieceType::Queen;
        bool isCastling = false;
        CastlingSide castlingSide = Kingside;
        bool isEnPassant = false;
        std::optional<Piece> captured = std::nullopt;
        QString notation;
    };

    ChessBoard();

    const std::optional<Piece>& at(int row, int col) const;

    void setupInitial();
    bool fromFen(const QString& fen);
    QString toFen() const;

    QVector<Move> generateLegalMoves() const;
    QVector<Move> generateMovesForPiece(int row, int col) const;
    void makeMove(const Move& move);

    Color sideToMove() const { return m_sideToMove; }
    int fullMoveNumber() const { return m_fullMoveNumber; }
    const QVector<Move>& moveHistory() const { return m_moveHistory; }
    QStringList notationHistory() const;

    bool isInCheck() const;
    bool isInCheck(Color side) const;
    bool isCheckmate() const;
    bool isStalemate() const;
    bool isGameOver() const { return m_gameOver; }
    bool gameOver() const { return m_gameOver; }

    static constexpr bool isLightSquare(int row, int col) {
        return (row + col) % 2 == 0;
    }

private:
    using BoardType = std::array<std::array<std::optional<Piece>, 8>, 8>;

    bool isInBounds(int row, int col) const;
    bool isSquareAttacked(int row, int col, Color byColor, const BoardType& board) const;
    bool isSquareAttacked(int row, int col, Color byColor) const;
    bool isMoveLegal(const Move& move) const;
    QVector<Move> generatePseudoLegalMoves() const;

    QString moveToNotation(const Move& move, bool givesCheck, bool givesCheckmate) const;

    BoardType m_board;
    Color m_sideToMove = Color::White;
    bool m_wk = true, m_wq = true, m_bk = true, m_bq = true;
    int m_epRow = -1, m_epCol = -1;
    int m_halfmoveClock = 0;
    int m_fullMoveNumber = 1;
    QVector<Move> m_moveHistory;
    bool m_gameOver = false;
};

#endif // CHESSBOARD_H
