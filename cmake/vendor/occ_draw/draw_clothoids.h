#ifndef DRAW_CLOTHOIDS_H
#define DRAW_CLOTHOIDS_H

#include "draw_primitives.h"
#include "Clothoids.hh"

class draw_clothoids
{
public:
    draw_clothoids();

    struct clothoid {
        gp_Pnt p1,p2;
        double theta0,theta1,kappa0,kappa1;
        double lenght;
    };

    static int draw_2d_clothoid_3arc_G2(gp_Pnt p0, gp_Pnt p1, double theta0, double theta1, double kappa0, double kappa1,std::vector<gp_Pnt> &pvec);
    static int draw_inbetween_2d_clothoid_3arc_G2(gp_Pnt p0, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4, gp_Pnt p5, std::vector<gp_Pnt> &pvec);
    static int draw_inbetween_3d_clothoid_3arc_G2(gp_Pnt &p0, gp_Pnt &p1, gp_Pnt &p2, gp_Pnt &p3, gp_Pnt &p4, gp_Pnt &p5, std::vector<gp_Pnt> &clotvec);
};

#endif // DRAW_CLOTHOIDS_H




















