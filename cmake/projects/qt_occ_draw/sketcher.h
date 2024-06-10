#ifndef SKETCHER_H
#define SKETCHER_H

#include <QColor>
#include "AIS_Line.hxx"
#include "AIS_Shape.hxx"
#include "Geom_CartesianPoint.hxx"
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <draw_primitives.h>

class Sketcher
{
public:
    Sketcher();

    // Scetch a line.
    Handle(AIS_Line) sketch_3d_line_set_startpoint_rubberline(gp_Pnt p);
    Handle(AIS_Line) sketch_3d_line_show_endpoint_rubberline(gp_Pnt p);

    Handle(AIS_Line) rubberline;
    Handle(AIS_Shape) rubberarc;
    Handle(AIS_Shape) plane_shape;
    gp_Pln plane;

    Quantity_Color QColorToQuantityColor(QColor color);
    Quantity_Color color;

    Handle(Geom_CartesianPoint) myFirstPoint;
    Handle(Geom_CartesianPoint)	mySecondPoint;

    //! Given a line startpoint p1, given a current mousepos p2. Calculate orhto pos.
    gp_Pnt to_ortho(gp_Pnt p1, gp_Pnt p2);
    //! Mouse postition projected to current plane.
    gp_Pnt to_plane(gp_Pnt p1);

    bool ortho=0, snap=0;
    gp_Pnt planeorigin={0,0,0};
    gp_Vec planevec=gp::DZ();

};

#endif // SKETCHER_H
