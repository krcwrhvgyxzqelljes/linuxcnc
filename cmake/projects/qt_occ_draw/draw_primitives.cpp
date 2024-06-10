#include "draw_primitives.h"
#include "BRepPrimAPI_MakeBox.hxx"
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include "ChFi2d_FilletAPI.hxx"
#include "ChFi2d_FilletAlgo.hxx"
#include <algorithm>
#include <stdio.h>
#include "ElCLib.hxx"
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

draw_primitives::draw_primitives()
{

}

// Draw 3d solids
Handle(AIS_Shape) draw_primitives::draw_3d_cone(gp_Pnt centerpoint, double bottomdiameter, double topdiameter, double height){
    gp_Dir axis=gp::DX();
    gp_Ax2 aplace(centerpoint,axis);
    TopoDS_Shape t_topo_cone = BRepPrimAPI_MakeCone(aplace,bottomdiameter,topdiameter,height).Shape();
    return new AIS_Shape(t_topo_cone);
}

Handle(AIS_Shape) draw_primitives::draw_3d_tcp_cone(gp_Pnt centerpoint, double bottomdiameter, double topdiameter, double height){
    gp_Dir axis=-gp::DX();
    gp_Ax2 aplace(centerpoint,axis);
    TopoDS_Shape t_topo_cone = BRepPrimAPI_MakeCone(aplace,bottomdiameter,topdiameter,height).Shape();
    return new AIS_Shape(t_topo_cone);
}

Handle(AIS_Shape) draw_primitives::draw_3d_cilinder(double radius, double height){
    TopoDS_Shape t_topo_cylinder = BRepPrimAPI_MakeCylinder(radius , height).Shape();
    return new AIS_Shape(t_topo_cylinder);
}

Handle(AIS_Shape) draw_primitives::draw_3d_sphere(double radius, gp_Pnt center){
    TopoDS_Shape t_topo_sphere = BRepPrimAPI_MakeSphere(center,radius).Shape();
    return new AIS_Shape(t_topo_sphere);
}

Handle(AIS_Shape) draw_primitives::draw_3d_box(double dx, double dy, double dz){
    // Create workframe box.
    TopoDS_Shape t_topo_box = BRepPrimAPI_MakeBox(dx,dy,dz).Shape();
    Handle(AIS_Shape) t_ais_box = new AIS_Shape(t_topo_box);
    return t_ais_box;
}

// Draw 2d primitives:
Handle(AIS_Shape) draw_primitives::draw_2d_circle(gp_Pnt center,double radius){
    gp_Dir dir(0,0,1);
    gp_Circ circle(gp_Ax2( center, dir),radius);
    BRepBuilderAPI_MakeEdge makeEdge(circle);
    Handle(AIS_Shape) shape = new AIS_Shape(TopoDS_Edge());
    shape ->Set(makeEdge.Edge());
    return shape;
}

Handle(AIS_Shape) draw_primitives::draw_cp_2d_arc(gp_Pnt center, gp_Pnt point1, gp_Pnt point2){

    double radius=center.Distance(point2);
    gp_Dir dir(0,0,1); // you can change this
    gp_Circ circle(gp_Ax2( center, dir),radius);
    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(circle,point1,point2,0);
    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return new AIS_Shape(aEdge2);
}

Handle(AIS_Shape) draw_primitives::draw_2d_acad_arc(gp_Pnt center, double radius, const Standard_Real alpha1, const Standard_Real alpha2){
    gp_Dir dir(0,0,1); // you can change this
    gp_Circ circle(gp_Ax2( center, dir),radius);
    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(circle,alpha1,alpha2,0);
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return new AIS_Shape(aEdge);
}

Handle(AIS_Shape) draw_primitives::draw_3d_acad_arc(gp_Pnt center, double radius, double alpha1, double alpha2, gp_Dir dir){
    //gp_Dir dir(0,0,1); // you can change this
    gp_Circ circle(gp_Ax2( center, dir),radius);
    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(circle,alpha1,alpha2,0);
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return new AIS_Shape(aEdge);
}

Handle(AIS_Shape) draw_primitives::draw_2d_ellipse(gp_Pnt center, gp_Pnt secpoint, double alpha_start, double alpha_end, double ratio){

    //https://github.com/grotius-cnc/QT_CadCam_rev0/blob/master/display/display.h
    // Acad's ellipse nr's, https://github.com/grotius-cnc/cadcam/blob/master/dxf/read_ellipse_AC1027.cpp
    // x,y,z centerpoint    10,20,30
    // x,y,z endpoint mayor 11,21,31 ( coordinates relative to ellipse centerpoint..)
    // ratio                40
    // start angle          41
    // end angle            42

    //Standard_Real alpha1=alpha_start;
    //Standard_Real alpha2=alpha_end;

    //center point to endpoint mayor axis..
    double MayorRadius = sqrt(pow(secpoint.X()-center.X(),2) + pow(secpoint.Y()-center.Y(),2) + pow(secpoint.Z()-center.Z(),2));
    //ratio minor axis to mayor axis..
    double MinorRadius = ratio*MayorRadius ;

    gp_Dir xDirection(secpoint.X()-center.X(),secpoint.Y()-center.Y(),secpoint.Z()-center.Z()); // Direction is auto normalized by occ.
    gp_Dir normalDirection(0,0,1);
    gp_Ax2 axis(center,normalDirection,xDirection);

    gp_Elips ellipse(axis,MayorRadius,MinorRadius);
    Handle(Geom_TrimmedCurve) aArcOfEllipse = GC_MakeArcOfEllipse(ellipse,alpha_start,alpha_end,0);
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(aArcOfEllipse);

    return new AIS_Shape(aEdge);
}

// Draw 3d tools:
Handle(AIS_Shape) draw_primitives::draw_3d_origin(gp_Pnt origin, double linelenght){

    double x=origin.X();
    double y=origin.Y();
    double z=origin.Z();

    TopoDS_Shape t_topo_sphere = BRepPrimAPI_MakeSphere(origin,2).Shape();
    Handle(AIS_Shape) aisbody_tcp_sphere = new AIS_Shape(t_topo_sphere);
    aisbody_tcp_sphere->SetColor(Quantity_NOC_YELLOW);
    aisbody_tcp_sphere->SetTransparency(0.75);

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(origin,{x+linelenght,y,z});
    Handle(AIS_Shape) aisbody_tcp_xaxis = new AIS_Shape(edge);
    aisbody_tcp_xaxis->SetColor(Quantity_NOC_RED);

    edge= BRepBuilderAPI_MakeEdge(origin,{x,y+linelenght,z});
    Handle(AIS_Shape) aisbody_tcp_yaxis = new AIS_Shape(edge);
    aisbody_tcp_yaxis->SetColor(Quantity_NOC_GREEN);

    edge= BRepBuilderAPI_MakeEdge(origin,{x,y,z+linelenght});
    Handle(AIS_Shape) aisbody_tcp_zaxis = new AIS_Shape(edge);
    aisbody_tcp_zaxis->SetColor(Quantity_NOC_BLUE);

    aisbody_tcp_sphere->AddChild(aisbody_tcp_xaxis);
    aisbody_tcp_sphere->AddChild(aisbody_tcp_yaxis);
    aisbody_tcp_sphere->AddChild(aisbody_tcp_zaxis);

    return aisbody_tcp_sphere;
}

//! Euler angles in radians.
Handle(AIS_Shape) draw_primitives::rotate_3d(Handle(AIS_Shape) shape, gp_Pnt center, double euler_z, double euler_y, double euler_x){

    // ** For euler angles you have to follow the euler sequence of rotation. First around z, then around y, then around x.
    // Otherwise you will get wrong solutions.

    gp_Trsf trsf1,trsf2,trsf3;
    trsf1.SetRotation(gp_Ax1(center,
                             gp_Dir(0,
                                    0,
                                    1)), euler_z);

    trsf2.SetRotation(gp_Ax1(center,
                             gp_Dir(0,
                                    1,
                                    0)), euler_y);

    trsf3.SetRotation(gp_Ax1(center,
                             gp_Dir(1,
                                    0,
                                    0)), euler_x);

    trsf1.Multiply(trsf2);
    trsf1.Multiply(trsf3);
    shape->SetLocalTransformation(trsf1);
    return shape;
}

Handle(AIS_Shape) draw_primitives::rotate_translate_3d_quaternion(Handle(AIS_Shape) shape, gp_Pnt translation, double euler_z, double euler_y, double euler_x){

    // ** For euler angles you have to follow the euler sequence of rotation. First around z, then around y, then around x.
    // Otherwise you will get wrong solutions.

    gp_Trsf trsf;
    gp_Quaternion aQuat;

    aQuat.SetEulerAngles (gp_YawPitchRoll, euler_z, euler_y, euler_x);
    trsf.SetRotation(aQuat);

    gp_Trsf trsf1;
    trsf1.SetTranslation({0,0,0},translation);

    trsf1.Multiply(trsf);

    shape->SetLocalTransformation(trsf1);
    return shape;
}

Handle(AIS_Shape) draw_primitives::translate_3d(Handle(AIS_Shape) shape, gp_Pnt current, gp_Pnt target){
    gp_Trsf trsf;
    trsf.SetTranslation(current,target);
    shape->SetLocalTransformation(trsf);
    return shape;
}

Handle(AIS_Shape) draw_primitives::colorize(Handle(AIS_Shape) shape, Quantity_Color color, double transparancy){
    shape->SetTransparency(transparancy); // 0.0 - 1.0
    shape->SetColor(color);
    shape->SetMaterial(Graphic3d_NOM_PLASTIC);
    return shape;
}

// Draw 3d primitives:
Handle(AIS_Shape) draw_primitives::draw_3d_point(gp_Pnt point){
    TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(point);
    return new AIS_Shape(vertex);
}

Handle(AIS_Shape) draw_primitives::draw_3d_line(gp_Pnt point1, gp_Pnt point2){

    if(point1.Distance(point2)==0){ //! Will trow error.
        point2.SetX(point2.X()+0.01);
    }
    print_gp_Pnt("p1",point1);
    print_gp_Pnt("p2",point2);
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(point1,point2);
    return new AIS_Shape(edge);
}

Handle(AIS_Shape) draw_primitives::draw_3d_line_wire(std::vector<gp_Pnt> points){

    BRepBuilderAPI_MakeWire wire;
    for(unsigned int i=0; i<points.size()-1; i++){
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(points.at(i),points.at(i+1));
        wire.Add(edge);
    }
    return new AIS_Shape(wire);
}

Handle(AIS_Shape) draw_primitives::draw_3p_3d_arc(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3){

    if(point2.Distance(point3)==0){
        point3.SetX(point3.X()+0.01);
    }

    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(point1,point2,point3);
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return new AIS_Shape(aEdge);
}

Handle(AIS_Shape) draw_primitives::draw_3d_pc_circle(gp_Pnt center, gp_Dir dir, double radius){

    gp_Circ circle(gp_Ax2( center, dir),radius);
    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(circle,0,2*M_PI,0);
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return new AIS_Shape(aEdge);
}

Handle(AIS_Shape) draw_primitives::draw_3d_surface(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4){

    // Create edges using the points
    BRepBuilderAPI_MakeEdge edge1(p1, p2);
    BRepBuilderAPI_MakeEdge edge2(p2, p3);
    BRepBuilderAPI_MakeEdge edge3(p3, p4);
    BRepBuilderAPI_MakeEdge edge4(p4, p1);

    // Create a wire from the edges
    BRepBuilderAPI_MakeWire wire;
    wire.Add(edge1.Edge());
    wire.Add(edge2.Edge());
    wire.Add(edge3.Edge());
    wire.Add(edge4.Edge());

    gp_Pln plane;
    create_3d_plane(p1,p2,p3,plane);

    // Create a face from the wire
    BRepBuilderAPI_MakeFace makeFace(plane, wire.Wire(), Standard_True);

    // Get the resulting face
    TopoDS_Face face = makeFace.Face();

    // Create an AIS_Shape from the resulting face
    return new AIS_Shape(face);
}

Handle(AIS_Shape) draw_primitives::draw_3d_pc_arc_closest(gp_Pnt point1,gp_Pnt point2,gp_Pnt center,gp_Dir dir ,gp_Pnt closest){

    gp_Circ circle(gp_Ax2( center, dir), point1.Distance(center));
    Standard_Real first, last;

    //! Cww or cw
    Handle(Geom_TrimmedCurve) aArcOfCircle1 = GC_MakeArcOfCircle(circle,point1,point2,0);
    TopoDS_Edge aEdge1 = BRepBuilderAPI_MakeEdge(aArcOfCircle1);

    Handle(Geom_Curve) aCurve1 = BRep_Tool::Curve(aEdge1, first, last);
    gp_Pnt pw1= aCurve1->Value((first+last)/2); //! Arc waypoint is more robust for defining a plane.

    //! Other dir.
    Handle(Geom_TrimmedCurve) aArcOfCircle2 = GC_MakeArcOfCircle(circle,point2,point1,0);
    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle2);
    Handle(Geom_Curve) aCurve2 = BRep_Tool::Curve(aEdge2, first, last);
    gp_Pnt pw2= aCurve2->Value((first+last)/2); //! Arc waypoint is more robust for defining a plane.

    double dist1=pw1.Distance(closest);
    double dist2=pw2.Distance(closest);

    if(dist1<dist2){
        return new AIS_Shape(aEdge1);
    }

    return new AIS_Shape(aEdge2);
}

Handle(AIS_Shape) draw_primitives::draw_3d_pc_arc(gp_Pnt point1,gp_Pnt point2,gp_Pnt center, double dir_Xv, double dir_Yv, double dir_Zv){

    double radius=center.Distance(point1);
    // std::cout<<"radius:"<<radius<<std::endl;
    //gp_Dir dir(0,0,1); // you can change this

    //gp_Dir(const Standard_Real Xv, const Standard_Real Yv, const Standard_Real Zv);
    //gp_Vec vec(point1,point2);
    gp_Dir dir(dir_Xv,dir_Yv,dir_Zv);
    gp_Circ circle(gp_Ax2( center, dir),radius);

    Handle(Geom_TrimmedCurve) aArcOfCircle;
    if(point1.Distance(point2)==0){
        aArcOfCircle = GC_MakeArcOfCircle(circle,0,2*M_PI,0);
    } else {
        aArcOfCircle = GC_MakeArcOfCircle(circle,point1,point2,0);
    }

    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    return new AIS_Shape(aEdge2);
}

Handle(AIS_Shape) draw_primitives::draw_3p_3d_circle(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3){
    Handle(Geom_Circle) aCircle = GC_MakeCircle(point1,point2,point3);
    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aCircle);
    return new AIS_Shape(aEdge2);
}

Handle(AIS_Shape) draw_primitives::draw_2d_text(std::string str, int textheight, gp_Pnt point, double rot_deg){ //https://www.youtube.com/watch?v=31SXQLpdNyE

    const char *chartext=str.c_str();

    if(textheight==0 || textheight<0){textheight=1;}

    Font_BRepFont aBrepFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", textheight);
    Font_BRepTextBuilder aTextBuilder;
    TopoDS_Shape a_text_shape = aTextBuilder.Perform(aBrepFont, NCollection_String(chartext));

    Handle(AIS_Shape) ais_shape_text = new AIS_Shape(a_text_shape);

    gp_Trsf MyTrsf_rot;
    MyTrsf_rot.SetRotation(gp_Ax1(gp_Pnt(
                                      0,
                                      0,
                                      0), gp_Dir(
                                      0,                         //rotation flag x
                                      0,                         //rotation flag y
                                      1)), rot_deg * M_PI /180);
    gp_Trsf MyTrsf_trans;
    MyTrsf_trans.SetTranslation(gp_Pnt(0,0,0),point);
    ais_shape_text->SetLocalTransformation(MyTrsf_trans*MyTrsf_rot);
    return ais_shape_text;
}

Handle(AIS_Shape) draw_primitives::draw_3d_point_origin_cone(gp_Pnt point, gp_Pnt euler){

    Handle(AIS_Shape) Ais_shape=draw_3d_point(point);
    Ais_shape=colorize(Ais_shape,Quantity_NOC_BLUE,0);

    // Draw the origin
    Handle(AIS_Shape) Ais_child=draw_3d_origin({0,0,0},25);
    Ais_child=rotate_translate_3d_quaternion(Ais_child,point,
                                             euler.Z(),
                                             euler.Y(),
                                             euler.X());
    Ais_shape->AddChild(Ais_child);

    // Draw the cone
    Ais_child=draw_3d_tcp_cone({0,0,0},0,5,25);
    Ais_child=rotate_translate_3d_quaternion(Ais_child,point,
                                             euler.Z(),
                                             euler.Y(),
                                             euler.X());

    Ais_child=colorize(Ais_child,Quantity_NOC_RED,0.5);
    Ais_shape->AddChild(Ais_child);

    return Ais_shape;
}

Handle(AIS_Shape) draw_primitives::draw_3d_point_origin_cone_text(gp_Pnt point, gp_Pnt euler, std::string text, int textheight, int textrotation){

    Handle(AIS_Shape) Ais_shape=draw_3d_point_origin_cone(point, euler);
    Ais_shape=colorize(Ais_shape,Quantity_NOC_RED,0);
    Handle(AIS_Shape) Ais_text=draw_2d_text(text, textheight, point, textrotation);
    Ais_text=colorize(Ais_text,Quantity_NOC_BLACK,0.5);
    Ais_shape->AddChild(Ais_text);
    return Ais_shape;
}

Handle(AIS_Shape) draw_primitives::draw_3d_line_origin_cone_text(gp_Pnt point1, gp_Pnt point2, gp_Pnt euler1, gp_Pnt euler2, std::string text, int textheight){

    // Draw the line
    Handle(AIS_Shape) Ais_shape=draw_3d_line(point1,point2);
    Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLUE,0);

    // Draw the first origin
    Handle(AIS_Shape) Ais_child=draw_primitives().draw_3d_origin({0,0,0},25);
    Ais_child=rotate_translate_3d_quaternion(Ais_child,point1,
                                             euler1.Z(),
                                             euler1.Y(),
                                             euler1.X());
    Ais_shape->AddChild(Ais_child);

    // Draw the second origin
    Ais_child=draw_primitives().draw_3d_origin({0,0,0},50);
    Ais_child=rotate_translate_3d_quaternion(Ais_child,point2,
                                             euler2.Z(),
                                             euler2.Y(),
                                             euler2.X());
    Ais_shape->AddChild(Ais_child);

    // Draw the first cone
    Ais_child=draw_primitives().draw_3d_tcp_cone({0,0,0},0,5,25);
    Ais_child=draw_primitives().colorize(Ais_child,Quantity_NOC_GREEN,0.5);
    Ais_child=draw_primitives().rotate_translate_3d_quaternion(Ais_child,point1,
                                                               euler1.Z(),
                                                               euler1.Y(),
                                                               euler1.X());
    Ais_shape->AddChild(Ais_child);

    // Draw the second cone
    Ais_child=draw_primitives().draw_3d_tcp_cone({0,0,0},0,5,25);
    Ais_child=draw_primitives().colorize(Ais_child,Quantity_NOC_BLACK,0.5);
    Ais_child=draw_primitives().rotate_translate_3d_quaternion(Ais_child,point2,
                                                               euler2.Z(),
                                                               euler2.Y(),
                                                               euler2.X());
    Ais_shape->AddChild(Ais_child);

    // Draw the text id.
    Handle(AIS_Shape) Ais_text=draw_2d_text(text, textheight, midpoint(point1,point2), 0);
    Ais_text=colorize(Ais_text,Quantity_NOC_BLACK,0.5);
    Ais_shape->AddChild(Ais_text);

    return Ais_shape;
}

Handle(AIS_Shape) draw_primitives::draw_3d_wire_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight){
    BRepBuilderAPI_MakeWire wire;
    for(unsigned int i=0; i<points.size()-1; i++){
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(points.at(i),points.at(i+1));
        wire.Add(edge);
    }
    Handle(AIS_Shape) Ais_shape=new AIS_Shape(wire);
    Ais_shape=colorize(Ais_shape,Quantity_NOC_BLUE,0);

    for(unsigned int i=0; i<points.size(); i++){
        // Draw the origin
        Handle(AIS_Shape) Ais_child=draw_3d_origin({0,0,0},25);
        Ais_child=rotate_translate_3d_quaternion(Ais_child,points.at(i),
                                                 euler.at(i).Z(),
                                                 euler.at(i).Y(),
                                                 euler.at(i).X());
        Ais_shape->AddChild(Ais_child);

        // Draw the cone
        Ais_child=draw_3d_tcp_cone({0,0,0},0,5,25);
        Ais_child=rotate_translate_3d_quaternion(Ais_child,points.at(i),
                                                 euler.at(i).Z(),
                                                 euler.at(i).Y(),
                                                 euler.at(i).X());
        if(i==0){
            Ais_child=colorize(Ais_child,Quantity_NOC_GREEN,0.5);
        } else if(i==points.size()-1){
            Ais_child=colorize(Ais_child,Quantity_NOC_BLACK,0.5);
        } else {
            Ais_child=colorize(Ais_child,Quantity_NOC_BLUE,0.5);
        }

        Ais_shape->AddChild(Ais_child);

    }

    // Draw the text id.
    for(unsigned int i=0; i<points.size()-1; i++){
        std::string subidtext=text+"."+std::to_string(i);
        Handle(AIS_Shape) Ais_text=draw_2d_text(subidtext, textheight, midpoint(points.at(i),points.at(i+1)), 0);
        Ais_text=colorize(Ais_text,Quantity_NOC_BLACK,0.5);
        Ais_shape->AddChild(Ais_text);
    }

    return Ais_shape;
}

Handle(AIS_Shape) draw_primitives::draw_3d_arc_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight){

    Handle(AIS_Shape) Ais_shape=draw_3p_3d_arc(points.at(0),points.at(1),points.at(2));
    Ais_shape=colorize(Ais_shape,Quantity_NOC_BLUE,0);

    for(unsigned int i=0; i<points.size(); i++){
        // Draw the origin
        Handle(AIS_Shape) Ais_child=draw_3d_origin({0,0,0},25);
        Ais_child=rotate_translate_3d_quaternion(Ais_child,points.at(i),
                                                 euler.at(i).Z(),
                                                 euler.at(i).Y(),
                                                 euler.at(i).X());
        Ais_shape->AddChild(Ais_child);

        // Draw the cone
        Ais_child=draw_3d_tcp_cone({0,0,0},0,5,25);
        Ais_child=rotate_translate_3d_quaternion(Ais_child,points.at(i),
                                                 euler.at(i).Z(),
                                                 euler.at(i).Y(),
                                                 euler.at(i).X());
        if(i==0){
            Ais_child=colorize(Ais_child,Quantity_NOC_GREEN,0.5);
        } else if(i==points.size()-1){
            Ais_child=colorize(Ais_child,Quantity_NOC_BLACK,0.5);
        } else {
            Ais_child=colorize(Ais_child,Quantity_NOC_BLUE,0.5);
        }

        Ais_shape->AddChild(Ais_child);
    }




    // Draw the text id.
    Handle(AIS_Shape) Ais_text=draw_2d_text(text, textheight, points.at(1) /*midpoint*/, 0);
    Ais_text=colorize(Ais_text,Quantity_NOC_BLACK,0.5);
    Ais_shape->AddChild(Ais_text);

    //std::vector<Handle(AIS_Shape)> shapevec=draw_primitives::draw_3d_arc_lenght(points.at(0), points.at(1), points.at(2));
    //for(unsigned int i=0; i<shapevec.size(); i++){
    //    Ais_shape->AddChild(shapevec.at(i));
    //}

    return Ais_shape;
}

Handle(AIS_Shape) draw_primitives::draw_3d_circle_origin_cone_text(std::vector<gp_Pnt> points, std::vector<gp_Pnt> euler, std::string text, int textheight){

    Handle(AIS_Shape) Ais_shape=draw_3p_3d_circle(points.at(0),points.at(1),points.at(2));
    Ais_shape=colorize(Ais_shape,Quantity_NOC_BLUE,0);

    for(unsigned int i=0; i<points.size(); i++){
        // Draw the origin
        Handle(AIS_Shape) Ais_child=draw_3d_origin({0,0,0},25);
        Ais_child=rotate_translate_3d_quaternion(Ais_child,points.at(i),
                                                 euler.at(i).Z(),
                                                 euler.at(i).Y(),
                                                 euler.at(i).X());
        Ais_shape->AddChild(Ais_child);

        // Draw the cone
        Ais_child=draw_3d_tcp_cone({0,0,0},0,5,25);
        Ais_child=rotate_translate_3d_quaternion(Ais_child,points.at(i),
                                                 euler.at(i).Z(),
                                                 euler.at(i).Y(),
                                                 euler.at(i).X());
        if(i==0){
            Ais_child=colorize(Ais_child,Quantity_NOC_GREEN,0.5);
        } else if(i==points.size()-1){
            Ais_child=colorize(Ais_child,Quantity_NOC_BLACK,0.5);
        } else {
            Ais_child=colorize(Ais_child,Quantity_NOC_BLUE,0.5);
        }

        Ais_shape->AddChild(Ais_child);
    }

    // Draw the text id.
    Handle(AIS_Shape) Ais_text=draw_2d_text(text, textheight, points.at(1) /*midpoint*/, 0);
    Ais_text=colorize(Ais_text,Quantity_NOC_BLACK,0.5);
    Ais_shape->AddChild(Ais_text);

    return Ais_shape;
}

gp_Pnt draw_primitives::midpoint(gp_Pnt point1, gp_Pnt point2){
    gp_Pnt midpoint;
    midpoint.SetX((point1.X()+point2.X())/2);
    midpoint.SetY((point1.Y()+point2.Y())/2);
    midpoint.SetZ((point1.Z()+point2.Z())/2);
    return midpoint;
}

std::vector<Handle(AIS_Shape)> draw_primitives::draw_3d_arc_lenght(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3){

    std::vector<Handle(AIS_Shape)> shapevec;

    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(point1,point2,point3);

    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);

    GeomAdaptor_Curve acrv(aArcOfCircle);
    TColgp_Array1OfPnt pkte(0, 10);
    GCPnts_UniformAbscissa algo(acrv, 10);
    if(algo.IsDone() && algo.NbPoints() > 0)
    {
        int n = algo.NbPoints();
        std::cout<<"total points:"<<n<<std::endl;
        std::cout<<"abscissa dist:"<<algo.Abscissa()<<std::endl; // Distance between the points. Totalcurvelenght = (points-1)*abscissa dist.

        for(int i=1; i<algo.NbPoints()+1; i++){
            double param = algo.Parameter(i);
            gp_Pnt p = aArcOfCircle->Value(param);
            std::cout<<"Px:"<<p.X()<< " Py:" << p.Y() << " Pz:" << p.Z() <<std::endl;

            Handle(AIS_Shape) Ais_shape=draw_3d_sphere(5,p);
            Ais_shape=colorize(Ais_shape,Quantity_NOC_BLUE,0);
            shapevec.push_back(Ais_shape);
        }

        double curvelenght=0;
        curvelenght=(algo.NbPoints()-1)*algo.Abscissa();
        std::cout<<"curvelenght:"<<curvelenght<<std::endl;
    }
    return shapevec;
}

double draw_primitives::get_3d_arc_lenght(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3, int divisions){

    double curvelenght=0;

    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(point1,point2,point3);

    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);

    GeomAdaptor_Curve acrv(aArcOfCircle);
    TColgp_Array1OfPnt pkte(0, 10);
    int ammount_of_points=divisions+1;
    GCPnts_UniformAbscissa algo(acrv, ammount_of_points);

    if(algo.IsDone() && algo.NbPoints() > 0)
    {
        int n = algo.NbPoints();
        //std::cout<<"total points:"<<n<<std::endl;
        //std::cout<<"abscissa, distance between the points:"<<algo.Abscissa()<<std::endl; // Distance between the points. Totalcurvelenght = (points-1)*abscissa dist.

        //for(int i=1; i<algo.NbPoints()+1; i++){
        //double param = algo.Parameter(i);
        //std::cout<<"algo parameter at i:"<<param<<std::endl;
        //gp_Pnt p = aArcOfCircle->Value(param);
        //std::cout<<"Px:"<<p.X()<< " Py:" << p.Y() << " Pz:" << p.Z() <<std::endl;
        //}
        curvelenght=(algo.NbPoints()-1)*algo.Abscissa();
    }

    //std::cout<<"curvelenght:"<<curvelenght<<std::endl;
    return curvelenght;
}

gp_Pnt draw_primitives::get_3d_arc_point_given_arclenght_fromstartpoint(gp_Pnt point1,gp_Pnt point2,gp_Pnt point3, double arclenght_from_startpoint){

    gp_Pnt p;

    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(point1,point2,point3);

    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);

    GeomAdaptor_Curve acrv(aArcOfCircle);

    GCPnts_AbscissaPoint test(acrv,arclenght_from_startpoint,0); // Tollerance
    if(test.IsDone()){
        double param = test.Parameter();
        p = aArcOfCircle->Value(param);
        std::cout<<"Px:"<<p.X()<< " Py:" << p.Y() << " Pz:" << p.Z() <<std::endl;
    };

    return p;
}

void draw_primitives::print_gp_Pnt(gp_Pnt pnt){
    std::cout<<"pnt x:"<<pnt.X()<<" y:"<<pnt.Y()<<" z:"<<pnt.Z()<<std::endl;
}

void draw_primitives::print_gp_Pnt(std::string text, gp_Pnt pnt){
    std::cout<<text<<" pnt x:"<<pnt.X()<<" y:"<<pnt.Y()<<" z:"<<pnt.Z()<<std::endl;
}

gp_Pnt draw_primitives::rotate_3d_point_around_line(gp_Pnt thePointToRotate, double theta, gp_Pnt theLineP1, gp_Pnt theLineP2)
{
    gp_Pnt q = {0.0,0.0,0.0};
    double costheta,sintheta;
    gp_Pnt r;

    r.SetX(theLineP2.X() - theLineP1.X());
    r.SetY(theLineP2.Y() - theLineP1.Y());
    r.SetZ(theLineP2.Z() - theLineP1.Z());
    thePointToRotate.SetX ( thePointToRotate.X()-theLineP1.X() );
    thePointToRotate.SetY ( thePointToRotate.Y()-theLineP1.Y() );
    thePointToRotate.SetZ ( thePointToRotate.Z()-theLineP1.Z() );

    //! Normalise(&r);
    Eigen::Vector3d v(r.X(),r.Y(),r.Z());
    v.norm();
    r.SetX(v.x());
    r.SetY(v.y());
    r.SetZ(v.z());

    costheta = cos(theta);
    sintheta = sin(theta);

    q.SetX( q.X()+ (costheta + (1 - costheta) * r.X() * r.X()) * thePointToRotate.X());
    q.SetX( q.X()+ ((1 - costheta) * r.X() * r.Y() - r.Z() * sintheta) * thePointToRotate.Y());
    q.SetX( q.X()+ ((1 - costheta) * r.X() * r.Z() + r.Y() * sintheta) * thePointToRotate.Z());

    q.SetY( q.Y()+ ((1 - costheta) * r.X() * r.Y() + r.Z() * sintheta) * thePointToRotate.X());
    q.SetY( q.Y()+ (costheta + (1 - costheta) * r.Y() * r.Y()) * thePointToRotate.Y());
    q.SetY( q.Y()+ ((1 - costheta) * r.Y() * r.Z() - r.X() * sintheta) * thePointToRotate.Z());

    q.SetZ( q.Z()+ ((1 - costheta) * r.X() * r.Z() - r.Y() * sintheta) * thePointToRotate.X());
    q.SetZ( q.Z()+  ((1 - costheta) * r.Y() * r.Z() + r.X() * sintheta) * thePointToRotate.Y());
    q.SetZ( q.Z()+ (costheta + (1 - costheta) * r.Z() * r.Z()) * thePointToRotate.Z());

    q.SetX( q.X()+  theLineP1.X());
    q.SetY( q.Y()+  theLineP1.Y());
    q.SetZ( q.Z()+  theLineP1.Z());

    return(q);
}

int draw_primitives::get_shapetype(Handle(AIS_Shape) shape){

    TopoDS_Shape underlyingShape = shape->Shape();

    if (underlyingShape.ShapeType() == TopAbs_VERTEX) {
        return 1;
    }
    if (underlyingShape.ShapeType() == TopAbs_EDGE) {

        const TopoDS_Edge& edge = TopoDS::Edge(shape->Shape());
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        // gp_Pnt p1= curve->Value(first);
        // gp_Pnt p2= curve->Value(last);

        if (curve->IsKind(STANDARD_TYPE(Geom_Line))) {
            // std::cout << "The shape is a line." << std::endl;
            return 2;

        }

        if (curve->IsKind(STANDARD_TYPE(Geom_Circle))) {
            // std::cout << "The shape is an arc or circle." << std::endl;
            return 3;

        }

    }

    return 0;
}

gp_Pnt draw_primitives::get_point(Handle(AIS_Shape) shape){
    gp_Pnt p1(0,0,0);
    TopExp_Explorer explorer(shape->Shape(), TopAbs_VERTEX);
    if (explorer.More()) {
        explorer.Next();
        if (!explorer.More()) {

            TopoDS_Vertex aVertex = TopoDS::Vertex(explorer.Current());
            p1 = BRep_Tool::Pnt(aVertex);
            // std::cout << "The shape is a point." << std::endl;

        }
    }
    return p1;
}

//! Tested ok.
bool draw_primitives::intersect_3d_sphere_sphere(gp_Pnt pc1, double radius1, gp_Pnt pc2, double radius2, gp_Pnt &pi)
{

    // Create the first sphere
    TopoDS_Shape sphere1 = BRepPrimAPI_MakeSphere(pc1, radius1);

    // Create the second sphere
    TopoDS_Shape sphere2 = BRepPrimAPI_MakeSphere(pc2, radius2);

    // Find the intersection between the two spheres
    BRepAlgoAPI_Section section(sphere1, sphere2);
    section.Build();
    if (section.IsDone()) {
        // Get the intersection result
        TopoDS_Shape intersectionShape = section.Shape();

        // Display the intersection points
        for (TopExp_Explorer explorer(intersectionShape, TopAbs_VERTEX); explorer.More(); explorer.Next()) {
            TopoDS_Vertex vertex = TopoDS::Vertex(explorer.Current());
            pi = BRep_Tool::Pnt(vertex);

        }
    } else {
        // std::cout << "No intersection found between the spheres." << std::endl;
        return 0;
    }
    return 1;
}

void draw_primitives::get_closest_point_to_line(gp_Pnt point, Handle(AIS_Shape) shape, gp_Pnt &intersection){

    gp_Pnt p1,p2;
    get_line_points(shape,p1,p2);
    get_closest_point_to_line(point,p1,p2,intersection);
}

//! This finds a intersection point on a given line, where the given point is in space somewhere.
//! The intersection point is the closest line between space point & given line.
void draw_primitives::get_closest_point_to_line( gp_Pnt point, gp_Pnt line_start, gp_Pnt line_end, gp_Pnt &intersection )
{
    double U;
    double LineMag = line_end.Distance(line_start);

    U = ( ( ( point.X() - line_start.X() ) * ( line_end.X() - line_start.X() ) ) +
          ( ( point.Y() - line_start.Y() ) * ( line_end.Y() - line_start.Y() ) ) +
          ( ( point.Z() - line_start.Z() ) * ( line_end.Z() - line_start.Z() ) ) ) /
            ( LineMag * LineMag );

    //if( U < 0.0f || U > 1.0f ) //! If you need it valid if on the line.
    // return 0;   // closest point does not fall within the line segment

    intersection.SetX( line_start.X() + U * ( line_end.X() - line_start.X() ) );
    intersection.SetY( line_start.Y() + U * ( line_end.Y() - line_start.Y() ) );
    intersection.SetZ( line_start.Z() + U * ( line_end.Z() - line_start.Z() ) );
}

//! Test to 4 quadrant directions:
//! Eigen::Vector3d C;
//! draw_primitives().offset_point_on_line({0,0,0},{100,0,0},25,C);
//! draw_primitives().offset_point_on_line({0,0,0},{0,-100,0},25,C);
//! draw_primitives().offset_point_on_line({0,0,0},{-100,0,0},25,C);
//! draw_primitives().offset_point_on_line({0,0,0},{0,100,0},25,C);
void draw_primitives::offset_3d_point_on_line(gp_Pnt A, gp_Pnt B, double offset_a, gp_Pnt &C){

    //    A-----------B------------C
    // (Xa,Ya)     (Xb,Yb)      (Xc,Yc)

    C.SetX( A.X() + (offset_a * (B.X() - A.X()) / (A.Distance(B)) ) );
    C.SetY( A.Y() + (offset_a * (B.Y() - A.Y()) / (A.Distance(B)) ) );
    C.SetZ( A.Z() + (offset_a * (B.Z() - A.Z()) / (A.Distance(B)) ) );

    if(A.Distance(B)==0){
        C=A;
    }
}

void draw_primitives::offset_3d_point_on_line(Eigen::Vector3d A, Eigen::Vector3d B, double offset_a, Eigen::Vector3d &C){

    //    A-----------B------------C
    // (Xa,Ya)     (Xb,Yb)      (Xc,Yc)

    C.x() = A.x() + (offset_a * (B.x() - A.x()) / (A-B).norm());
    C.y() = A.y() + (offset_a * (B.y() - A.y()) / (A-B).norm());
    C.z() = A.z() + (offset_a * (B.z() - A.z()) / (A-B).norm());


    if((A-B).norm()==0){
        C=A;
    }
}

//! This only returns 0 or 2 solutions.
bool draw_primitives::intersect_3d_line_sphere(gp_Pnt cp, gp_Pnt p0, gp_Pnt p1, double radius, gp_Pnt &pi){

    double cx = cp.X();
    double cy = cp.Y();
    double cz = cp.Z();

    double px = p0.X();
    double py = p0.Y();
    double pz = p0.Z();

    double vx = p1.X() - px;
    double vy = p1.Y() - py;
    double vz = p1.Z() - pz;

    double A = vx * vx + vy * vy + vz * vz;
    double B = 2.0 * (px * vx + py * vy + pz * vz - vx * cx - vy * cy - vz * cz);
    double C = px * px - 2 * px * cx + cx * cx + py * py - 2 * py * cy + cy * cy +
            pz * pz - 2 * pz * cz + cz * cz - radius * radius;

    //! discriminant
    double D = B * B - 4 * A * C;
    // std::cout<<"D:"<<D<<std::endl;

    if(D<0){
        std::cout<<"3d_line_sphere, no intersections found in general."<<std::endl;
        return 0;
    }

    double t1 = (-B - Sqrt(D)) / (2.0 * A);
    // std::cout<<"T1:"<<t1<<std::endl;

    gp_Pnt solution1, solution2;

    solution1.SetX(p0.X() * (1 - t1) + t1 * p1.X());
    solution1.SetY(p0.Y() * (1 - t1) + t1 * p1.Y());
    solution1.SetZ(p0.Z() * (1 - t1) + t1 * p1.Z());

    double t2 = (-B + Sqrt(D)) / (2.0 * A);
    // std::cout<<"T2:"<<t2<<std::endl;

    solution2.SetX(p0.X() * (1 - t2) + t2 * p1.X());
    solution2.SetY(p0.Y() * (1 - t2) + t2 * p1.Y());
    solution2.SetZ(p0.Z() * (1 - t2) + t2 * p1.Z());

    if(is_point_on_line(p0,p1,solution2)){
        pi=solution2;
        return 1;
    }
    if(is_point_on_line(p0,p1,solution1)){
        pi=solution1;
        return 1;
    }

    return 0;
}

bool draw_primitives::intersect_line_arc_sphere(Handle(AIS_Shape) lineShape, Handle(AIS_Shape) arcShape, gp_Pnt sphere_pc, double sphere_radius){

    gp_Pnt p1,p2,pi1;
    get_line_points(lineShape,p1,p2);

    gp_Pnt p3,p4,pw,pc,pi2;
    get_arc_points(arcShape,p3,p4,pw,pc);
    double r=get_arc_radius(arcShape);

    //! Intersect lineShape with sphere.
    if(intersect_3d_line_sphere(sphere_pc,p1,p2,sphere_radius,pi1)){
        // one intersect ok.
    } else {
        return 0;
    }

    //! Intersect arcShape with sphere.
    if(intersect_3d_sphere_sphere(sphere_pc,sphere_radius,pc,r,pi2)){
        // one intersect ok.
    } else {
        return 0;
    }

    //! Are intersection points on the curve?
    if(is_point_on_line(lineShape,pi1)==1 && is_point_on_arc(arcShape,pi2)==1){
        return 1;
    }

    return 0;
}

bool draw_primitives::create_3d_plane(gp_Pnt p_origin, gp_Pnt p2, gp_Pnt p3, gp_Pln &plane){

    // Define vectors using gp_Vec (Open CASCADE vectors)
    gp_Vec v1(p2, p_origin);
    gp_Vec v2(p3, p_origin);

    // Normalize vectors
    v1.Normalize();
    v2.Normalize();

    // Calculate angle between vectors
    double angle = v1.Angle(v2);

    // std::cout<<"plane angle:"<<angle*toDegrees<<std::endl;

    if (angle*toDegrees == 180) { // Checking if angle is pi radians (180 degrees)
        std::cout << "lines are colinear, no fillet." << std::endl;
        return 0;
    }

    // Calculate axis for the arc
    gp_Vec n = v1.Crossed(v2);
    n.Normalize();

    // Create the direction for the arc's plane
    gp_Dir dir(n.X(), n.Y(), n.Z());

    // Create the plane passing through p1 with direction dir
    gp_Pln p(p_origin,dir);
    plane=p;

    return 1;
}

bool draw_primitives::draw_3d_arc_line_fillet(Handle(AIS_Shape) shapeArc, Handle(AIS_Shape) shapeLine, double radius, Handle(AIS_Shape) &shape){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeLine->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    //! Line point start, end.
    gp_Pnt p3= BRep_Tool::Pnt(vp1);
    gp_Pnt p4= BRep_Tool::Pnt(vp2);

    const TopoDS_Edge& edgeB = TopoDS::Edge(shapeArc->Shape());
    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeB, first, last);

    //! Arc start-, endpoint.
    gp_Pnt p1= aCurve->Value(first);
    gp_Pnt p2= aCurve->Value(last);
    gp_Pnt pw= aCurve->Value((first+last)/2); //! Arc waypoint is more robust for defining a plane.

    // Assuming p1, p2, p3 are gp_Pnt objects
    if (pw.IsEqual(p3, 1e-6) || pw.IsEqual(p4, 1e-6)) {
        std::cout << "Input points are not distinct. Aborting." << std::endl;
        // return 0; // Or any other appropriate action for non-distinct points
    }

    gp_Pln plane;
    if(!create_3d_plane(p3,p4,pw,plane)){
        return 0;
    }

    //! Create 2d fillets on a plane.
    ChFi2d_FilletAPI api(edgeA,edgeB,plane);

    if(!api.Perform(radius)){
        std::cout<<"error in perform api arc-line."<<std::endl;
        return 0;
    }

    int results=api.NbResults(p3);
    std::cout<<"results:"<<results<<std::endl;

    TopoDS_Edge theEdge1,theEdge2;
    TopoDS_Edge edge=api.Result(p1,theEdge1,theEdge2,-1);

    shape=new AIS_Shape(edge);
    shape=colorize(shape,Quantity_NOC_GREEN,0);
    return 1;
}

bool draw_primitives::draw_3d_line_arc_fillet(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double radius, Handle(AIS_Shape) &shape){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeLine->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    //! Line point start, end.
    gp_Pnt p1= BRep_Tool::Pnt(vp1);
    gp_Pnt p2= BRep_Tool::Pnt(vp2);

    const TopoDS_Edge& edgeB = TopoDS::Edge(shapeArc->Shape());
    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeB, first, last);

    //! Arc start-, endpoint.
    gp_Pnt p3= aCurve->Value(first);
    gp_Pnt p4= aCurve->Value(last);
    gp_Pnt pw= aCurve->Value((first+last)/2); //! Arc waypoint is more robust for defining a plane.

    // Assuming p1, p2, p3 are gp_Pnt objects
    if (p1.IsEqual(p2, 1e-6) || p3.IsEqual(p4, 1e-6)) {
        std::cout << "Input points are not distinct. Aborting." << std::endl;
        return 0; // Or any other appropriate action for non-distinct points
    }

    gp_Pln plane;
    if(!create_3d_plane(p1,p2,pw,plane)){
        return 0;
    }

    //! Create 2d fillets on a plane.
    ChFi2d_FilletAPI api(edgeA,edgeB,plane);

    if(!api.Perform(radius)){
        std::cout<<"error in perform api line-arc."<<std::endl;
        return 0;
    }

    TopoDS_Edge theEdge1,theEdge2;
    TopoDS_Edge edge=api.Result(p2,theEdge1,theEdge2,-1);

    shape=new AIS_Shape(edge);
    shape=colorize(shape,Quantity_NOC_RED,0);
    return 1;
}


bool draw_primitives::draw_3d_line_line_fillet(Handle(AIS_Shape) shapeLineA, Handle(AIS_Shape) shapeLineB, double radius, Handle(AIS_Shape) &shape){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeLineA->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    gp_Pnt p1= BRep_Tool::Pnt(vp1);
    gp_Pnt p2= BRep_Tool::Pnt(vp2);

    const TopoDS_Edge& edgeB = TopoDS::Edge(shapeLineB->Shape());
    TopoDS_Vertex vp3,vp4;
    TopExp::Vertices(edgeB,vp3,vp4);
    // gp_Pnt p3= BRep_Tool::Pnt(vp3);
    gp_Pnt p4= BRep_Tool::Pnt(vp4);

    // Assuming p1, p2, p3 are gp_Pnt objects
    if (p1.IsEqual(p2, 0.0001) || p1.IsEqual(p4, 0.0001) || p2.IsEqual(p4, 0.0001)) {
        std::cout << "Input points are not distinct. Aborting." << std::endl;
        return 0; // Or any other appropriate action for non-distinct points
    }

    gp_Pln plane;
    if(create_3d_plane(p2,p1,p4,plane)==0){
        return 0;
    }

    //! Create 2d fillets on a plane.
    ChFi2d_FilletAPI api(edgeA,edgeB,plane);

    if(!api.Perform(radius)){
        std::cout<<"error in perform api line-line"<<std::endl;
        return 0;
    }

    TopoDS_Edge theEdge1,theEdge2;
    TopoDS_Edge edge=api.Result(p2,theEdge1,theEdge2,-1);

    shape=new AIS_Shape(edge);
    shape=colorize(shape,Quantity_NOC_BLUE,0);

    return 1;
}

bool draw_primitives::draw_3d_line_line_fillet_conventional(Handle(AIS_Shape) shapeA, Handle(AIS_Shape) shapeB, double radius, Handle(AIS_Shape) &shape){

    Handle(AIS_Shape) lineA=shapeA;
    Handle(AIS_Shape) lineB=shapeB;

    // Handle(AIS_Shape) lineA=draw_3d_line({0,0,0},{0,100,0});
    // Handle(AIS_Shape) lineB=draw_3d_line({0,100,0},{100,100,0});

    const TopoDS_Edge& edgeA = TopoDS::Edge(lineA->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    gp_Pnt p1= BRep_Tool::Pnt(vp1);
    gp_Pnt p2= BRep_Tool::Pnt(vp2);

    const TopoDS_Edge& edgeB = TopoDS::Edge(lineB->Shape());
    TopoDS_Vertex vp3,vp4;
    TopExp::Vertices(edgeB,vp3,vp4);
    // gp_Pnt p3= BRep_Tool::Pnt(vp3);
    gp_Pnt p4= BRep_Tool::Pnt(vp4);

    Eigen::Vector3d p1_(p1.X(),p1.Y(),p1.Z());
    Eigen::Vector3d p2_(p2.X(),p2.Y(),p2.Z());
    Eigen::Vector3d p3_(p4.X(),p4.Y(),p4.Z());

    Eigen::Vector3d v1 = (p1_-p2_);
    Eigen::Vector3d v2 = (p3_-p2_);

    Eigen::Vector3d va = v1.normalized();
    Eigen::Vector3d vb = v2.normalized();
    // std::cout<<"va x:"<<va.x()<<" y:"<<va.y()<<" z:"<<va.z()<<std::endl;
    // std::cout<<"vb x:"<<vb.x()<<" y:"<<vb.y()<<" z:"<<vb.z()<<std::endl;

    //! This can be a negative angle if angle > 180 degrrees. Solution is below.
    double angle=acos(va.dot(vb));
    double half_angle=0.5*angle;
    // std::cout<<"arc angle in radians:"<<angle<<std::endl;
    // std::cout<<"arc angle in degrees:"<<angle*toDegrees<<std::endl;

    if(angle*toDegrees==180){ //! Colineair.
        std::cout<<"lines are colineair, no fillet."<<std::endl;
        return 0;
    }

    //! Arc direction, in arc plane between p1,p3 or v1,v2, doesn't really matter.
    Eigen::Vector3d n=v1.cross(v2);
    double nl=n.norm();
    //! Axis to arc's origin.
    Eigen::Vector3d axis=n/sqrt(nl);

    //! Axis to arc's origin.
    Eigen::Vector3d an=axis.normalized();
    // std::cout<<"axis trough line p2 normalized x:"<<an.x()<<" y:"<<an.y()<<" z:"<<an.z()<<std::endl;

    // double radius=3;

    //! Half angle line, fromm arc center point to line-line intersect.
    double lh=radius/sin(half_angle);
    // std::cout<<"lenght half angle line:"<<lh<<std::endl;

    // std::cout<<"first line lenght: "<<v1.norm()<<std::endl;
    // std::cout<<"second lenght: "<<v2.norm()<<std::endl;

    //! Lenght where the arc touched (intersects) the line.
    double li=radius/tan(half_angle);

    //! Get 3d Arc contact point on line p1-p2.
    Eigen::Vector3d C1;
    offset_3d_point_on_line(p1_,p2_,v1.norm()-li,C1);

    // std::cout<<"arc contact point 1 X: "<<C1.x()<<std::endl;
    // std::cout<<"arc contact point 1 Y: "<<C1.y()<<std::endl;

    Eigen::Vector3d C2;
    offset_3d_point_on_line(p2_,p3_,li,C2);

    // std::cout<<"arc contact point 2 X: "<<C2.x()<<std::endl;
    // std::cout<<"arc contact point 2 Y: "<<C2.y()<<std::endl;

    Eigen::Vector3d C3;
    offset_3d_point_on_line(p1_,p2_,v1.norm()-lh,C3);

    // std::cout<<"dist arc center to corner projected on line p1-p2 C3 X: "<<C3.x()<<std::endl;
    // std::cout<<"dist arc center to corner projected on line p1-p2 C3 Y: "<<C3.y()<<std::endl;

    //! Arc center.
    gp_Pnt arc_center=draw_primitives().rotate_3d_point_around_line({C3.x(),C3.y(),C3.z()},half_angle,{p2_.x(),p2_.y(),p2_.z()},{p2_.x()+an.x(),p2_.y()+an.y(),p2_.z()+an.z()});

    // std::cout<<"arc center x: "<<arc_center.X()<<std::endl;
    // std::cout<<"arc center y: "<<arc_center.Y()<<std::endl;

    gp_Dir dir(an.x(),an.y(),an.z());
    gp_Ax1 ax;
    ax.SetDirection(dir);
    ax.SetLocation(arc_center);

    gp_Circ circ;
    circ.SetRadius(radius);
    circ.SetLocation(arc_center);
    circ.SetAxis(ax);

    Handle(Geom_TrimmedCurve) aArcOfCircle;
    aArcOfCircle = GC_MakeArcOfCircle(circ,{C2.x(),C2.y(),C2.z()},{C1.x(),C1.y(),C1.z()},0);

    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    shape=new AIS_Shape(aEdge);

    return 1;
}


// Example one:
/*
Handle(AIS_Shape) shape;
shape = draw_primitives().draw_3d_origin(pointvec.back(),50);
double x,y,z;
cart.M.GetEulerZYX(z,y,x);
shape = draw_primitives().rotate_3d(shape,pointvec.back(),z,y,x);
OpencascadeWidget->show_shape(shape);
*/

/* Example two:
Handle(AIS_Shape) shape;
shape = draw_primitives().draw_3d_origin({0,0,0},50);
double x,y,z;
cart.M.GetEulerZYX(z,y,x);
shape = draw_primitives().rotate_3d_quaternion(shape,pointvec.back(),z,y,x);
OpencascadeWidget->show_shape(shape);
*/

//! This function finds the common intersection point. Opencascade has no logic about arc start, arc endpoints.
//! The result is a intersection set at line_p1 to arc_p0
void draw_primitives::organize_line_arc_points(gp_Pnt &line_p0, gp_Pnt &line_p1, gp_Pnt &arc_p0, gp_Pnt &arc_p1){

    gp_Pnt p0=line_p0;
    gp_Pnt p1=line_p1;
    gp_Pnt a0=arc_p0;
    gp_Pnt a1=arc_p1;

    //! Find if line_p0 with arc_p0 match intersect.
    if(line_p0.Distance(arc_p0)<line_p0.Distance(arc_p1) && line_p0.Distance(arc_p0)<line_p1.Distance(arc_p0) ){
        line_p0=p1;
        line_p1=p0;
        arc_p0=a0;
        arc_p1=a1;
        return;
    }
    //! Find if line_p0 with arc_p1 match intersect.
    if(line_p0.Distance(arc_p1)<line_p0.Distance(arc_p0) && line_p0.Distance(arc_p1)<line_p1.Distance(arc_p1) ){
        line_p0=p1;
        line_p1=p0;
        arc_p0=a1;
        arc_p1=a0;
        return;
    }
    //! Find if line_p1 with arc_p0 match intersect.
    if(line_p1.Distance(arc_p0)<line_p1.Distance(arc_p1) && line_p1.Distance(arc_p0)<line_p0.Distance(arc_p0) ){
        line_p0=p0;
        line_p1=p1;
        arc_p0=a0;
        arc_p1=a1;
        return;
    }
    //! Find if line_p1 with arc_p1 match intersect.
    if(line_p1.Distance(arc_p1)<line_p1.Distance(arc_p0) && line_p1.Distance(arc_p1)<line_p0.Distance(arc_p1) ){
        line_p0=p0;
        line_p1=p1;
        arc_p0=a1;
        arc_p1=a0;
        return;
    }
}

//! This function creates a transformation matrix, where x axis is p0-p1, given a third point on a plane.
gp_Trsf draw_primitives::create_transformation_matrix(gp_Pnt p0, gp_Pnt p1, gp_Pnt point_on_plane){

    // Calculate vectors
    gp_Vec x_axis(p0, p1);
    x_axis.Normalize();
    gp_Vec v2(p0,point_on_plane);

    // Calculate the normal vector using cross product
    gp_Vec z_axis = x_axis.Crossed(v2).Normalized();
    z_axis.Normalize();

    gp_Vec y_axis = z_axis.Crossed(x_axis);
    y_axis.Normalize();

    // Create the transformation matrix using the calculated axes
    gp_Trsf transformation;
    transformation.SetValues(x_axis.X(), y_axis.X(), z_axis.X(), 0,
                             x_axis.Y(), y_axis.Y(), z_axis.Y(), 0,
                             x_axis.Z(), y_axis.Z(), z_axis.Z(), 0);

    return transformation;
}

gp_Dir draw_primitives::create_normal_from_3_points_on_plane(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3){

    // Calculate vectors
    gp_Vec x_axis(p1, p2);
    x_axis.Normalize();
    gp_Vec v2(p1,p3);

    // Calculate the normal vector using cross product
    gp_Vec z_axis = x_axis.Crossed(v2).Normalized();
    z_axis.Normalize();

    gp_Vec y_axis = z_axis.Crossed(x_axis);
    y_axis.Normalize();

    return z_axis;
}


void draw_primitives::offset_line_on_a_plane(gp_Pnt line_p0, gp_Pnt line_p1, gp_Pnt plane_p0, double offset, gp_Pnt &offset_line_p0, gp_Pnt &offset_line_p1){

    gp_Trsf transformation=create_transformation_matrix(line_p0,line_p1,plane_p0);

    // Create the point (0, 5, 0) in the original coordinate system
    gp_Pnt originalPoint(0, offset, 0);

    // Apply the transformation to move the point to the new coordinate system
    gp_Pnt transformedPoint = originalPoint.Transformed(transformation);

    offset_line_p0.SetXYZ({transformedPoint.X()+line_p0.X(),
                           transformedPoint.Y()+line_p0.Y(),
                           transformedPoint.Z()+line_p0.Z()});
    offset_line_p1.SetXYZ({((line_p1.X()-line_p0.X()+transformedPoint.X())+line_p0.X()),
                           ((line_p1.Y()-line_p0.Y())+transformedPoint.Y())+line_p0.Y(),
                           ((line_p1.Z()-line_p0.Z())+transformedPoint.Z())+line_p0.Z()
                          });
}

gp_Pnt draw_primitives::get_arc_centerpoint(Handle(AIS_Shape) shapeArc){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeArc->Shape());
    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeA, first, last);

    Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(aCurve);
    return circle->Location();
}

void draw_primitives::get_arc_points(Handle(AIS_Shape) shapeArc, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &pw ,gp_Pnt &pc){

    const TopoDS_Edge& edgeB = TopoDS::Edge(shapeArc->Shape());
    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeB, first, last);

    //! Arc start-, endpoint.
    p1= aCurve->Value(first);
    p2= aCurve->Value(last);
    pw= aCurve->Value((first+last)/2);

    Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(aCurve);
    pc=circle->Location();
}

double draw_primitives::get_arc_radius(Handle(AIS_Shape) shapeArc){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeArc->Shape());
    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeA, first, last);

    Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(aCurve);
    return circle->Radius();
}

void draw_primitives::get_line_points(Handle(AIS_Shape) shapeLine, gp_Pnt &p1, gp_Pnt &p2){
    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeLine->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    p1= BRep_Tool::Pnt(vp1);
    p2= BRep_Tool::Pnt(vp2);
}

bool draw_primitives::is_point_on_line(Handle(AIS_Shape) shapeLine, gp_Pnt pi){
    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeLine->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    gp_Pnt p1= BRep_Tool::Pnt(vp1);
    gp_Pnt p2= BRep_Tool::Pnt(vp2);

    return is_point_on_line(p1,p2,pi);
}

bool draw_primitives::is_point_on_line(gp_Pnt p1, gp_Pnt p2, gp_Pnt pi){
    double tol=0.001;

    if((pi.Distance(p1)+pi.Distance(p2))>p1.Distance(p2)-tol && (pi.Distance(p1)+pi.Distance(p2))<p1.Distance(p2)+tol ){
        return 1;
    }
    return 0;
}

//! Is pi on plane p1,p2,p3?
bool draw_primitives::is_point_on_plane(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt pi){
    double a1 = p2.X() - p1.X();
    double b1 = p2.Y() - p1.Y();
    double c1 = p2.Z() - p1.Z();
    double a2 = p3.X() - p1.X();
    double b2 = p3.Y() - p1.Y();
    double c2 = p3.Z() - p1.Z();
    double a = b1 * c2 - b2 * c1 ;
    double b = a2 * c1 - a1 * c2 ;
    double c = a1 * b2 - b1 * a2 ;
    double d = (- a * p1.X() - b * p1.Y() - c * p1.Z()) ;

    //! equation of plane is: a*x + b*y + c*z = 0 #

    //! checking if the 4th point satisfies
    //! the above equation
    if(a * pi.X() + b * pi.Y() + c * pi.Z() + d ==0){
        // std::cout<<"Coplanar (on plane)"<<std::endl;
        return 1;
    }
    return 0;
}

bool draw_primitives::is_point_on_arc(Handle(AIS_Shape) shapeArc, gp_Pnt pi){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeArc->Shape());
    double first, last;
    double tol=1e-6;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeA, first, last);
    // gp_Pnt p1= aCurve->Value(first);

    Handle(Geom_Circle) aCircle = Handle(Geom_Circle)::DownCast(aCurve);

    // std::cout<<"first:"<<first<<std::endl;
    // std::cout<<"last:"<<last<<std::endl;

    if(aCurve->IsKind(STANDARD_TYPE (Geom_Circle))){

        // Create a projector to find the parameter on the arc for the test point
        GeomAPI_ProjectPointOnCurve projector(pi, aCurve);

        // Get the parameter value on the curve for the projected point
        double parameter = projector.LowerDistanceParameter();
        // std::cout<<"parameter:"<<parameter<<std::endl;

        // Get the point on the curve corresponding to the calculated parameter
        // gp_Pnt projectedPoint = aCurve->Value(parameter);
        // print_gp_Pnt("projected point:",projectedPoint);

        //! Check radius.
        if(aCircle->Location().Distance(pi)<aCircle->Radius()+tol &&  aCircle->Location().Distance(pi)>aCircle->Radius()-tol){

        } else {
            // std::cout<<"point is outside of arc radius."<<std::endl;
            return 0;
        }

        if(parameter>=first && parameter<=last){
            // std::cout<<"point is on arc segment."<<std::endl;
            return 1;
        }
        if(parameter<first || parameter>last){
            //! Try adding 2*M_PI, if the range by opencascade given 2*M_PI ahead.
            if((parameter+M_PI+M_PI)>first && (parameter+M_PI+M_PI)<last ){
                // std::cout<<"point is on arc segment."<<std::endl;
                return 1;
            } else {
                // std::cout<<"point is not on arc segment."<<std::endl;
                return 0;
            }
        }
        // std::cout<<""<<std::endl;
    }
    return 0;
}

std::vector<Handle(AIS_Shape)> draw_primitives::create_line_arc_intersections(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset){

    std::vector<Handle(AIS_Shape)> Vec;

    gp_Pnt p1,p2;
    get_line_points(shapeLine,p1,p2);
    gp_Pnt p3,p4,pc,pw;
    get_arc_points(shapeArc,p3,p4,pw,pc);
    organize_line_arc_points(p1,p2,p3,p4);

    // print_gp_Pnt("shape arc-line intersect at",p2);

    double r=get_arc_radius(shapeArc);

    // 1. Create offset lines on both sides of the shapeLine with the shapeArc's plane's waypoint to set the plane.
    gp_Pnt po1,po2,po3,po4;
    create_3d_line_arc_offset_lines(shapeLine,shapeArc,offset,po1,po2,po3,po4);
    // Vec.push_back(draw_3d_line(po1,po2));
    // Vec.push_back(draw_3d_line(po3,po4));

    // 2. Create normal.
    gp_Dir normal=create_normal_from_3_points_on_plane(po1,po2,po3);

    // 3. Create offset arcs.
    // Vec.push_back(draw_3d_pc_circle(pc,normal,r+offset));
    // Vec.push_back( colorize(draw_3d_sphere(r+offset,pc),Quantity_NOC_BEET,0.9) );
    if(r-offset>0){
        // Vec.push_back(draw_3d_pc_circle(pc,normal,r-offset));
        // Vec.push_back( colorize(draw_3d_sphere(r-offset,pc),Quantity_NOC_BEET,0.9) );
    }

    // 4. Create the 2 intersection points with the inside offset shapeArc.
    gp_Pnt pc_offset_sphere;
    gp_Pnt pi_arc_contour;
    gp_Pnt pi_line_contour;

    if(r-offset>0){
        if(intersect_3d_line_sphere(pc,po1,po2,r-offset,pc_offset_sphere)){ //! Centerpoint offset sphere.
            if(intersect_3d_sphere_sphere(pc_offset_sphere,offset,pc,r,pi_arc_contour)){ //! Intersect contour arc with offset spehe.
                if(is_point_on_arc(shapeArc,pi_arc_contour)){
                    //! Draw intersecton points.
                    // Vec.push_back(draw_3d_point(pi_arc_contour));
                    get_closest_point_to_line(pc_offset_sphere,p1,p2,pi_line_contour);
                    // Vec.push_back(draw_3d_point(pi_line_contour));
                    //! Draw intersection sphere.
                    // Vec.push_back( colorize(draw_3d_sphere(offset,pc_offset_sphere),Quantity_NOC_GREEN,0.9) );
                    if(pi_arc_contour.Distance(pi_line_contour)>1e-6 && pi_arc_contour.Distance(pc_offset_sphere)>1e-6){ //! Check for valid arc construct.
                        //! Draw 3p arc.
                        Vec.push_back( colorize(draw_3d_pc_arc_closest(pi_line_contour,pi_arc_contour,pc_offset_sphere,normal,p2),Quantity_NOC_RED,0.9) );
                    }
                }
            }
        }

        if(intersect_3d_line_sphere(pc,po3,po4,r-offset,pc_offset_sphere)){ //! Centerpoint offset sphere.
            if(intersect_3d_sphere_sphere(pc_offset_sphere,offset,pc,r,pi_arc_contour)){ //! Intersect 1.
                if(is_point_on_arc(shapeArc,pi_arc_contour)){
                    // Vec.push_back(draw_3d_point(pi_arc_contour));
                    get_closest_point_to_line(pc_offset_sphere,p1,p2,pi_line_contour);
                    // Vec.push_back(draw_3d_point(pi_line_contour));
                    // Vec.push_back( colorize(draw_3d_sphere(offset,pc_offset_sphere),Quantity_NOC_GREEN,0.9) );
                    if(pi_arc_contour.Distance(pi_line_contour)>1e-6 && pi_arc_contour.Distance(pc_offset_sphere)>1e-6){ //! Check for valid arc construct.
                        //! Draw 3p arc.
                        Vec.push_back( colorize(draw_3d_pc_arc_closest(pi_line_contour,pi_arc_contour,pc_offset_sphere,normal,p2),Quantity_NOC_RED,0.9) );
                    }
                }
            }
        }
    }

    if(intersect_3d_line_sphere(pc,po1,po2,r+offset,pc_offset_sphere)){ //! Centerpoint offset sphere.

        if(intersect_3d_sphere_sphere(pc_offset_sphere,offset,pc,r,pi_arc_contour)){ //! Intersect 1.
            if(is_point_on_arc(shapeArc,pi_arc_contour)){
                // Vec.push_back(draw_3d_point(pi_arc_contour));
                get_closest_point_to_line(pc_offset_sphere,p1,p2,pi_line_contour);
                // Vec.push_back(draw_3d_point(pi_line_contour));
                // Vec.push_back( colorize(draw_3d_sphere(offset,pc_offset_sphere),Quantity_NOC_GREEN,0.9) );
                if(pi_arc_contour.Distance(pi_line_contour)>1e-6 && pi_arc_contour.Distance(pc_offset_sphere)>1e-6){ //! Check for valid arc construct.
                    //! Draw 3p arc.
                    Vec.push_back( colorize(draw_3d_pc_arc_closest(pi_line_contour,pi_arc_contour,pc_offset_sphere,normal,p2),Quantity_NOC_RED,0.9) );
                }
            }
        }
    }

    if(intersect_3d_line_sphere(pc,po3,po4,r+offset,pc_offset_sphere)){ //! Centerpoint offset sphere.
        if(intersect_3d_sphere_sphere(pc_offset_sphere,offset,pc,r,pi_arc_contour)){ //! Intersect 1.
            if(is_point_on_arc(shapeArc,pi_arc_contour)){
                // Vec.push_back(draw_3d_point(pi_arc_contour));
                get_closest_point_to_line(pc_offset_sphere,p1,p2,pi_line_contour);
                // Vec.push_back(draw_3d_point(pi_line_contour));
                // Vec.push_back( colorize(draw_3d_sphere(offset,pc_offset_sphere),Quantity_NOC_GREEN,0.9) );
                if(pi_arc_contour.Distance(pi_line_contour)>1e-6 && pi_arc_contour.Distance(pc_offset_sphere)>1e-6){ //! Check for valid arc construct.
                    //! Draw 3p arc.
                    Vec.push_back( colorize(draw_3d_pc_arc_closest(pi_line_contour,pi_arc_contour,pc_offset_sphere,normal,p2),Quantity_NOC_RED,0.9) );
                    //Vec.push_back( colorize(draw_3d_pc_arc(pi_line_contour,pi_arc_contour,pc_offset_sphere,normal.X(),normal.Y(),normal.Z()),Quantity_NOC_RED,0.9) );
                }
            }
        }
    }

    return Vec;
}

void draw_primitives::create_3d_line_arc_offset_lines(Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset, gp_Pnt &po1, gp_Pnt &po2, gp_Pnt &po3, gp_Pnt &po4){

    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeLine->Shape());
    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeA,vp1,vp2);
    //! Line point start, end.
    gp_Pnt p1= BRep_Tool::Pnt(vp1);
    gp_Pnt p2= BRep_Tool::Pnt(vp2);

    const TopoDS_Edge& edgeB = TopoDS::Edge(shapeArc->Shape());
    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeB, first, last);

    //! Arc start-, endpoint.
    gp_Pnt p3= aCurve->Value(first);
    gp_Pnt p4= aCurve->Value(last);
    gp_Pnt pw= aCurve->Value((first+last)/2); //! Arc waypoint is more robust for defining a plane.

    //! Ensure correct point flow.
    organize_line_arc_points(p1,p2,p3,p4);

    //! Create both line offset lines. Leftside offset + rightside offset.
    offset_line_on_a_plane(p1,p2,pw,offset,po1,po2);
    offset_line_on_a_plane(p1,p2,pw,-offset,po3,po4);
}

bool draw_primitives::draw_3d_line_arc_offset_lines( Handle(AIS_Shape) shapeLine, Handle(AIS_Shape) shapeArc, double offset, Handle(AIS_Shape) &shapeA, Handle(AIS_Shape) &shapeB){

    gp_Pnt po1,po2,po3,po4;
    create_3d_line_arc_offset_lines(shapeLine,shapeArc,offset,po1,po2,po3,po4);

    shapeA=draw_3d_line(po1,po2);
    shapeA=colorize(shapeA,Quantity_NOC_BLUE,0);
    shapeB=draw_3d_line(po3,po4);
    shapeB=colorize(shapeB,Quantity_NOC_RED,0);

    return 1;
}

bool draw_primitives::draw_3d_arc_line_fillet_conventional(Handle(AIS_Shape) shapeArc, Handle(AIS_Shape) shapeLine, double radius, Handle(AIS_Shape) &shape){

    //! Or use:
    const TopoDS_Edge& edgeA = TopoDS::Edge(shapeArc->Shape());
    const TopoDS_Edge& edgeB = TopoDS::Edge(shapeLine->Shape());

    TopoDS_Vertex vp1,vp2;
    TopExp::Vertices(edgeB,vp1,vp2);
    //! Line point start, end.
    gp_Pnt p1= BRep_Tool::Pnt(vp1);
    gp_Pnt p2= BRep_Tool::Pnt(vp2);

    Standard_Real first, last;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edgeA, first, last);
    gp_Pnt p3= aCurve->Value(first);
    gp_Pnt p4= aCurve->Value(last);
    gp_Pnt pw= aCurve->Value((first+last)/2); //! Arc waypoint.

    gp_Pln plane;
    create_3d_plane(p2,pw,p1,plane);

    //! Create 2d fillets on a plane.
    ChFi2d_FilletAlgo api;
    api.Init(edgeA,edgeB,plane);

    if(!api.Perform(radius)){
        std::cout<<"error in perform api arc-line."<<std::endl;
        return 0;
    }
    TopoDS_Edge theEdge1,theEdge2;
    TopoDS_Edge edge1=api.Result(gp_Pnt(0,0,0),theEdge1,theEdge2,-1);

    shape=new AIS_Shape(edge1);
    shape=colorize(shape,Quantity_NOC_RED,0);
    return 1;
}

// Function to project a point onto a plane
gp_Pnt draw_primitives::projectPointOnPlane(gp_Pnt point, gp_Pnt planeOrigin, gp_Vec planeNormal) {
    gp_Vec pointToPlane(planeOrigin, point);
    Standard_Real dotProduct = pointToPlane.Dot(planeNormal);
    Standard_Real distance = planeNormal.Magnitude();

    Standard_Real projectionFactor = dotProduct / distance;
    gp_Vec projectedVector = planeNormal.Multiplied(projectionFactor);

    gp_Pnt projectedPoint = point.Translated(-projectedVector);
    return projectedPoint;
}

void draw_primitives::Visit(const TDF_Label& theLabel)
{
    //! theLabel.EntryDump(std::cout);
    Quantity_Color aColor;
    TopoDS_Shape aShape;
    Handle(TDataStd_Name) aName;
    if (theLabel.FindAttribute(TDataStd_Name::GetID(), aName)){
        //! std::cout << "  Name: " << aName->Get() << std::endl;
    }
    bool skip=0;
    if (aShapeTool->IsShape(theLabel))
    {
        //! if a shape can be made from the current label, look for the color of that label:
        if(aShapeTool->GetShape(theLabel, aShape)){
            if(aColorTool->GetColor(aShape,XCAFDoc_ColorSurf,aColor)){
                skip=0;
            } else { skip=1;}
        }
        if(skip==0){
            Ais_ShapeVec.push_back(new AIS_Shape(aShape));
            Ais_ShapeVec.back()->SetColor(aColor);
            Ais_ShapeVec.back()->SetDisplayMode(AIS_Shaded);
            Ais_ShapeVec.back()->Attributes()->SetFaceBoundaryDraw(true);
            Ais_ShapeVec.back()->Attributes()->SetFaceBoundaryAspect(
                        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
            Ais_ShapeVec.back()->Attributes()->SetIsoOnTriangulation(true);
            //! Temporairy stepfile preview:
            //! m_context->Display(Ais_ShapeVec.back(),Standard_False);
        }
    }
    /*! Repeat the visit function for each childmember. */
    for (TDF_ChildIterator c(theLabel); c.More(); c.Next())
    {
        Visit(c.Value());
    }
}

std::vector<Handle(AIS_Shape)> draw_primitives::load_stepfile(std::string filename)
{
    //! Empty temponairy bucket, the visit function will fill up the bucket for one stepfile.
    Ais_ShapeVec.clear();

    STEPCAFControl_Controller::Init();

    Handle(TDocStd_Document) aDoc;
    Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
    anApp->NewDocument("MDTV-XCAF", aDoc);

    STEPCAFControl_Reader aStepReader;
    aStepReader.SetColorMode(true);
    aStepReader.SetNameMode(true);
    aStepReader.SetLayerMode(true);
    aStepReader.SetPropsMode(true);

    aStepReader.ReadFile (filename.c_str());
    aStepReader.Transfer (aDoc);

    TDF_Label aRootLabel = aDoc->Main();

    aShapeTool = XCAFDoc_DocumentTool::ShapeTool(aRootLabel);
    aColorTool = XCAFDoc_DocumentTool::ColorTool(aRootLabel);

    Visit(aRootLabel);

    return Ais_ShapeVec;
}

bool draw_primitives::snap_to_closest_endpoint(std::vector<Handle(AIS_Shape)> shapevec, gp_Pnt mousepos, double dist, gp_Pnt &snappos){

    /*
    for(uint i=0; i<shapevec.size(); i++){
        TopoDS_Shape underlyingShape = shapevec.at(i)->Shape();

        if (underlyingShape.ShapeType() == TopAbs_COMPSOLID) {
            std::cout << "The shape is a compsolid." << std::endl;
            break;
        }
        if (underlyingShape.ShapeType() == TopAbs_COMPOUND) {
            std::cout << "The shape is a compound." << std::endl;
            break;
        }
        if (underlyingShape.ShapeType() == TopAbs_SOLID) {
            break;
        }
        if (underlyingShape.ShapeType() == TopAbs_SHELL) {
            break;
        }

        if (underlyingShape.ShapeType() == TopAbs_FACE) {
            break;
        }
        if (underlyingShape.ShapeType() == TopAbs_WIRE) {
            break;
        }
        if (underlyingShape.ShapeType() == TopAbs_EDGE) {

            const TopoDS_Edge& edge = TopoDS::Edge(shapevec.at(i)->Shape());
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (curve->IsKind(STANDARD_TYPE(Geom_Line))) {
                std::cout << "The shape is a line." << std::endl;
                gp_Pnt p1,p2;
                p1= curve->Value(first);
                p2= curve->Value(last);

                if(mousepos.Distance(p1)<dist){
                    snappos=p1;
                    print_gp_Pnt("snap to",p1);
                    return 1;
                }
                if(mousepos.Distance(p2)<dist){
                    snappos=p2;
                    print_gp_Pnt("snap to",p2);
                    return 1;
                }
            }

            if (curve->IsKind(STANDARD_TYPE(Geom_Circle))) {
                std::cout << "The shape is an arc or circle." << std::endl;
                return 1;
            }
        }
        if (underlyingShape.ShapeType() == TopAbs_VERTEX) {

            TopExp_Explorer explorer(shapevec.at(i)->Shape(), TopAbs_VERTEX);
            TopoDS_Vertex aVertex = TopoDS::Vertex(explorer.Current());
            gp_Pnt p1 = BRep_Tool::Pnt(aVertex);
            std::cout << "The shape represents a single point." << std::endl;
            if(mousepos.Distance(p1)<dist){
                snappos=p1;
                print_gp_Pnt("snap to",p1);
                return 1;
            }
        }
        if (underlyingShape.ShapeType() == TopAbs_SHAPE) {
            break;
        }

    }*/


    for(uint i=0; i<shapevec.size(); i++){

        TopExp_Explorer explorer(shapevec.at(i)->Shape(), TopAbs_VERTEX);
        if (explorer.More()) {
            explorer.Next();
            if (!explorer.More()) {

                TopoDS_Vertex aVertex = TopoDS::Vertex(explorer.Current());
                gp_Pnt p1 = BRep_Tool::Pnt(aVertex);

                // std::cout << "The shape represents a single point." << std::endl;
                if(mousepos.Distance(p1)<dist){
                    snappos=p1;
                    print_gp_Pnt("snap to",p1);
                    return 1;
                }

            } else {
                // std::cout << "The shape does not represent a single point." << std::endl;

                Handle(AIS_Shape) shape=shapevec.at(i);
                const TopoDS_Edge& edge = TopoDS::Edge(shape->Shape());
                Standard_Real first, last;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);

                if(curve->IsKind(STANDARD_TYPE(Geom_Line))){
                    gp_Pnt p1,p2;
                    p1= curve->Value(first);
                    p2= curve->Value(last);

                    if(mousepos.Distance(p1)<dist){
                        snappos=p1;
                        print_gp_Pnt("snap to",p1);
                        return 1;
                    }
                    if(mousepos.Distance(p2)<dist){
                        snappos=p2;
                        print_gp_Pnt("snap to",p2);
                        return 1;
                    }

                }
            }
        }

    }

    snappos=mousepos;
    return 0;
}

bool draw_primitives::snap_to_closest_midpoint(std::vector<Handle(AIS_Shape)> shapevec, gp_Pnt mousepos, double dist, gp_Pnt &snappos){

    for(uint i=0; i<shapevec.size(); i++){

        Handle(AIS_Shape) shape=shapevec.at(i);
        const TopoDS_Edge& edge = TopoDS::Edge(shape->Shape());
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        gp_Pnt p1= curve->Value((first+last)/2);

        if(mousepos.Distance(p1)<dist){
            snappos=p1;
            return 1;
        }
    }
    snappos=mousepos;
    return 0;
}

Handle(AIS_Shape) draw_primitives::downcast(Handle(AIS_InteractiveObject) interactiveObject){
    return  Handle(AIS_Shape)::DownCast(interactiveObject);
}

















