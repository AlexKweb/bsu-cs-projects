#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->hide();

    bindUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bindUi()
{
    connect(ui->pushButton, &QPushButton::clicked, this, [this] {
        m_format = DateFormat::European;
        Date::setIoFormat(m_format);
        Date d;
        std::istringstream iss(ui->lineEdit->text().toStdString());
        iss >> d;
        if (!d.isValid()) {
            m_hasDate = false;
            ui->label_4->setText(QStringLiteral("ошибка: неверная дата"));
            return;
        }
        m_date = d;
        m_hasDate = true;
        std::ostringstream os;
        os << d;
        ui->label_4->setText(QString::fromStdString(os.str()));
    });

    connect(ui->pushButton_2, &QPushButton::clicked, this, [this] {
        m_format = DateFormat::RussianWords;
        Date::setIoFormat(m_format);
        Date d;
        std::istringstream iss(ui->lineEdit->text().toStdString());
        iss >> d;
        if (!d.isValid()) {
            m_hasDate = false;
            ui->label_4->setText(QStringLiteral("ошибка: неверная дата"));
            return;
        }
        m_date = d;
        m_hasDate = true;
        std::ostringstream os;
        os << d;
        ui->label_4->setText(QString::fromStdString(os.str()));
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [this] {
        m_format = DateFormat::American;
        Date::setIoFormat(m_format);
        Date d;
        std::istringstream iss(ui->lineEdit->text().toStdString());
        iss >> d;
        if (!d.isValid()) {
            m_hasDate = false;
            ui->label_4->setText(QStringLiteral("ошибка: неверная дата"));
            return;
        }
        m_date = d;
        m_hasDate = true;
        std::ostringstream os;
        os << d;
        ui->label_4->setText(QString::fromStdString(os.str()));
    });

    connect(ui->pushButton_4, &QPushButton::clicked, this, [this] {
        if (!m_hasDate) {
            ui->label_4->setText(QStringLiteral("…"));
            return;
        }
        bool ok = false;
        const int n = ui->lineEdit_2->text().toInt(&ok);
        if (!ok) {
            ui->label_4->setText(QStringLiteral("ошибка: не число"));
            return;
        }
        Date::setIoFormat(m_format);
        const Date res = m_date + n;
        if (!res.isValid()) {
            ui->label_4->setText(QStringLiteral("ошибка"));
            return;
        }
        std::ostringstream os;
        os << res;
        ui->label_4->setText(QString::fromStdString(os.str()));
    });
}
