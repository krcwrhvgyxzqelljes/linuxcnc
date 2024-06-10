#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "../scurve_engine/scurve_reset.h"

// For each scurve axis we store the data in here.
scurve_data Xaxis,Yaxis,Zaxis;

// C style functions to use in c and c++.
extern "C" struct scurve_data set_init_values_c(double jermax,
                                                double accmax,
                                                double maxvel,
                                                double cyctim,
                                                struct scurve_data data);

// STEP 1.
// Check jog conditions for enable, direction, finished.
extern "C" struct scurve_data jog_velocity_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos);
extern "C" struct scurve_data jog_velocity_enum_c(struct scurve_data data, int enable, double endvel, double endacc, jog_direction dir);
extern "C" struct scurve_data jog_position_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos, int jog_fwd, int jog_rev);
extern "C" struct scurve_data jog_position_enum_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos, jog_direction dir);
extern "C" void jog_results_c(struct scurve_data data, double *velocity, double *acceleration, double *position, int *finished);

// STEP 2.
// Construct a curve respecting the given tarpos. STEP 2.
extern "C" struct scurve_data jog_update_c(struct scurve_data data);

struct scurve_data axis_update(struct scurve_data data){
    if(data.jog.mode==POSITION_SC){
        if(data.guipos<data.tarpos){
            data.jog.dir=FORWARD_SC;
        }
        if(data.guipos>data.tarpos){
            data.jog.dir=REVERSE_SC;
        }
        data=jog_position_enum_c(data,data.jog.enable,data.endvel,data.endacc,data.tarpos,data.jog.dir);
    }
    if(data.jog.mode==VELOCITY_SC){
        data=jog_velocity_enum_c(data,data.jog.enable,data.endvel,data.endacc,data.jog.dir);
    }
    data=jog_update_c(data);
    std::cout<<"return code:"<<data.rcode<<std::endl;
    return data;
}

void MainWindow::init_plot(QCustomPlot *Plot,  QCPItemText *ValueIndex){

    // Add a graph to the plot widget ( the main graph )
    Plot->addGraph();
    // Add a graph to the plot widget ( this will be a horizontal line )
    // that folows the current value of the main graph
    Plot->addGraph();
    Plot->addGraph();

    // tell the plot widget to display the x axis values as a time
    // instead of numbers
    Plot->xAxis->setTickLabelType( QCPAxis::ltDateTime );
    // Set the x axis time string format
    Plot->xAxis->setDateTimeFormat( "" );
    Plot->xAxis->setAutoTickStep( true );
    Plot->yAxis->setAutoTickStep( true );

    // Allocating memory for the text item that will
    // display the current value as a text.
    ValueIndex = new QCPItemText( Plot );
    // Set the font of the text item
    ValueIndex->setFont(QFont(font().family(), 8)); // make font a bit larger
    // Add the text item to the plt widget
    Plot->addItem( ValueIndex );

    // Change the color of the graphs
    QColor brushClr = Qt::green;
    brushClr.setAlphaF( .5 );
    Plot->graph( 0 )->setBrush( brushClr );
    Plot->graph( 0 )->setPen( QColor() );

    QColor brushClr1 = Qt::yellow;
    brushClr1.setAlphaF( .5 );
    Plot->graph( 1 )->setBrush( brushClr1 );
    Plot->graph( 1 )->setPen( QColor() );

    QColor brushClr2 = Qt::blue;
    brushClr2.setAlphaF( .5 );
    Plot->graph( 2 )->setBrush( brushClr2 );
    Plot->graph( 2 )->setPen( QColor() );
}

void MainWindow::update_plot(qreal timeValue, QCustomPlot *Plot, scurve_data Data, int axis_nr){

    int i=axis_nr;
    axis[i].append(timeValue);

    // Write graph values.
    qreal value=Data.guivel;
    axisVel[i].append( value );

    value=Data.guiacc;
    axisAcc[i].append( value );

    value=Data.guipos*0.1;
    axisPos[i].append( value );

    // Keep the data buffers size under 100 value each, so our moemoty won't explode with random numbers
    if( axis[i].size() > 100 ){
        axis[i].remove( 0 );
        axisVel[i].remove( 0 );
        axisAcc[i].remove( 0 );
        axisPos[i].remove( 0 );
    }

    Plot->graph( 0 )->setData( axis[i] , axisVel[i] );
    Plot->graph( 1 )->setData( axis[i] , axisPos[i] );
    Plot->graph( 2 )->setData( axis[i] , axisAcc[i] );

    // Set the range of the vertical and horizontal axis of the plot ( not the graph )
    // so all the data will be centered. first we get the min and max of the x and y data
    QVector<double>::iterator xMaxIt = std::max_element( axis[i].begin() , axis[i].end() );
    QVector<double>::iterator xMinIt = std::min_element( axis[i].begin() , axis[i].end() );
    QVector<double>::iterator yMinIt = std::min_element( axisVel[i].begin() , axisVel[i].end() );
    QVector<double>::iterator yMaxIt = std::max_element( axisVel[i].begin() , axisVel[i].end() );

    // yMinIt = std::min_element( m_YData1.begin() , m_YData1.end() );
    // yMaxIt = std::max_element( m_YData1.begin() , m_YData1.end() );

    qreal yPlotMin = *yMinIt;;
    // qreal yPlotMax = *yMaxIt;

    qreal xPlotMin = *xMinIt;
    qreal xPlotMax = *xMaxIt;

    Plot->xAxis->setRange( xPlotMin , xPlotMax );
    Plot->yAxis->setRange( yPlotMin - Data.maxvel -1 , Data.maxvel +1 );

    // m_CustomPlot->yAxis->setRange( yPlotMin - maxvel -10 , maxvel+10 );

    // Update the plot widget
    Plot->replot();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    // m_XaxisPlot( 0 ),
    timeInterval( 10 )
{
    ui->setupUi(this);
    setMinimumWidth( 1000 );
    setMinimumHeight( 700);

    m_XaxisPlot = new QCustomPlot( this );
    init_plot(m_XaxisPlot,m_XaxisValueIndex);
    ui->gridLayout_x->addWidget(m_XaxisPlot);

    m_YaxisPlot = new QCustomPlot( this );
    init_plot(m_YaxisPlot,m_YaxisValueIndex);
    ui->gridLayout_y->addWidget(m_YaxisPlot);

    m_ZaxisPlot = new QCustomPlot( this );
    init_plot(m_ZaxisPlot,m_ZaxisValueIndex);
    ui->gridLayout_z->addWidget(m_ZaxisPlot);

    // setup a timer and start it.
    timer = new QTimer(this);
    connect(timer, SIGNAL( timeout() ), this, SLOT( updatePlot() ) );
    timer->start( timeInterval );

    scurve_data_reset(&Xaxis);
    scurve_data_sample(&Xaxis);

    scurve_data_reset(&Yaxis);
    scurve_data_sample(&Yaxis);

    scurve_data_reset(&Zaxis);
    scurve_data_sample(&Zaxis);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent( QKeyEvent* event ) {
    std::cout<<"key nr pressed:"<<event->key()<<std::endl;

    switch ( event->key() ) {
    case Qt::Key_X:
        //act on 'X'
        break;
    case Qt::Key_Y:
        //act on 'Y'
        break;
    default:
        event->ignore();
        break;
    }
}

void MainWindow::updatePlot()
{
    QTime tm = QTime::currentTime();   // Get the current time
    qreal seconds = 60 * 60 * tm.hour() + 60 * tm.minute() + tm.second();  // Convert the curretn time to millseconds
    qreal timeValue = 1000 * seconds + tm.msec();

    Xaxis=axis_update(Xaxis);
    Yaxis=axis_update(Yaxis);
    Zaxis=axis_update(Zaxis);

    ui->label_xinfo->setText("vel:"+QString::number(Xaxis.guivel,'f',3)+" pos:"+QString::number(Xaxis.guipos,'f',3)+" acc:"+QString::number(Xaxis.guiacc,'f',3));
    ui->label_yinfo->setText("vel:"+QString::number(Yaxis.guivel,'f',3)+" pos:"+QString::number(Yaxis.guipos,'f',3)+" acc:"+QString::number(Yaxis.guiacc,'f',3));
    ui->label_zinfo->setText("vel:"+QString::number(Zaxis.guivel,'f',3)+" pos:"+QString::number(Zaxis.guipos,'f',3)+" acc:"+QString::number(Zaxis.guiacc,'f',3));

    // Finished status.
    ui->checkBox_x_finished->setChecked(Xaxis.finish);
    ui->checkBox_y_finished->setChecked(Yaxis.finish);
    ui->checkBox_z_finished->setChecked(Zaxis.finish);

    update_plot(timeValue,m_XaxisPlot,Xaxis,0);
    update_plot(timeValue,m_YaxisPlot,Yaxis,1);
    update_plot(timeValue,m_ZaxisPlot,Zaxis,2);
}

void MainWindow::on_toolButton_xmin_pressed()
{
    Xaxis.tarpos=-INFINITY;
    Xaxis.jog.enable=1;
    Xaxis.jog.mode=VELOCITY_SC;
    Xaxis.jog.dir=REVERSE_SC;
}

void MainWindow::on_toolButton_xplus_pressed()
{
    Xaxis.tarpos=INFINITY;
    Xaxis.jog.enable=1;
    Xaxis.jog.mode=VELOCITY_SC;
    Xaxis.jog.dir=FORWARD_SC;
}

void MainWindow::on_toolButton_xstop_pressed()
{
    Xaxis.jog.enable=0;
}

void MainWindow::on_toolButton_ymin_pressed()
{
    Yaxis.tarpos=-INFINITY;
    Yaxis.jog.enable=1;
    Yaxis.jog.mode=VELOCITY_SC;
    Yaxis.jog.dir=REVERSE_SC;
}

void MainWindow::on_toolButton_yplus_pressed()
{
    Yaxis.tarpos=INFINITY;
    Yaxis.jog.enable=1;
    Yaxis.jog.mode=VELOCITY_SC;
    Yaxis.jog.dir=FORWARD_SC;
}

void MainWindow::on_toolButton_ystop_pressed()
{
    Yaxis.jog.enable=0;
}

void MainWindow::on_toolButton_zmin_pressed()
{
    Zaxis.tarpos=-INFINITY;
    Zaxis.jog.enable=1;
    Zaxis.jog.mode=VELOCITY_SC;
    Zaxis.jog.dir=REVERSE_SC;
}

void MainWindow::on_toolButton_zplus_pressed()
{
    Zaxis.tarpos=INFINITY;
    Zaxis.jog.enable=1;
    Zaxis.jog.mode=VELOCITY_SC;
    Zaxis.jog.dir=FORWARD_SC;
}

void MainWindow::on_toolButton_zstop_pressed()
{
    Zaxis.jog.enable=0;
}

void MainWindow::on_toolButton_x_go_pressed()
{
    Xaxis.tarpos=ui->lineEdit_tarpos_x->text().toDouble();
    Xaxis.jog.enable=1;
    Xaxis.jog.mode=POSITION_SC;
}

void MainWindow::on_toolButton_y_go_pressed()
{
    Yaxis.tarpos=ui->lineEdit_tarpos_y->text().toDouble();
    Yaxis.jog.enable=1;
    Yaxis.jog.mode=POSITION_SC;

    if(Yaxis.guipos<Yaxis.tarpos){
        Yaxis.jog.dir=FORWARD_SC;
    }
    if(Yaxis.guipos>Yaxis.tarpos){
        Yaxis.jog.dir=REVERSE_SC;
    }
    if(Yaxis.guipos==Yaxis.tarpos){
        std::cout<<"Y at tarpos"<<std::endl;
    }
}

void MainWindow::on_toolButton_z_go_pressed()
{
    Zaxis.tarpos=ui->lineEdit_tarpos_z->text().toDouble();
    Zaxis.jog.enable=1;
    Zaxis.jog.mode=POSITION_SC;

    if(Zaxis.guipos<Zaxis.tarpos){
        Zaxis.jog.dir=FORWARD_SC;
    }
    if(Zaxis.guipos>Zaxis.tarpos){
        Zaxis.jog.dir=REVERSE_SC;
    }
    if(Zaxis.guipos==Zaxis.tarpos){
        std::cout<<"Z at tarpos"<<std::endl;
    }
}

void MainWindow::on_checkBox_pause_timer_clicked(bool checked)
{
    if(checked){
        timer->stop();  // Pause the plot to study the results.
    } else {
        timer->start();
    }
}

void MainWindow::on_pushButton_pressed()
{
    Xaxis.pd.stopinit=0;
}

