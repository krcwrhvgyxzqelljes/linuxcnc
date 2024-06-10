#ifndef CLOTHOID_INTERFACE_H
#define CLOTHOID_INTERFACE_H

#include "Clothoid.hh"
#include "gp_Pnt.hxx"
#include "vector"

struct clothoid {

    // Inputs.
    gp_Pnt p0;          // coordinates of the initial point
    double theta0 = 0;  // orientation (angle) of the clothoid at the initial point
    gp_Pnt p1;          // coordinates of the final point
    double theta1 = 0;  // orientation (angle) of the clothoid at the final point
    double nr_of_points = 0;  // number of points (not further specified in original comments)

    // Outputs.
    std::vector<gp_Pnt> pvec;  // vector of points representing the clothoid curve
    double L = 0;              // length of the clothoid curve from initial to final point
    double k = 0;              // curvature at the initial point
    double dk = 0;             // derivative of curvature with respect to arc length
    double iter = 0;           // Newton iterations used to solve the interpolation problem

    // Optional output.
    double k_1 = 0;            // partial derivative of the solution with respect to theta0
    double dk_1 = 0;           // partial derivative of the solution with respect to theta0
    double L_1 = 0;            // partial derivative of the solution with respect to theta0
    double k_2 = 0;            // partial derivative of the solution with respect to theta1
    double dk_2 = 0;           // partial derivative of the solution with respect to theta1
    double L_2 = 0;            // partial derivative of the solution with respect to theta1
};

class clothoid_interface
{
public:
    clothoid_interface();

    void test(){
        clothoid c;
        build_clothoid({0,0,0},0.1,{100,0,0},0.1,c);

         std::vector<gp_Pnt> pvec=eval_clothoid(c,100);
         // Print your pvec vector to your needs.
    }

    void build_clothoid(gp_Pnt p0, double theta0, gp_Pnt p1, double theta1, clothoid &clothoid){
        clothoid.p0=p0;
        clothoid.theta0=theta0;
        clothoid.p1=p1;
        clothoid.theta1=theta1;
        build_clothoid(clothoid);
    }

    std::vector<gp_Pnt> eval_clothoid(clothoid &in, int nr_of_points){
        in.nr_of_points=nr_of_points;
        eval_clothoid(in);
        return in.pvec;
    }

    void build_clothoid(clothoid &in);
    void eval_clothoid(clothoid &in);
    gp_Pnt interpolate_clothoid_point_displacement(clothoid &in, double s); // s is displacement related to total curve lenght L.
    gp_Pnt interpolate_clothoid_point_progress(clothoid &in, double progress);
};

#endif // CLOTHOID_INTERFACE_H














