#include "mainwindow.h"
#include "mymodel.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , phoneModel(new MyModel(this))
    , addrModel(new MyAddrModel(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_4_clicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Выберите файл с данными"),
        selectedFilePath.isEmpty() ? QDir::currentPath() : QFileInfo(selectedFilePath).absolutePath(),
        tr("Text files (*.txt);;All files (*.*)")
    );

    if( filePath.isEmpty() ) {
        return;
    }

    selectedFilePath = filePath;
    updateSelectedFileLabel();
}

void MainWindow::on_pushButton_3_clicked()
{
    if( selectedFilePath.isEmpty() ) {
        return;
    }
    loadFile(selectedFilePath);
}

void MainWindow::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        return;
    }

    QTextStream in(&file);
    if( isAddressFile(filePath) ) {
        in >> *addrModel;
        ui->tableView->setModel(addrModel);
    } else {
        in >> *phoneModel;
        ui->tableView->setModel(phoneModel);
    }
    file.close();
}

void MainWindow::updateSelectedFileLabel()
{
    if( selectedFilePath.isEmpty() ) {
        ui->label->setText(tr("Файл не выбран"));
        return;
    }

    ui->label->setText(tr("Файл: %1").arg(QFileInfo(selectedFilePath).fileName()));
}

bool MainWindow::isAddressFile(const QString &filePath) const
{
    QFile file(filePath);
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        return false;
    }

    QTextStream in(&file);
    QString name;
    QString second;
    QString third;
    in >> name >> second >> third;

    bool secondIsNumber = false;
    second.toLongLong(&secondIsNumber);

    return !secondIsNumber;
}

void MainWindow::on_pushButton_clicked()
{
    if( selectedFilePath.isEmpty() ) {
        return;
    }

    if( ui->tableView->model() == addrModel ) {
        addrModel->saveToFile(selectedFilePath);
    } else if( ui->tableView->model() == phoneModel ) {
        phoneModel->saveToFile(selectedFilePath);
    }
}
