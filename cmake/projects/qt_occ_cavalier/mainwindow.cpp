#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "cavc/polylineoffset.hpp"
#include "iostream"
#include "draw_primitives.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include "cavalier_contours/cavalier_offset.h"
#include "cavalier_contours/cavalier_3d.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // occ = new Opencascade();
    occ = new  OcctQtViewer();

    //! Add gridlayout on top of the occ widget.
    QGridLayout *layout=new QGridLayout(occ);

    //! Add the controls into the occ gridlayout..
    layout->addWidget(form);

    ui->gridLayout_occ->addWidget(occ);
    occ->create_tp_cone();

    //! This activates a screen update when robot is moving and screen needs to be updated automaticly.
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(50);


    // run_offset_example(occ->myContext);
    // cavalier_3d().run_2d_example(occ->myContext, 0);
    // cavalier_3d().run_2d_plus_example(occ->myContext, 0);
    //  cavalier_3d().run_3d_example(occ->myContext, 0);
    cavalier_3d().run_3d_plus_example(occ->myContext, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(){

    //! To update tp moves.
    occ->redraw();
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    // cavalier_3d().run_2d_plus_example(occ->myContext, arg1);
    //cavalier_3d().run_3d_example(occ->myContext, arg1);
    cavalier_3d().run_3d_plus_example(occ->myContext, arg1);
}

