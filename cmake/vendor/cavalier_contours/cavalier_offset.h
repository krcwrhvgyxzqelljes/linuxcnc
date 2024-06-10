#ifndef CAVALIER_OFFSET_H
#define CAVALIER_OFFSET_H

#include <gp_Pnt.hxx>
#include "iostream"
#include "gp_Pnt.hxx"
#include <gp_Pln.hxx>
#include <Geom_Plane.hxx>
#include <GeomAPI.hxx>
#include <gp_Ax3.hxx>
#include <gp_Quaternion.hxx> // Include for gp_Quaternion
#include <iostream>
#include "../occ_draw/draw_primitives.h"
#include "include/cavc/polylineoffset.hpp"
#include "gp_Pnt.hxx"

class cavalier_offset
{
public:
    cavalier_offset();

    gp_Pnt rotate_3d(double x_to_rotate,double y_to_rotate, double z_to_rotate, double rotate_degrees_x, double rotate_degrees_y, double rotate_degrees_z);
    gp_Pnt offset_point_on_line(double xs, double ys, double xe, double ye, double offset_from_xs_ys);
    gp_Pnt bulge_to_arc_waypoint(gp_Pnt p1, gp_Pnt p2, double bulge);
    double waypoint_to_bulge(gp_Pnt p1, gp_Pnt pw, gp_Pnt p2);
    void drawOriginalPolyline(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context);
    void drawPolylineArcsOnly(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> &context);
    void drawPolylineLinesOnly(const cavc::Polyline<double>& polyline, Quantity_Color color , opencascade::handle<AIS_InteractiveContext> & context);
    void drawPolyline(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context);
    void run_offset_example(opencascade::handle<AIS_InteractiveContext> & context);

};

#endif // CAVALIER_OFFSET_H






