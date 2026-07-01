#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);

    first = new MyPlenty_int();
    second = new MyPlenty_int();
    result = new MyPlenty_int();

    ui->label_2->setText(" Выбран INT ");
    ui->plainTextEdit->setPlainText(" Введите множества ");
}

MainWindow::~MainWindow()
{
    delete first;
    delete second;
    delete result;
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    delete first;
    QStringList words = ui->lineEdit->text().split(" ", Qt::SkipEmptyParts);

    if(ui->radioButton->isChecked()){
        QVector<int> data;
        for(QString word : words){
            data.push_back(word.toInt());
        }
        first = new MyPlenty_int(data);
    } else {
        QVector<QString> data;
        for(QString word : words){
            data.push_back(word);
        }
        first = new MyPlenty_sring(data);
    }

    delete second;
    words = ui->lineEdit_2->text().split(" ", Qt::SkipEmptyParts);

    if(ui->radioButton->isChecked()){
        QVector<int> data;
        for(QString word : words){
            data.push_back(word.toInt());
        }
        second = new MyPlenty_int(data);
    } else {
        QVector<QString> data;
        for(QString word : words){
            data.push_back(word);
        }
        second = new MyPlenty_sring(data);
    }

    QString text;

    delete result;
    if(ui->radioButton->isChecked()){
        result = new MyPlenty_int();
    } else {
        result = new MyPlenty_sring();
    }
    result->add(*first, *second);
    text += "add: " + result->toString() + "\n";

    delete result;
    if(ui->radioButton->isChecked()){
        result = new MyPlenty_int();
    } else {
        result = new MyPlenty_sring();
    }
    result->mult(*first, *second);
    text += "mult: " + result->toString() + "\n";

    delete result;
    if(ui->radioButton->isChecked()){
        result = new MyPlenty_int();
    } else {
        result = new MyPlenty_sring();
    }
    result->sim_sub(*first, *second);
    text += "sim_sub: " + result->toString();

    ui->plainTextEdit->setPlainText(text);
    ui->label_2->setText(" Посчитано ");
}

void MainWindow::on_pushButton_7_clicked()
{
    QString file1 = QFileDialog::getOpenFileName();
    QString file2 = QFileDialog::getOpenFileName();

    if(file1.isEmpty() || file2.isEmpty()){
        return;
    }

    delete first;
    delete second;

    if(ui->radioButton->isChecked()){
        first = new MyPlenty_int();
        second = new MyPlenty_int();
    } else {
        first = new MyPlenty_sring();
        second = new MyPlenty_sring();
    }

    if(!first->readFromFile(file1) || !second->readFromFile(file2)){
        QMessageBox::warning(this, "Error", "File open error");
    }

    ui->lineEdit->setText(first->toString());
    ui->lineEdit_2->setText(second->toString());
}

void MainWindow::on_pushButton_10_clicked()
{
    QString file1 = QFileDialog::getSaveFileName();
    QString file2 = QFileDialog::getSaveFileName();

    if(file1.isEmpty() || file2.isEmpty()){
        return;
    }

    first->writeToFile(file1);
    second->writeToFile(file2);
}

void MainWindow::on_radioButton_clicked()
{
    delete first;
    delete second;
    delete result;

    first = new MyPlenty_int();
    second = new MyPlenty_int();
    result = new MyPlenty_int();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->label_2->setText(" Выбран INT ");
    ui->plainTextEdit->setPlainText(" Введите множества ");
}

void MainWindow::on_radioButton_2_clicked()
{
    delete first;
    delete second;
    delete result;

    first = new MyPlenty_sring();
    second = new MyPlenty_sring();
    result = new MyPlenty_sring();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->label_2->setText(" Выбран STRING ");
    ui->plainTextEdit->setPlainText(" Введите множества ");

}
