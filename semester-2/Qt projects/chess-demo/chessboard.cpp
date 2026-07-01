#include "chessboard.h"

ChessBoard::ChessBoard()
{
    setupInitial();
}

const std::optional<Piece>& ChessBoard::at(int row, int col) const
{
    return m_board[row][col];
}

void ChessBoard::setupInitial()
{
    for (auto& row : m_board)
        for (auto& cell : row)
            cell = std::nullopt;

    m_board[0][0] = Piece{PieceType::Rook, Color::Black};
    m_board[0][1] = Piece{PieceType::Knight, Color::Black};
    m_board[0][2] = Piece{PieceType::Bishop, Color::Black};
    m_board[0][3] = Piece{PieceType::Queen, Color::Black};
    m_board[0][4] = Piece{PieceType::King, Color::Black};
    m_board[0][5] = Piece{PieceType::Bishop, Color::Black};
    m_board[0][6] = Piece{PieceType::Knight, Color::Black};
    m_board[0][7] = Piece{PieceType::Rook, Color::Black};
    for (int c = 0; c < 8; ++c)
        m_board[1][c] = Piece{PieceType::Pawn, Color::Black};
    for (int c = 0; c < 8; ++c)
        m_board[6][c] = Piece{PieceType::Pawn, Color::White};
    m_board[7][0] = Piece{PieceType::Rook, Color::White};
    m_board[7][1] = Piece{PieceType::Knight, Color::White};
    m_board[7][2] = Piece{PieceType::Bishop, Color::White};
    m_board[7][3] = Piece{PieceType::Queen, Color::White};
    m_board[7][4] = Piece{PieceType::King, Color::White};
    m_board[7][5] = Piece{PieceType::Bishop, Color::White};
    m_board[7][6] = Piece{PieceType::Knight, Color::White};
    m_board[7][7] = Piece{PieceType::Rook, Color::White};

    m_sideToMove = Color::White;
    m_wk = true; m_wq = true; m_bk = true; m_bq = true;
    m_epRow = -1; m_epCol = -1;
    m_halfmoveClock = 0;
    m_fullMoveNumber = 1;
    m_moveHistory.clear();
    m_gameOver = false;
}

bool ChessBoard::fromFen(const QString& fen)
{
    QStringList parts = fen.split(' ');
    if (parts.isEmpty()) return false;

    for (auto& row : m_board)
        for (auto& cell : row)
            cell = std::nullopt;

    QStringList ranks = parts[0].split('/');
    if (ranks.size() != 8) return false;

    for (int r = 0; r < 8; ++r) {
        int c = 0;
        for (const QChar& ch : ranks[r]) {
            if (c >= 8) return false;
            if (ch.isDigit()) {
                int empty = ch.digitValue();
                for (int i = 0; i < empty; ++i) {
                    if (c >= 8) return false;
                    m_board[r][c++] = std::nullopt;
                }
            } else {
                Color color = ch.isUpper() ? Color::White : Color::Black;
                PieceType type = fenToPiece(ch);
                m_board[r][c++] = Piece{type, color};
            }
        }
        if (c != 8) return false;
    }

    m_sideToMove = Color::White;
    m_wk = m_wq = m_bk = m_bq = false;
    m_epRow = m_epCol = -1;
    m_halfmoveClock = 0;
    m_fullMoveNumber = 1;
    m_moveHistory.clear();
    m_gameOver = false;

    if (parts.size() >= 2)
        m_sideToMove = (parts[1] == "w") ? Color::White : Color::Black;
    if (parts.size() >= 3) {
        m_wk = parts[2].contains('K');
        m_wq = parts[2].contains('Q');
        m_bk = parts[2].contains('k');
        m_bq = parts[2].contains('q');
    }
    if (parts.size() >= 4 && parts[3] != "-") {
        if (parts[3].length() >= 2) {
            m_epCol = parts[3][0].toLatin1() - 'a';
            m_epRow = 8 - (parts[3][1].digitValue());
        }
    }
    if (parts.size() >= 5) m_halfmoveClock = parts[4].toInt();
    if (parts.size() >= 6) m_fullMoveNumber = parts[5].toInt();

    return true;
}

QString ChessBoard::toFen() const
{
    QString fen;
    for (int r = 0; r < 8; ++r) {
        int empty = 0;
        for (int c = 0; c < 8; ++c) {
            if (m_board[r][c].has_value()) {
                if (empty > 0) { fen += QString::number(empty); empty = 0; }
                fen += pieceToFen(m_board[r][c].value());
            } else {
                ++empty;
            }
        }
        if (empty > 0) fen += QString::number(empty);
        if (r < 7) fen += '/';
    }
    fen += ' ';
    fen += (m_sideToMove == Color::White) ? 'w' : 'b';
    fen += ' ';
    QString castling;
    if (m_wk) castling += 'K';
    if (m_wq) castling += 'Q';
    if (m_bk) castling += 'k';
    if (m_bq) castling += 'q';
    fen += castling.isEmpty() ? "-" : castling;
    fen += ' ';
    if (m_epRow >= 0 && m_epCol >= 0) {
        fen += QChar('a' + m_epCol);
        fen += QChar('8' - m_epRow);
    } else {
        fen += '-';
    }
    fen += ' ';
    fen += QString::number(m_halfmoveClock);
    fen += ' ';
    fen += QString::number(m_fullMoveNumber);
    return fen;
}

bool ChessBoard::isInBounds(int row, int col) const
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool ChessBoard::isSquareAttacked(int row, int col, Color byColor, const BoardType& board) const
{
    const int pawnDir = (byColor == Color::White) ? -1 : 1;
    for (int dc : {-1, 1}) {
        int pr = row + pawnDir;
        int pc = col + dc;
        if (isInBounds(pr, pc) && board[pr][pc].has_value()) {
            const auto& p = board[pr][pc].value();
            if (p.type == PieceType::Pawn && p.color == byColor)
                return true;
        }
    }

    const int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (int i = 0; i < 8; ++i) {
        int kr = row + knightMoves[i][0];
        int kc = col + knightMoves[i][1];
        if (isInBounds(kr, kc) && board[kr][kc].has_value()) {
            const auto& p = board[kr][kc].value();
            if (p.type == PieceType::Knight && p.color == byColor)
                return true;
        }
    }

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int kr = row + dr;
            int kc = col + dc;
            if (isInBounds(kr, kc) && board[kr][kc].has_value()) {
                const auto& p = board[kr][kc].value();
                if (p.type == PieceType::King && p.color == byColor)
                    return true;
            }
        }
    }

    const int diagDirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (int i = 0; i < 4; ++i) {
        int r = row + diagDirs[i][0];
        int c = col + diagDirs[i][1];
        while (isInBounds(r, c)) {
            if (board[r][c].has_value()) {
                const auto& p = board[r][c].value();
                if (p.color == byColor &&
                    (p.type == PieceType::Bishop || p.type == PieceType::Queen))
                    return true;
                break;
            }
            r += diagDirs[i][0];
            c += diagDirs[i][1];
        }
    }

    const int straightDirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int i = 0; i < 4; ++i) {
        int r = row + straightDirs[i][0];
        int c = col + straightDirs[i][1];
        while (isInBounds(r, c)) {
            if (board[r][c].has_value()) {
                const auto& p = board[r][c].value();
                if (p.color == byColor &&
                    (p.type == PieceType::Rook || p.type == PieceType::Queen))
                    return true;
                break;
            }
            r += straightDirs[i][0];
            c += straightDirs[i][1];
        }
    }

    return false;
}

bool ChessBoard::isSquareAttacked(int row, int col, Color byColor) const
{
    return isSquareAttacked(row, col, byColor, m_board);
}

bool ChessBoard::isInCheck() const
{
    return isInCheck(m_sideToMove);
}

bool ChessBoard::isInCheck(Color side) const
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (m_board[r][c].has_value() &&
                m_board[r][c]->type == PieceType::King &&
                m_board[r][c]->color == side) {
                return isSquareAttacked(r, c, otherColor(side));
            }
        }
    }
    return false;
}

bool ChessBoard::isMoveLegal(const Move& move) const
{
    BoardType copy = m_board;

    copy[move.toRow][move.toCol] = copy[move.fromRow][move.fromCol];
    copy[move.fromRow][move.fromCol] = std::nullopt;

    if (move.isEnPassant) {
        copy[move.fromRow][move.toCol] = std::nullopt;
    }

    if (move.isCastling) {
        if (move.castlingSide == Kingside) {
            copy[move.toRow][5] = copy[move.toRow][7];
            copy[move.toRow][7] = std::nullopt;
        } else {
            copy[move.toRow][3] = copy[move.toRow][0];
            copy[move.toRow][0] = std::nullopt;
        }
    }

    if (move.isPromotion) {
        copy[move.toRow][move.toCol] = Piece{move.promotion, move.color};
    }

    Color opp = otherColor(move.color);
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (copy[r][c].has_value() &&
                copy[r][c]->type == PieceType::King &&
                copy[r][c]->color == move.color) {
                return !isSquareAttacked(r, c, opp, copy);
            }
        }
    }
    return false;
}

QVector<ChessBoard::Move> ChessBoard::generatePseudoLegalMoves() const
{
    QVector<Move> moves;
    Color side = m_sideToMove;
    Color opp = otherColor(side);
    int pawnDir = (side == Color::White) ? -1 : 1;
    int pawnStart = (side == Color::White) ? 6 : 1;
    int promoRow = (side == Color::White) ? 0 : 7;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (!m_board[r][c].has_value() || m_board[r][c]->color != side)
                continue;

            PieceType pt = m_board[r][c]->type;

            if (pt == PieceType::Pawn) {
                int nr = r + pawnDir;
                if (isInBounds(nr, c) && !m_board[nr][c].has_value()) {
                    if (nr == promoRow) {
                        for (auto promo : {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight}) {
                            Move m;
                            m.fromRow = r; m.fromCol = c;
                            m.toRow = nr; m.toCol = c;
                            m.piece = PieceType::Pawn; m.color = side;
                            m.isPromotion = true; m.promotion = promo;
                            moves.push_back(m);
                        }
                    } else {
                        Move m;
                        m.fromRow = r; m.fromCol = c;
                        m.toRow = nr; m.toCol = c;
                        m.piece = PieceType::Pawn; m.color = side;
                        moves.push_back(m);

                        if (r == pawnStart) {
                            int nr2 = r + 2 * pawnDir;
                            if (!m_board[nr2][c].has_value()) {
                                Move m2;
                                m2.fromRow = r; m2.fromCol = c;
                                m2.toRow = nr2; m2.toCol = c;
                                m2.piece = PieceType::Pawn; m2.color = side;
                                moves.push_back(m2);
                            }
                        }
                    }
                }

                for (int dc : {-1, 1}) {
                    int nc = c + dc;
                    if (!isInBounds(nr, nc)) continue;
                    if (m_board[nr][nc].has_value() && m_board[nr][nc]->color == opp) {
                        if (nr == promoRow) {
                            for (auto promo : {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight}) {
                                Move m;
                                m.fromRow = r; m.fromCol = c;
                                m.toRow = nr; m.toCol = nc;
                                m.piece = PieceType::Pawn; m.color = side;
                                m.captured = m_board[nr][nc];
                                m.isPromotion = true; m.promotion = promo;
                                moves.push_back(m);
                            }
                        } else {
                            Move m;
                            m.fromRow = r; m.fromCol = c;
                            m.toRow = nr; m.toCol = nc;
                            m.piece = PieceType::Pawn; m.color = side;
                            m.captured = m_board[nr][nc];
                            moves.push_back(m);
                        }
                    }
                    if (nr == m_epRow && nc == m_epCol && m_epRow >= 0) {
                        Move m;
                        m.fromRow = r; m.fromCol = c;
                        m.toRow = nr; m.toCol = nc;
                        m.piece = PieceType::Pawn; m.color = side;
                        m.isEnPassant = true;
                        m.captured = Piece{PieceType::Pawn, opp};
                        moves.push_back(m);
                    }
                }
            } else if (pt == PieceType::Knight) {
                const int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                for (int i = 0; i < 8; ++i) {
                    int nr = r + offsets[i][0];
                    int nc = c + offsets[i][1];
                    if (!isInBounds(nr, nc)) continue;
                    if (!m_board[nr][nc].has_value() || m_board[nr][nc]->color == opp) {
                        Move m;
                        m.fromRow = r; m.fromCol = c;
                        m.toRow = nr; m.toCol = nc;
                        m.piece = PieceType::Knight; m.color = side;
                        if (m_board[nr][nc].has_value())
                            m.captured = m_board[nr][nc];
                        moves.push_back(m);
                    }
                }
            } else if (pt == PieceType::Bishop) {
                const int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
                for (int i = 0; i < 4; ++i) {
                    int nr = r + dirs[i][0];
                    int nc = c + dirs[i][1];
                    while (isInBounds(nr, nc)) {
                        if (!m_board[nr][nc].has_value()) {
                            Move m;
                            m.fromRow = r; m.fromCol = c;
                            m.toRow = nr; m.toCol = nc;
                            m.piece = PieceType::Bishop; m.color = side;
                            moves.push_back(m);
                        } else {
                            if (m_board[nr][nc]->color == opp) {
                                Move m;
                                m.fromRow = r; m.fromCol = c;
                                m.toRow = nr; m.toCol = nc;
                                m.piece = PieceType::Bishop; m.color = side;
                                m.captured = m_board[nr][nc];
                                moves.push_back(m);
                            }
                            break;
                        }
                        nr += dirs[i][0];
                        nc += dirs[i][1];
                    }
                }
            } else if (pt == PieceType::Rook) {
                const int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
                for (int i = 0; i < 4; ++i) {
                    int nr = r + dirs[i][0];
                    int nc = c + dirs[i][1];
                    while (isInBounds(nr, nc)) {
                        if (!m_board[nr][nc].has_value()) {
                            Move m;
                            m.fromRow = r; m.fromCol = c;
                            m.toRow = nr; m.toCol = nc;
                            m.piece = PieceType::Rook; m.color = side;
                            moves.push_back(m);
                        } else {
                            if (m_board[nr][nc]->color == opp) {
                                Move m;
                                m.fromRow = r; m.fromCol = c;
                                m.toRow = nr; m.toCol = nc;
                                m.piece = PieceType::Rook; m.color = side;
                                m.captured = m_board[nr][nc];
                                moves.push_back(m);
                            }
                            break;
                        }
                        nr += dirs[i][0];
                        nc += dirs[i][1];
                    }
                }
            } else if (pt == PieceType::Queen) {
                const int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
                for (int i = 0; i < 8; ++i) {
                    int nr = r + dirs[i][0];
                    int nc = c + dirs[i][1];
                    while (isInBounds(nr, nc)) {
                        if (!m_board[nr][nc].has_value()) {
                            Move m;
                            m.fromRow = r; m.fromCol = c;
                            m.toRow = nr; m.toCol = nc;
                            m.piece = PieceType::Queen; m.color = side;
                            moves.push_back(m);
                        } else {
                            if (m_board[nr][nc]->color == opp) {
                                Move m;
                                m.fromRow = r; m.fromCol = c;
                                m.toRow = nr; m.toCol = nc;
                                m.piece = PieceType::Queen; m.color = side;
                                m.captured = m_board[nr][nc];
                                moves.push_back(m);
                            }
                            break;
                        }
                        nr += dirs[i][0];
                        nc += dirs[i][1];
                    }
                }
            } else if (pt == PieceType::King) {
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr;
                        int nc = c + dc;
                        if (!isInBounds(nr, nc)) continue;
                        if (!m_board[nr][nc].has_value() || m_board[nr][nc]->color == opp) {
                            Move m;
                            m.fromRow = r; m.fromCol = c;
                            m.toRow = nr; m.toCol = nc;
                            m.piece = PieceType::King; m.color = side;
                            if (m_board[nr][nc].has_value())
                                m.captured = m_board[nr][nc];
                            moves.push_back(m);
                        }
                    }
                }

                if (!isInCheck(side)) {
                    if (m_wk && side == Color::White && r == 7 && c == 4) {
                        if (!m_board[7][5].has_value() && !m_board[7][6].has_value() &&
                            !isSquareAttacked(7, 5, Color::Black) && !isSquareAttacked(7, 6, Color::Black)) {
                            Move m;
                            m.fromRow = 7; m.fromCol = 4;
                            m.toRow = 7; m.toCol = 6;
                            m.piece = PieceType::King; m.color = side;
                            m.isCastling = true; m.castlingSide = Kingside;
                            moves.push_back(m);
                        }
                    }
                    if (m_wq && side == Color::White && r == 7 && c == 4) {
                        if (!m_board[7][3].has_value() && !m_board[7][2].has_value() && !m_board[7][1].has_value() &&
                            !isSquareAttacked(7, 3, Color::Black) && !isSquareAttacked(7, 2, Color::Black)) {
                            Move m;
                            m.fromRow = 7; m.fromCol = 4;
                            m.toRow = 7; m.toCol = 2;
                            m.piece = PieceType::King; m.color = side;
                            m.isCastling = true; m.castlingSide = Queenside;
                            moves.push_back(m);
                        }
                    }
                    if (m_bk && side == Color::Black && r == 0 && c == 4) {
                        if (!m_board[0][5].has_value() && !m_board[0][6].has_value() &&
                            !isSquareAttacked(0, 5, Color::White) && !isSquareAttacked(0, 6, Color::White)) {
                            Move m;
                            m.fromRow = 0; m.fromCol = 4;
                            m.toRow = 0; m.toCol = 6;
                            m.piece = PieceType::King; m.color = side;
                            m.isCastling = true; m.castlingSide = Kingside;
                            moves.push_back(m);
                        }
                    }
                    if (m_bq && side == Color::Black && r == 0 && c == 4) {
                        if (!m_board[0][3].has_value() && !m_board[0][2].has_value() && !m_board[0][1].has_value() &&
                            !isSquareAttacked(0, 3, Color::White) && !isSquareAttacked(0, 2, Color::White)) {
                            Move m;
                            m.fromRow = 0; m.fromCol = 4;
                            m.toRow = 0; m.toCol = 2;
                            m.piece = PieceType::King; m.color = side;
                            m.isCastling = true; m.castlingSide = Queenside;
                            moves.push_back(m);
                        }
                    }
                }
            }
        }
    }

    return moves;
}

QVector<ChessBoard::Move> ChessBoard::generateLegalMoves() const
{
    QVector<Move> pseudo = generatePseudoLegalMoves();
    QVector<Move> legal;
    legal.reserve(pseudo.size());
    for (const auto& m : pseudo) {
        if (isMoveLegal(m))
            legal.push_back(m);
    }
    return legal;
}

QVector<ChessBoard::Move> ChessBoard::generateMovesForPiece(int row, int col) const
{
    if (!isInBounds(row, col) || !m_board[row][col].has_value())
        return {};

    QVector<Move> all = generateLegalMoves();
    QVector<Move> result;
    for (const auto& m : all) {
        if (m.fromRow == row && m.fromCol == col)
            result.push_back(m);
    }
    return result;
}

QString ChessBoard::moveToNotation(const Move& move, bool givesCheck, bool givesCheckmate) const
{
    if (move.isCastling) {
        QString n = (move.castlingSide == Kingside) ? "O-O" : "O-O-O";
        if (givesCheckmate) n += "#";
        else if (givesCheck) n += "+";
        return n;
    }

    QString n;

    if (move.piece != PieceType::Pawn) {
        switch (move.piece) {
            case PieceType::King:   n = "K"; break;
            case PieceType::Queen:  n = "Q"; break;
            case PieceType::Rook:   n = "R"; break;
            case PieceType::Bishop: n = "B"; break;
            case PieceType::Knight: n = "N"; break;
            default: break;
        }
    }

    for (int r2 = 0; r2 < 8; ++r2) {
        for (int c2 = 0; c2 < 8; ++c2) {
            if (r2 == move.fromRow && c2 == move.fromCol) continue;
            if (!m_board[r2][c2].has_value()) continue;
            const auto& p = m_board[r2][c2].value();
            if (p.type == move.piece && p.color == move.color) {
                Move dummy = move;
                dummy.fromRow = r2; dummy.fromCol = c2;
                if (isMoveLegal(dummy) &&
                    !(dummy.isCastling || dummy.isEnPassant)) {
                    if (c2 != move.fromCol) {
                        n += QChar('a' + move.fromCol);
                    } else if (r2 != move.fromRow) {
                        n += QChar('8' - move.fromRow);
                    }
                    break;
                }
            }
        }
    }

    if (move.captured.has_value()) {
        if (move.piece == PieceType::Pawn)
            n += QChar('a' + move.fromCol);
        n += 'x';
    }

    n += QChar('a' + move.toCol);
    n += QChar('8' - move.toRow);

    if (move.isPromotion) {
        switch (move.promotion) {
            case PieceType::Queen:  n += "=Q"; break;
            case PieceType::Rook:   n += "=R"; break;
            case PieceType::Bishop: n += "=B"; break;
            case PieceType::Knight: n += "=N"; break;
            default: break;
        }
    }

    if (givesCheckmate) n += "#";
    else if (givesCheck) n += "+";

    return n;
}

void ChessBoard::makeMove(const Move& move)
{
    m_board[move.toRow][move.toCol] = m_board[move.fromRow][move.fromCol];
    m_board[move.fromRow][move.fromCol] = std::nullopt;

    if (move.isEnPassant) {
        m_board[move.fromRow][move.toCol] = std::nullopt;
    }

    if (move.isCastling) {
        m_wk = m_wq = m_bk = m_bq = false;
        if (move.castlingSide == Kingside) {
            m_board[move.toRow][5] = m_board[move.toRow][7];
            m_board[move.toRow][7] = std::nullopt;
        } else {
            m_board[move.toRow][3] = m_board[move.toRow][0];
            m_board[move.toRow][0] = std::nullopt;
        }
    }

    if (move.isPromotion) {
        m_board[move.toRow][move.toCol] = Piece{move.promotion, move.color};
    }

    if (move.piece == PieceType::Pawn) {
        int pawnStart = (move.color == Color::White) ? 6 : 1;
        int pawnDir = (move.color == Color::White) ? -1 : 1;
        if (move.fromRow == pawnStart && move.toRow == move.fromRow + 2 * pawnDir) {
            m_epRow = move.fromRow + pawnDir;
            m_epCol = move.fromCol;
        } else {
            m_epRow = m_epCol = -1;
        }
    } else {
        m_epRow = m_epCol = -1;
    }

    if (move.piece == PieceType::King) {
        if (move.color == Color::White) { m_wk = false; m_wq = false; }
        else { m_bk = false; m_bq = false; }
    }
    if (move.piece == PieceType::Rook) {
        if (move.fromCol == 0 && move.fromRow == 7) m_wq = false;
        else if (move.fromCol == 7 && move.fromRow == 7) m_wk = false;
        else if (move.fromCol == 0 && move.fromRow == 0) m_bq = false;
        else if (move.fromCol == 7 && move.fromRow == 0) m_bk = false;
    }
    if (move.captured.has_value() && move.captured->type == PieceType::Rook) {
        if (move.toCol == 0 && move.toRow == 7) m_wq = false;
        else if (move.toCol == 7 && move.toRow == 7) m_wk = false;
        else if (move.toCol == 0 && move.toRow == 0) m_bq = false;
        else if (move.toCol == 7 && move.toRow == 0) m_bk = false;
    }

    if (move.piece == PieceType::Pawn || move.captured.has_value())
        m_halfmoveClock = 0;
    else
        ++m_halfmoveClock;

    m_sideToMove = otherColor(m_sideToMove);
    if (m_sideToMove == Color::White)
        ++m_fullMoveNumber;

    bool givesCheck = isInCheck(m_sideToMove);
    bool givesCheckmate = false;
    bool stale = false;

    if (givesCheck) {
        givesCheckmate = (generateLegalMoves().isEmpty());
    } else {
        stale = (generateLegalMoves().isEmpty());
    }

    Move recorded = move;
    recorded.notation = moveToNotation(move, givesCheck, givesCheckmate);
    m_moveHistory.push_back(recorded);

    if (givesCheckmate || stale)
        m_gameOver = true;
}

QStringList ChessBoard::notationHistory() const
{
    QStringList lines;
    for (int i = 0; i < m_moveHistory.size(); i += 2) {
        int moveNum = (i / 2) + 1;
        QString line = QString::number(moveNum) + ". " + m_moveHistory[i].notation;
        if (i + 1 < m_moveHistory.size())
            line += "  " + m_moveHistory[i + 1].notation;
        lines.append(line);
    }
    return lines;
}

bool ChessBoard::isCheckmate() const
{
    return isInCheck(m_sideToMove) && generateLegalMoves().isEmpty();
}

bool ChessBoard::isStalemate() const
{
    return !isInCheck(m_sideToMove) && generateLegalMoves().isEmpty();
}
