#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QColor>

#include "paintscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PaintScene *scene;
    QTimer *timer;
    QColor m_currentColor; // added v2

private:
    QString storageFilePath() const; // added v2
    void resizeEvent(QResizeEvent *event);

private slots:
    void slotTimer();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked(); // added v2
    void on_pushButton_5_clicked(); // added v2
    void on_pushButton_6_clicked(); // added v2
    void on_pushButton_7_clicked(); // added v2
    void on_pushButton_8_clicked(); // added v2
    void on_pushButton_9_clicked(); // added v2
    void on_pushButton_10_clicked(); // added v2
};

#endif // MAINWINDOW_H
