#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <iomanip>
#include <sstream>

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

void MainWindow::on_btnTask1_clicked()
{
    int a = ui->inputTask1A->text().toInt();
    int b = ui->inputTask1B->text().toInt();

    int x = std::abs(a);
    int y = std::abs(b);

    while (x != 0 && y != 0) {
        if (x > y) {
            x = x % y;
        } else {
            y = y % x;
        }
    }

    int nod = x + y;
    long long nok = 0;

    if (nod != 0) {
        nok = (long long)std::abs(a * b) / nod;
    }

    ui->task1ResultGcd->setText(QString::number(nod));
    ui->task1ResultLcm->setText(QString::number(nok));
}

void MainWindow::on_btnTask2_clicked()
{
    int a = ui->inputTask2->text().toInt();
    bool isPrime = true;

    if (a < 2) {
        isPrime = false;
    }

    for (int i = 2; i <= std::sqrt(a); i++) {
        if (a % i == 0) {
            isPrime = false;
            break;
        }
    }

    if (isPrime) {
        ui->task2Result->setText("Число простое");
    } else {
        ui->task2Result->setText("Число не простое");
    }
}

void MainWindow::on_btnTask3_clicked()
{
    long long a = ui->inputTask3Start->text().toLongLong();
    long long b = ui->inputTask3End->text().toLongLong();

    if (a > b) {
        long long t = a;
        a = b;
        b = t;
    }

    if (a < 0) {
        a = 0;
    }

    QString result = "Числа Армстронга:\n";

    for (long long i = a; i <= b; ++i) {
        long long temp = i;
        int n = 0;

        if (temp == 0) {
            n = 1;
        } else {
            while (temp > 0) {
                temp /= 10;
                n++;
            }
        }

        temp = i;
        long long sum = 0;

        if (temp == 0) {
            sum = 0;
        } else {
            while (temp > 0) {
                int digit = temp % 10;
                long long p = 1;

                for (int k = 0; k < n; ++k) {
                    p *= digit;
                }

                sum += p;
                temp /= 10;
            }
        }

        if (sum == i) {
            result += QString::number(i) + " ";
        }
    }

    ui->task3Result->setPlainText(result);
}

void MainWindow::on_btnTask4_clicked()
{
    double a = ui->inputTask4A->text().toDouble();
    double b = ui->inputTask4B->text().toDouble();
    double c = ui->inputTask4C->text().toDouble();

    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                ui->task4Result->setText("Бесконечно много решений");
            } else {
                ui->task4Result->setText("Решений нет");
            }
        } else {
            double x = -c / b;
            ui->task4Result->setText("x = " + QString::number(x));
        }

        return;
    }

    double D = b * b - 4 * a * c;

    if (D < 0) {
        ui->task4Result->setText("Корней нет");
    } else if (D == 0) {
        double x = -b / (2 * a);
        ui->task4Result->setText("Один корень: x = " + QString::number(x));
    } else {
        double x1 = (-b + std::sqrt(D)) / (2 * a);
        double x2 = (-b - std::sqrt(D)) / (2 * a);
        ui->task4Result->setText("x1 = " + QString::number(x1) + ", x2 = " + QString::number(x2));
    }
}

void MainWindow::on_btnTask5_clicked()
{
    int n = ui->inputTask5->text().toInt();
    long double sum = 0.0;

    for (int k = 1; k <= n; k++) {
        long double sign;

        if (k % 2 == 0) {
            sign = 1.0;
        } else {
            sign = -1.0;
        }

        long double d = 2.0 * k - 1.0;
        sum += sign / (d * d);
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(n + 1) << (double)sum;

    ui->task5Result->setText(QString::fromStdString(out.str()));
}
