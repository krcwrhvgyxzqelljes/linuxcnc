#include "tp_corners.h"

tp_corners::tp_corners( )
{

}

void tp_corners::line_line_angle(sc_pnt p0, sc_pnt p1, sc_pnt p2, double &angle_deg){

    Eigen::Vector3d p00(p0.x,p0.y,p0.z);
    Eigen::Vector3d p11(p1.x,p1.y,p1.z); //! Common point.
    Eigen::Vector3d p22(p2.x,p2.y,p2.z);

    Eigen::Vector3d v1 = p00-p11;
    Eigen::Vector3d v2 = p22-p11;

    v1.normalize();
    v2.normalize();
    double dot = v1.dot(v2);
    double angle_rad = acos(dot);
    angle_deg = angle_rad*to_degrees;
}

void tp_corners::line_arc_angle(sc_pnt p0,
                                sc_pnt p1,
                                sc_pnt p2,
                                sc_pnt p3,
                                double &angle_deg){

    sc_pnt pi,pdummy;
    tp_arcs().sc_interpolate_arc(p1,p2,p3,pdummy,0.1,pi);
    line_line_angle(p0,p1,pi,angle_deg);
}

void tp_corners::arc_line_angle(sc_pnt p0,
                                sc_pnt p1,
                                sc_pnt p2,
                                sc_pnt p3,
                                double &angle_deg){

    sc_pnt pi,pdummy;
    tp_arcs().sc_interpolate_arc(p0,p1,p2,pdummy,0.9,pi);
    line_line_angle(pi,p2,p3,angle_deg);
}

void tp_corners::arc_arc_angle(sc_pnt p0,
                               sc_pnt p1,
                               sc_pnt p2,
                               sc_pnt p3,
                               sc_pnt p4,
                               double &angle_deg){

    sc_pnt pi0, pi1, pdummy;
    tp_arcs().sc_interpolate_arc(p0,p1,p2,pdummy,0.9,pi0);
    tp_arcs().sc_interpolate_arc(p2,p3,p4,pdummy,0.1,pi1);
    line_line_angle(pi0,p2,pi1,angle_deg);
}

void tp_corners::segment_angle(tp_segment s0, tp_segment s1, double &angle_deg){

    if(s0.primitive_id==sc_primitive_id::sc_line && s1.primitive_id==sc_primitive_id::sc_line){
        line_line_angle(s0.pnt_s,
                        s0.pnt_e,
                        s1.pnt_e,
                        angle_deg);
        return;
    }

    if(s0.primitive_id==sc_primitive_id::sc_line && s1.primitive_id==sc_primitive_id::sc_arc){
        line_arc_angle(s0.pnt_s,
                       s0.pnt_e,
                       s1.pnt_w,
                       s1.pnt_e,
                       angle_deg);
        return;
    }

    if(s0.primitive_id==sc_primitive_id::sc_arc && s1.primitive_id==sc_primitive_id::sc_line){
        arc_line_angle(s0.pnt_s,
                       s0.pnt_w,
                       s0.pnt_e,
                       s1.pnt_e,angle_deg);
        return;
    }

    if(s0.primitive_id==sc_primitive_id::sc_arc && s1.primitive_id==sc_primitive_id::sc_arc){
        arc_arc_angle(s0.pnt_s,
                      s0.pnt_w,
                      s0.pnt_e,
                      s1.pnt_w,
                      s1.pnt_e,angle_deg);
        return;
    }
}


extern "C" double line_line_angle(struct sc_pnt p0,
                                  struct sc_pnt p1,
                                  struct sc_pnt p2){

    double angle=0;
    tp_corners().line_line_angle(p0,p1,p2,angle);
    return angle;
}

extern "C" double line_arc_angle(struct sc_pnt p0,
                                 struct sc_pnt p1,
                                 struct sc_pnt p2,
                                 struct sc_pnt p3){
    double angle_deg=0;
    tp_corners().line_arc_angle(p0,p1,p2,p3,angle_deg);
    return angle_deg;
}

extern "C" double arc_line_angle(struct sc_pnt p0,
                                 struct sc_pnt p1,
                                 struct sc_pnt p2,
                                 struct sc_pnt p3){

    double angle_deg=0;
    tp_corners().arc_line_angle(p0,p1,p2,p3,angle_deg);
    return angle_deg;
}

extern "C" double arc_arc_angle(struct sc_pnt p0,
                                struct sc_pnt p1,
                                struct sc_pnt p2,
                                struct sc_pnt p3,
                                struct sc_pnt p4){

    double angle_deg=0;
    tp_corners().arc_arc_angle(p0,p1,p2,p3,p4,angle_deg);
    return angle_deg;
}

extern "C" double segment_angle(struct tp_segment s0,
                                struct tp_segment s1){

    double angle_deg=0;
    tp_corners().segment_angle(s0,s1,angle_deg);
    return angle_deg;
}







