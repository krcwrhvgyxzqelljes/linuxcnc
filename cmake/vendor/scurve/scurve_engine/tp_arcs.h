#ifndef TP_ARCS_H
#define TP_ARCS_H

//! Author  : SKynet Cyberdyne
//! Licence : MIT
//! Date    : 2023

#include "tp_scurve.h"

#ifdef __cplusplus

#include <vector>
#include <iostream>
#include <Dense>
#include <Geometry>

using namespace Eigen;

class tp_arcs
{
public:
    tp_arcs();

    sc_pnt vectorBetween(const sc_pnt& p1, const sc_pnt& p2);
    double dotProduct(const sc_pnt& v1, const sc_pnt& v2);
    double magnitude(const sc_pnt& v);

    double calculateArcAngleRadians(const sc_pnt& p0,  //! Startpoint.
                                    const sc_pnt& p1,  //! Waypoint.
                                    const sc_pnt& p2); //! Endpoint.

    void sc_interpolate_arc(sc_pnt p0,          //! Startpoint.
                            sc_pnt p1,          //! Waypoint.
                            sc_pnt p2,          //! Endpoint.
                            sc_pnt p3_,         //! Centerpoint.
                            double progress,    //! 0-1.
                            sc_pnt &pi);        //! Interpolated point.

    double sc_arc_lenght(sc_pnt p0,  //! Start.
                         sc_pnt p1,  //! Waypoint.
                         sc_pnt p2,  //! End.
                         sc_pnt p3); //! Center.

    void sc_arc_radius(sc_pnt p0,
                       sc_pnt p1,  //! Waypoint.
                       sc_pnt p2,
                       double &radius);

    //! If points are colinear, output is xy plane, type clockwise g2.
    void sc_arc_get_mid_waypoint(sc_pnt p0, //! Start.
                                 sc_pnt p1, //! Center.
                                 sc_pnt p2, //! End.
                                 sc_pnt &pi);

private:
    sc_pnt sc_rotate_point_around_line(sc_pnt thePointToRotate,
                                       double theta,            //! Input in radians.
                                       sc_pnt theLineP1,
                                       sc_pnt theLineP2);

    //! Keeping as is from original github code example.
    struct sc_arc {
        std::vector<sc_pnt> pntVec;
        double radius=0;
        double diameter=0;
        double arcLenght=0;
        double arcAngleRad=0; //! Radians
        double arcCircumFence=0;
        bool arcAngleNegative=0; //! When sign of the angle < 0, set true.
        sc_pnt center;
        sc_pnt pointOnArcAxis;
    };

    sc_arc sc_arc_points(Eigen::Vector3d p1,
                         Eigen::Vector3d p2,
                         Eigen::Vector3d p3,
                         double division);
};
#else
typedef struct tp_arcs tp_arcs;
#endif //! cplusplus

#endif

