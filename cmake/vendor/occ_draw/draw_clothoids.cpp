#include "draw_clothoids.h"

draw_clothoids::draw_clothoids(){ }

int draw_clothoids::draw_2d_clothoid_3arc_G2(gp_Pnt p0, gp_Pnt p1, double theta0, double theta1, double kappa0, double kappa1, std::vector<gp_Pnt> &pvec){

    //std::vector<gp_Pnt> pvec;
    G2lib::G2solve3arc g2solve3arc;

    g2solve3arc.setTolerance(1e-6);
    g2solve3arc.setMaxIter(10);

    G2lib::real_type x0  = p0.X();
    G2lib::real_type y0  = p0.Y();
    G2lib::real_type th0 = theta0;
    G2lib::real_type k0  = kappa0;
    G2lib::real_type x1  = p1.X();
    G2lib::real_type y1  = p1.Y();
    G2lib::real_type th1 = theta1;
    G2lib::real_type k1  = kappa1;

    int iter = g2solve3arc.build( x0, y0, th0, k0, x1, y1, th1, k1 );
    if(iter==-1){
        std::cout<<"clothoid terminated."<<std::endl;
        return 0;
    }

    G2lib::ClothoidCurve const & S0 = g2solve3arc.getS0();
    G2lib::ClothoidCurve const & S1 = g2solve3arc.getS1();
    G2lib::ClothoidCurve const & SM = g2solve3arc.getSM();

    double ltot= g2solve3arc.totalLength();
    // std::cout<<"ltot"<<ltot<<" iter:"<<iter<<std::endl;
    int segments=20;

    if(ltot==0){
        return 0;
    }

    int cycles = segments+1;
    double increment = ltot / segments;

    for (int cycle = 0; cycle < cycles; ++cycle) {
        double i = cycle * increment;
        pvec.push_back({g2solve3arc.X(i), g2solve3arc.Y(i), 0});
    }
    return 1;
}

int draw_clothoids::draw_inbetween_2d_clothoid_3arc_G2(gp_Pnt p0, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4, gp_Pnt p5, std::vector<gp_Pnt> &pvec){

    int type0=draw_primitives::get_shape_type(p0,p1,p2);
    int type1=draw_primitives::get_shape_type(p3,p4,p5);

    // std::cout<<"shapetype0:"<<type0<<std::endl;
    // std::cout<<"shapetype1:"<<type1<<std::endl;

    if(type0==draw_primitives::point || type0==draw_primitives::circle || type0==draw_primitives::unknown){
        return 0;
    }
    if(type1==draw_primitives::point || type1==draw_primitives::circle || type1==draw_primitives::unknown){
        return 0;
    }

    double theta0=0, theta1=0, dir0=0, dir1=0, kappa0=0, kappa1=0;

    // Shape0
    if(type0==draw_primitives::line){ // Line.
        theta0=draw_primitives::get_2d_line_angle(p0, p2);
        kappa0=0;
    }
    if(type0==draw_primitives::arc){ // Arc.
        theta0=draw_primitives::get_2d_arc_end_angle(p0, p1, p2);
        dir0=draw_primitives::get_2d_arc_direction(p0,p1,p2);
        if(dir0==-1){ // cw.
            theta0+=0.5*M_PI;
        }
        if(dir0==1){ // ccw.
            theta0-=0.5*M_PI;
        }
        kappa0=1/draw_primitives::get_2d_arc_radius(p0,p1,p2);
    }

    // Shape1
    if(type1==draw_primitives::line){ // Line.
        theta1=draw_primitives::get_2d_line_angle(p3, p5);
        kappa1=0;
    }
    if(type1==draw_primitives::arc){ // Arc.
        theta1=draw_primitives::get_2d_arc_start_angle(p3, p4, p5);
        dir1=draw_primitives::get_2d_arc_direction(p3,p4,p5);
        if(dir1==-1){ // cw.
            theta1-=0.5*M_PI;
        }
        if(dir1==1){ // ccw.
            theta1+=0.5*M_PI;
        }
        kappa1=1/draw_primitives::get_2d_arc_radius(p3,p4,p5);
    }

    // Construct the tri-clothoid.
    int r=draw_2d_clothoid_3arc_G2(p2, p3, theta0, theta1,  kappa0, kappa1, pvec);
    return r;
}

int draw_clothoids::draw_inbetween_3d_clothoid_3arc_G2(gp_Pnt &p0, gp_Pnt &p1, gp_Pnt &p2,  gp_Pnt &p3, gp_Pnt &p4, gp_Pnt &p5, std::vector<gp_Pnt> &clotvec){

    int type0=draw_primitives::get_shape_type(p0,p1,p2);
    int type1=draw_primitives::get_shape_type(p3,p4,p5);

    // std::cout<<"shapetype0:"<<type0<<std::endl;
    // std::cout<<"shapetype1:"<<type1<<std::endl;

    if(type0==draw_primitives::point || type0==draw_primitives::circle || type0==draw_primitives::unknown){
        return 0;
    }
    if(type1==draw_primitives::point || type1==draw_primitives::circle || type1==draw_primitives::unknown){
        return 0;
    }

    gp_Pln plane3d;
    if(!draw_primitives::create_3d_plane(p0,p2,p4,plane3d)){
        clotvec={p2,p3};
        return 1;
    }
    gp_Pln plane2d=draw_primitives::align_plane_to_origin(plane3d);

    draw_primitives::port_point_between_planes(plane3d,plane2d,p0);
    draw_primitives::port_point_between_planes(plane3d,plane2d,p1);
    draw_primitives::port_point_between_planes(plane3d,plane2d,p2);
    draw_primitives::port_point_between_planes(plane3d,plane2d,p3);
    draw_primitives::port_point_between_planes(plane3d,plane2d,p4);
    draw_primitives::port_point_between_planes(plane3d,plane2d,p5);

    int r=draw_inbetween_2d_clothoid_3arc_G2(p0,p1,p2,p3,p4,p5,clotvec);
    draw_primitives::port_points_between_planes(plane2d,plane3d,clotvec);

    return r;
}

































