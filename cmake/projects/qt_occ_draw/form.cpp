#include "form.h"
#include "ui_form.h"

#include "mainwindow.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

Form::~Form()
{
    delete ui;
}

void Form::set_positions(QString text){
    ui->label_positions->setText(text);
}

void Form::set_message(QString text){
    ui->label_message->setText(text);
}

void Form::set_keyboard_input_text(QString text){
    ui->label_keyboard_input->setText(text);
}

void Form::on_toolButton_zoom_plus_pressed()
{
    emit toolButton_zoom_plus_pressed();
}

void Form::on_toolButton_zoom_min_pressed()
{
    emit toolButton_zoom_min_pressed();
}

void Form::on_toolButton_fit_all_pressed()
{
    emit toolButton_fit_all_pressed();
}

void Form::on_toolButton_view_front_pressed()
{
    emit toolButton_view_front_pressed();
}

void Form::on_toolButton_view_back_pressed()
{
    emit toolButton_view_back_pressed();
}

void Form::on_toolButton_view_top_pressed()
{
    emit toolButton_view_top_pressed();
}

void Form::on_toolButton_view_bottom_pressed()
{
    emit toolButton_view_bottom_pressed();
}

void Form::on_toolButton_view_left_pressed()
{
    emit toolButton_view_left_pressed();
}

void Form::on_toolButton_view_right_pressed()
{
    emit toolButton_view_right_pressed();
}

void Form::on_toolButton_view_3d_pressed()
{
    emit toolButton_view_3d_pressed();
}

void Form::on_toolButton_draw_3d_2p_line_pressed()
{
    emit toolButton_draw_3d_2p_line_pressed();
}

void Form::on_toolButton_color_pressed()
{
    emit toolButton_color_pressed();
}




























