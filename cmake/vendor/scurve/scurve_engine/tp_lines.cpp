#include "tp_lines.h"
#include "stdio.h"

tp_lines::tp_lines()
{

}

extern "C" struct sc_pnt tp_lines_midpoint(struct sc_pnt p1, struct sc_pnt p2) {
    sc_pnt midpoint;
    midpoint.x=(p1.x+p2.x)/2;
    midpoint.y=(p1.y+p2.y)/2;
    midpoint.z=(p1.z+p2.z)/2;
    return midpoint;
}

int tp_lines::are_points_equal(struct sc_pnt p1, struct sc_pnt p2, double tolerance) {
    return (fabs(p1.x - p2.x) < tolerance) &&
            (fabs(p1.y - p2.y) < tolerance) &&
            (fabs(p1.z - p2.z) < tolerance);
}

extern "C" int tp_lines_are_points_equal_c(struct sc_pnt p1, struct sc_pnt p2, double tolerance) {
    return tp_lines().are_points_equal(p1,p2,tolerance);
}

int tp_lines::are_lengths_equal(double length1, double length2, double tolerance) {
    return fabs(length1 - length2) < tolerance;
}

extern "C" int tp_lines_are_lengths_equal_c(double length1, double length2, double tolerance) {
    return tp_lines().are_lengths_equal(length1,length2,tolerance);
}

// Function to calculate the distance between two 3D points
double tp_lines::distance(sc_pnt p1, sc_pnt p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
}

// Function to check if three 3D points are collinear
int tp_lines::colinear(sc_pnt a, sc_pnt b, sc_pnt c) {
    double dist_ab = distance(a, b);
    double dist_bc = distance(b, c);
    double dist_ac = distance(a, c);

    // Check if the sum of distances between a and b, and between b and c, equals the distance between a and c
    double tolerance=1e-6;
    return fabs((dist_ab + dist_bc) - dist_ac) <= tolerance;
}

extern "C" int tp_lines_colinear_c(struct sc_pnt p0, struct sc_pnt p1, struct  sc_pnt p2){
    return tp_lines::colinear(p0,p1,p2);
}

extern "C" int tp_lines_point_on_line_c(struct sc_pnt p0, struct sc_pnt lp1, struct  sc_pnt lp2){
    return tp_lines::colinear(lp1,p0,lp2);
}

// Function to calculate the dot product of two vectors
double tp_lines::dot_product(sc_pnt v1, sc_pnt v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Function to create the direction vector from two points
sc_pnt tp_lines::direction_vector(sc_pnt p1, sc_pnt p2) {
    sc_pnt vec;
    vec.x = p2.x - p1.x;
    vec.y = p2.y - p1.y;
    vec.z = p2.z - p1.z;
    return vec;
}

// Function to check if two lines are perpendicular
int tp_lines::perpendicular(sc_pnt p0, sc_pnt p1, sc_pnt p2) {
    // Calculate the dot product of the direction vectors

    sc_pnt v1 = direction_vector(p0, p1);
    sc_pnt v2 = direction_vector(p1, p2);

    double dot = dot_product(v1, v2);

    // Check if the dot product is zero (indicating perpendicularity)
    double tolerance=1e-6;
    return fabs(dot - 0.0) <= tolerance;
}

extern "C" int tp_lines_perpendicular_c(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2){
    // Check if the lines are perpendicular
    return tp_lines::perpendicular(p0, p1, p2);
}

void tp_lines::sc_interpolate_lin(sc_pnt p0, sc_pnt p1, double progress, sc_pnt &pi){
    sc_interpolate_lenght(p0.x,p1.x,progress,pi.x);
    sc_interpolate_lenght(p0.y,p1.y,progress,pi.y);
    sc_interpolate_lenght(p0.z,p1.z,progress,pi.z);
}

//! Progress 0-1.
extern "C" void interpolate_line_c(struct sc_pnt p0, struct sc_pnt p1, double progress, struct  sc_pnt *pi){
    struct sc_pnt p;
    tp_lines().sc_interpolate_lin(p0,p1,progress,p);
    *pi=p;
}


void tp_lines::sc_interpolate_dir(sc_dir d0, sc_dir d1, double progress, sc_dir &di){
    sc_interpolate_lenght(d0.a,d1.a,progress,di.a);
    sc_interpolate_lenght(d0.b,d1.b,progress,di.b);
    sc_interpolate_lenght(d0.c,d1.c,progress,di.c);
}

//! Progress 0-1.
extern "C" void interpolate_dir_c(struct sc_dir p0, struct sc_dir p1, double progress, struct sc_dir *pi){
    struct sc_dir p;
    tp_lines().sc_interpolate_dir(p0,p1,progress,p);
    *pi=p;
}

void tp_lines::sc_interpolate_ext(sc_ext e0, sc_ext e1, double progress, sc_ext &ei){
    sc_interpolate_lenght(e0.u,e1.u,progress,ei.u);
    sc_interpolate_lenght(e0.v,e1.v,progress,ei.v);
    sc_interpolate_lenght(e0.w,e1.w,progress,ei.w);
}

//! Progress 0-1.
extern "C" void interpolate_ext_c(struct sc_ext p0, struct sc_ext p1, double progress, struct sc_ext *pi){
    struct sc_ext p;
    tp_lines().sc_interpolate_ext(p0,p1,progress,p);
    *pi=p;
}

double netto_difference_of_2_values(double a, double b){

    double diff=0;
    if(a<0 && b<0){
        a=fabs(a);
        b=fabs(b);
        diff=fabs(a-b);
    }
    if(a>=0 && b>=0){
        diff=fabs(a-b);
    }
    if(a<=0 && b>=0){;
        diff=fabs(a)+b;
    }
    if(a>=0 && b<=0){
        diff=a+fabs(b);
    }
    return diff;
}

void tp_lines::sc_interpolate_lenght(double start, double end, double progress, double &li){
    if(start<end){
        li=start+(progress*netto_difference_of_2_values(start,end));
        return;
    }
    if(start>end){
        li=start-(progress*netto_difference_of_2_values(start,end));
        return;
    }
    if(start==end){
        li=end;
    }
}

double tp_lines::sc_line_lenght(sc_pnt p0, sc_pnt p1){
    return sqrt(pow(p1.x-p0.x,2)+pow(p1.y-p0.y,2)+pow(p1.z-p0.z,2));
}

extern "C" double line_lenght_c(struct sc_pnt start, struct sc_pnt end){

    double l=tp_lines().sc_line_lenght(start,end);
    if(isnanf(l)){
        printf("line_lenght_c isnan \n");
        return 0;
    }
    return l;
}

















