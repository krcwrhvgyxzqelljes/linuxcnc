#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "../scurve_engine/scurve_engine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init_plot(QCustomPlot *Plot, QCPItemText *ValueIndex);
    void update_plot(qreal timeValue, QCustomPlot *Plot, scurve_data Data, int axis_nr);

private slots:
    // This function is responsible for ploting
    // and updating the graphs , with each timer tick
    void updatePlot();

    void on_toolButton_xmin_pressed();

    void on_toolButton_xplus_pressed();

    void on_toolButton_xstop_pressed();

    void on_toolButton_ymin_pressed();

    void on_toolButton_yplus_pressed();

    void on_toolButton_ystop_pressed();

    void on_toolButton_zmin_pressed();

    void on_toolButton_zplus_pressed();

    void on_toolButton_zstop_pressed();

    void on_toolButton_x_go_pressed();

    void on_toolButton_y_go_pressed();

    void on_toolButton_z_go_pressed();

    void on_checkBox_pause_timer_clicked(bool checked);

protected:
    void keyPressEvent( QKeyEvent* event ); // Just for info.

private slots:
    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    qreal timeInterval;             // The time between each update, this will be used by the timer to call "updatePlot" slot

    QCustomPlot *m_XaxisPlot;       // Our custom plot widget
    QCPItemText *m_XaxisValueIndex; // This object will hold the current value as a text, that will appear at the extreme right of the plot.

    QCustomPlot *m_YaxisPlot;
    QCPItemText *m_YaxisValueIndex; // This object will hold the current value as a text, that will appear at the extreme right of the plot.

    QCustomPlot *m_ZaxisPlot;
    QCPItemText *m_ZaxisValueIndex; // This object will hold the current value as a text, that will appear at the extreme right of the plot.

    QVector<qreal> axisVel[4];      // Data buffers for qt plot utility.
    QVector<qreal> axisAcc[4];
    QVector<qreal> axisPos[4];
    QVector<qreal> axis[4];

};

#endif // MAINWINDOW_H
