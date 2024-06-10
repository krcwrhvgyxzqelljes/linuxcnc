#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>

#include <variable.h>

#include <dx_data.h>
#include <dx_iface.h>

#include <cubic_spline.h>
using namespace Eigen;

#include "opencascade.h"
#include "draw_primitives.h"
using namespace occ;

#include "contours.h"
#include "offsets.h"
#include "gcode.h"

#include <QMainWindow>
#include <QTimer>

// File read operations.
#include <iostream>
#include <fstream>
#include <string>

// Dialog file open library, https://github.com/samhocevar/portable-file-dialogs/blob/master/examples/example.cpp
#include <portable-file-dialogs.h>
#if _WIN32
#define DEFAULT_PATH "C:\\"
#else
#define DEFAULT_PATH "/tmp"
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool open_dxf_file(std::string filename);
    bool save_dxf_file(std::string filename);
    bool write_entity();

    void Update_Opencascade();
    void load_opencascade_primitives();

    void gcode_preview();
    void gcode_get_user_settings();

    void generate_contours();
    void generate_pockets();

private slots:
    void process();

    void rotate_lead_in_out();

    void add_operation();

    void clear_operations();

    void process_operations();

    void on_toolButton_open_dxf_pressed();

private:
    Ui::MainWindow *ui;
    
    // Dxf data
    dx_data fData;
    dx_data fCleanData; // To clean the fData.
    dx_iface *iface = new dx_iface;
    Opencascade* OpencascadeWidget;
};
#endif // MAINWINDOW_H
