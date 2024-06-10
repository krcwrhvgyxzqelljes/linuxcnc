#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <iostream>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

#ifdef Success
#undef Success
#endif

//! libocc.
#include "draw_primitives.h"
#include "OcctQtViewer.h"

//! occ overlay, ensure it's a created from qt->add_new->dialog_gui_class->widget.
#include "form.h"

#include "clothoid_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void calculate_curve();

public slots:
    void update();

private slots:
    void on_doubleSpinBox_angle_end_valueChanged(double arg1);

    void on_doubleSpinBox_y_end_valueChanged(double arg1);

    void on_doubleSpinBox_x_end_valueChanged(double arg1);

    void on_doubleSpinBox_angle_begin_valueChanged(double arg1);

    void on_doubleSpinBox_y_begin_valueChanged(double arg1);

    void on_doubleSpinBox_x_begin_valueChanged(double arg1);

    void on_spinBox_nr_of_points_valueChanged(int arg1);

    void on_pushButton_pressed();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_calculate_pressed();

private:
    Ui::MainWindow *ui;

    OcctQtViewer *occ;

    Form *form = new Form();
    std::vector<Handle(AIS_Shape)> shapevec;

public:

    double x0=0;
    double y0=0;
    double angle0=0;
    double x1=0;
    double y1=5;
    double angle1=0;
    int number_of_points=0;
    std::vector<gp_Pnt> pntvec;
    double progress=0;

    clothoid_interface *ci;
};
#endif
