#include "mainwindow.h"
#include "./ui_mainwindow.h"

// axis splash:  file:/home/user/linuxcnc/share/axis/images/axis.ngc

std::string oldfile;
double oldx=0,oldy=0,oldz=0;
double jog_speed=0;
bool lock_feed_override=0, lock_maxvel=0, lock_spindle_override=0, lock_rapid_override=0, lock_flood=0, lock_mist=0, lock_spindle=0;

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

    editor = new QGCodeEditor();
    ui->gridLayout_gcode->addWidget(editor);
    editor->setStyleSheet("background-color: rgb(255, 255, 255);\ncolor: rgb(0, 0, 0);");

    jog_speed=ui->horizontalSlider_jog_speed->value();

    //! This activates a screen update when robot is moving and screen needs to be updated automaticly.
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(50);

    hal_connection();

    std::string file;
    file += EMC2_IMAGE_DIR;
    file+="/share/axis/images/axis.ngc";
    nml->load(file);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Worker function definition
void MainWindow::workerFunction(const std::string &filename, std::atomic<bool> &done) {
    try {
        load_cad_data_from_interpreter(filename);
    } catch (const Standard_Failure &e) {
        std::cerr << "OpenCASCADE error: " << e.GetMessageString() << std::endl;
        // Handle the error appropriately
    } catch (const std::exception &e) {
        std::cerr << "Standard error: " << e.what() << std::endl;
        // Handle the error appropriately
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        // Handle the error appropriately
    }
    done = true;
}

void MainWindow::update(){

    //! Update nml.
    nml->update();

    // std::cout<<"loaded gcode file:"<<nml->theStatus.filename<<std::endl;

    //! Loaded gcode file.
    if(oldfile!=nml->theStatus.filename && nml->theStatus.filename.size()>0){

        nml->close();
        nml->mode_manual();

        // If a worker thread is still running, detach it
        if (workerThread.joinable()) {
            workerThread.detach();
        }

        // Start a new worker thread
        done = false; // Reset the done flag
        workerThread = std::thread(&MainWindow::workerFunction, this, nml->theStatus.filename, std::ref(done));

        // load_cad_data_from_interpreter(nml->theStatus.filename);
        load_gcode_text(nml->theStatus.filename);

        // timer->stop();
        // make_fillets(5);
        // timer->start(50);

        oldfile=nml->theStatus.filename;
    }

    //! Update xyz dro.
    form->update_dro(nml->theStatus.x,
                     nml->theStatus.y,
                     nml->theStatus.z,
                     nml->theStatus.dtgx,
                     nml->theStatus.dtgy,
                     nml->theStatus.dtgz,
                     nml->theStatus.current_velocity,
                     nml->theStatus.homed_x,
                     nml->theStatus.homed_y,
                     nml->theStatus.homed_z);

    //! Update cad cone.
    occ->translate_tp_cone(nml->theStatus.x,
                           nml->theStatus.y,
                           nml->theStatus.z,0,-0.5*M_PI,0);


    editor->highlightLine(nml->theStatus.motion_line);

    if(!lock_feed_override){
        ui->horizontalSlider_feed_override->setValue(nml->theStatus.feed_override*100);
    }

    if(!lock_rapid_override){
        ui->horizontalSlider_rapid_override->setValue(nml->theStatus.rapid_override*100);
    }

    if(!lock_spindle_override){
        ui->horizontalSlider_spindle_override->setValue(nml->theStatus.spindle_override*100);
    }

    if(!lock_maxvel){
        ui->horizontalSlider_max_velocity->setValue(nml->theStatus.max_velocity*60);
    }

    if(!lock_mist){
        ui->checkBox_mist->setChecked(nml->theStatus.mist);
    }

    if(!lock_flood){
        ui->checkBox_flood->setChecked(nml->theStatus.flood);
    }

    if(nml->theStatus.spindle_speed==0){
        ui->toolButton_spindle_stop->setChecked(true);
    } else {
        ui->toolButton_spindle_stop->setChecked(false);
    }
    if(nml->theStatus.spindle_speed>0){
        ui->toolButton_spindle_cw->setChecked(true);
    } else {
        ui->toolButton_spindle_cw->setChecked(false);
    }
    if(nml->theStatus.spindle_speed<0){
        ui->toolButton_spindle_ccw->setChecked(true);
    } else {
        ui->toolButton_spindle_ccw->setChecked(false);
    }

    ui->label_feed_override->setText(QString::number(nml->theStatus.feed_override*100,'f',0)+QString(" %"));
    ui->label_rapid_override->setText(QString::number(nml->theStatus.rapid_override*100,'f',0)+QString(" %"));
    ui->label_spindle_override->setText(QString::number(nml->theStatus.spindle_override*100,'f',0)+QString(" %"));
    ui->label_max_velocity->setText(QString::number(nml->theStatus.max_velocity*60,'f',0)+QString(" mm/min"));

    ui->label_spindle_rpm->setText(QString::number(nml->theStatus.spindle_speed/60,'f',1)+QString(" rps"));

    ui->label_jog_speed->setText(QString::number(ui->horizontalSlider_jog_speed->value(),'f',0)+QString(" mm/min"));

    //! Set active gcodes.
    ui->plainTextEdit_active_gcodes->clear();
    ui->plainTextEdit_active_gcodes->appendPlainText(nml->theStatus.task_active_gcodes_string);

    if(nml->theStatus.machine_on){
        ui->toolButton_machine_on->setChecked(true);
        ui->label_machine_on_status->setStyleSheet("background-color: rgb(0, 170, 0);");

    } else {
        ui->toolButton_machine_on->setChecked(false);
        ui->label_machine_on_status->setStyleSheet("background-color: rgb(170, 85, 0);");
    }
    if(nml->theStatus.estop){
        ui->toolButton_emergency->setChecked(true);
        ui->label_emergency_status->setStyleSheet("background-color: rgb(170, 0, 0);");
        ui->label_machine_status->setText("ESTOP");
    } else {
        ui->label_emergency_status->setStyleSheet("background-color: rgb(0, 170, 0);");
        ui->label_machine_status->setText("OFF");
    }

    if(nml->theStatus.machine_on && !nml->theStatus.estop){
        ui->label_machine_status->setText("ON");
    }

    //! To update tp moves.
    occ->redraw();

    // Ensure the worker thread has completed
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void MainWindow::get_segvec_from_caddata(std::vector<cad_data> cvec, std::vector<tp_segment> &svec){

    for(uint i=0; i<cvec.size(); i++){

        cad_data d=cvec.at(i);

        tp_segment seg;

        seg.motion_tolerance=d.motion_tolerance;        // G64 P
        seg.naivecam_tolerance=d.naivecam_tolerance;    // G64 Q

        if(d.type==1 /*G1*/ || d.type==3 /*G0*/){ //! Draw opencascade line.
            gp_Pnt p0(oldx,oldy,oldz);
            gp_Pnt p1(d.pose.tran.x, d.pose.tran.y, d.pose.tran.z);
            gp_Pnt pw( (oldx+d.pose.tran.x)/2 , (oldy+d.pose.tran.y)/2 , (oldz+d.pose.tran.z)/2 ); // Create waypoint for the line.
            if(p0.Distance(p1)>0){ // Validate.

                seg.primitive_id=sc_line;
                seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                seg.pnt_e={p1.X(),p1.Y(), p1.Z()};
                seg.pnt_w={pw.X(),pw.Y(), pw.Z()};
                seg.path_lenght=p0.Distance(p1);

                if(d.type==1){ // G1
                    //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_line(p0,p1),Quantity_NOC_GRAY50,0) );
                    seg.motion_type=sc_linear;
                }
                if(d.type==3){ // G0
                    //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_line(p0,p1),Quantity_NOC_GRAY15,0) );
                    seg.motion_type=sc_rapid;
                }
                // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                svec.push_back(seg);
            }
        }
        if(d.type==2){ //! Draw opencascade arc.

            seg.primitive_id=sc_arc;
            seg.motion_type=sc_circle;

            if(d.plane==1){ //! XY plane
                // std::cout<<"plane 1, XY"<<std::endl; //! See 3dtest.ngc how to check this planes.

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.x, d.pose.tran.y,  d.pose.tran.z);
                gp_Pnt pc(d.cx,d.cy, d.pose.tran.z);
                gp_Pnt pw;

                if(p0.Distance(pc)>0 && p0.Distance(p1)>0){ // Validate its a arc.

                    seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                    seg.pnt_e={p1.X(),p1.Y(),p1.Z()};
                    seg.pnt_c={pc.X(),pc.Y(),pc.Z()};

                    /*
                         std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                         std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                         std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                         std::cout<<""<<std::endl;*/

                    if(d.rotation==-1){
                        // occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p1,p0,pc,0,0,1,pw),Quantity_NOC_GRAY50,0) );
                    } else {
                        // occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p0,p1,pc,0,0,1,pw),Quantity_NOC_GRAY50,0) );
                    }

                    seg.pnt_w={pw.X(),pw.Y(),pw.Z()};
                    seg.path_lenght=draw_primitives::get_3d_arc_lenght(p0,pw,p1);
                    // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                    svec.push_back(seg);
                }
                if(p0.Distance(pc)>0  && p0.Distance(p1)==0){
                    if(d.rotation==-1){
                        //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,0,1,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    } else {
                        // occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,0,1,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    }
                    svec.push_back(seg);
                }
            }
            if(d.plane==2){ //!
                // std::cout<<"plane 2, YZ"<<std::endl;

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.z,d.pose.tran.x, d.pose.tran.y);
                gp_Pnt pc(d.pose.tran.z,d.cx,d.cy);
                gp_Pnt pw;

                if(p0.Distance(pc)>0 && p0.Distance(p1)>0){ // Validate its a arc.

                    seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                    seg.pnt_e={p1.X(),p1.Y(),p1.Z()};
                    seg.pnt_c={pc.X(),pc.Y(),pc.Z()};

                    /*
                     std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                     std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                     std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                     std::cout<<""<<std::endl;*/

                    if(d.rotation==-1){
                        //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p1,p0,pc,1,0,0,pw),Quantity_NOC_GRAY50,0) );
                    } else {
                        //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p0,p1,pc,1,0,0,pw),Quantity_NOC_GRAY50,0) );
                    }

                    seg.pnt_w={pw.X(),pw.Y(),pw.Z()};
                    seg.path_lenght=draw_primitives::get_3d_arc_lenght(p0,pw,p1);
                    // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                    svec.push_back(seg);
                }
                if(p0.Distance(pc)>0  && p0.Distance(p1)==0){
                    if(d.rotation==-1){
                        // occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,1,0,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    } else {
                        // occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,1,0,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    }
                    svec.push_back(seg);
                }
            }
            if(d.plane==3){ //! Y swap Z.
                // std::cout<<"plane 3, XZ"<<std::endl;

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.x, d.pose.tran.z,d.pose.tran.y );
                gp_Pnt pc(d.cx, d.pose.tran.z,d.cy);
                gp_Pnt pw;

                if(p0.Distance(pc)>0 && p0.Distance(p1)>0){ // Validate its a arc.

                    seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                    seg.pnt_e={p1.X(),p1.Y(),p1.Z()};
                    seg.pnt_c={pc.X(),pc.Y(),pc.Z()};

                    /*
                     std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                     std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                     std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                     std::cout<<""<<std::endl;*/

                    if(d.rotation==-1){
                        //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p1,p0,pc,0,1,0,pw),Quantity_NOC_GRAY50,0) );
                    } else {
                        //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p0,p1,pc,0,1,0,pw),Quantity_NOC_GRAY50,0) );
                    }

                    seg.pnt_w={pw.X(),pw.Y(),pw.Z()};
                    seg.path_lenght=draw_primitives::get_3d_arc_lenght(p0,pw,p1);
                    // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                    svec.push_back(seg);
                }
                if(p0.Distance(pc)>0  && p0.Distance(p1)==0){
                    if(d.rotation==-1){
                        //occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,1,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    } else {
                        // occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,1,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    }
                    svec.push_back(seg);
                }
            }
        }

        oldx=d.pose.tran.x;
        oldy=d.pose.tran.y;
        oldz=d.pose.tran.z;
    }
}

//! Todo colorize rapids little different.
void MainWindow::load_cad_data_from_interpreter(std::string filename)
{
    occ->clear_shapevec();
    segvec.clear();

    std::vector<cad_data> cadvec=runInterpreter(filename);

    std::cout<<"cadvec size:"<<cadvec.size()<<std::endl;
    if(cadvec.size()>20000){

        std::cout<<"cadvec size > 20.000 limit, selecting polygon preview."<<std::endl;

        get_segvec_from_caddata(cadvec,segvec);

        std::cout<<"segvec loaded ok"<<std::endl;

        //! Basic impementation example:
        TopoDS_Shape ResultShape;
        //TColgp_Array1OfPnt array (1,segvec.size()-1); // sizing array

        TColgp_Array1OfPnt array (1,segvec.size()+10); // sizing array


        for(uint i=1; i<segvec.size(); i++){
            gp_Pnt p0={segvec[i].pnt_s.x, segvec[i].pnt_s.y, segvec[i].pnt_s.z};
            gp_Pnt p1={segvec[i].pnt_w.x, segvec[i].pnt_w.y, segvec[i].pnt_w.z};
            gp_Pnt p2={segvec[i].pnt_e.x, segvec[i].pnt_e.y, segvec[i].pnt_e.z};
            if(p0.Distance(p2)>0.1){
                    array.SetValue(i,p2);
            }
        }

        Handle(Poly_Polygon3D) aPolygon = new Poly_Polygon3D(array);
        BRep_Builder Brep;
        TopoDS_Edge& E = TopoDS::Edge(ResultShape);
        Brep.MakeEdge(E,aPolygon);
        Handle(AIS_Shape) aShape=new AIS_Shape(ResultShape);
        aShape=draw_primitives::colorize(aShape,Quantity_NOC_WHITE,0);
        occ->add_shapevec(aShape);

        return;
    }

    // Normal loading.
    for(uint i=0; i<cadvec.size(); i++){

        cad_data d=cadvec.at(i);

        tp_segment seg;

        if(d.type==1 /*G1*/ || d.type==3 /*G0*/){ //! Draw opencascade line.
            gp_Pnt p0(oldx,oldy,oldz);
            gp_Pnt p1(d.pose.tran.x, d.pose.tran.y, d.pose.tran.z);
            gp_Pnt pw( (oldx+d.pose.tran.x)/2 , (oldy+d.pose.tran.y)/2 , (oldz+d.pose.tran.z)/2 ); // Create waypoint for the line.
            if(p0.Distance(p1)>0){ // Validate.

                seg.primitive_id=sc_line;
                seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                seg.pnt_e={p1.X(),p1.Y(), p1.Z()};
                seg.pnt_w={pw.X(),pw.Y(), pw.Z()};
                seg.path_lenght=p0.Distance(p1);

                if(d.type==1){ // G1
                    occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_line(p0,p1),Quantity_NOC_GRAY50,0) );
                    seg.motion_type=sc_linear;
                }
                if(d.type==3){ // G0
                    occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_line(p0,p1),Quantity_NOC_GRAY15,0) );
                    seg.motion_type=sc_rapid;
                }
                // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                segvec.push_back(seg);
            }
        }
        if(d.type==2){ //! Draw opencascade arc.

            seg.primitive_id=sc_arc;
            seg.motion_type=sc_circle;

            if(d.plane==1){ //! XY plane
                // std::cout<<"plane 1, XY"<<std::endl; //! See 3dtest.ngc how to check this planes.

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.x, d.pose.tran.y,  d.pose.tran.z);
                gp_Pnt pc(d.cx,d.cy, d.pose.tran.z);
                gp_Pnt pw;

                if(p0.Distance(pc)>0 && p0.Distance(p1)>0){ // Validate its a arc.

                    seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                    seg.pnt_e={p1.X(),p1.Y(),p1.Z()};
                    seg.pnt_c={pc.X(),pc.Y(),pc.Z()};

                    /*
                    std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                    std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                    std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                    std::cout<<""<<std::endl;*/

                    if(d.rotation==-1){
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p1,p0,pc,0,0,1,pw),Quantity_NOC_GRAY50,0) );
                    } else {
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p0,p1,pc,0,0,1,pw),Quantity_NOC_GRAY50,0) );
                    }

                    seg.pnt_w={pw.X(),pw.Y(),pw.Z()};
                    seg.path_lenght=draw_primitives::get_3d_arc_lenght(p0,pw,p1);
                    // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                    segvec.push_back(seg);
                }
                if(p0.Distance(pc)>0  && p0.Distance(p1)==0){
                    if(d.rotation==-1){
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,0,1,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    } else {
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,0,1,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    }
                    segvec.push_back(seg);
                }
            }
            if(d.plane==2){ //!
                // std::cout<<"plane 2, YZ"<<std::endl;

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.z,d.pose.tran.x, d.pose.tran.y);
                gp_Pnt pc(d.pose.tran.z,d.cx,d.cy);
                gp_Pnt pw;

                if(p0.Distance(pc)>0 && p0.Distance(p1)>0){ // Validate its a arc.

                    seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                    seg.pnt_e={p1.X(),p1.Y(),p1.Z()};
                    seg.pnt_c={pc.X(),pc.Y(),pc.Z()};

                    /*
                std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                std::cout<<""<<std::endl;*/

                    if(d.rotation==-1){
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p1,p0,pc,1,0,0,pw),Quantity_NOC_GRAY50,0) );
                    } else {
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p0,p1,pc,1,0,0,pw),Quantity_NOC_GRAY50,0) );
                    }

                    seg.pnt_w={pw.X(),pw.Y(),pw.Z()};
                    seg.path_lenght=draw_primitives::get_3d_arc_lenght(p0,pw,p1);
                    // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                    segvec.push_back(seg);
                }
                if(p0.Distance(pc)>0  && p0.Distance(p1)==0){
                    if(d.rotation==-1){
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,1,0,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    } else {
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,1,0,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    }
                    segvec.push_back(seg);
                }
            }
            if(d.plane==3){ //! Y swap Z.
                // std::cout<<"plane 3, XZ"<<std::endl;

                gp_Pnt p0(oldx,oldy,oldz);
                gp_Pnt p1(d.pose.tran.x, d.pose.tran.z,d.pose.tran.y );
                gp_Pnt pc(d.cx, d.pose.tran.z,d.cy);
                gp_Pnt pw;

                if(p0.Distance(pc)>0 && p0.Distance(p1)>0){ // Validate its a arc.

                    seg.pnt_s={p0.X(),p0.Y(),p0.Z()};
                    seg.pnt_e={p1.X(),p1.Y(),p1.Z()};
                    seg.pnt_c={pc.X(),pc.Y(),pc.Z()};

                    /*
                std::cout<<"arc start  p0x:"<<p0.X()<<" p0y:"<<p0.Y()<<" p0z:"<<p0.Z()<<std::endl;
                std::cout<<"arc end    p1x:"<<p1.X()<<" p1y:"<<p1.Y()<<" p1z:"<<p1.Z()<<std::endl;
                std::cout<<"arc center pcx:"<<pc.X()<<" pcy:"<<pc.Y()<<" pcz:"<<pc.Z()<<std::endl;
                std::cout<<""<<std::endl;*/

                    if(d.rotation==-1){
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p1,p0,pc,0,1,0,pw),Quantity_NOC_GRAY50,0) );
                    } else {
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_arc(p0,p1,pc,0,1,0,pw),Quantity_NOC_GRAY50,0) );
                    }

                    seg.pnt_w={pw.X(),pw.Y(),pw.Z()};
                    seg.path_lenght=draw_primitives::get_3d_arc_lenght(p0,pw,p1);
                    // occ->add_shapevec(draw_primitives::draw_3d_point(pw));
                    segvec.push_back(seg);
                }
                if(p0.Distance(pc)>0  && p0.Distance(p1)==0){
                    if(d.rotation==-1){
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,1,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    } else {
                        occ->add_shapevec(draw_primitives::colorize( draw_primitives::draw_3d_pc_circle(pc,0,1,0,p1.Distance(pc)),Quantity_NOC_GRAY50,0) );
                    }
                    segvec.push_back(seg);
                }
            }
        }

        oldx=d.pose.tran.x;
        oldy=d.pose.tran.y;
        oldz=d.pose.tran.z;
    }

    // occ->fit_all();
}

int MainWindow::digits(int nr)
{
    int digits = 0;
    if (nr < 0) digits = 1; // remove this line if '-' counts as a digit
    while (nr) {
        nr /= 10;
        digits++;
    }
    return digits;
}

void MainWindow::load_gcode_text(std::string filename){

    editor->clear();

    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {

        QString line;
        line.append(in.readLine());
        editor->appendNewPlainText(line);
    }
    file.close();
}

void MainWindow::on_toolButton_fit_all_pressed()
{
    occ->fit_all();
}

void MainWindow::on_toolButton_view_front_pressed()
{
    occ->set_view_front();
}

void MainWindow::on_toolButton_view_back_pressed()
{
    occ->set_view_back();
}

void MainWindow::on_toolButton_view_top_pressed()
{
    occ->set_view_top();
}

void MainWindow::on_toolButton_view_bottom_pressed()
{
    occ->set_view_bottom();
}

void MainWindow::on_toolButton_view_left_pressed()
{
    occ->set_view_left();
}

void MainWindow::on_toolButton_view_right_pressed()
{
    occ->set_view_right();
}

void MainWindow::on_toolButton_view_3d_pressed()
{
    occ->set_view_3d();
}

void MainWindow::on_toolButton_zoom_min_pressed()
{
    occ->zoom_min();
}

void MainWindow::on_toolButton_zoom_plus_pressed()
{
    occ->zoom_plus();
}

void MainWindow::on_toolButton_stop_pressed()
{
    nml->stop();
    nml->mode_manual();
}

void MainWindow::on_toolButton_pause_toggled(bool checked)
{
    if(checked){
        nml->pause();
    } else {
        nml->resume();
    }
}

void MainWindow::on_toolButton_run_pressed()
{
    nml->mode_auto();
    nml->run();
}

void MainWindow::on_toolButton_file_open_pressed()
{
    occ->clear_shapevec();
    segvec.clear();

    nml->mode_auto();
    nml->close();
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/jana", tr("Image Files (*.txt *.ngc)"));
    nml->load(filename.toStdString());
}

void MainWindow::on_toolButton_reload_pressed()
{
    nml->mode_auto();
    std::string filename=nml->theStatus.filename;
    nml->close();
    nml->load(filename);

}

void MainWindow::on_toolButton_machine_on_pressed()
{
    if(ui->toolButton_machine_on->isChecked()){
        nml->machine_off();
    } else {
        nml->machine_on();
    }
}

void MainWindow::on_toolButton_emergency_pressed()
{
    if(ui->toolButton_emergency->isChecked()){
        nml->estop_reset();
    } else {
        nml->estop();
    }
}

void MainWindow::on_horizontalSlider_max_velocity_sliderMoved(int position)
{
    nml->setMaxVelocity(position/60);
    lock_maxvel=1;
}

void MainWindow::on_horizontalSlider_max_velocity_sliderReleased()
{
    lock_maxvel=0;
}

void MainWindow::on_horizontalSlider_feed_override_sliderMoved(int position)
{
    nml->setFeedOveride(position*0.01);
    lock_feed_override=1;
}

void MainWindow::on_horizontalSlider_feed_override_sliderReleased()
{
    lock_feed_override=0;
}

void MainWindow::on_horizontalSlider_rapid_override_sliderMoved(int position)
{
    nml->setRapidOverride(position*0.01);
    lock_rapid_override=1;
}

void MainWindow::on_horizontalSlider_rapid_override_sliderReleased()
{
    lock_rapid_override=0;
}

void MainWindow::on_horizontalSlider_spindle_override_sliderMoved(int position)
{
    nml->setSpindleOverride(0,position*0.01);
    lock_spindle_override=1;
}

void MainWindow::on_horizontalSlider_spindle_override_sliderReleased()
{
    lock_spindle_override=0;
}

int MainWindow::get_jog_axis(){
    int jog_axis=0;
    if(ui->radioButton_x->isChecked()){
        jog_axis=0;
    }
    if(ui->radioButton_y->isChecked()){
        jog_axis=1;
    }
    if(ui->radioButton_z->isChecked()){
        jog_axis=2;
    }
    return jog_axis;
}

void MainWindow::on_pushButton_jog_min_pressed()
{
    nml->jog(get_jog_axis(),-ui->horizontalSlider_jog_speed->value()/60);
}

void MainWindow::on_pushButton_jog_min_released()
{
    nml->jog_stop(get_jog_axis());
}

void MainWindow::on_pushButton_jog_plus_pressed()
{
    nml->jog(get_jog_axis(),ui->horizontalSlider_jog_speed->value()/60);
}

void MainWindow::on_pushButton_jog_plus_released()
{
    nml->jog_stop(get_jog_axis());
}

void MainWindow::on_checkBox_mist_clicked(bool checked)
{
    if(checked){
        nml->mist_on();
    } else {
        nml->mist_off();
    }
}

void MainWindow::on_checkBox_flood_clicked(bool checked)
{
    if(checked){
        nml->flood_on();
    } else {
        nml->flood_off();
    }
}

void MainWindow::on_toolButton_spindle_stop_pressed()
{
    nml->spindle_off(0);
}

void MainWindow::on_toolButton_spindle_ccw_pressed()
{
    nml->spindle_on(0,-ui->spinBox_spindlespeed->value()*60);
}

void MainWindow::on_toolButton_spindle_cw_pressed()
{
    nml->spindle_on(0,ui->spinBox_spindlespeed->value()*60);
}

void MainWindow::on_pushButton_spindle_plus_pressed()
{
    ui->spinBox_spindlespeed->setValue(ui->spinBox_spindlespeed->value()+1);

    if(nml->theStatus.spindle_speed<0){
        nml->spindle_on(0,-ui->spinBox_spindlespeed->value()*60);
    }
    if(nml->theStatus.spindle_speed>0){
        nml->spindle_on(0,ui->spinBox_spindlespeed->value()*60);
    }
}

void MainWindow::on_pushButton_spindle_min_pressed()
{
    ui->spinBox_spindlespeed->setValue(ui->spinBox_spindlespeed->value()-1);

    if(nml->theStatus.spindle_speed<0){
        nml->spindle_on(0,-ui->spinBox_spindlespeed->value()*60);
    }
    if(nml->theStatus.spindle_speed>0){
        nml->spindle_on(0,ui->spinBox_spindlespeed->value()*60);
    }
}

void MainWindow::on_toolButton_mdi_command_exec_pressed()
{
    QString mdi_text=ui->lineEdit_mdi_command->text();
    nml->mdi(mdi_text.toStdString());
    nml->mdi_execute();

    ui->plainTextEdit_mdi_history->appendPlainText(mdi_text);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index==0){
        nml->mode_manual();
    }
    if(index==1){
        nml->mode_mdi();
    }
}

void MainWindow::on_pushButton_home_all_pressed()
{
    nml->home_all();
}

void MainWindow::hal_connection(){
    // Create hal component from here.
    int comp_id = hal_init("QT_C++_Interface");
    hal_comp_name(comp_id);
    hal_malloc(200);
    // int sigml= hal_signal_new("xxx", HAL_FLOAT );
    hal_ready(comp_id);
}

void MainWindow::on_pushButton_test_pressed()
{
    //std::string file;
    //file += EMC2_IMAGE_DIR;
    //file+="/share/axis/images/axis.ngc";

    // load_cad_data_from_interpreter(oldfile);

    //if(ui->checkBox->isChecked()){
    occ->clear_shapevec();
    // }

    create_clothoid_fillets();
}

void MainWindow::on_pushButton_calc_pressed()
{
    on_pushButton_test_pressed();
}

void MainWindow::create_clothoid_fillets(){

    double offset=ui->doubleSpinBox_offset->value();
    double filter=ui->doubleSpinBox_filter->value();

    struct gp_Tri {
        gp_Pnt p0, p1,p2;
    };
    std::vector<gp_Tri> trivec;
    bool edit_next = false;
    gp_Pnt last_valid_point; // Track the last valid point

    for (uint i = 0; i < segvec.size(); ++i) {
        gp_Pnt p0 = {segvec[i].pnt_s.x, segvec[i].pnt_s.y, segvec[i].pnt_s.z};
        gp_Pnt p1 = {segvec[i].pnt_w.x, segvec[i].pnt_w.y, segvec[i].pnt_w.z};
        gp_Pnt p2 = {segvec[i].pnt_e.x, segvec[i].pnt_e.y, segvec[i].pnt_e.z};

        if (edit_next) {
            p0 = last_valid_point;
            p1 = draw_primitives::get_line_midpoint(p0, p2); // Treat as line. Avoid arc.
            edit_next = false; // Reset flag
        }

        if (p0.Distance(p2) >= filter) {
            trivec.push_back({p0, p1, p2});
            last_valid_point = p2; // Update last valid point
        } else {
            edit_next = true;
        }
    }

    // Validatoin check if points are connected.
    for(uint i=0; i<trivec.size()-1; i++){

        std::cout<<"trivec at:"<<i<<std::endl;

        gp_Pnt p0=trivec[i].p0;
        gp_Pnt p1=trivec[i].p1;
        gp_Pnt p2=trivec[i].p2;

        gp_Pnt p3=trivec[i+1].p0;
        gp_Pnt p4=trivec[i+1].p1;
        gp_Pnt p5=trivec[i+1].p2;

        std::cout<<"dist seg to seg:"<<p2.Distance(p3)<<std::endl;

    }

    // Huge file, show only a polygon.
    if(trivec.size()>20000){

        //! Basic impementation example:
        TopoDS_Shape ResultShape;
        //TColgp_Array1OfPnt array (1,segvec.size()-1); // sizing array

        TColgp_Array1OfPnt array (1,trivec.size()+10); // sizing array

        for(uint i=0; i<trivec.size()-1; i++){
               array.SetValue(i+1,trivec[i].p2);
        }

        Handle(Poly_Polygon3D) aPolygon = new Poly_Polygon3D(array);
        BRep_Builder Brep;
        TopoDS_Edge& E = TopoDS::Edge(ResultShape);
        Brep.MakeEdge(E,aPolygon);
        Handle(AIS_Shape) aShape=new AIS_Shape(ResultShape);
        aShape=draw_primitives::colorize(aShape,Quantity_NOC_WHITE,0);
        occ->add_shapevec(aShape);

        return;
    }

    // Normal creation.
    for(uint i=0; i<trivec.size()-1; i++){

        gp_Pnt p0=trivec[i].p0;
        gp_Pnt p1=trivec[i].p1;
        gp_Pnt p2=trivec[i].p2;

        gp_Pnt p3=trivec[i+1].p0;
        gp_Pnt p4=trivec[i+1].p1;
        gp_Pnt p5=trivec[i+1].p2;

        draw_primitives::get_3d_fillet_start_end_point(p0,p1,p2,p3,p4,p5,offset);

        trivec[i].p1=p1;
        trivec[i].p2=p2;
        trivec[i+1].p0=p3;
        trivec[i+1].p1=p4;

        //occ->add_shapevec( draw_primitives::colorize( draw_primitives::draw_3d_line(p2,p3), Quantity_NOC_YELLOW,0));
        std::vector<gp_Pnt> clotvec;
        draw_clothoids::draw_inbetween_3d_clothoid_3arc_G2(p0,p1,p2,p3,p4,p5,clotvec);
        occ->add_shapevec( draw_primitives::colorize(  draw_primitives::draw_3d_line_wire( clotvec ), Quantity_NOC_WHITE,0));
    }

    for(uint i=0; i<trivec.size(); i++){
        occ->add_shapevec( draw_primitives::colorize( draw_primitives::draw_3p_3d_arc(trivec[i].p0,trivec[i].p1,trivec[i].p2), Quantity_NOC_WHITE,0));
    }
}
