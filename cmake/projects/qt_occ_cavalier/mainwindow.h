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

#include "cavc/polyline.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    //void drawPolyline(const cavc::Polyline<double>& polyline);
    //voiplnne(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> &context);
    //void drawPolylineLinesOnly(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> &context);
    //void drawPolylineArcsOnly(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context);
    //void drawPolyline(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> &context);

private slots:
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    OcctQtViewer *occ;

    Form *form = new Form();
};
#endif
