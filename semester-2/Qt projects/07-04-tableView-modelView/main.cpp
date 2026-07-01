#include "mainwindow.h"
#include "mymodel.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QTableView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath() + "/../../..");

    MainWindow w;
    w.show();
    return a.exec();
}
