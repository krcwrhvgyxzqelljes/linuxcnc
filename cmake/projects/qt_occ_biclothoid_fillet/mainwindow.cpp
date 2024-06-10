#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "draw_primitives.h"
#include <QGridLayout>
#include <QTimer>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <chrono>
#include "clothoid_interface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    occ = new OcctQtViewer();

    QGridLayout *layout = new QGridLayout(occ);

    layout->addWidget(form);

    ui->gridLayout_occ->addWidget(occ);
    occ->create_tp_cone();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(50);

    // calculate_curve();

    ci = new clothoid_interface;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Function to convert degrees to radians
double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}
int init_shape=0;

void MainWindow::calculate_curve(){
    auto start = std::chrono::high_resolution_clock::now();
    std::cout.precision(15);

    clothoid c;
    c.p0={x0,y0,0};
    c.theta0=angle0;
    c.p1={x1,y1,0};
    c.theta1=angle1;
    c.nr_of_points=20;

    ci->build_clothoid(c);
    ci->eval_clothoid(c);

    ui->label_L->setText(QString::number(c.L,'f',3));

    ui->label_k->setText(QString::number(c.k,'f',3));
    ui->label_dk->setText(QString::number(c.dk,'f',3));
    ui->label_iter->setText(QString::number(c.iter,'f',3));

    ui->label_k_1->setText(QString::number(c.k_1,'f',3));
    ui->label_dk_1->setText(QString::number(c.dk_1,'f',3));
    ui->label_L_1->setText(QString::number(c.L_1,'f',3));

    ui->label_k_2->setText(QString::number(c.k_2,'f',3));
    ui->label_dk_2->setText(QString::number(c.dk_2,'f',3));
    ui->label_L_2->setText(QString::number(c.L_2,'f',3));

    // Check a interpolated point on the curve given a progress 0-1.
    gp_Pnt pi=ci->interpolate_clothoid_point_progress(c,progress);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // Convert duration to milliseconds with higher precision
    double duration_in_milliseconds = duration.count() / 1e6;

    std::cout << "Function duration: " << std::fixed << std::setprecision(9) << duration_in_milliseconds << " milliseconds" << std::endl;

    shapevec.push_back(draw_primitives().draw_3d_line_wire(c.pvec));
    shapevec.back()=draw_primitives().colorize(shapevec.back(),Quantity_NOC_ALICEBLUE,0);
    occ->myContext->Display(shapevec.back(), false);

    shapevec.push_back(draw_primitives().draw_3d_point(pi));
    occ->myContext->Display(shapevec.back(), false);

    shapevec.push_back(draw_primitives().draw_3d_line({0,0,0},{0,100,0}));
    occ->myContext->Display(shapevec.back(), false);
    shapevec.push_back(draw_primitives().draw_3d_line({0,100,0},{100,100,0}));
    occ->myContext->Display(shapevec.back(), false);
    shapevec.push_back(draw_primitives().draw_3p_3d_circle({0,80,0},{20,100,0},{40,80,0}));
    occ->myContext->Display(shapevec.back(), false);



    occ->redraw();
}

void MainWindow::update()
{    // Get the start time
    //occ->redraw();
}

void MainWindow::on_spinBox_nr_of_points_valueChanged(int arg1)
{
    number_of_points=arg1;
    calculate_curve();
}

void MainWindow::on_doubleSpinBox_x_begin_valueChanged(double arg1)
{
    x0=arg1;
    calculate_curve();
}

void MainWindow::on_doubleSpinBox_y_begin_valueChanged(double arg1)
{
    y0=arg1;
    calculate_curve();
}

void MainWindow::on_doubleSpinBox_angle_begin_valueChanged(double arg1)
{
    angle0=degreesToRadians(arg1);
    calculate_curve();
}

void MainWindow::on_doubleSpinBox_x_end_valueChanged(double arg1)
{
    x1=arg1;
    calculate_curve();
}

void MainWindow::on_doubleSpinBox_y_end_valueChanged(double arg1)
{
    y1=arg1;
    calculate_curve();
}

void MainWindow::on_doubleSpinBox_angle_end_valueChanged(double arg1)
{
    angle1=degreesToRadians(arg1);
    calculate_curve();
}

void MainWindow::on_pushButton_pressed()
{
    shapevec.clear();

    // Get all displayed interactive objects (shapes)
    TopTools_ListOfShape displayedShapes;
    AIS_ListOfInteractive list;
    occ->myContext->DisplayedObjects(list);

    // Set all shapes in the list as selected
    for (AIS_ListIteratorOfListOfInteractive it(list); it.More(); it.Next()) {
        const Handle(AIS_InteractiveObject)& obj = it.Value();
        if (obj->IsKind(STANDARD_TYPE(AIS_InteractiveObject))) {
            occ->myContext->AddOrRemoveSelected(obj, Standard_True);
        }
    }

    // Get all displayed interactive objects
    AIS_ListOfInteractive displayedObjects;
    occ->myContext->DisplayedObjects(displayedObjects);

    // Remove selected shapes from the context

    for (AIS_ListIteratorOfListOfInteractive it(displayedObjects); it.More(); it.Next()) {
        Handle(AIS_InteractiveObject) obj = it.Value();
        if (occ->myContext->IsSelected(obj)) {

            occ->myContext->Erase(obj, Standard_True); // Erase selected object from context
            obj.Nullify();
        }
    }
    occ->redraw();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    progress=arg1;
    calculate_curve();
}

void MainWindow::on_pushButton_calculate_pressed()
{
    calculate_curve();
}

