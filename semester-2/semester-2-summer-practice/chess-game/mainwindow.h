#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "chessboard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNewGame();
    void onSaveGame();
    void onLoadGame();

private:
    void onSquareClicked(int row, int col);

private:
    void setupBoard();
    void updateBoard();
    void resetSelection();
    void setStatus(const QString &text);

    Ui::MainWindow *ui;
    Board *board;
    QPushButton *boardButtons[8][8] = {};
    int selectedRow = -1;
    int selectedCol = -1;
};

#endif
