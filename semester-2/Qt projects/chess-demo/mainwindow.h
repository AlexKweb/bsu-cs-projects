#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QVector>
#include "chessboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onCellClicked();
    void onNewGame();
    void onLoadFen();
    void onSaveFen();
    void onUndoMove();

private:
    void setupBoard();
    void updateBoard();
    void updateHistory();
    void updateStatus();
    void selectPiece(int row, int col);
    void deselectPiece();
    void executeMove(const ChessBoard::Move& move);
    QString cellStyle(int row, int col, bool isLegalTarget, bool isCaptureTarget) const;

    QPushButton* m_cells[8][8] = {};
    QListWidget* m_historyList = nullptr;
    ChessBoard m_board;
    int m_selectedRow = -1;
    int m_selectedCol = -1;
    QVector<ChessBoard::Move> m_legalMoves;
    int m_lastFromRow = -1, m_lastFromCol = -1;
    int m_lastToRow = -1, m_lastToCol = -1;
};

#endif // MAINWINDOW_H
