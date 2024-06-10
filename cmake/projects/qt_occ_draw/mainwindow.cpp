#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    occ = new OcctQtViewer();

    //! Add gridlayout on top of the occ widget.
    QGridLayout *layout=new QGridLayout(occ);

    //! Add the controls into the occ gridlayout..
    layout->addWidget(form);

    ui->gridLayout_occ->addWidget(occ);

    // Pointcloud.

    /*
    for(double x=0; x<100; x+=10){
        for(double y=0; y<100; y+=10){
            for(double z=0; z<100; z+=10){
            Handle(AIS_Shape) shape=draw->draw_3d_point({x,y,z});
            shape=draw->colorize(shape,Quantity_NOC_GRAY,0);
            occ->add_shapevec(shape);
            }
        }
    }*/

    occ->create_snap_sphere(snap_size);

    occ->set_view_top();
    occ->set_orthographic();

    //! This activates a screen update when robot is moving and screen needs to be updated automaticly.
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(50);

    connect(form, &Form::toolButton_zoom_plus_pressed, this, &MainWindow::zoom_plus);
    connect(form, &Form::toolButton_zoom_min_pressed, this, &MainWindow::zoom_min);
    connect(form, &Form::toolButton_fit_all_pressed, this, &MainWindow::zoom_all);
    connect(form, &Form::toolButton_view_front_pressed, this, &MainWindow::view_front);
    connect(form, &Form::toolButton_view_back_pressed, this, &MainWindow::view_back);
    connect(form, &Form::toolButton_view_top_pressed, this, &MainWindow::view_top);
    connect(form, &Form::toolButton_view_bottom_pressed, this, &MainWindow::view_bottom);
    connect(form, &Form::toolButton_view_left_pressed, this, &MainWindow::view_left);
    connect(form, &Form::toolButton_view_right_pressed, this, &MainWindow::view_right);
    connect(form, &Form::toolButton_view_3d_pressed, this, &MainWindow::view_3d);
    connect(form, &Form::toolButton_draw_3d_2p_line_pressed, this, &MainWindow::draw_3d_2p_line);
    connect(form, &Form::toolButton_color_pressed, this, &MainWindow::set_color);
}

enum MODE {
    none,
    aborting,
    line,
    arc_3p,
    plane_origin
} mode;

void MainWindow::update(){

    //sketcher->planevec=occ->viewdir;
    //sketcher->planeorigin.SetXYZ({0,0,0});
    // occ->mousepos=sketcher->to_plane(occ->mousepos);

    gp_Trsf trsf1,trsf2;

    switch (mode) {
    case none:
        form->set_message("Click or write a command.");
        occ->hide_snap_sphere();
        break;
    case aborting:
        form->set_message("Aborting command.");
        left_mouse_click_counter=0;
        occ->Context()->Remove(sketcher->rubberline,true);
        mode=none;
        break;
    case line:

        switch (left_mouse_click_counter) {
        case 0:
            form->set_message("Click startpoint.");

            keyboard_value=0;

            if(sketcher->snap){
                if(draw->snap_to_closest_endpoint(occ->shapevec,occ->mousepos,snap_size,p)){
                    occ->show_snap_sphere(p);
                }
            }
            if(!sketcher->snap){
                occ->hide_snap_sphere();
            }
            break;
        case 1:
            if(sketcher->snap){
                sketcher->rubberline=sketcher->sketch_3d_line_set_startpoint_rubberline(p);
            } else {
                sketcher->rubberline=sketcher->sketch_3d_line_set_startpoint_rubberline(occ->mousepos);
            }

            occ->Context()->Display(sketcher->rubberline,AIS_WireFrame,-1,true,AIS_DS_None); // non selectable rubberline.
            left_mouse_click_counter++;
            break;
        case 2:
            form->set_message("Click endpoint.");

            if(sketcher->snap){
                if(draw->snap_to_closest_endpoint(occ->shapevec,occ->mousepos,snap_size,p)){
                    occ->show_snap_sphere(p);
                } else {
                    occ->hide_snap_sphere();
                }
            }

            if(sketcher->ortho && !sketcher->snap){
                occ->mousepos=sketcher->to_ortho(sketcher->myFirstPoint->Pnt(),occ->mousepos);
            }
            if(sketcher->ortho && sketcher->snap){
                occ->mousepos=sketcher->to_ortho(sketcher->myFirstPoint->Pnt(),p);
            }

            //! Keyboard lenght input.
            if(keyboard_value!=0 && sketcher->snap ){
                draw_primitives().offset_3d_point_on_line(sketcher->myFirstPoint->Pnt(),occ->mousepos,keyboard_value,p);
                left_mouse_click_counter++;
            }
            if(keyboard_value!=0 && !sketcher->snap ){
                draw_primitives().offset_3d_point_on_line(sketcher->myFirstPoint->Pnt(),occ->mousepos,keyboard_value,occ->mousepos);
                left_mouse_click_counter++;
            }

            sketcher->rubberline=sketcher->sketch_3d_line_show_endpoint_rubberline(occ->mousepos);
            occ->Context()->Redisplay(sketcher->rubberline, true);

            break;
        case 3:
            if(sketcher->snap){
                occ->add_shapevec(
                            draw->colorize(
                                draw->draw_3d_line( sketcher->myFirstPoint->Pnt(), p), sketcher->color, 0 )
                            );

            } else {
                occ->add_shapevec(
                            draw->colorize(
                                draw->draw_3d_line( sketcher->myFirstPoint->Pnt(), occ->mousepos), sketcher->color, 0 )
                            );
            }

            occ->Context()->Remove(sketcher->rubberline,true);
            //! Linecolor...
            left_mouse_click_counter=0;
            break;
        default:

            break;
        }

    case arc_3p:
        switch (left_mouse_click_counter) {
        case 0:
            form->set_message("Click arc start point.");

            break;
        case 1:
            sketcher->rubberline=sketcher->sketch_3d_line_set_startpoint_rubberline(occ->mousepos);
            occ->Context()->Display(sketcher->rubberline,AIS_WireFrame,-1,true,AIS_DS_None); // non selectable rubberline.
            left_mouse_click_counter++;
            break;
        case 2:
            form->set_message("Click arc way point.");
            sketcher->rubberline=sketcher->sketch_3d_line_show_endpoint_rubberline(occ->mousepos);
            occ->Context()->Redisplay(sketcher->rubberline, true);
            break;
        case 3:
            occ->Context()->Remove(sketcher->rubberline,true);

            //! Create 3p_arc.
            sketcher->rubberarc=draw_primitives().colorize(draw_primitives().draw_3p_3d_arc(sketcher->myFirstPoint->Pnt(),
                                                                                            sketcher->mySecondPoint->Pnt(),
                                                                                            {sketcher->mySecondPoint->X()+10,sketcher->mySecondPoint->Y(),sketcher->mySecondPoint->Z()}),
                                                           sketcher->color,0);
            occ->Context()->Display(sketcher->rubberarc,AIS_WireFrame,-1,true,AIS_DS_None); // non selectable rubberline.
            left_mouse_click_counter++;
            break;
        case 4:
            form->set_message("Click arc end point.");

            occ->Context()->Remove(sketcher->rubberarc,true);
            sketcher->rubberarc.Nullify();
            sketcher->rubberarc=draw_primitives().colorize(draw_primitives().draw_3p_3d_arc(sketcher->myFirstPoint->Pnt(),sketcher->mySecondPoint->Pnt(),occ->mousepos),sketcher->color,0);
            occ->Context()->Display(sketcher->rubberarc,AIS_WireFrame,-1,true,AIS_DS_None); // non selectable rubberline.
            break;
        case 5:

            occ->add_shapevec(draw_primitives().colorize(draw_primitives().draw_3p_3d_arc(sketcher->myFirstPoint->Pnt(),sketcher->mySecondPoint->Pnt(),occ->mousepos),
                                                         sketcher->color,0));
            occ->Context()->Remove(sketcher->rubberarc,true);
            sketcher->rubberarc.Nullify();
            left_mouse_click_counter=0;
            mode=none;
            break;
        default:

            break;
        }
        break;

    case plane_origin:
        switch (left_mouse_click_counter) {
        case 0:
            form->set_message("Click new plane origin point.");

            if(sketcher->snap){
                if(draw->snap_to_closest_endpoint(occ->shapevec,occ->mousepos,snap_size,p)){
                    occ->show_snap_sphere(p);
                }
            }
            if(!sketcher->snap){
                occ->hide_snap_sphere();
            }

            break;
        case 1:
            if(sketcher->snap){
                occ->plane.SetLocation(p);

            } else {
                occ->plane.SetLocation(occ->mousepos);

            }

            trsf1.SetTranslation({0,0,0},occ->plane.Location());
            trsf2=trsf1*occ->plane_trsf;
            occ->plane_shape->SetLocalTransformation(trsf2);


            mode=none;
            break;

        default:
            break;
        }
        break;

    default:

        break;
    }

    //! Update mouse position.
    QString x=QString::number(occ->mousepos.X(),'f',3);
    QString y=QString::number(occ->mousepos.Y(),'f',3);
    QString z=QString::number(occ->mousepos.Z(),'f',3);
    QString plane_x=QString::number(occ->plane.Location().X(),'f',3);
    QString plane_y=QString::number(occ->plane.Location().Y(),'f',3);
    QString plane_z=QString::number(occ->plane.Location().Z(),'f',3);
    QString plane_dir_x=QString::number(occ->plane.Position().Direction().X(),'f',3);
    QString plane_dir_y=QString::number(occ->plane.Position().Direction().Y(),'f',3);
    QString plane_dir_z=QString::number(occ->plane.Position().Direction().Z(),'f',3);

    form->set_positions("mouse x:"+x+" y:"+y+" z:"+z+" plane location x:"+plane_x+" y:"+plane_y+" z:"+plane_z+" plane dir x:"+plane_dir_x+" y:"+plane_dir_y+" z:"+plane_dir_z);

    //! To update tp moves.
    occ->redraw();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    std::cout<<"keyevent key:"<<event->key()<<std::endl;


    if(mode==line && left_mouse_click_counter==2){
        //! Collect keyboard input until enter.

        switch (event->key()) {

        case 48:
            keyboard_text.push_back("0");
            break;
        case 49:
            keyboard_text.push_back("1");
            break;
        case 50:
            keyboard_text.push_back("2");
            break;
        case 51:
            keyboard_text.push_back("3");
            break;
        case 52:
            keyboard_text.push_back("4");
            break;
        case 53:
            keyboard_text.push_back("5");
            break;
        case 54:
            keyboard_text.push_back("6");
            break;
        case 66:
            keyboard_text.push_back("7");
            break;
        case 56:
            keyboard_text.push_back("8");
            break;
        case 57:
            keyboard_text.push_back("9");
            break;
        case 16777219: // Backspace.
            if(!keyboard_text.isEmpty()){
                keyboard_text=keyboard_text.chopped(1);
            }
            break;
        case 16777220: // Enter.
            keyboard_value=keyboard_text.toDouble();
            break;
        case 16777221: // Enter.
            keyboard_value=keyboard_text.toDouble();
            break;
        default:
            break;

        }
        form->set_keyboard_input_text(keyboard_text);
    }


    if (event->key() == 16777220) { //! Key enter.
        // std::cout<<"enter"<<std::endl;
    }
    if (event->key() == 16777216) { //! Esc enter.
        mode=aborting;
        // std::cout<<"escape"<<std::endl;
    }
    if (event->key() ==  16777223) { //! Del delete.
        occ->remove_selected_shape();
        // occ->remove_all();
        // std::cout<<"delete"<<std::endl;
    }
    if (event->key()==65 && oldkey==16777249) { //! Control+a
        occ->select_all();
        // std::cout<<"control+a"<<std::endl;
    }
    if (event->key()==90 && oldkey==16777249) { //! Control+z
        occ->undo();
        // std::cout<<"control+z"<<std::endl;
    }


    oldkey=event->key();

    occ->redraw();
}

void MainWindow::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton) {
        // Left mouse button pressed
        left_mouse_click_counter++;
    } else if (event->button() == Qt::RightButton) {
        // Right mouse button pressed
        std::cout<<"right mouse pressed"<<std::endl;

        // FormOptions *opt=new FormOptions();
        // Set the position of the dialog to the current mouse cursor position
        // opt->move(event->globalX(), event->globalY());
        // opt->setWindowFlags(Qt::FramelessWindowHint); // Set the frameless window flag
        // opt->show();
    }

    if (event->modifiers() & Qt::ShiftModifier) {
        // Shift key is held down
    }

}

void MainWindow::set_color(){

    // Open the color chooser dialog
    QColor color = QColorDialog::getColor(Qt::white, nullptr, "Choose a color");

    // Check if a color was selected
    if (color.isValid()) {
        sketcher->color=sketcher->QColorToQuantityColor(color);
    }
}

void MainWindow::draw_3d_2p_line(){
    mode=line;
    left_mouse_click_counter=0;
}

void MainWindow::zoom_plus(){
    occ->zoom_plus();
}

void MainWindow::zoom_min(){
    occ->zoom_min();
}

void MainWindow::zoom_all(){
    occ->fit_all();
}

void MainWindow::view_front(){
    occ->set_view_front();
}

void MainWindow::view_back(){
    occ->set_view_back();
}

void MainWindow::view_top(){
    occ->set_view_top();
}

void MainWindow::view_bottom(){
    occ->set_view_bottom();
}

void MainWindow::view_left(){
    occ->set_view_left();
}

void MainWindow::view_right(){
    occ->set_view_right();
}

void MainWindow::view_3d(){
    occ->set_view_3d();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionorthographic_triggered()
{
    occ->set_orthographic();
}

void MainWindow::on_actionperspective_triggered()
{
    occ->set_perspective();
}

void MainWindow::on_actionopen_triggered()
{
    std::vector<Handle(AIS_Shape)> shapevec;

    filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Cad Files (*.dxf *.step)"));


    QFileInfo fileInfo(filename);
    QString extension = fileInfo.suffix();

    if (!extension.isEmpty()) {
        // std::cout << "File extension: " << extension.toStdString() << std::endl;
    } else {
        // std::cout << "No file extension found." << std::endl;
    }

    if(extension.toStdString()=="dxf"){
        shapevec=libdxfrw_functions().open_dxf_file(filename.toStdString());
    }

    if(extension.toStdString()=="step"){
        shapevec=draw_primitives().load_stepfile(filename.toStdString());
    }

    if(extension.toStdString()=="ngc"){
        // Todo. Use a nice interpreter for it.
    }

    for(uint i=0; i<shapevec.size(); i++){
        occ->add_shapevec(shapevec.at(i));
    }
}

void MainWindow::on_actionsave_triggered()
{
    // Assuming 'parent' is a valid pointer to a QWidget or QMainWindow

    QString filePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), tr("Cad Files (*.dxf)"));

    if (!filePath.isEmpty()) {

        std::cout<<"filepath:"<<filePath.toStdString()<<std::endl;
        // The user selected a file, 'filePath' contains the selected file path
        // You can perform actions like saving to this file

        libdxfrw_functions().save_dxf_file(filePath.toStdString(),occ->shapevec);
    }
}

void MainWindow::on_actionset_plane_top_triggered()
{
    // Define the axis for the plane (center and normal direction using predefined direction)
    gp_Pnt centerPoint(occ->plane.Location()); // Current centerpoint.
    gp_Dir xAxisDirection = gp::DZ();
    gp_Ax3 planeAxis(centerPoint, xAxisDirection); // Axis for the plane
    gp_Pln myPlane(planeAxis);
    occ->plane=myPlane;

    //    occ->plane_trsf.SetRotation(gp_Ax1(occ->plane.Location(),gp::DZ()),0),
    //    occ->plane_shape->SetLocalTransformation(occ->plane_trsf);

    // Get the origin and direction vectors from the axis
    gp_Pnt origin = planeAxis.Location();
    gp_Dir xDir = planeAxis.XDirection();
    gp_Dir yDir = planeAxis.YDirection();

    // Calculate points on the plane based on the origin and directions
    gp_Pnt point1 = origin.Translated(xDir.XYZ() * 5).Translated(yDir.XYZ() * 5); // Offset by 5 units in X and Y directions
    gp_Pnt point2 = origin.Translated(xDir.XYZ() * -5).Translated(yDir.XYZ() * 5); // Offset by -5 units in X and 5 units in Y
    gp_Pnt point3 = origin.Translated(xDir.XYZ() * -5).Translated(yDir.XYZ() * -5); // Offset by -5 units in X and Y
    gp_Pnt point4 = origin.Translated(xDir.XYZ() * 5).Translated(yDir.XYZ() * -5); // Offset by 5 units in X and -5 units in Y

    // occ->add_shapevec(draw_primitives().draw_3d_surface(point1,point2,point3,point4));

}

void MainWindow::on_actionset_plane_left_triggered()
{
    // Define the axis for the plane (center and normal direction using predefined direction)
    gp_Pnt centerPoint(occ->plane.Location()); // Current centerpoint.
    gp_Dir xAxisDirection = gp::DX();
    gp_Ax3 planeAxis(centerPoint, xAxisDirection); // Axis for the plane
    gp_Pln myPlane(planeAxis);
    occ->plane=myPlane;

    //    occ->plane_trsf.SetRotation(gp_Ax1(occ->plane.Location(),gp::DY()),-0.5*M_PI),
    //    occ->plane_shape->SetLocalTransformation(occ->plane_trsf);
    // Get the origin and direction vectors from the axis
    gp_Pnt origin = planeAxis.Location();
    gp_Dir xDir = planeAxis.XDirection();
    gp_Dir yDir = planeAxis.YDirection();

    // Calculate points on the plane based on the origin and directions
    gp_Pnt point1 = origin.Translated(xDir.XYZ() * 5).Translated(yDir.XYZ() * 5); // Offset by 5 units in X and Y directions
    gp_Pnt point2 = origin.Translated(xDir.XYZ() * -5).Translated(yDir.XYZ() * 5); // Offset by -5 units in X and 5 units in Y
    gp_Pnt point3 = origin.Translated(xDir.XYZ() * -5).Translated(yDir.XYZ() * -5); // Offset by -5 units in X and Y
    gp_Pnt point4 = origin.Translated(xDir.XYZ() * 5).Translated(yDir.XYZ() * -5); // Offset by 5 units in X and -5 units in Y

    // occ->add_shapevec(draw_primitives().draw_3d_surface(point1,point2,point3,point4));

}

void MainWindow::on_actionset_plane_front_triggered()
{
    // Define the axis for the plane (center and normal direction using predefined direction)
    gp_Pnt centerPoint(occ->plane.Location()); // Current centerpoint.
    gp_Dir xAxisDirection = gp::DY();
    gp_Ax3 planeAxis(centerPoint, xAxisDirection); // Axis for the plane
    gp_Pln myPlane(planeAxis);
    occ->plane=myPlane;

    //occ->plane_trsf.SetRotation(gp_Ax1(occ->plane.Location(),gp::DX()),0.5*M_PI),
    //occ->plane_shape->SetLocalTransformation(occ->plane_trsf);

    // Get the origin and direction vectors from the axis
    gp_Pnt origin = planeAxis.Location();
    gp_Dir xDir = planeAxis.XDirection();
    gp_Dir yDir = planeAxis.YDirection();

    // Calculate points on the plane based on the origin and directions
    gp_Pnt point1 = origin.Translated(xDir.XYZ() * 5).Translated(yDir.XYZ() * 5); // Offset by 5 units in X and Y directions
    gp_Pnt point2 = origin.Translated(xDir.XYZ() * -5).Translated(yDir.XYZ() * 5); // Offset by -5 units in X and 5 units in Y
    gp_Pnt point3 = origin.Translated(xDir.XYZ() * -5).Translated(yDir.XYZ() * -5); // Offset by -5 units in X and Y
    gp_Pnt point4 = origin.Translated(xDir.XYZ() * 5).Translated(yDir.XYZ() * -5); // Offset by 5 units in X and -5 units in Y

    // occ->add_shapevec(draw_primitives().draw_3d_surface(point1,point2,point3,point4));

}

void MainWindow::on_actionreset_plane_triggered()
{
    //    gp_Pnt centerPoint(0,0,0);
    //    gp_Dir xAxisDirection = gp::DZ();
    //    gp_Ax3 planeAxis(centerPoint, xAxisDirection); // Axis for the plane
    //    gp_Pln myPlane(planeAxis);
    //    occ->plane=myPlane;

    //    occ->plane_trsf.SetRotation(gp_Ax1(occ->plane.Location(),gp::DZ()),0),
    //    occ->plane_shape->SetLocalTransformation(occ->plane_trsf);
}

void MainWindow::on_action_ortho_on_triggered()
{
    sketcher->ortho=1;
}

void MainWindow::on_action_ortho_off_triggered()
{
    sketcher->ortho=0;
}

void MainWindow::on_action_snap_on_triggered()
{
    sketcher->snap=1;
}

void MainWindow::on_action_snap_off_triggered()
{
    sketcher->snap=0;
}

void MainWindow::on_action2p_3d_line_triggered()
{
    mode=line;
}

void MainWindow::on_actionEsc_triggered()
{
    mode=aborting;
}

void MainWindow::on_actiontop_triggered()
{
    occ->set_view_top();
}

void MainWindow::on_actionbottom_triggered()
{
    occ->set_view_bottom();
}

void MainWindow::on_actionfront_triggered()
{
    occ->set_view_front();
}

void MainWindow::on_actionback_triggered()
{
    occ->set_view_back();
}

void MainWindow::on_actionleft_triggered()
{
    occ->set_view_left();
}

void MainWindow::on_actionright_triggered()
{
    occ->set_view_right();
}

void MainWindow::on_actionzoom_plus_triggered()
{
    occ->zoom_plus();
}

void MainWindow::on_actionzoom_min_triggered()
{
    occ->zoom_min();
}

void MainWindow::on_actionzoom_all_triggered()
{
    occ->fit_all();
}

void MainWindow::on_actionview_3d_triggered()
{
    occ->set_view_3d();
}

void MainWindow::on_actionclick_plane_origin_triggered()
{
    mode=plane_origin;
    left_mouse_click_counter=0;
}

void MainWindow::on_action3p_3d_arc_triggered()
{
    mode=arc_3p;
    left_mouse_click_counter=0;
}
