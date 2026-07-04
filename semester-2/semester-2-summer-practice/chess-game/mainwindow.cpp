#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

static const char *LIGHT = "rgb(240, 218, 181)";
static const char *DARK  = "rgb(180, 134, 99)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , board(new Board(this))
{
    ui->setupUi(this);
    setupBoard();
    board->setup();
    updateBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            QString name = QString("%1%2").arg(QChar('A' + c)).arg(8 - r);
            QPushButton *btn = findChild<QPushButton*>(name);
            if (!btn) continue;
            boardButtons[r][c] = btn;
            connect(btn, &QPushButton::clicked, this, [this, r, c]() {
                onSquareClicked(r, c);
            });
        }
    }

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onSaveGame);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::onLoadGame);
}

void MainWindow::onSquareClicked(int row, int col)
{
    const Figure *f = board->getFigure(row, col);

    if (selectedRow == -1) {
        // Nothing selected yet
        if (f && f->getColor() == board->getCurrentTurn()) {
            selectedRow = row;
            selectedCol = col;
            updateBoard();
        }
        return;
    }

    // Already have a selection
    if (row == selectedRow && col == selectedCol) {
        resetSelection();
        return;
    }

    // Click on own piece -> re-select
    if (f && f->getColor() == board->getCurrentTurn()) {
        selectedRow = row;
        selectedCol = col;
        updateBoard();
        return;
    }

    // Try to move
    if (board->moveFigure(selectedRow, selectedCol, row, col)) {
        resetSelection();
        updateBoard();
    } else {
        // Invalid move — just deselect
        resetSelection();
        updateBoard();
    }
}

void MainWindow::onNewGame()
{
    board->setup();
    resetSelection();
    updateBoard();
    setStatus("New game — White's turn");
}

void MainWindow::onSaveGame()
{
    QString path = QFileDialog::getSaveFileName(this, "Save position", "",
                                                "Position files (*.txt)");
    if (path.isEmpty()) return;
    if (!path.endsWith(".txt", Qt::CaseInsensitive))
        path += ".txt";
    if (board->saveToFile(path))
        setStatus("Position saved");
    else
        setStatus("Failed to save position");
}

void MainWindow::onLoadGame()
{
    QString path = QFileDialog::getOpenFileName(this, "Load position", "",
                                                "Position files (*.txt)");
    if (path.isEmpty()) return;
    if (board->loadFromFile(path)) {
        resetSelection();
        updateBoard();
        setStatus("Position loaded");
    } else {
        setStatus("Failed to load position");
    }
}

void MainWindow::resetSelection()
{
    selectedRow = -1;
    selectedCol = -1;
}

void MainWindow::updateBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            QPushButton *btn = boardButtons[r][c];
            if (!btn) continue;

            const Figure *f = board->getFigure(r, c);
            bool isLight = (r + c) % 2 == 0;
            QString bg = isLight ? LIGHT : DARK;

            btn->setText(f ? QString(f->symbol()) : QString());

            if (r == selectedRow && c == selectedCol) {
                QString selBg = isLight ? "rgb(255, 235, 150)" : "rgb(220, 190, 100)";
                QString textColor = (f && f->getColor() == Color::White) ? "#ffffff" : "#000000";
                btn->setStyleSheet(QString(
                    "background-color: %1; color: %2; border: none; font-size: 48px;")
                    .arg(selBg).arg(textColor));
                continue;
            }

            if (f) {
                QString textColor = (f->getColor() == Color::White) ? "#ffffff" : "#000000";
                btn->setStyleSheet(QString(
                    "background-color: %1; color: %2; border: none; font-size: 48px;")
                    .arg(bg).arg(textColor));
            } else {
                btn->setStyleSheet(QString(
                    "background-color: %1; color: #000000; border: none; font-size: 48px;")
                    .arg(bg));
            }
        }
    }

    Color turn = board->getCurrentTurn();
    if (board->isCheckmate(turn)) {
        QString winner = (turn == Color::White) ? "Black" : "White";
        setStatus(QString("Checkmate! %1 wins!").arg(winner));
        QMessageBox::information(this, "Checkmate",
                                 QString("Checkmate!\n%1 wins!").arg(winner));
    } else if (board->isStalemate(turn)) {
        setStatus("Stalemate — draw");
        QMessageBox::information(this, "Stalemate", "Stalemate — it's a draw!");
    } else if (board->isInCheck(turn)) {
        QString color = (turn == Color::White) ? "White" : "Black";
        setStatus(QString("Check! %1's turn").arg(color));
    } else {
        QString color = (turn == Color::White) ? "White" : "Black";
        setStatus(QString("%1's turn").arg(color));
    }
}

void MainWindow::setStatus(const QString &text)
{
    statusBar()->showMessage(text);
}
