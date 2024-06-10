#ifndef TP_LINES_H
#define TP_LINES_H

//! Author  : SKynet Cyberdyne
//! Licence : MIT
//! Date    : 2023

#include "tp_scurve.h"

#ifdef __cplusplus
class tp_lines
{
public:
    tp_lines();

    static int are_points_equal(struct sc_pnt p1, struct sc_pnt p2, double tolerance);
    static int are_lengths_equal(double length1, double length2, double tolerance);
    static double distance(sc_pnt p1, sc_pnt p2);
    static int colinear(sc_pnt a, sc_pnt b, sc_pnt c);
    static double dot_product(sc_pnt v1, sc_pnt v2);
    static int perpendicular(sc_pnt p0, sc_pnt p1, sc_pnt p2);
    static sc_pnt direction_vector(sc_pnt p1, sc_pnt p2);

    void sc_interpolate_lin(sc_pnt p0,
                            sc_pnt p1,
                            double progress, //! 0-1
                            sc_pnt &pi);

    void sc_interpolate_dir(sc_dir d0,
                            sc_dir d1,
                            double progress,
                            sc_dir &di);

    void sc_interpolate_ext(sc_ext e0,
                            sc_ext e1,
                            double progress,
                            sc_ext &ei);

    void sc_interpolate_lenght(double start,
                               double end,
                               double progress,
                               double &li);

    double sc_line_lenght(sc_pnt p0, sc_pnt p1);

    void sc_interpolate_lenght(sc_pnt start,
                               sc_pnt end,
                               double progress,
                               sc_pnt &pnt);
};
//! Here it tells if this code is used in c, convert the class to a struct. This is handy!
#else
typedef struct tp_lines tp_lines;
#endif //! cplusplus

#endif
