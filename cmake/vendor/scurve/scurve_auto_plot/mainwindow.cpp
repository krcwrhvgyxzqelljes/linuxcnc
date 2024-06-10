#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "iostream"

#include "../scurve_engine/scurve_engine.h"
#include "../scurve_engine/scurve_vector.h"

scurve_data s;
double maxvel=0;
double maxacc=0;
double maxjer=0;
double trjlen=0; // Traject lenght.
double intval=0.001;
double curvel=0,curacc=0,curpos=0;
int pause=0;
int finished=0;
int run=0;
int nr=0;
double maxvel_fo=0; // Max velocity including feed overide.
double maxvel_fo_af=0; // Max velocity including feed overide & adaptive feed.
double qtime=0;
extern "C" struct scurve_data set_init_values_c(double jermax,
                                                double accmax,
                                                double maxvel,
                                                double cyctim,
                                                struct scurve_data data);
extern "C" struct scurve_data jog_update_c(struct scurve_data data);
extern "C" struct scurve_data jog_velocity_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos);
extern "C" struct scurve_data jog_position_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos, int jog_fwd, int jog_rev);
extern "C" void jog_results_c(struct scurve_data data, double *velocity, double *acceleration, double *position, int *finished);

scurve_vector* ptr;
extern "C" { scurve_vector* vector_init_ptr_c(); }
extern "C" void vector_add_segment_c(scurve_vector *ptr, struct sc_segment b);
extern "C" struct sc_segment vector_at_c(scurve_vector *ptr, size_t index);
extern "C" void vector_at_id_set_tarpositions_c(scurve_vector *ptr);
extern "C" int vector_size_c(scurve_vector *ptr);
extern "C" void vector_nr_increment(scurve_vector *ptr, int *n);
extern "C" void vector_nr_decrement(scurve_vector *ptr, int *n);
extern "C" void vector_traject_lenght(scurve_vector *ptr, double *l);

double lenght(sc_pnt p0, sc_pnt p1){
    return sqrt(pow(p1.x-p0.x,2)+pow(p1.y-p0.y,2)+pow(p1.z-p0.z,2));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_CustomPlot( 0 ),
    timeInterval( 1 )
{
    ui->setupUi(this);
    setMinimumWidth( 1000 );
    setMinimumHeight( 700);
    // Allocate memory for the plot widget object
    m_CustomPlot = new QCustomPlot( this );
    // The next line is responsible for adding
    // the plot widget to our application's main window
    //setCentralWidget( m_CustomPlot ); // there is no need to add and promote a widget

    ui->gridLayout->addWidget(m_CustomPlot);

    // Add a graph to the plot widget ( the main graph )
    m_CustomPlot->addGraph();
    // Add a graph to the plot widget ( this will be a horizontal line )
    // that folows the current value of the main graph
    m_CustomPlot->addGraph();
    m_CustomPlot->addGraph();

    // tell the plot widget to display the x axis values as a time
    // instead of numbers
    m_CustomPlot->xAxis->setTickLabelType( QCPAxis::ltDateTime );
    // Set the x axis time string format
    m_CustomPlot->xAxis->setDateTimeFormat( "" );
    m_CustomPlot->xAxis->setAutoTickStep( true );
    m_CustomPlot->yAxis->setAutoTickStep( true );

    // Allocating memory for the text item that will
    // display the current value as a text.
    m_ValueIndex = new QCPItemText( m_CustomPlot );
    // Set the font of the text item
    m_ValueIndex->setFont(QFont(font().family(), 8)); // make font a bit larger
    // Add the text item to the plt widget
    m_CustomPlot->addItem( m_ValueIndex );

    // Change the color of the graphs
    QColor brushClr = Qt::green;
    brushClr.setAlphaF( .5 );
    m_CustomPlot->graph( 0 )->setBrush( brushClr );
    m_CustomPlot->graph( 0 )->setPen( QColor() );

    QColor brushClr1 = Qt::yellow;
    brushClr1.setAlphaF( .5 );
    m_CustomPlot->graph( 1 )->setBrush( brushClr1 );
    m_CustomPlot->graph( 1 )->setPen( QColor() );

    QColor brushClr2 = Qt::blue;
    brushClr2.setAlphaF( .5 );
    m_CustomPlot->graph( 2 )->setBrush( brushClr2 );
    m_CustomPlot->graph( 2 )->setPen( QColor() );

    // setup a timer and start it.
    timer = new QTimer(this);
    connect(timer, SIGNAL( timeout() ), this, SLOT( updatePlot() ) );
    timer->start( timeInterval );

    maxacc=ui->lineEdit_maxacc->text().toDouble();
    maxvel=ui->lineEdit_maxvel->text().toDouble();
    maxjer=ui->lineEdit_maxjer->text().toDouble();

    ptr = vector_init_ptr_c();


    // Example auto mode.
    double dist=100*25.4;
    maxvel=100*25.4;
    maxacc=50*25.4;
    maxjer=20*25.4;
    double a=0, b=0;
    int nr=0;
    sc_segment seg;

    // First segment up to vmax.
    seg.pnt_s={a,0,0};
    seg.pnt_e={a+dist,0,0};
    seg.vo=0;
    seg.ve=maxvel;
    seg.primitive_id=sc_primitive_id::sc_line;
    seg.type=sc_motion_type::sc_linear;
    seg.gcode_line_nr=nr;
    seg.path_lenght=dist; //lenght(seg.pnt_s,seg.pnt_e);
    vector_add_segment_c(ptr,seg);
    vector_at_id_set_tarpositions_c(ptr);

    a+=dist;
    nr++;

    // 98 segments velmax.
    for(uint i=0; i<98; i++){

        seg.pnt_s={a,0,0};
        seg.pnt_e={a+dist,0,0};
        seg.vo=maxvel;
        seg.ve=maxvel;
        seg.primitive_id=sc_primitive_id::sc_line;
        seg.type=sc_motion_type::sc_linear;
        seg.gcode_line_nr=nr;
        seg.path_lenght=dist; //lenght(seg.pnt_s,seg.pnt_e);
        vector_add_segment_c(ptr,seg);
        vector_at_id_set_tarpositions_c(ptr);

        a+=dist;
        nr++;
    }

    // Last segment to zero.
    seg.pnt_s={a,0,0};
    seg.pnt_e={a+dist,0,0};
    seg.vo=maxvel;
    seg.ve=0;
    seg.primitive_id=sc_primitive_id::sc_line;
    seg.type=sc_motion_type::sc_linear;
    seg.gcode_line_nr=nr;
    seg.path_lenght=dist; //lenght(seg.pnt_s,seg.pnt_e);
    vector_add_segment_c(ptr,seg);
    vector_at_id_set_tarpositions_c(ptr);

    a+=dist;
    nr++;

    vector_traject_lenght(ptr,&trjlen);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool init_stop=0;

void MainWindow::updatePlot()
{
    qtime+=intval;
    m_XData.append( qtime );

    if(run){

        double adafee=ui->doubleSpinBox_adafee->value(); // Adaptive feed value.
        if(adafee<0){
            ui->checkBox_revmot_status->setChecked(1);
        } else {
            ui->checkBox_revmot_status->setChecked(0);
        }

        double feeove=ui->doubleSpinBox_feed_override->value(); // Feed override value.
        maxvel_fo=(feeove/100)*maxvel; // Calculate maxvel including feed override.
        maxvel_fo_af=(abs(adafee)/100)*maxvel_fo; // Calculate maxvel including feed override & adaptive feed.

        if(!finished){ // Don't set a new velmax value on the edge of a segment to prevent a standstill.
            s=set_init_values_c(maxjer,maxacc,maxvel_fo_af,intval,s);
        }

        double endvel=0;
        double endacc=0;
        int jog_fwd=0;
        int jog_rev=0;
        double tarpos=0;
        double jog_tarpos=0;
        if(ui->checkBox_revmot->isChecked() || adafee<0){ // User command reverse motion, or negative adaptive feed input.
            jog_fwd=0;
            jog_rev=1;
            tarpos=vector_at_c(ptr,nr).lenbeg;
            endvel=vector_at_c(ptr,nr).vo;
            jog_tarpos=-INFINITY;
        } else { // Forward motion.
            jog_fwd=1;
            jog_rev=0;
            tarpos=vector_at_c(ptr,nr).lenend;
            endvel=vector_at_c(ptr,nr).ve;
            jog_tarpos=INFINITY;
        }

        // Look at the vector end to stop at zero vel, acc.
        scurve_data s_stop=s;
        s.pd.stopinit=0;
        s_stop=jog_position_c(s_stop,1,0,0,trjlen,jog_fwd,jog_rev);
        if(curpos+s_stop.pd.stopdist>trjlen){
            // std::cout<<"setting endvel to zero."<<std::endl;
            endvel=0;
            endacc=0;
            if(!init_stop){
                s.pd.stopinit=0;
                init_stop=1;
            }
        }
        // End.

        if(!pause){ // No pause request.
            int pos_enable=1;
            s=jog_position_c(s,pos_enable,endvel,endacc,tarpos,jog_fwd,jog_rev);
        }

        if(pause){ // Pause request.
            double jog_enable=0; // Stop jogging to zero velocity.
            double jog_endvel=0;
            double jog_endacc=0;
            s=jog_velocity_c(s,jog_enable,jog_endvel,jog_endacc,jog_tarpos); // Stop asap.
        }

        s=jog_update_c(s); // Calculate update cycle.

        jog_results_c(s,&curvel,&curacc,&curpos,&finished); // Extract results.

        if(finished && jog_fwd){ // Segment finished, goto next segment.
            vector_nr_increment(ptr,&nr); // Vector safe solution.
        }
        if(finished && jog_rev){  // Segment finished, goto previous segment.
            vector_nr_decrement(ptr,&nr); // Vector safe solution.
        }

    }

    // Write graph values.
    qreal value=curvel;
    m_YData.append( value );

    value=curacc;
    m_YData1.append( value );

    value=curpos*0.1;
    m_YData2.append( value );

    ui->label_curvel->setText(QString::number(curvel,'f',10));
    ui->label_curacc->setText(QString::number(curacc,'f',10));
    ui->label_curpos->setText(QString::number(curpos,'f',10));
    ui->label_vectnr->setText(QString::number(nr,'f',0));
    ui->label_segbeg->setText(QString::number(vector_at_c(ptr,nr).lenbeg,'f',3));
    ui->label_segend->setText(QString::number(vector_at_c(ptr,nr).lenend,'f',3));
    ui->checkBox_finished->setChecked(finished);
    ui->checkBox_pause->setChecked(pause);
    ui->label_trjlen->setText(QString::number(trjlen,'f',3));
    ui->label_trjpro->setText(QString::number((curpos/trjlen),'f',2));
    ui->label_segpro->setText(QString::number(( (curpos - vector_at_c(ptr,nr).lenbeg) / vector_at_c(ptr,nr).path_lenght ),'f',2));
    ui->label_seg_vo->setText(QString::number(vector_at_c(ptr,nr).vo,'f',3));
    ui->label_seg_ve->setText(QString::number(vector_at_c(ptr,nr).ve,'f',3));

    // Keep the data buffers size under 100 value each,
    // so our moemoty won't explode with random numbers
    if( m_XData.size() > 500 ){
        m_XData.remove( 0 );
        m_YData.remove( 0 );
        m_YData1.remove( 0 );
        m_YData2.remove( 0 );
    }

    m_CustomPlot->graph( 0 )->setData( m_XData , m_YData );
    m_CustomPlot->graph( 1 )->setData( m_XData , m_YData2 );
    m_CustomPlot->graph( 2 )->setData( m_XData , m_YData1 );

    // Set the range of the vertical and horizontal axis of the plot ( not the graph )
    // so all the data will be centered. first we get the min and max of the x and y data
    QVector<double>::iterator xMaxIt = std::max_element( m_XData.begin() , m_XData.end() );
    QVector<double>::iterator xMinIt = std::min_element( m_XData.begin() , m_XData.end() );
    QVector<double>::iterator yMinIt = std::min_element( m_YData.begin() , m_YData.end() );
    QVector<double>::iterator yMaxIt = std::max_element( m_YData.begin() , m_YData.end() );

    //yMinIt = std::min_element( m_YData1.begin() , m_YData1.end() );
    //  yMaxIt = std::max_element( m_YData1.begin() , m_YData1.end() );

    qreal yPlotMin = *yMinIt;;
    // qreal yPlotMax = *yMaxIt;

    qreal xPlotMin = *xMinIt;
    qreal xPlotMax = *xMaxIt;

    m_CustomPlot->xAxis->setRange( xPlotMin , xPlotMax );
    //m_CustomPlot->yAxis->setRange( yPlotMin - fmax(maxvel_fo,maxvel) -1 , fmax(maxvel_fo,maxvel) +1 );

    m_CustomPlot->yAxis->setRange( -500 , 500);

    // Update the plot widget
    m_CustomPlot->replot();
}



void MainWindow::on_pushButton_setall_pressed()
{
    maxacc=ui->lineEdit_maxacc->text().toDouble();
    maxvel=ui->lineEdit_maxvel->text().toDouble();
    maxjer=ui->lineEdit_maxjer->text().toDouble();
}

void MainWindow::on_pushButton_pause_pressed()
{
    pause=1;
}

void MainWindow::on_pushButton_resume_pressed()
{
    pause=0;
}

void MainWindow::on_pushButton_run_pressed()
{
    run=1;
}

void MainWindow::on_pushButton_pause_timer_pressed()
{
    if(timer->isActive()){
        timer->stop();
        ui->checkBox_pause_timer->setChecked(1);
    } else {
        timer->start();
        ui->checkBox_pause_timer->setChecked(0);
    }
}
