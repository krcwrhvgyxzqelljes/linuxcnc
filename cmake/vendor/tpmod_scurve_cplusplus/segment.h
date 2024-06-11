#ifndef SEGMENT_H
#define SEGMENT_H


#ifdef __cplusplus

#include "gp_Pnt.hxx"
#include "gp_Ax3.hxx"
#include "gp_Pln.hxx"
#include "vector"

struct segment {

    gp_Pnt p0;              // Startpoint.
    gp_Pnt pw;              // Arc, line waypoint.
    gp_Pnt p1;              // Endpoint.
    gp_Pnt pc;              // Arc center.
    gp_Pnt pn;              // Point on axis normal.
    gp_Dir normal;          // Normal direction
    gp_Ax3 axis;            // Line from pc in axis direction.
    gp_Pln plane;           // Plane of arc, circle where p0,pw,p1,pc are on.
    double a0,a1;
    double b0,b1;
    double c0,c1;
    double u0,u1;
    double v0,v1;
    double w0,w1;

    double vel;
    int turn;               // -1=ccw.
    int canon_motion_type;  // 1=rapid, 2=linear, 3=circle,
    int gcode_line_nr;      // Refers to the gcode textfile line nr of this segment.

    double g64_p, g64_q;
};

#else
typedef struct segment segment;
#endif // cplusplus

#endif // SEGMENT_H
