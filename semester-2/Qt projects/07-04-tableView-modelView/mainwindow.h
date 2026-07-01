#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MyModel;
class MyAddrModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();

private:
    void loadFile(const QString &filePath);
    void updateSelectedFileLabel();
    bool isAddressFile(const QString &filePath) const;

    Ui::MainWindow *ui;
    MyModel *phoneModel;
    MyAddrModel *addrModel;
    QString selectedFilePath;
};
#endif // MAINWINDOW_H
