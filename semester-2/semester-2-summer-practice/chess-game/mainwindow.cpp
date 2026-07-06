#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTime>

static const char *LIGHT = "rgb(240, 218, 181)";
static const char *DARK  = "rgb(180, 134, 99)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , board(new Board(this))
{
    ui->setupUi(this);

    ui->timeEdit->setDisplayFormat("mm:ss");
    ui->timeEdit->setTime(QTime(0, 10, 0));
    resetTimers();
    updateTimerLabels();

    chessTimer = new QTimer(this);
    connect(chessTimer, &QTimer::timeout, this, &MainWindow::onTimerTick);

    setupBoard();
    board->setup();
    updateBoard();

    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    updateMoveHistoryList();
}

MainWindow::~MainWindow() { delete ui; }

// ─── Board setup ───────────────────────────────────────────────────

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

    connect(ui->pushButton,   &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onSavePosition);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::onLoadPosition);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onTimerToggle);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::onLoadGameHistory);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::onSaveGameHistory);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::onNextMove);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::onPreviousMove);
}

// ─── Board clicks ──────────────────────────────────────────────────

void MainWindow::onSquareClicked(int row, int col)
{
    if (replayMode)
        return;

    const Figure *f = board->getFigure(row, col);

    if (selectedRow == -1) {
        if (f && f->getColor() == board->getCurrentTurn()) {
            selectedRow = row;
            selectedCol = col;
            updateBoard();
        }
        return;
    }

    if (row == selectedRow && col == selectedCol) {
        resetSelection();
        updateBoard();
        return;
    }

    if (f && f->getColor() == board->getCurrentTurn()) {
        selectedRow = row;
        selectedCol = col;
        updateBoard();
        return;
    }

    Type promotionType = Type::Queen;
    if (const Figure *selected = board->getFigure(selectedRow, selectedCol)) {
        if (selected->getType() == Type::Pawn && (row == 0 || row == 7))
            promotionType = choosePromotionType();
    }

    const Figure *moving = board->getFigure(selectedRow, selectedCol);
    Type pieceType = moving ? moving->getType() : Type::Pawn;
    if (board->moveFigure(selectedRow, selectedCol, row, col, promotionType)) {
        moveHistory.append(Move{selectedRow, selectedCol, row, col, pieceType, promotionType});
        resetSelection();
        updateMoveHistoryList();
        updateBoard();
    } else {
        resetSelection();
        updateBoard();
    }
}

// ─── New Game ──────────────────────────────────────────────────────

void MainWindow::onNewGame()
{
    board->setup();
    moveHistory.clear();
    replayMoves.clear();
    replayIndex = 0;
    replayMode = false;
    resetSelection();

    resetTimers();
    stopTimer();
    timerRunning = false;
    ui->pushButton_3->setText("Таймер");
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    updateMoveHistoryList();

    updateBoard();
    setStatus("Новая партия — ход белых");
}

// ─── Save / Load position ──────────────────────────────────────────

void MainWindow::onSavePosition()
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

void MainWindow::onLoadPosition()
{
    QString path = QFileDialog::getOpenFileName(this, "Load position", "",
                                                "Position files (*.txt)");
    if (path.isEmpty()) return;
    if (board->loadFromFile(path)) {
        resetSelection();
        moveHistory.clear();
        replayMoves.clear();
        replayIndex = 0;
        replayMode = false;
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        updateMoveHistoryList();
        updateBoard();
        setStatus("Position loaded");
    } else {
        setStatus("Failed to load position");
    }
}

// ─── Timer ─────────────────────────────────────────────────────────

void MainWindow::onTimerToggle()
{
    if (replayMode)
        return;

    timerRunning = !timerRunning;
    if (timerRunning) {
        startTimer();
        ui->pushButton_3->setText("Стоп");
    } else {
        stopTimer();
        ui->pushButton_3->setText("Таймер");
    }
}

void MainWindow::onTimerTick()
{
    if (board->getCurrentTurn() == Color::White)
        whiteSeconds = qMax(0, whiteSeconds - 1);
    else
        blackSeconds = qMax(0, blackSeconds - 1);

    updateTimerLabels();

    if (whiteSeconds == 0) {
        stopTimer();
        timerRunning = false;
        ui->pushButton_3->setText("Таймер");
        QMessageBox::information(this, "Time", "Время белых вышло!\nЧёрные победили!");
        setStatus("Время белых вышло — победили чёрные");
    } else if (blackSeconds == 0) {
        stopTimer();
        timerRunning = false;
        ui->pushButton_3->setText("Таймер");
        QMessageBox::information(this, "Time", "Время чёрных вышло!\nБелые победили!");
        setStatus("Время чёрных вышло — победили белые");
    }
}

void MainWindow::updateTimerLabels()
{
    auto fmt = [](int s) {
        return QString("%1:%2")
            .arg(s / 60, 2, 10, QChar('0'))
            .arg(s % 60, 2, 10, QChar('0'));
    };
    ui->labelWhiteTimer->setText(fmt(whiteSeconds));
    ui->labelBlackTimer->setText(fmt(blackSeconds));
}

void MainWindow::resetTimers()
{
    QTime t = ui->timeEdit->time();
    whiteSeconds = t.minute() * 60 + t.second();
    blackSeconds = whiteSeconds;
    updateTimerLabels();
}

void MainWindow::startTimer()
{
    chessTimer->start(1000);
}

void MainWindow::stopTimer()
{
    chessTimer->stop();
}

// ─── Save / Load game history ──────────────────────────────────────

void MainWindow::onSaveGameHistory()
{
    if (moveHistory.isEmpty()) {
        setStatus("Нет ходов для сохранения");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Save game", "",
                                                "Chess game (*.txt)");
    if (path.isEmpty()) return;
    if (!path.endsWith(".txt", Qt::CaseInsensitive))
        path += ".txt";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setStatus("Failed to save game");
        return;
    }

    QTextStream out(&file);
    for (const Move &m : moveHistory)
        out << moveToString(m) << '\n';
    file.close();
    setStatus("Партия сохранена");
}

void MainWindow::onLoadGameHistory()
{
    QString path = QFileDialog::getOpenFileName(this, "Load game", "",
                                                "Chess game (*.txt)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStatus("Failed to load game");
        return;
    }

    replayMoves.clear();
    replayIndex = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        Move m;
        if (stringToMove(line, m))
            replayMoves.append(m);
    }
    file.close();

    if (replayMoves.isEmpty()) {
        setStatus("Нет валидных ходов в файле");
        return;
    }

    board->setup();
    moveHistory.clear();
    resetSelection();
    replayMode = true;

    stopTimer();
    timerRunning = false;
    resetTimers();
    ui->pushButton_3->setText("Таймер");

    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(false);

    updateMoveHistoryList();
    updateBoard();
    setStatus(QString("Воспроизведение — ход 1 из %1").arg(replayMoves.size()));
}

// ─── Replay: Next / Previous ───────────────────────────────────────

void MainWindow::onNextMove()
{
    if (!replayMode)
        return;
    if (replayIndex >= replayMoves.size())
        return;

    const Move &m = replayMoves[replayIndex];
    const Figure *moving = board->getFigure(m.fromRow, m.fromCol);
    Type pieceType = moving ? moving->getType() : Type::Pawn;
    if (!board->moveFigure(m.fromRow, m.fromCol, m.toRow, m.toCol, m.promotionType)) {
        setStatus(QString("Невалидный ход в файле: %1").arg(moveToString(m)));
        return;
    }

    Move applied = m;
    applied.pieceType = pieceType;
    moveHistory.append(applied);
    replayIndex++;

    // Clear board selection so the view is clean
    resetSelection();
    updateBoard();
    updateMoveHistoryList();

    bool finished = (replayIndex >= replayMoves.size());
    ui->pushButton_8->setEnabled(true);
    ui->pushButton_7->setEnabled(!finished);

    if (finished) {
        setStatus("Воспроизведение завершено");
    } else {
        setStatus(QString("Воспроизведение — ход %1 из %2")
                      .arg(replayIndex + 1).arg(replayMoves.size()));
    }
}

void MainWindow::onPreviousMove()
{
    if (replayIndex <= 0)
        return;

    replayIndex--;
    board->undoMove();
    if (!moveHistory.isEmpty())
        moveHistory.removeLast();
    resetSelection();
    updateBoard();
    updateMoveHistoryList();

    ui->pushButton_8->setEnabled(replayIndex > 0);
    ui->pushButton_7->setEnabled(true);

    setStatus(QString("Воспроизведение — ход %1 из %2")
                  .arg(replayIndex + 1).arg(replayMoves.size()));
}

// ─── Selection helpers ─────────────────────────────────────────────

void MainWindow::resetSelection()
{
    selectedRow = -1;
    selectedCol = -1;
}

void MainWindow::updateBoard()
{
    QVector<QPair<int,int>> validMoves;
    if (selectedRow != -1)
        validMoves = board->getValidMoves(selectedRow, selectedCol);

    bool hasLastMove = !moveHistory.isEmpty();
    Move lastMove;
    if (hasLastMove)
        lastMove = moveHistory.last();

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            QPushButton *btn = boardButtons[r][c];
            if (!btn) continue;

            const Figure *f = board->getFigure(r, c);
            bool isLight = (r + c) % 2 == 0;
            QString bg = isLight ? LIGHT : DARK;
            QString border = "none";
            bool isSelected = (r == selectedRow && c == selectedCol);
            bool isMoveTarget = isHighlightedMove(r, c, validMoves);
            bool isLastMoveSquare = hasLastMove
                && ((r == lastMove.fromRow && c == lastMove.fromCol)
                    || (r == lastMove.toRow && c == lastMove.toCol));

            btn->setText(f ? QString(f->symbol()) : QString());

            if (isLastMoveSquare) {
                bg = isLight ? "rgb(178, 205, 235)" : "rgb(130, 165, 205)";
                border = "2px solid rgb(60, 105, 155)";
            }

            if (isMoveTarget) {
                if (f) {
                    bg = isLight ? "rgb(235, 145, 125)" : "rgb(205, 105, 90)";
                    border = "3px solid rgb(150, 55, 45)";
                } else {
                    bg = isLight ? "rgb(178, 220, 145)" : "rgb(130, 178, 105)";
                    border = "3px solid rgb(70, 130, 65)";
                }
            }

            if (isSelected) {
                bg = isLight ? "rgb(255, 235, 150)" : "rgb(220, 190, 100)";
                border = "3px solid rgb(150, 115, 35)";
            }

            QString textColor = (f && f->getColor() == Color::White) ? "#ffffff" : "#000000";
            btn->setStyleSheet(QString(
                "background-color: %1; color: %2; border: %3; font-size: 48px;")
                .arg(bg).arg(textColor).arg(border));
        }
    }

    Color turn = board->getCurrentTurn();
    bool gameOver = false;

    if (board->isCheckmate(turn)) {
        QString winner = (turn == Color::White) ? "Чёрные" : "Белые";
        stopTimer();
        timerRunning = false;
        ui->pushButton_3->setText("Таймер");
        setStatus(QString("Мат! %1 победили!").arg(winner));
        QMessageBox::information(this, "Мат",
                                 QString("Мат!\n%1 победили!").arg(winner));
        gameOver = true;
    } else if (board->isStalemate(turn)) {
        stopTimer();
        timerRunning = false;
        ui->pushButton_3->setText("Таймер");
        setStatus("Пат — ничья");
        QMessageBox::information(this, "Пат", "Пат — ничья!");
        gameOver = true;
    } else if (board->isInCheck(turn)) {
        setStatus(QString("%1 под шахом").arg(turn == Color::White ? "Белые" : "Чёрные"));
    } else {
        setStatus(QString("Ход %1").arg(turn == Color::White ? "белых" : "чёрных"));
    }

    if (!gameOver && timerRunning) {
        startTimer();
    }
}

void MainWindow::setStatus(const QString &text)
{
    statusBar()->showMessage(text);
}

bool MainWindow::isHighlightedMove(int row, int col, const QVector<QPair<int,int>> &moves) const
{
    for (const auto &move : moves)
        if (move.first == row && move.second == col)
            return true;
    return false;
}

Type MainWindow::choosePromotionType()
{
    QStringList items;
    items << "Ферзь" << "Ладья" << "Слон" << "Конь";
    bool ok = false;
    QString selected = QInputDialog::getItem(this, "Превращение пешки",
                                             "Выберите фигуру:", items, 0, false, &ok);
    if (!ok)
        return Type::Queen;
    if (selected == "Ладья") return Type::Rook;
    if (selected == "Слон") return Type::Bishop;
    if (selected == "Конь") return Type::Knight;
    return Type::Queen;
}

void MainWindow::updateMoveHistoryList()
{
    ui->historyList->clear();
    for (int i = 0; i < moveHistory.size(); ++i)
        ui->historyList->addItem(moveDescription(moveHistory[i], i));
    if (!moveHistory.isEmpty())
        ui->historyList->scrollToBottom();
}

// ─── Helpers: move ↔ string ───────────────────────────────────────

QString MainWindow::moveToString(const Move &m)
{
    auto cell = [](int row, int col) {
        return QString(QChar('a' + col)) + QString::number(8 - row);
    };
    QString text = cell(m.fromRow, m.fromCol) + cell(m.toRow, m.toCol);
    if (m.pieceType == Type::Pawn && (m.toRow == 0 || m.toRow == 7))
        text += promotionChar(m.promotionType);
    return text;
}

bool MainWindow::stringToMove(const QString &s, Move &m)
{
    if (s.size() != 4 && s.size() != 5) return false;

    QString move = s.toLower();
    int fc = move[0].toLatin1() - 'a';
    int fr = move[1].digitValue();
    int tc = move[2].toLatin1() - 'a';
    int tr = move[3].digitValue();

    if (fc < 0 || fc > 7 || tc < 0 || tc > 7) return false;
    if (fr < 1 || fr > 8 || tr < 1 || tr > 8) return false;

    m.fromCol = fc; m.fromRow = 8 - fr;
    m.toCol   = tc; m.toRow   = 8 - tr;
    if (move.size() == 5)
        m.promotionType = promotionTypeFromChar(move[4]);
    return true;
}

QString MainWindow::moveDescription(const Move &m, int index)
{
    QString prefix = (index % 2 == 0)
        ? QString("%1. ").arg(index / 2 + 1)
        : QString("... ");
    QString suffix;
    if (m.pieceType == Type::Pawn && (m.toRow == 0 || m.toRow == 7))
        suffix = QString("=%1").arg(pieceName(m.promotionType));
    return prefix + pieceName(m.pieceType) + " " + moveToString(m).left(4).insert(2, "-") + suffix;
}

QString MainWindow::pieceName(Type type)
{
    switch (type) {
    case Type::Pawn: return "Пешка";
    case Type::Rook: return "Ладья";
    case Type::Knight: return "Конь";
    case Type::Bishop: return "Слон";
    case Type::Queen: return "Ферзь";
    case Type::King: return "Король";
    }
    return "Фигура";
}

QChar MainWindow::promotionChar(Type type)
{
    switch (type) {
    case Type::Rook: return 'r';
    case Type::Bishop: return 'b';
    case Type::Knight: return 'n';
    case Type::Queen:
    default: return 'q';
    }
}

Type MainWindow::promotionTypeFromChar(QChar ch)
{
    switch (ch.toLower().toLatin1()) {
    case 'r': return Type::Rook;
    case 'b': return Type::Bishop;
    case 'n': return Type::Knight;
    case 'q':
    default: return Type::Queen;
    }
}
