#ifndef CAVALIER_3D_H
#define CAVALIER_3D_H

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
#include "cavalier_offset.h"
#include "AIS_Shape.hxx"

struct cavline {
    gp_Pnt p0, p1, pw, pc;
    double bulge;
    double lenght;
};

class cavalier_3d
{
public:
    cavalier_3d();

    gp_Pln AlignPlaneToOrigin(const gp_Pln& originalPlane);
    void PortCavLineBetweenPlanes(const gp_Pln& sourcePlane, const gp_Pln& targetPlane, cavline& line);
    std::vector<cavline> convertToCavLines(const std::vector<cavc::Polyline<double>>& outside);
    void drawPolyline_(const cavc::Polyline<double>& polyline, Quantity_Color color, opencascade::handle<AIS_InteractiveContext> & context);
    void drawCavLine(const cavline& line, Quantity_Color color, opencascade::handle<AIS_InteractiveContext>& context);
    Handle(AIS_Shape) cavline_to_shape(cavline line);
    void run_2d_example(opencascade::handle<AIS_InteractiveContext> & context, double offset);
    void run_2d_plus_example(opencascade::handle<AIS_InteractiveContext> & context, double offset);
    void run_3d_example(opencascade::handle<AIS_InteractiveContext> & context, double offset);
    void run_3d_plus_example(opencascade::handle<AIS_InteractiveContext> & context, double offset);
    std::vector<cavline> get_2d_fillet_path_2_segments(cavline l0, cavline l1, double radius);
     std::vector<cavline> get_2d_fillet_pathvec(std::vector<cavline> cvec, double radius);
    int get_3d_fillet_path_2_segments(cavline l0, cavline l1, double radius, std::vector<cavline> &result_vec);
    int get_3d_fillet_pathvec(std::vector<cavline> cavvec, double radius, std::vector<cavline> &result_vec);
    gp_Pnt get_2d_arc_center(gp_Pnt p0, gp_Pnt pw, gp_Pnt p1);
    double get_angle_between_vectors(const gp_Pnt& pc, const gp_Pnt& p0, const gp_Pnt& p1);
    double get_2d_arc_length(const gp_Pnt& p0, const gp_Pnt& pw, const gp_Pnt& p1, const gp_Pnt& pc);
};

#endif // CAVALIER_3D_H





