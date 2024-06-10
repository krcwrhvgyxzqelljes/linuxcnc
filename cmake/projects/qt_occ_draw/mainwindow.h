#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QKeyEvent>
#include <QColorDialog>
#include <QDebug>
#include <iostream>

#ifdef Success
#undef Success
#endif

//! Draw lib.
#include "draw_primitives.h"

//! Sketch lib.
#include "sketcher.h"

//! Opencascade.
#include "OcctQtViewer.h"

//! Gui overlay.
#include "form.h"

//! Right mousepress pop up window.
#include "formoptions.h"

//! Dxf.
#include "libdxfrw_functions.h"

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

private slots:

    void zoom_plus();
    void zoom_min();
    void zoom_all();
    void view_front();
    void view_back();
    void view_top();
    void view_bottom();
    void view_left();
    void view_right();
    void view_3d();
    void draw_3d_2p_line();
    void set_color();

    void on_actionorthographic_triggered();

    void on_actionperspective_triggered();

    void on_actionopen_triggered();

    void on_actionsave_triggered();

    void on_actionset_plane_top_triggered();

    void on_actionset_plane_left_triggered();

    void on_actionset_plane_front_triggered();

    void on_action_ortho_on_triggered();

    void on_action_ortho_off_triggered();

    void on_action_snap_on_triggered();

    void on_action_snap_off_triggered();

    void on_action2p_3d_line_triggered();

    void on_actionEsc_triggered();

    void on_actiontop_triggered();

    void on_actionbottom_triggered();

    void on_actionfront_triggered();

    void on_actionback_triggered();

    void on_actionleft_triggered();

    void on_actionright_triggered();

    void on_actionzoom_plus_triggered();

    void on_actionzoom_min_triggered();

    void on_actionzoom_all_triggered();

    void on_actionview_3d_triggered();

    void on_actionclick_plane_origin_triggered();

    void on_actionreset_plane_triggered();

    void on_action3p_3d_arc_triggered();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent* event);
private:
    Ui::MainWindow *ui;

    QTimer *timer = new QTimer(this);

    OcctQtViewer *occ;

    Form *form = new Form();

    Sketcher *sketcher=new Sketcher();
    draw_primitives *draw=new draw_primitives();

    int left_mouse_click_counter=0;
    int oldkey;
    double snap_size=2;
    double keyboard_input=0;
    QString filename, keyboard_text;
    double keyboard_value=0;
    gp_Pnt p;
};
#endif
