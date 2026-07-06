#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

#include "chessboard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Move
{
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNewGame();
    void onSavePosition();
    void onLoadPosition();
    void onTimerToggle();
    void onTimerTick();
    void onSaveGameHistory();
    void onLoadGameHistory();
    void onNextMove();
    void onPreviousMove();

private:
    void onSquareClicked(int row, int col);
    void setupBoard();
    void updateBoard();
    void resetSelection();
    void setStatus(const QString &text);

    void updateTimerLabels();
    void resetTimers();
    void stopTimer();
    void startTimer();

    static QString moveToString(const Move &m);
    static bool stringToMove(const QString &s, Move &m);

    Ui::MainWindow *ui;
    Board *board;
    QPushButton *boardButtons[8][8] = {};
    int selectedRow = -1;
    int selectedCol = -1;

    QVector<Move> moveHistory;
    QVector<Move> replayMoves;
    int replayIndex = 0;
    bool replayMode = false;

    QTimer *chessTimer = nullptr;
    int whiteSeconds = 600;
    int blackSeconds = 600;
    bool timerRunning = false;
};

#endif
