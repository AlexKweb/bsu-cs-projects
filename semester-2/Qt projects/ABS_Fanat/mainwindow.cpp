#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FillTable1()
{
    ui->tableWidget->setRowCount(M1.GetCount());

    for (int i = 0; i < M1.GetCount(); i++)
    {
        Fanat1 f = M1.GetFanat(i);

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(f.Name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(f.Nationality));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(f.Year)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(f.N)));
    }
}
void MainWindow::FillTable2()
{
    ui->tableWidget_2->setRowCount(M2.GetCount());

    for (int i = 0; i < M2.GetCount(); i++)
    {
        Fanat2 f = M2.GetFanat(i);

        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(f.Name));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString::number(f.Code)));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString::number(f.N)));
    }
}
void MainWindow::FillTable3(){
    ui->tableWidget_3->setRowCount(M3.GetCount());

    for (int i = 0; i < M3.GetCount(); i++)
    {
        Fanat3 f = M3.GetFanat(i);

        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(f.Nationality));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(f.Name));
        ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(QString::number(f.Year)));
        ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem(QString::number(f.N)));
        ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(QString::number(f.Code)));
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString fileName1 = QFileDialog::getOpenFileName(this);
    if (fileName1.isEmpty())
        return;
    QString fileName2 = QFileDialog::getOpenFileName(this);
    if (fileName2.isEmpty())
        return;

    QFile file1(fileName1);
    QFile file2(fileName2);

    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text) || file1.size() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть первый файл");
        return;
    }

    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text) || file2.size() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть второй файл");
        return;
    }

    QTextStream in1(&file1);
    QTextStream in2(&file2);

    A = &M1;
    A->GetFanats(in1);

    A = &M2;
    A->GetFanats(in2);

    FillTable1();
    FillTable2();

    QMessageBox::warning(this, "Готово", "Файлы успешно загружены");
}

void MainWindow::on_pushButton_4_clicked()
{
    A = &M1;
    A->SortUp();

    A = &M2;
    A->SortUp();

    A = &M3;
    A->SortUp();

    FillTable1();
    FillTable2();
    FillTable3();

    ui->statusbar->showMessage("Массивы отсортированы от А до Я");
}

void MainWindow::on_pushButton_3_clicked()
{
    A = &M1;
    A->SortDown();

    A = &M2;
    A->SortDown();

    A = &M3;
    A->SortDown();

    FillTable1();
    FillTable2();
    FillTable3();

    ui->statusbar->showMessage("Массивы отсортированы от Я до А");
};

void MainWindow::on_pushButton_2_clicked()
{
    if (M1.GetCount() == 0 || M2.GetCount() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите");
        return;
    }

    M3.Common(M1, M2);

    FillTable3();

    ui->statusbar->showMessage("Массив Главные нарушители сформирован");
}

void MainWindow::on_pushButton_clicked()
{
    QFile file("out.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать out.txt");
        return;
    }

    QTextStream out(&file);

    out << "База фанатов:\n";
    A = &M1;
    A->PutFanats(out);

    out << "\nФанаты 2024:\n";
    A = &M2;
    A->PutFanats(out);

    out << "\nГлавные нарушители:\n";
    A = &M3;
    A->PutFanats(out);

    file.close();

    ui->statusbar->showMessage("Данные сохранены в out.txt");
}



void MainWindow::on_pushButton_6_clicked()
{
    QString toFind = ui->lineEdit->text();

    if (toFind.isEmpty())
    {
        ui->label->setText("Введите имя для поиска");
        return;
    }

    M1.SortUp();
    M2.SortUp();
    M3.SortUp();

    QVector<int> range1 = M1.Find(toFind);
    QVector<int> range2 = M2.Find(toFind);
    QVector<int> range3 = M3.Find(toFind);

    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);

    int foundCount = 0;

    int count1;
    if (range1.size() == 2)
        count1 = range1[1] - range1[0];
    else
        count1 = 0;

    int count2;
    if (range2.size() == 2)
        count2 = range2[1] - range2[0];
    else
        count2 = 0;

    int count3;
    if (range3.size() == 2)
        count3 = range3[1] - range3[0];
    else
        count3 = 0;

    ui->tableWidget->setRowCount(count1);
    for (int i = 0;i < count1; i++)
    {
        Fanat1 f = M1.GetFanat(range1[0] + i);

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(f.Name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(f.Nationality));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(f.Year)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(f.N)));

        foundCount++;
    }

    ui->tableWidget_2->setRowCount(count2);
    for (int i = 0;i < count2; i++)
    {
        Fanat2 f = M2.GetFanat(range2[0] + i);

        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(f.Name));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString::number(f.Code)));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString::number(f.N)));

        foundCount++;
    }

    ui->tableWidget_3->setRowCount(count3);
    for (int i = 0; i < count3; i++)
    {
        Fanat3 f = M3.GetFanat(range3[0] + i);

        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(f.Nationality));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(f.Name));
        ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(QString::number(f.Year)));
        ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem(QString::number(f.N)));
        ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(QString::number(f.Code)));

        foundCount++;
    }
    ui->statusbar->showMessage("Найдено записей: " + QString::number(foundCount));
}
