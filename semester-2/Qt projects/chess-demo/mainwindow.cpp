#include "mainwindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QFont>
#include <QClipboard>
#include <QApplication>

static const char* LIGHT_COLOR = "#f0d9b5";
static const char* DARK_COLOR  = "#b58863";
static const char* SELECTED_COLOR = "#ffd700";
static const char* LEGAL_COLOR  = "#90ee90";
static const char* CAPTURE_COLOR = "#ff9999";
static const char* LAST_MOVE_COLOR = "#add8e6";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Chess Demo");
    setMinimumSize(750, 600);

    auto *menuBar = new QMenuBar(this);
    auto *gameMenu = menuBar->addMenu("&Game");
    gameMenu->addAction("&New Game", QKeySequence::New, this, &MainWindow::onNewGame);
    gameMenu->addAction("&Load FEN...", this, &MainWindow::onLoadFen);
    gameMenu->addAction("&Save FEN", this, &MainWindow::onSaveFen);
    gameMenu->addSeparator();
    gameMenu->addAction("&Undo Move", QKeySequence::Undo, this, &MainWindow::onUndoMove);
    setMenuBar(menuBar);

    auto *central = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    auto *boardContainer = new QWidget;
    auto *boardLayout = new QGridLayout(boardContainer);
    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    setupBoard();

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            boardLayout->addWidget(m_cells[r][c], r, c);

    QFont labelFont("Helvetica", 11, QFont::Bold);
    for (int r = 0; r < 8; ++r) {
        auto *label = new QLabel(QString::number(8 - r));
        label->setFont(labelFont);
        label->setAlignment(Qt::AlignCenter);
        label->setFixedSize(24, 64);
        boardLayout->addWidget(label, r, 8);
    }
    for (int c = 0; c < 8; ++c) {
        auto *label = new QLabel(QChar('a' + c));
        label->setFont(labelFont);
        label->setAlignment(Qt::AlignCenter);
        label->setFixedSize(64, 24);
        boardLayout->addWidget(label, 8, c);
    }

    auto *rightPanel = new QWidget;
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(8);

    auto *headerLabel = new QLabel("History");
    headerLabel->setFont(QFont("Helvetica", 14, QFont::Bold));
    rightLayout->addWidget(headerLabel);

    m_historyList = new QListWidget;
    m_historyList->setFont(QFont("Monaco", 11));
    m_historyList->setMinimumWidth(200);
    m_historyList->setMaximumWidth(250);
    rightLayout->addWidget(m_historyList);

    mainLayout->addWidget(boardContainer);
    mainLayout->addWidget(rightPanel);

    setCentralWidget(central);

    statusBar()->showMessage("White to move");

    updateBoard();
    updateHistory();
}

MainWindow::~MainWindow() {}

void MainWindow::setupBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto *btn = new QPushButton;
            btn->setFixedSize(64, 64);
            btn->setFont(QFont("Helvetica", 28));
            btn->setCursor(Qt::PointingHandCursor);
            btn->setFocusPolicy(Qt::NoFocus);
            btn->setProperty("row", r);
            btn->setProperty("col", c);
            connect(btn, &QPushButton::clicked, this, &MainWindow::onCellClicked);
            m_cells[r][c] = btn;
        }
    }
}

QString MainWindow::cellStyle(int row, int col, bool isLegalTarget, bool isCaptureTarget) const
{
    bool isLight = ChessBoard::isLightSquare(row, col);
    QString base = isLight ? LIGHT_COLOR : DARK_COLOR;

    bool isSelected = (row == m_selectedRow && col == m_selectedCol);
    bool isLastMove = (row == m_lastFromRow && col == m_lastFromCol) ||
                      (row == m_lastToRow && col == m_lastToCol);

    QString bgColor;
    if (isSelected)
        bgColor = SELECTED_COLOR;
    else if (isCaptureTarget)
        bgColor = CAPTURE_COLOR;
    else if (isLegalTarget)
        bgColor = LEGAL_COLOR;
    else if (isLastMove)
        bgColor = LAST_MOVE_COLOR;
    else
        bgColor = base;

    bool textLight = !isLight;
    QString textColor = textLight ? "#ffffff" : "#000000";

    if (isSelected || isLegalTarget || isCaptureTarget)
        textColor = "#000000";

    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  border-radius: 0px;"
        "  font-size: 30px;"
        "}"
    ).arg(bgColor, textColor, base);
}

void MainWindow::updateBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto *btn = m_cells[r][c];
            const auto& piece = m_board.at(r, c);

            if (piece.has_value())
                btn->setText(QString(pieceToUnicode(piece->type, piece->color)));
            else
                btn->setText("");

            btn->setStyleSheet(cellStyle(r, c, false, false));
        }
    }
    updateStatus();
}

void MainWindow::updateHistory()
{
    m_historyList->clear();
    QStringList notation = m_board.notationHistory();
    for (const auto& line : notation)
        m_historyList->addItem(line);
    if (!notation.isEmpty())
        m_historyList->scrollToBottom();
}

void MainWindow::updateStatus()
{
    if (m_board.gameOver()) {
        if (m_board.isCheckmate())
            statusBar()->showMessage("Checkmate! " +
                QString(m_board.sideToMove() == Color::White ? "Black" : "White") + " wins!");
        else if (m_board.isStalemate())
            statusBar()->showMessage("Stalemate! Game is a draw.");
        return;
    }

    QString msg = (m_board.sideToMove() == Color::White) ? "White" : "Black";
    msg += " to move";

    if (m_board.isInCheck())
        msg += " (check!)";

    statusBar()->showMessage(msg);
}

void MainWindow::onCellClicked()
{
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    if (m_board.gameOver()) {
        deselectPiece();
        return;
    }

    const auto& piece = m_board.at(row, col);

    if (m_selectedRow == -1) {
        if (piece.has_value() && piece->color == m_board.sideToMove())
            selectPiece(row, col);
    } else {
        if (piece.has_value() && piece->color == m_board.sideToMove()) {
            if (row == m_selectedRow && col == m_selectedCol) {
                deselectPiece();
            } else {
                deselectPiece();
                selectPiece(row, col);
            }
        } else {
            for (const auto& move : m_legalMoves) {
                if (move.toRow == row && move.toCol == col) {
                    executeMove(move);
                    return;
                }
            }
            deselectPiece();
        }
    }
}

void MainWindow::selectPiece(int row, int col)
{
    m_selectedRow = row;
    m_selectedCol = col;
    m_legalMoves = m_board.generateMovesForPiece(row, col);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            bool isLegal = false;
            bool isCapture = false;
            for (const auto& m : m_legalMoves) {
                if (m.toRow == r && m.toCol == c) {
                    isLegal = true;
                    isCapture = m.captured.has_value();
                    break;
                }
            }
            m_cells[r][c]->setStyleSheet(cellStyle(r, c, isLegal, isCapture));
        }
    }
}

void MainWindow::deselectPiece()
{
    m_selectedRow = -1;
    m_selectedCol = -1;
    m_legalMoves.clear();

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            m_cells[r][c]->setStyleSheet(cellStyle(r, c, false, false));
        }
    }
}

void MainWindow::executeMove(const ChessBoard::Move& move)
{
    m_lastFromRow = move.fromRow;
    m_lastFromCol = move.fromCol;
    m_lastToRow = move.toRow;
    m_lastToCol = move.toCol;

    m_board.makeMove(move);

    deselectPiece();
    updateBoard();
    updateHistory();
    updateStatus();
}

void MainWindow::onNewGame()
{
    deselectPiece();
    m_board.setupInitial();
    m_lastFromRow = m_lastFromCol = m_lastToRow = m_lastToCol = -1;
    updateBoard();
    updateHistory();
    updateStatus();
}

void MainWindow::onLoadFen()
{
    bool ok;
    QString fen = QInputDialog::getText(this,
        "Load FEN",
        "Enter FEN string:",
        QLineEdit::Normal,
        m_board.toFen(),
        &ok);

    if (!ok || fen.isEmpty()) return;

    ChessBoard test;
    if (!test.fromFen(fen)) {
        QMessageBox::warning(this, "Error", "Invalid FEN string.");
        return;
    }

    deselectPiece();
    m_board = test;
    m_lastFromRow = m_lastFromCol = m_lastToRow = m_lastToCol = -1;
    updateBoard();
    updateHistory();
    updateStatus();
}

void MainWindow::onSaveFen()
{
    QString fen = m_board.toFen();
    QApplication::clipboard()->setText(fen);
    statusBar()->showMessage("FEN copied to clipboard: " + fen, 5000);
}

void MainWindow::onUndoMove()
{
    QMessageBox::information(this, "Undo",
        "Undo is not implemented in this demo.\n"
        "Use 'Load FEN' to restore a position.");
}
