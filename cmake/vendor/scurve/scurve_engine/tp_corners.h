#ifndef TP_CORNERS_H
#define TP_CORNERS_H

//! Author  : SKynet Cyberdyne
//! Licence : MIT
//! Date    : 2023

#ifdef __cplusplus

#include <vector>
#include <iostream>
#include <cmath>
#include <Dense>
#include <Geometry>

#include "tp_scurve.h"
#include "tp_arcs.h"

//! Make conversion's easy:
#define to_radians  M_PI/180.0
#define to_degrees  (180.0/M_PI)

class tp_corners
{
public:

    //! Constructor.
    tp_corners();

    //! p1 = common point.
    void line_line_angle(sc_pnt p0,
                      sc_pnt p1,
                      sc_pnt p2,
                      double &angle_deg);

    //! p0 = line start.
    //! p1 = arc startpoint, common point.
    //! p2 = arc waypoint.
    //! p3 = arc endpoint.
    void line_arc_angle(sc_pnt p0,
                     sc_pnt p1,
                     sc_pnt p2,
                     sc_pnt p3,
                     double &angle_deg);

    //! p0 = arc start.
    //! p1 = arc waypoint.
    //! p2 = arc endpoint, common point.
    //! p3 = line endpoint.
    void arc_line_angle(sc_pnt p0,
                     sc_pnt p1,
                     sc_pnt p2,
                     sc_pnt p3,
                     double &angle_deg);

    //! p0 = arc start.
    //! p1 = arc waypoint.
    //! p2 = arc endpoint, common point.
    //! p3 = arc waypoint.
    //! p4 = arc endpoint.
    void arc_arc_angle(sc_pnt p0,
                    sc_pnt p1,
                    sc_pnt p2,
                    sc_pnt p3,
                    sc_pnt p4,
                    double &angle_deg);

    //! If ange=180, this means colineair.
    void segment_angle(tp_segment s0, tp_segment s1, double &angle_deg);
};

//! Here it tells if this code is used in c, convert the class to a struct. This is handy!
#else
typedef struct tp_corners tp_corners;
#endif //! cplusplus

#endif




















