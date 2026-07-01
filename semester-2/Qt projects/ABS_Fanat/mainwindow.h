#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "abstractfanat.h"
#include "basefanats.h"
#include "fanats2024.h"
#include "maindestroer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    MB1 M1;
    MB2 M2;
    MB3 M3;

    ABS_Fanat* A = nullptr;

    void FillTable1();
    void FillTable2();
    void FillTable3();
};
#endif // MAINWINDOW_H
