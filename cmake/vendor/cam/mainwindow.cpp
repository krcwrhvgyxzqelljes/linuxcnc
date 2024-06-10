#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace Eigen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // https://www.youtube.com/watch?v=2VIGb31iwds

    ui->stackedWidget->setCurrentIndex(1);

    OpencascadeWidget = new Opencascade(this);
    ui->gridLayout_opencascade->addWidget(OpencascadeWidget);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::Update_Opencascade);
    timer->start(200);

    QObject::connect(ui->toolButton_edit_lead_in_out, SIGNAL(pressed()),SLOT(rotate_lead_in_out()));
    QObject::connect(ui->toolButton_add_operation, SIGNAL(pressed()),SLOT(add_operation()));
    QObject::connect(ui->toolButton_process_gcode, SIGNAL(pressed()),SLOT(process_operations()));
    QObject::connect(ui->toolButton_clear_operations, SIGNAL(pressed()),SLOT(clear_operations()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_operation(){
    gcode_get_user_settings(); // Update gc.
    gcvec.push_back(gc);
    ui->listWidget->addItem(QString::fromStdString(gc.layer));
    process();
}

void MainWindow::process_operations(){
    linenumber=0; // Reset gcode linenumber.
    gcode().clear();

    gc.operation_type="intro";
    gcode().generate();

    for(unsigned int i=0; i<gcvec.size(); i++){
        // Set the gcode parameters for this job.
        gc=gcvec.at(i);
        // Create offsets, pockets, etc.
        process();
        // Create gcode for this job.
        gcode().generate();
    }

    gc.operation_type="outtro";
    gcode().generate();

    gcode_preview();    // Update the mainwindow gcode preview:

    for(unsigned int i=0; i<rapidvec.size(); i++){
        OpencascadeWidget->show_shape(rapidvec.at(i).ashape); // Show gcode rapids.
    }
}

void MainWindow::clear_operations(){
    OpencascadeWidget->erase_all(); // Erases all content from the screen.
    contourvec.clear();
    pocketvec.clear();
    datavec.clear();
    rapidvec.clear();

    // Draw only the dxf data back on the screen:
    for(unsigned int i=0; i<dxfvec.size(); i++){
        OpencascadeWidget->show_shape(dxfvec.at(i).ashape); // Dxf drawing preview.
    }
    OpencascadeWidget->show_3d_interactive_box();

    gcvec.clear();
    ui->listWidget->clear();
    gcode().clear(); // Empty gcode.ngc file
    gcode_preview(); // Preview empty gcode.ngc file
}

void MainWindow::Update_Opencascade()
{
    OpencascadeWidget->Redraw();
}

bool MainWindow::open_dxf_file(std::string filename){

    // OpencascadeWidget->erase_all();
    contourvec.clear();
    pocketvec.clear();
    datavec.clear();
    dxfvec.clear();
    rapidvec.clear();
    ui->comboBox_layer->clear();
    dx_iface().clear(&fData);           // Clear previous loaded dxf data.

    bool ok = iface->fileImport( filename, &fData );
    if (!ok) {
        std::cout << "Error reading file " << filename << std::endl;
    } else {
        std::cout << "Ok reading file " << filename << std::endl;

        // Process opencascade primitives.
        load_opencascade_primitives();
        OpencascadeWidget->zoom_all();
    }
    return ok;
}

bool MainWindow::save_dxf_file(std::string filename){

    bool ok = iface->fileExport(filename, DRW::AC1027 , false, &fData);
    if (!ok) {
        std::cout << "Error saving file " << filename << std::endl;
    } else {
        std::cout << "Ok saving file " << filename << std::endl;
    }
    return ok;
}

//! Example how to write a primitive to the dxf file.
bool MainWindow::write_entity(){

    std::cout<<"add line item"<<std::endl;
    DRW_Line testline;
    testline.basePoint.x = 10;
    testline.basePoint.y = 20;
    testline.secPoint.x = 30;
    testline.secPoint.y = 30;
    iface->addLine(testline);
    return 1;
}

void MainWindow::load_opencascade_primitives(){

    // Print the dxf output.
    auto blocks = fData.mBlock->ent;
    for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
        if(DRW::POINT == (*iter)->eType){ // std::cout<<"point"<<std::endl;

            DRW_Point *point = dynamic_cast<DRW_Point*>(*iter);
            Handle(AIS_Shape) ashape=draw_primitives().draw_3d_point({point->basePoint.x,point->basePoint.y,point->basePoint.z});
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::point;
            d.start={point->basePoint.x,point->basePoint.y,point->basePoint.z};
            d.end={point->basePoint.x,point->basePoint.y,point->basePoint.z};
            d.acad_layer=point->layer;
            dxfvec.push_back(d);
        }

        if(DRW::LINE == (*iter)->eType){

            DRW_Line *line = dynamic_cast<DRW_Line*>(*iter);
            Handle(AIS_Shape) ashape=draw_primitives().draw_3d_line({line->basePoint.x, line->basePoint.y, line->basePoint.z},{line->secPoint.x, line->secPoint.y, line->secPoint.z});
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::line;
            d.start={line->basePoint.x, line->basePoint.y, line->basePoint.z};
            // Line middle-point, half-way point.
            d.control.push_back({(line->basePoint.x+line->secPoint.x)/2, (line->basePoint.y+line->secPoint.y)/2,(line->basePoint.z+line->secPoint.z)/2});
            // std::cout<<"controlpoint line x:"<<d.control.back().X()<<" y:"<<d.control.back().Y()<<" z:"<<d.control.back().Z()<<std::endl;
            d.end={line->secPoint.x, line->secPoint.y, line->secPoint.z};
            d.acad_layer=line->layer;
            dxfvec.push_back(d);
        }

        // Lwpolyline is saved as clockwise [cw]
        if(DRW::LWPOLYLINE == (*iter)->eType){ // std::cout<<"lwpolyline"<<std::endl;

            DRW_LWPolyline *lwpolyline = dynamic_cast<DRW_LWPolyline*>(*iter);
            std::vector<gp_Pnt> pntvec;
            for(unsigned int i=0; i<lwpolyline->vertlist.size(); i++){
                pntvec.push_back({lwpolyline->vertlist.at(i)->x,lwpolyline->vertlist.at(i)->y,0});
            } // Close the polyline.
            pntvec.push_back({lwpolyline->vertlist.front()->x,lwpolyline->vertlist.front()->y,0});

            Handle(AIS_Shape) ashape=draw_primitives().draw_3d_line_wire(pntvec);
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);

            // Calculate if lwpolyline (wire) is cw or ccw. If area<0 => ccw. If area>0 => cw.
            double area=0;
            std::vector<gp_Pnt>::iterator it,it1;

            for(it=pntvec.begin(); it<pntvec.end()-1; it++){
                it1=it+1;
                area+=(it1->X()-it->X())*(it1->Y()+it->Y());
            }
            area=area/2; // Area / 2.
            // std::cout<<"lwpolyline area: "<<area<<std::endl; // https://www.youtube.com/watch?v=HG7I4oniOyA

            // If a ccw, invert to cw.
            if(area<0){
                std::reverse(pntvec.begin(),pntvec.end());
            }

            // Check output agian.
            area=0;
            for(it=pntvec.begin(); it<pntvec.end()-1; it++){
                it1=it+1;
                area+=(it1->X()-it->X())*(it1->Y()+it->Y());
            }
            area=area/2; // Area / 2.
            // std::cout<<"lwpolyline cw area : "<<area<<std::endl; // https://www.youtube.com/watch?v=HG7I4oniOyA

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::lwpolyline;

            d.start={pntvec.front().X(),pntvec.front().Y(),0};                      // std::cout<<"lwpolyline points x: "<<d.start.X()<<" y:"<<d.start.Y()<<" no z"<<std::endl;
            for(unsigned int i=1; i<pntvec.size()-1; i++){
                d.control.push_back({pntvec.at(i).X(),pntvec.at(i).Y(),0});         // std::cout<<"lwpolyline points x: "<<pntvec.at(i).X()<<" y:"<<pntvec.at(i).Y()<<" no z"<<std::endl;
            }
            d.end={pntvec.back().X(),pntvec.back().Y(),0};                          // std::cout<<"lwpolyline points x: "<<d.end.X()<<" y:"<<d.end.Y()<<" no z"<<std::endl;
            d.acad_layer=lwpolyline->layer;
            dxfvec.push_back(d);
        }

        // The spline has no direction just as the line.
        if(DRW::SPLINE == (*iter)->eType){ // std::cout<<"spline"<<std::endl;

            DRW_Spline *spline = dynamic_cast<DRW_Spline*>(*iter);
            std::vector<gp_Pnt> pntvec; // input
            for(unsigned int i=0; i<spline->controllist.size(); i++){
                pntvec.push_back({spline->controllist.at(i)->x,spline->controllist.at(i)->y,spline->controllist.at(i)->z});
            }
            Handle(AIS_Shape) ashape=draw_primitives().draw_3d_spline(pntvec,5);
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);
            OpencascadeWidget->show_shape(ashape);
            //std::cout<<"spline fitlist.size:"<<spline->fitlist.size()<<std::endl;
            //std::cout<<"spline controllist.size:"<<spline->controllist.size()<<std::endl;

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::spline;
            d.start={spline->controllist.front()->x,spline->controllist.front()->y,spline->controllist.front()->z};
            for(unsigned int i=1; i<spline->controllist.size()-1; i++){
                d.control.push_back({spline->controllist.at(i)->x,spline->controllist.at(i)->y,spline->controllist.at(i)->z});
            }
            d.end={spline->controllist.back()->x,spline->controllist.back()->y,spline->controllist.back()->z};
            d.acad_layer=spline->layer;
            dxfvec.push_back(d);
        }

        // Arc's are saved as clockwise [cw] by draw_primivite->get points funtion !
        if(DRW::ARC == (*iter)->eType){ // std::cout<<"arc"<<std::endl;

            DRW_Arc *arc = dynamic_cast<DRW_Arc*>(*iter);
            Handle(AIS_Shape) ashape=draw_primitives().draw_2d_acad_arc({arc->center().x,arc->center().y,arc->center().z}, arc->radius(),
                                                                        arc->startAngle(),arc->endAngle());
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::arc;
            d.radius=arc->radious;
            d.center={arc->center().x,arc->center().y,arc->center().z};

            // Request a arc midpoint for the cavalier bulge function when we need 2 arc's to form a circle.
            int division=3;
            d.arcmid=draw_primitives().get_arc_circumfence_points({arc->center().x,arc->center().y,arc->center().z}, arc->radius(),
                                                                  arc->startAngle(),arc->endAngle(),division);
            // std::cout<<"arcmid.size, must be 1 controlpoint: "<<d.arcmid.size()<<std::endl;
            // std::cout<<"arcmid controlpoint x: "<<d.arcmid.at(1).X()<<" y:"<<d.arcmid.at(1).Y()<<" z:"<<d.arcmid.at(1).Z()<<std::endl;

            // Request a few arc circumfence points.
            division=8;
            d.control=draw_primitives().get_arc_circumfence_points({arc->center().x,arc->center().y,arc->center().z}, arc->radius(),
                                                                   arc->startAngle(),arc->endAngle(),division);

            d.start=d.control.front();  // std::cout<<"arc starpoint x:"<<d.start.X()<<" y:"<<d.start.Y()<<" z:"<<d.start.Z()<<std::endl;
            d.end=d.control.back();     // std::cout<<"arc endpoint x:"<<d.end.X()<<" y:"<<d.end.Y()<<" z:"<<d.end.Z()<<std::endl;

            // std::cout<<"arc startpoint x: "<<d.start.X()<<" y:"<<d.start.Y()<<" z:"<<d.start.Z()<<std::endl;
            // std::cout<<"arc endpoint x: "<<d.end.X()<<" y:"<<d.end.Y()<<" z:"<<d.end.Z()<<std::endl;
            d.control.pop_back();
            d.control.erase(d.control.begin());

            for(unsigned int i=0; i<d.control.size(); i++){
                //std::cout<<"arc controlpoints x:"<<d.control.at(i).X()<<" y:"<<d.control.at(i).Y()<<" z:"<<d.control.at(i).Z()<<std::endl;
            }
            d.acad_layer=arc->layer;
            dxfvec.push_back(d);
        }

        // Circles are saved clockwise [cw], startpoint is right side of circle.
        if(DRW::CIRCLE == (*iter)->eType){  // std::cout<<"circle"<<std::endl;

            DRW_Circle *circle = dynamic_cast<DRW_Circle*>(*iter);
            Handle(AIS_Shape) ashape=draw_primitives().draw_2d_circle({circle->basePoint.x,circle->basePoint.y,circle->basePoint.z},circle->radious);
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::circle;
            d.radius=circle->radious;
            d.center={circle->basePoint.x,circle->basePoint.y,circle->basePoint.z};

            // For contour recognize we need a few circle circumfence points to perform the pip (point in polygon) algoritme.
            // Clockwise output, division (segmentation value) :
            int division=8;
            d.control=draw_primitives().get_cirlce_circumfence_points({circle->basePoint.x,circle->basePoint.y,circle->basePoint.z},circle->radious,division);

            d.start=d.control.front();
            d.end=d.control.back();

            d.control.pop_back();
            d.control.erase(d.control.begin());
            d.acad_layer=circle->layer;
            dxfvec.push_back(d);
        }

        // Ellipses are saved clockwise [cw], startpoint is secpoint.
        if(DRW::ELLIPSE == (*iter)->eType){ // std::cout<<"ellipse"<<std::endl;

            DRW_Ellipse *ellipse = dynamic_cast<DRW_Ellipse*>(*iter);

            // To be reviewed when we have more time:
            Handle(AIS_Shape) ashape=draw_primitives().draw_2d_ellipse({ellipse->basePoint.x,ellipse->basePoint.y,ellipse->basePoint.z},
                                                                       {ellipse->secPoint.x,ellipse->secPoint.y,ellipse->secPoint.z}, // offset from base
                                                                       ellipse->staparam,
                                                                       ellipse->endparam,
                                                                       ellipse->ratio);
            ashape=draw_primitives().colorize(ashape,Quantity_NOC_BLACK,1);

            // std::cout<<"ellipse secpoint x:"<<ellipse->secPoint.x<<" y:"<<ellipse->secPoint.y<<" z:"<<ellipse->secPoint.z<<std::endl;
            // std::cout<<"ellipse extpoint x:"<<ellipse->extPoint.x<<" y:"<<ellipse->extPoint.y<<" z:"<<ellipse->extPoint.z<<std::endl;

            datas d;
            d.ashape=ashape;
            d.primitivetype=primitive_type::ellipse;

            // Clockwise output, division (segmentation value) :
            int division=8;
            d.control=draw_primitives().get_ellipse_circumfence_points({ellipse->basePoint.x,ellipse->basePoint.y,ellipse->basePoint.z},
                                                                       {ellipse->secPoint.x,ellipse->secPoint.y,ellipse->secPoint.z}, // offset from base
                                                                       ellipse->staparam,
                                                                       ellipse->endparam,
                                                                       ellipse->ratio,division);

            d.start=d.control.front();
            d.end=d.control.back();

            d.control.pop_back();
            d.control.erase(d.control.begin());
            d.acad_layer=ellipse->layer;
            dxfvec.push_back(d);

            // Acad's ellipse nr's, https://github.com/grotius-cnc/cadcam/blob/master/dxf/read_ellipse_AC1027.cpp
            // x,y,z centerpoint    10,20,30
            // x,y,z endpoint mayor 11,21,31 ( coordinates relative to ellipse centerpoint..)
            // ratio                40
            // start angle          41
            // end angle            42
        }
    }


    std::vector<std::string>layervec;
    for(unsigned int i=0; i<dxfvec.size(); i++){
        // Preview primairy dxf data.
        OpencascadeWidget->show_shape(dxfvec.at(i).ashape);
        // List cad_layers.
        std::string layer=dxfvec.at(i).acad_layer;
        // No duplicates
        if(std::find(layervec.begin(), layervec.end(),layer)!=layervec.end()){
            // Do nothing
        } else {
            layervec.push_back(layer);
        }
    }

    // Populate the layer combobox.
    for(unsigned int i=0; i<layervec.size(); i++){
        ui->comboBox_layer->addItem(QString::fromStdString(layervec.at(i)));
    }
}

//! This function is a buttonpress slot.
void MainWindow::process(){

    contourvec.clear();
    pocketvec.clear();
    datavec.clear();

    // Check what user has selected. Did he select the combobox [offset] or [pocket] item?
    if(gc.operation_type=="contour"){
        generate_contours();
    }
    if(gc.operation_type=="pocket"){
        generate_pockets();
    }
}

//! Class to perform pockets. With or without islands.
void MainWindow::generate_pockets(){
    // Finding contourpoints hit tollerance in mm.
    // The function also calculates the "depth sequence" and the "keep parts together" algoritme.
    contours().main(0.1, gc.layer);
    // Stay with cw contours. We need cw contour for islands.
    offsets().do_pocket();

    for(unsigned int i=0; i<dxfvec.size(); i++){
        OpencascadeWidget->show_shape(dxfvec.at(i).ashape);                                     // Dxf drawing preview.
    }

    for(unsigned int i=0; i<pocketvec.size(); i++){
        for(unsigned int j=0; j<pocketvec.at(i).offset_sequence.size(); j++){                  // Offset contour data.
            OpencascadeWidget->show_shape(pocketvec.at(i).offset_sequence.at(j).ashape);
        }
    }

    OpencascadeWidget->show_3d_interactive_box();
}

//! Class to perform offsets.
void MainWindow::generate_contours(){

    // Finding contourpoints hit tollerance in mm.
    contours().main(0.1, gc.layer);
    // Morph cw into ccw contours for the odd depth's.
    contours().add_contour_ccw();

    // Generate offsets.
    offsets().do_offset(gc.offset,offset_action::offset_contour,gc.lead_in,gc.lead_out);
    offsets().do_offset(gc.offset,offset_action::lead_base_contour,gc.lead_in,gc.lead_out);
    offsets().do_offset(gc.offset,offset_action::lead_in_contour,gc.lead_in,gc.lead_out);
    offsets().do_offset(gc.offset,offset_action::lead_out_contour,gc.lead_in,gc.lead_out);

    for(unsigned int i=0; i<dxfvec.size(); i++){
        OpencascadeWidget->show_shape(dxfvec.at(i).ashape);                                     // Dxf drawing preview.
    }

    for(unsigned int i=0; i<contourvec.size(); i++){
        for(unsigned int j=0; j<contourvec.at(i).offset_sequence.size(); j++){                  // Offset contour data.
            OpencascadeWidget->show_shape(contourvec.at(i).offset_sequence.at(j).ashape);
        }
    }

    for(unsigned int i=0; i<contourvec.size(); i++){
        OpencascadeWidget->show_shape(contourvec.at(i).lead_in.ashape);                         // Show lead-in, lead-out shapes.
        OpencascadeWidget->show_shape(contourvec.at(i).lead_out.ashape);
    }

    OpencascadeWidget->show_3d_interactive_box();
}

//! Move the lead_in-out to next position of the contourvec.at[i]. We simply rotote the primitive sequence vector +1.
//! Then we perform a new contour offset and reload the opencascade view. The gcode output .ngc file is updated.
void MainWindow::rotate_lead_in_out(){

    rapidvec.clear();

    OpencascadeWidget->erase_all();
    unsigned int i=OpencascadeWidget->selected_contour;

    for(unsigned int i=0; i<contourvec.size(); i++){                                            // Remove all offsets.
        contourvec.at(i).offset_sequence.clear();
        contourvec.at(i).lead_base.points.clear();
        contourvec.at(i).lead_in.points.clear();
        contourvec.at(i).lead_out.points.clear();
    }

    gcode_get_user_settings();
    double offset=gc.offset;                                                                    // Gc = gcode setup.
    double lead_in=gc.lead_in;
    double lead_out=gc.lead_out;

    offsets().rotate_primairy_contour(i);                                                       // Rotate selected contour +1.
    offsets().do_offset(offset,offset_action::offset_contour,lead_in,lead_out);                 // Create new offsets.
    offsets().do_offset(offset,offset_action::lead_base_contour,lead_in,lead_out);
    offsets().do_offset(offset,offset_action::lead_in_contour,lead_in,lead_out);
    offsets().do_offset(offset,offset_action::lead_out_contour,lead_in,lead_out);

    linenumber=0; // Reset gcode linenumber.
    gcode().clear();

    gc.operation_type="intro";
    gcode().generate();

    gc.operation_type="pocket";
    gcode().generate();

    gc.operation_type="contour";
    gcode().generate();                                                                         // Generate gcode.

    gc.operation_type="outtro";
    gcode().generate();  // Generate gcode.

    // Update the mainwindow gcode preview:
    gcode_preview();

    for(unsigned int i=0; i<dxfvec.size(); i++){
        OpencascadeWidget->show_shape(dxfvec.at(i).ashape);                                     // Dxf drawing preview.
    }

    for(unsigned int i=0; i<contourvec.size(); i++){
        for(unsigned int j=0; j<contourvec.at(i).offset_sequence.size(); j++){                  // Offset contour data.
            OpencascadeWidget->show_shape(contourvec.at(i).offset_sequence.at(j).ashape);
        }
    }

    for(unsigned int i=0; i<contourvec.size(); i++){
        OpencascadeWidget->show_shape(contourvec.at(i).lead_in.ashape);                         // Show lead-in, lead-out shapes.
        OpencascadeWidget->show_shape(contourvec.at(i).lead_out.ashape);
    }

    for(unsigned int i=0; i<pocketvec.size(); i++){
        for(unsigned int j=0; j<pocketvec.at(i).offset_sequence.size(); j++){                  // Offset contour data.
            OpencascadeWidget->show_shape(pocketvec.at(i).offset_sequence.at(j).ashape);
        }
    }

    for(unsigned int i=0; i<rapidvec.size(); i++){
        OpencascadeWidget->show_shape(rapidvec.at(i).ashape);                                   // Show gcode rapids.
    }

    OpencascadeWidget->show_3d_interactive_box();
}

void MainWindow::gcode_get_user_settings(){

    gc.linenumber_format=ui->lineEdit_gcode_linenumber_format->text().toStdString();
    gc.print_linenumbers=ui->radioButton_use_line_numbers->isChecked();
    gc.feedrate=ui->lineEdit_gcode_feedrate->text().toDouble();
    gc.power=ui->lineEdit_gcode_power->text().toDouble();
    gc.lead_in=ui->lineEdit_lead_in->text().toDouble();
    gc.lead_out=ui->lineEdit_lead_out->text().toDouble();
    gc.travelheight=ui->lineEdit_travel_height->text().toDouble();
    gc.pierceheight=ui->lineEdit_plunge_height->text().toDouble();
    gc.piercespeed=ui->lineEdit_plunge_speed->text().toDouble();
    gc.piercedelay=ui->lineEdit_plunge_delay->text().toDouble();
    gc.offset=ui->lineEdit_offset->text().toDouble();
    gc.layer=ui->comboBox_layer->currentText().toStdString();
    gc.internal_pocket_offset=ui->lineEdit_pocket_internal_offset->text().toDouble();
    gc.z_axis_format=ui->lineEdit_z_axis_format->text().toStdString();
    gc.tooloffset_x=ui->lineEdit_tooloffset_x->text().toDouble();
    gc.tooloffset_y=ui->lineEdit_tooloffset_y->text().toDouble();
    gc.tooloffset_z=ui->lineEdit_tooloffset_z->text().toDouble();
    gc.tool_on_macro=ui->lineEdit_tool_on_macro->text().toStdString();

    // Intro
    QString plainTextEditContents = ui->plainTextEdit_intro->toPlainText();
    QStringList lines = plainTextEditContents.split("\n");
    gc.intro.clear();
    for(int i=0; i<lines.size(); i++){
        std::string str=lines.at(i).toStdString();
        str+="\n";
        gc.intro.push_back(str);
    }
    // Outtro
    gc.outtro.clear();
    plainTextEditContents = ui->plainTextEdit_outtro->toPlainText();
    lines = plainTextEditContents.split("\n");
    for(int i=0; i<lines.size(); i++){
        std::string str=lines.at(i).toStdString();
        str+="\n";
        gc.outtro.push_back(str);
    }

    // Operation type, contour, pocket, drill.
    gc.operation_type=ui->comboBox_operation_type->currentText().toStdString();
}

void MainWindow::gcode_preview()
{
    std::string line,lines;
    std::ifstream myfile ("gcode.ngc");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            lines+=line;
            lines+="\n";
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";

    ui->plainTextEdit_gcode->setPlainText(QString::fromStdString(lines));
}

void MainWindow::on_toolButton_open_dxf_pressed(){
    if (!pfd::settings::available()){
        std::cout << "Portable File Dialogs are not available on this platform. \n"
                     "On linux install zenity, $ sudo apt-get install zenity\n";
    }
    auto f = pfd::open_file("Choose files to read", DEFAULT_PATH,
                            { "Dxf Files (.dxf)", "*.dxf",
                              "All Files", "*" }, pfd::opt::none); // Or ::multiselect.
    open_dxf_file(f.result().at(0)); // This lib can open multiple results.
}

//void MainWindow::on_toolButton_save_dxf_pressed()
//{
//    std::string filename=ui->lineEdit_dxf_filename->text().toStdString();
//    save_dxf_file(filename);
//}

//void MainWindow::on_toolButton_add_line_pressed()
//{
//    write_entity();
//}

//void MainWindow::on_toolButton_stacket_page_plus_pressed()
//{
//    if(ui->stackedWidget->currentIndex() != ui->stackedWidget->count()-1){
//        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
//    } else {

//        ui->stackedWidget->setCurrentIndex(0);
//    }
//    // std::cout<<"curindex:"<<ui->stackedWidget->currentIndex()<<std::endl;
//}












































