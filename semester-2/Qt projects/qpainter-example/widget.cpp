#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

/* Метод, в котором происходит рисование
 * */
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    /* Проверяем, какой из радиобаттонов выбран
     * */
    if(ui->radioButton_red->isChecked()){
        // Если красный, то отрисовываем красный круг
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawEllipse(100, 50, 15, 15);
    } else if(ui->radioButton_green->isChecked()){
        // Если зелёный, то отрисовываем зеленый круг
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.drawEllipse(100, 50, 15, 150);
    } else if(ui->radioButton_blue->isChecked()){
        // Если синий, то отрисовываем синий круг
        painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
        painter.drawEllipse(10, 5, 150, 150);
    } else {
        // Если ничего не выбрано, то отрисовываем белый круг
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawEllipse(100, 50, 150, 150);
    }
}

/* Как только один из радиобаттонов оказывается нажатым,
 * вызываем перерисовку содержимого виджета,
 * с которым работаем.
 * */
void Widget::on_radioButton_red_clicked()
{
    repaint();
}

void Widget::on_radioButton_green_clicked()
{
    repaint();
}

void Widget::on_radioButton_blue_clicked()
{
    repaint();
}
