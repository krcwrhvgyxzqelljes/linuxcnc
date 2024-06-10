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

//! nml.
#include <config.h>
#include <emc.hh>
#include <emc_nml.hh>

//! cad data from interpreter
#include "common.h"

//! occ overlay, ensure it's a created from qt->add_new->dialog_gui_class->widget.
#include "form.h"

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
    void load_cad_data_from_interpreter();

private:
    Ui::MainWindow *ui;

    RCS_STAT_CHANNEL *stat;
    EMC_STAT  *emcStatus;

    OcctQtViewer *occ;

    Form *form = new Form();
};
#endif
