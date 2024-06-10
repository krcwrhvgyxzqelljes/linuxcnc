#include "mainwindow.h"
#include "./ui_mainwindow.h"

std::string oldfile="";
std::string filename="";
double oldx=0,oldy=0,oldz=0;
extern std::vector<cad_data> runInterpreter(std::string filename);

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

    //! Create a nml interface. From here we can see wich gcode file to load.
    stat = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", EMC2_DEFAULT_NMLFILE);

    //! This activates a screen update when robot is moving and screen needs to be updated automaticly.
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int s=0;

void MainWindow::update(){

    //! Update nml.
    for(int i=0; i<1; i++){
        usleep(1);
        if(!stat->valid()) continue;
        if(stat->peek() != EMC_STAT_TYPE) continue;
        emcStatus = static_cast<EMC_STAT*>(stat->get_address());
    }

    //! Loaded gcode file.
    filename=emcStatus->task.file;
    if(oldfile!=filename && filename.size()>0){
        load_cad_data_from_interpreter();
        oldfile=filename;
    }

    //! Set xyz dro.
    double x=emcStatus->motion.traj.position.tran.x;
    double y=emcStatus->motion.traj.position.tran.y;
    double z=emcStatus->motion.traj.position.tran.z;
    double dtgx=emcStatus->motion.traj.dtg.tran.x;
    double dtgy=emcStatus->motion.traj.dtg.tran.y;
    double dtgz=emcStatus->motion.traj.dtg.tran.z;
    double vel=emcStatus->motion.traj.current_vel;
    bool homedx=emcStatus->motion.joint[0].homed;
    bool homedy=emcStatus->motion.joint[1].homed;
    bool homedz=emcStatus->motion.joint[2].homed;

    form->update_dro(x,y,z,dtgx,dtgy,dtgz,vel,homedx,homedy,homedz);
    occ->translate_tp_cone(x,y,z,0,-0.5*M_PI,0);

    //! Trigger overlay button press.
    if(form->is_fit_all()){
        occ->fit_all();
        form->reset_btn_press();
    }

    //! To update tp moves.
    occ->redraw();
}

//! Todo colorize rapids little different.
void MainWindow::load_cad_data_from_interpreter()
{  
    occ->clear_shapevec();

    std::vector<cad_data> cadvec=runInterpreter(filename);
    s=cadvec.size();

    for(uint i=0; i<cadvec.size(); i++){

        cad_data d=cadvec.at(i);

        if(d.type==1 /*G1*/ || d.type==3 /*G0*/){ //! Draw opencascade line.
            gp_Pnt p0(oldx,oldy,oldz);
            gp_Pnt p1(d.pose.tran.x, d.pose.tran.y, d.pose.tran.z);

            if(d.type==1){
                occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_3d_line(p0,p1),Quantity_NOC_GRAY50,0) );
            }
            if(d.type==3){
                occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_3d_line(p0,p1),Quantity_NOC_GRAY15,0) );
            }
        }
        if(d.type==2){ //! Draw opencascade arc.

            if(d.plane==1){ //! XY plane
                // std::cout<<"plane 1, XY"<<std::endl; //! See 3dtest.ngc how to check this planes.

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.x, d.pose.tran.y,  d.pose.tran.z);
                gp_Pnt pc(d.cx,d.cy, d.pose.tran.z);
                /*
                std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                std::cout<<""<<std::endl;*/

                if(d.rotation==-1){
                    occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_cp_3d_arc(p1,p0,pc,0,0,1),Quantity_NOC_GRAY50,0) );
                } else {
                    occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_cp_3d_arc(p0,p1,pc,0,0,1),Quantity_NOC_GRAY50,0) );
                }
            }
            if(d.plane==2){ //!
                // std::cout<<"plane 2, YZ"<<std::endl;

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.z,d.pose.tran.x, d.pose.tran.y);
                gp_Pnt pc(d.pose.tran.z,d.cx,d.cy);
                /*
                std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                std::cout<<""<<std::endl;*/

                if(d.rotation==-1){
                    occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_cp_3d_arc(p1,p0,pc,1,0,0),Quantity_NOC_GRAY50,0) );
                } else {
                    occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_cp_3d_arc(p0,p1,pc,1,0,0),Quantity_NOC_GRAY50,0) );
                }
            }
            if(d.plane==3){ //! Y swap Z.
                // std::cout<<"plane 3, XZ"<<std::endl;

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.x, d.pose.tran.z,d.pose.tran.y );
                gp_Pnt pc(d.cx, d.pose.tran.z,d.cy);
                /*
                std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                std::cout<<""<<std::endl;*/

                if(d.rotation==-1){
                    occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_cp_3d_arc(p1,p0,pc,0,1,0),Quantity_NOC_GRAY50,0) );
                } else {
                    occ->add_shapevec(draw_primitives().colorize( draw_primitives().draw_cp_3d_arc(p0,p1,pc,0,1,0),Quantity_NOC_GRAY50,0) );
                }
            }
        }

        oldx=d.pose.tran.x;
        oldy=d.pose.tran.y;
        oldz=d.pose.tran.z;
    }

    occ->fit_all();
}

