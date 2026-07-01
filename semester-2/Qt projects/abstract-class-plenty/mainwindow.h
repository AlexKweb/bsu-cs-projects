#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plenty.h"

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
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_10_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Abstract_Plenty *first;
    Abstract_Plenty *second;
    Abstract_Plenty *result;
};
#endif // MAINWINDOW_H
