#include "tp_arcs.h"

tp_arcs::tp_arcs()
{

}

// Function to calculate the vector between two points
sc_pnt tp_arcs::vectorBetween(const sc_pnt& p1, const sc_pnt& p2) {
    return { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
}

// Function to calculate the dot product of two vectors
double tp_arcs::dotProduct(const sc_pnt& v1, const sc_pnt& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Function to calculate the magnitude of a vector
double tp_arcs::magnitude(const sc_pnt& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Function to calculate the central angle in radians given start, waypoint, and end points
double tp_arcs::calculateArcAngleRadians(const sc_pnt& p0, const sc_pnt& p1, const sc_pnt& p2) {
    // Calculate vectors
    sc_pnt vec1 = vectorBetween(p0, p1);
    sc_pnt vec2 = vectorBetween(p2, p1);

    // Calculate the dot product of the vectors
    double dot = dotProduct(vec1, vec2);

    // Calculate the magnitudes of the vectors
    double mag1 = magnitude(vec1);
    double mag2 = magnitude(vec2);

    // Calculate the cosine of the angle
    double cosTheta = dot / (mag1 * mag2);

    // Ensure the cosine value is within the valid range [-1, 1] to avoid domain errors in acos
    if (cosTheta > 1.0) cosTheta = 1.0;
    if (cosTheta < -1.0) cosTheta = -1.0;

    // Calculate the angle in radians
    double theta = std::acos(cosTheta);

    return theta;
}

void tp_arcs::sc_interpolate_arc(sc_pnt p0_,        //! Startpoint.
                                 sc_pnt p1_,        //! Waypoint.
                                 sc_pnt p2_,        //! Endpoint.
                                 sc_pnt p3_,        //! Centerpoint.
                                 double progress,
                                 sc_pnt &pi){


    //! Is it a circle?
    if(p0_.x==p2_.x && p0_.y==p2_.y && p0_.z==p2_.z){
        // printf("interpolating arc as 2d circle. \n");

        sc_pnt p33_=p3_;
        p33_.z+=1;
        pi=sc_rotate_point_around_line(p0_,2*M_PI*progress,p3_,p33_);
        return;
    }

    Eigen::Vector3d p1(p0_.x,p0_.y,p0_.z);
    Eigen::Vector3d p2(p1_.x,p1_.y,p1_.z);
    Eigen::Vector3d p3(p2_.x,p2_.y,p2_.z);

    sc_arc arc;

    Eigen::Vector3d v1 = p2-p1;
    Eigen::Vector3d v2 = p3-p1;
    double v1v1, v2v2, v1v2;
    v1v1 = v1.dot(v1);
    v2v2 = v2.dot(v2);
    v1v2 = v1.dot(v2);

    double base = 0.5/(v1v1*v2v2-v1v2*v1v2);
    double k1 = base*v2v2*(v1v1-v1v2);
    double k2 = base*v1v1*(v2v2-v1v2);

    //! Center of arc.
    Eigen::Vector3d pc = p1 + v1*k1 + v2*k2;
    arc.center={pc.x(),pc.y(),pc.z()};
    //! std::cout<<"arc center x:"<<pc.x()<<" y:"<<pc.y()<<" z:"<<pc.z()<<std::endl;

    double radius = (pc-p1).norm();
    arc.radius=radius;
    //! std::cout<<"radius: "<<radius<<std::endl;
    arc.diameter=radius*2;

    //! Arc angle.
    Eigen::Vector3d va=(p1-pc).normalized();
    Eigen::Vector3d vb=(p3-pc).normalized();
    //! std::cout<<"va x:"<<va.x()<<" y:"<<va.y()<<" z:"<<va.z()<<std::endl;
    //! std::cout<<"vb x:"<<vb.x()<<" y:"<<vb.y()<<" z:"<<vb.z()<<std::endl;

    //! Arc direction, in arc plane between p1,p3 or v1,v2, doesn't really matter.
    Eigen::Vector3d n=v1.cross(v2);
    double nl=n.norm();
    //! Axis to arc's origin.
    Eigen::Vector3d axis=n/sqrt(nl);
    //! std::cout<<"axis trough arc origin x:"<<axis.x()<<" y:"<<axis.y()<<" z:"<<axis.z()<<" l:"<<nl<<std::endl;

    //! Axis to arc's origin.
    Eigen::Vector3d an=axis.normalized();
    //! std::cout<<"axis trough arc origin normalized x:"<<an.x()<<" y:"<<an.y()<<" z:"<<an.z()<<std::endl;

    //! This can be a negative angle if angle > 180 degrrees. Solution is below.
    double angle=acos(va.dot(vb));

    //! https://stackoverflow.com/questions/5188561/signed-angle-between-two-3d-vectors-with-same-origin-within-the-same-plane
    //! Without checking if dot<0, angles > 180 degrees will fail.
    //!
    //!     Determine the sign of the angle
    //!     Find vector V3 = cross product of Va, Vb. (the order is important)
    //!     If (dot product of V3, Vn) is negative, theta is negative. Otherwise, theta is positive.
    //!
    Eigen::Vector3d vab=va.cross(vb);
    double dot=vab.dot(an);
    //! std::cout<<"sign of the angle <0 or >0:"<<dot<<std::endl;
    arc.arcAngleNegative=false; //! Reset flag.
    if(dot<0){
        double diff=M_PI-angle;
        angle=M_PI+diff;
        arc.arcAngleNegative=true; //! Set flag so user can see there is something going on.
    }

    arc.arcAngleRad=angle;
    //! std::cout<<"arc angle in radians:"<<angle<<std::endl;
    //! std::cout<<"arc angle in degrees:"<<angle*toDegrees<<std::endl;

    //! Arc, circle circumfence pi*diameter.
    arc.arcCircumFence=(M_PI*(2*arc.radius));

    //! Arc lenght.
    arc.arcLenght=(arc.arcAngleRad/(2*M_PI))*arc.arcCircumFence;

    //! Point on arc center line. (Arc center + Axis vector)
    arc.pointOnArcAxis={pc.x()+an.x(),pc.y()+an.y(),pc.z()+an.z()};

    //!         Point to rotate.             Arc center             Point on arc center line. (Arc center + Axis vector)
    pi=sc_rotate_point_around_line({p1.x(),p1.y(),p1.z()},progress*angle,{pc.x(),pc.y(),pc.z()},{pc.x()+an.x(),pc.y()+an.y(),pc.z()+an.z()});
}

extern "C" void interpolate_arc_c(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2, struct sc_pnt p3, double progress, struct sc_pnt *pi){
    sc_pnt p;
    tp_arcs().sc_interpolate_arc(p0,p1,p2,p3,progress,p);
    *pi=p;
}

void tp_arcs::sc_arc_radius(sc_pnt p0,
                            sc_pnt p1,
                            sc_pnt p2,
                            double &radius){

    Eigen::Vector3d pa,pb,pc;
    pa.x()=p0.x;
    pa.y()=p0.y;
    pa.z()=p0.z;

    pb.x()=p1.x;
    pb.y()=p1.y;
    pb.z()=p1.z;

    pc.x()=p2.x;
    pc.y()=p2.y;
    pc.z()=p2.z;

    sc_arc arc;

    Eigen::Vector3d v1 = pb-pa;
    Eigen::Vector3d v2 = pc-pa;
    double v1v1, v2v2, v1v2;
    v1v1 = v1.dot(v1);
    v2v2 = v2.dot(v2);
    v1v2 = v1.dot(v2);

    double base = 0.5/(v1v1*v2v2-v1v2*v1v2);
    double k1 = base*v2v2*(v1v1-v1v2);
    double k2 = base*v1v1*(v2v2-v1v2);

    //! Center of arc.
    Eigen::Vector3d pcenter = pa + v1*k1 + v2*k2;
    arc.center={pcenter.x(),pcenter.y(),pcenter.z()};
    //! std::cout<<"arc center x:"<<pc.x()<<" y:"<<pc.y()<<" z:"<<pc.z()<<std::endl;

    radius = (pcenter-pa).norm();
}

double tp_arcs::sc_arc_lenght(sc_pnt p0,    //! Start
                              sc_pnt p1,    //! Waypoint
                              sc_pnt p2,    //! Endpoint
                              sc_pnt p3){   //! Center

    //! Is it a circle?
    if(p0.x==p2.x && p0.y==p2.y && p0.z==p2.z){
        // printf("processing arc lenght for circle. \n");
        double radius=sqrt(pow(p3.x-p0.x,2)+pow(p3.y-p0.y,2)+pow(p3.z-p0.z,2));
        // printf("radius: %f \n",radius);

        double circumfece=(M_PI*2)*radius;
        return circumfece;
    }

    Eigen::Vector3d pa,pb,pc;
    pa.x()=p0.x;
    pa.y()=p0.y;
    pa.z()=p0.z;

    pb.x()=p1.x;
    pb.y()=p1.y;
    pb.z()=p1.z;

    pc.x()=p2.x;
    pc.y()=p2.y;
    pc.z()=p2.z;

    sc_arc arc;

    Eigen::Vector3d v1 = pb-pa;
    Eigen::Vector3d v2 = pc-pa;
    double v1v1, v2v2, v1v2;
    v1v1 = v1.dot(v1);
    v2v2 = v2.dot(v2);
    v1v2 = v1.dot(v2);

    double base = 0.5/(v1v1*v2v2-v1v2*v1v2);
    double k1 = base*v2v2*(v1v1-v1v2);
    double k2 = base*v1v1*(v2v2-v1v2);

    //! Center of arc.
    Eigen::Vector3d pcenter = pa + v1*k1 + v2*k2;
    arc.center={pcenter.x(),pcenter.y(),pcenter.z()};
    // std::cout<<"arc center x:"<<pc.x()<<" y:"<<pc.y()<<" z:"<<pc.z()<<std::endl;

    double radius = (pcenter-pa).norm();
    arc.radius=radius;
    // std::cout<<"radius: "<<radius<<std::endl;
    arc.diameter=radius*2;

    //! Arc angle.
    Eigen::Vector3d va=(pa-pcenter).normalized();
    Eigen::Vector3d vb=(pc-pcenter).normalized();
    //! std::cout<<"va x:"<<va.x()<<" y:"<<va.y()<<" z:"<<va.z()<<std::endl;
    //! std::cout<<"vb x:"<<vb.x()<<" y:"<<vb.y()<<" z:"<<vb.z()<<std::endl;

    //! Arc direction, in arc plane between p1,p3 or v1,v2, doesn't really matter.
    Eigen::Vector3d n=v1.cross(v2);
    double nl=n.norm();
    //! Axis to arc's origin.
    Eigen::Vector3d axis=n/sqrt(nl);
    //! std::cout<<"axis trough arc origin x:"<<axis.x()<<" y:"<<axis.y()<<" z:"<<axis.z()<<" l:"<<nl<<std::endl;

    //! Axis to arc's origin.
    Eigen::Vector3d an=axis.normalized();
    //! std::cout<<"axis trough arc origin normalized x:"<<an.x()<<" y:"<<an.y()<<" z:"<<an.z()<<std::endl;

    //! This can be a negative angle if angle > 180 degrrees. Solution is below.
    double angle=acos(va.dot(vb));

    //! https://stackoverflow.com/questions/5188561/signed-angle-between-two-3d-vectors-with-same-origin-within-the-same-plane
    //! Without checking if dot<0, angles > 180 degrees will fail.
    //!
    //!     Determine the sign of the angle
    //!     Find vector V3 = cross product of Va, Vb. (the order is important)
    //!     If (dot product of V3, Vn) is negative, theta is negative. Otherwise, theta is positive.
    //!
    Eigen::Vector3d vab=va.cross(vb);
    double dot=vab.dot(an);
    //! std::cout<<"sign of the angle <0 or >0:"<<dot<<std::endl;
    arc.arcAngleNegative=false; //! Reset flag.
    if(dot<0){
        double diff=M_PI-angle;
        angle=M_PI+diff;
        arc.arcAngleNegative=true; //! Set flag so user can see there is something going on.
    }

    arc.arcAngleRad=angle;
    //! std::cout<<"arc angle in radians:"<<angle<<std::endl;
    //! std::cout<<"arc angle in degrees:"<<angle*toDegrees<<std::endl;

    //! Arc, circle circumfence pi*diameter.
    arc.arcCircumFence=(M_PI*(2*arc.radius));

    //! Arc lenght.
    arc.arcLenght=(arc.arcAngleRad/(2*M_PI))*arc.arcCircumFence;

    //! std::cout<<"arc lenght:"<<arc.arcLenght<<std::endl;
    return arc.arcLenght;
}

extern "C" double arc_lenght_c(struct sc_pnt start, struct sc_pnt way, struct sc_pnt end, struct sc_pnt center){
    double l=tp_arcs().sc_arc_lenght(start,way,end,center);
    if(isnanf(l)){
        return 0;
    }
    return l;
}

//! http://paulbourke.net/geometry/rotate/
//!
//!     Rotate a point p by angle theta around an arbitrary line segment p1-p2
//!     Return the rotated point.
//!     Positive angles are anticlockwise looking down the axis
//!     towards the origin.
//!     Assume right hand coordinate system.
//!
sc_pnt tp_arcs::sc_rotate_point_around_line(sc_pnt thePointToRotate, double theta,sc_pnt theLineP1,sc_pnt theLineP2)
{
    sc_pnt q = {0.0,0.0,0.0};
    double costheta,sintheta;
    sc_pnt r;

    r.x = theLineP2.x - theLineP1.x;
    r.y = theLineP2.y - theLineP1.y;
    r.z = theLineP2.z - theLineP1.z;
    thePointToRotate.x -= theLineP1.x;
    thePointToRotate.y -= theLineP1.y;
    thePointToRotate.z -= theLineP1.z;

    //! Normalise(&r);
    Eigen::Vector3d v(r.x,r.y,r.z);
    v.norm();
    r.x=v.x();
    r.y=v.y();
    r.z=v.z();

    costheta = cos(theta);
    sintheta = sin(theta);

    q.x += (costheta + (1 - costheta) * r.x * r.x) * thePointToRotate.x;
    q.x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * thePointToRotate.y;
    q.x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * thePointToRotate.z;

    q.y += ((1 - costheta) * r.x * r.y + r.z * sintheta) * thePointToRotate.x;
    q.y += (costheta + (1 - costheta) * r.y * r.y) * thePointToRotate.y;
    q.y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * thePointToRotate.z;

    q.z += ((1 - costheta) * r.x * r.z - r.y * sintheta) * thePointToRotate.x;
    q.z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * thePointToRotate.y;
    q.z += (costheta + (1 - costheta) * r.z * r.z) * thePointToRotate.z;

    q.x += theLineP1.x;
    q.y += theLineP1.y;
    q.z += theLineP1.z;

    return(q);
}

//! Calculate 3d arc waypoints, given 3 arc circumfence points.
//! https://stackoverflow.com/questions/13977354/build-circle-from-3-points-in-3d-space-implementation-in-c-or-c
tp_arcs::sc_arc tp_arcs::sc_arc_points(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3, double division){

    sc_arc arc;

    Eigen::Vector3d v1 = p2-p1;
    Eigen::Vector3d v2 = p3-p1;
    double v1v1, v2v2, v1v2;
    v1v1 = v1.dot(v1);
    v2v2 = v2.dot(v2);
    v1v2 = v1.dot(v2);

    double base = 0.5/(v1v1*v2v2-v1v2*v1v2);
    double k1 = base*v2v2*(v1v1-v1v2);
    double k2 = base*v1v1*(v2v2-v1v2);

    //! Center of arc.
    Eigen::Vector3d pc = p1 + v1*k1 + v2*k2;
    arc.center={pc.x(),pc.y(),pc.z()};
    //! std::cout<<"arc center x:"<<pc.x()<<" y:"<<pc.y()<<" z:"<<pc.z()<<std::endl;

    double radius = (pc-p1).norm();
    arc.radius=radius;
    //! std::cout<<"radius: "<<radius<<std::endl;
    arc.diameter=radius*2;

    //! Arc angle.
    Eigen::Vector3d va=(p1-pc).normalized();
    Eigen::Vector3d vb=(p3-pc).normalized();
    //! std::cout<<"va x:"<<va.x()<<" y:"<<va.y()<<" z:"<<va.z()<<std::endl;
    //! std::cout<<"vb x:"<<vb.x()<<" y:"<<vb.y()<<" z:"<<vb.z()<<std::endl;

    //! Arc direction, in arc plane between p1,p3 or v1,v2, doesn't really matter.
    Eigen::Vector3d n=v1.cross(v2);
    double nl=n.norm();
    //! Axis to arc's origin.
    Eigen::Vector3d axis=n/sqrt(nl);
    //! std::cout<<"axis trough arc origin x:"<<axis.x()<<" y:"<<axis.y()<<" z:"<<axis.z()<<" l:"<<nl<<std::endl;

    //! Axis to arc's origin.
    Eigen::Vector3d an=axis.normalized();
    //! std::cout<<"axis trough arc origin normalized x:"<<an.x()<<" y:"<<an.y()<<" z:"<<an.z()<<std::endl;

    //! This can be a negative angle if angle > 180 degrrees. Solution is below.
    double angle=acos(va.dot(vb));

    //! https://stackoverflow.com/questions/5188561/signed-angle-between-two-3d-vectors-with-same-origin-within-the-same-plane
    //! Without checking if dot<0, angles > 180 degrees will fail.
    //!
    //!     Determine the sign of the angle
    //!     Find vector V3 = cross product of Va, Vb. (the order is important)
    //!     If (dot product of V3, Vn) is negative, theta is negative. Otherwise, theta is positive.
    //!
    Eigen::Vector3d vab=va.cross(vb);
    double dot=vab.dot(an);
    //! std::cout<<"sign of the angle <0 or >0:"<<dot<<std::endl;
    arc.arcAngleNegative=false; //! Reset flag.
    if(dot<0){
        double diff=M_PI-angle;
        angle=M_PI+diff;
        arc.arcAngleNegative=true; //! Set flag so user can see there is something going on.
    }

    arc.arcAngleRad=angle;
    //! std::cout<<"arc angle in radians:"<<angle<<std::endl;
    //! std::cout<<"arc angle in degrees:"<<angle*toDegrees<<std::endl;

    //! Arc, circle circumfence pi*diameter.
    arc.arcCircumFence=(M_PI*(2*arc.radius));

    //! Arc lenght.
    arc.arcLenght=(arc.arcAngleRad/(2*M_PI))*arc.arcCircumFence;

    //! Point on arc center line. (Arc center + Axis vector)
    arc.pointOnArcAxis={pc.x()+an.x(),pc.y()+an.y(),pc.z()+an.z()};

    std::vector<sc_pnt> pvec;
    double step=angle/division;
    for(double i=0; i<angle; i+=step){
        //!         Point to rotate.             Arc center             Point on arc center line. (Arc center + Axis vector)
        sc_pnt res=sc_rotate_point_around_line({p1.x(),p1.y(),p1.z()},i,{pc.x(),pc.y(),pc.z()},{pc.x()+an.x(),pc.y()+an.y(),pc.z()+an.z()});
        //! std::cout<<"res x:"<<res.x<<" y:"<<res.y<<" z:"<<res.z<<std::endl;
        arc.pntVec.push_back({res.x,res.y,res.z});
    }
    //! Last point.
    arc.pntVec.push_back({p3.x(),p3.y(),p3.z()});

    return arc;
}

void tp_arcs::sc_arc_get_mid_waypoint(sc_pnt p0, //! Start.
                                      sc_pnt p1, //! Center.
                                      sc_pnt p2, //! End.
                                      sc_pnt &pi){

    //! Is it a circle?
    if(p0.x==p2.x && p0.y==p2.y && p0.z==p2.z){
        printf("no arc but circle, processed as 2d circle. \n");

        sc_pnt p11=p1;
        p11.z+=100;
        pi=sc_rotate_point_around_line(p0,M_PI,p1,p11);
        return;
    }

    sc_arc arc;
    arc.center={p1.x ,p1.y ,p1.z};

    //! Arc start to eigen vector 3d.
    Eigen::Vector3d vp0;
    vp0={p0.x,p0.y,p0.z};

    //! Arc start center to eigen vector 3d.
    Eigen::Vector3d vp1;
    vp1={p1.x, p1.y, p1.z};

    //! Arc end center to eigen vector 3d.
    Eigen::Vector3d vp2;
    vp2={p2.x, p2.y, p2.z};

    double radius = (vp1-vp0).norm();
    arc.radius=radius;
    //! std::cout<<"radius: "<<radius<<std::endl;
    arc.diameter=radius*2;

    //! Arc angle.
    Eigen::Vector3d va=(vp0-vp1).normalized();
    Eigen::Vector3d vb=(vp2-vp1).normalized();
    //! std::cout<<"va x:"<<va.x()<<" y:"<<va.y()<<" z:"<<va.z()<<std::endl;
    //! std::cout<<"vb x:"<<vb.x()<<" y:"<<vb.y()<<" z:"<<vb.z()<<std::endl;

    //! Arc direction, in arc plane between p1,p3 or v1,v2, doesn't really matter.
    Eigen::Vector3d n=va.cross(vb);
    double nl=n.norm();

    //! When the arc start,center,end are colinear, we assume the plane is xy. The normal is then in z.
    if(nl==0){
        n.x()=0;
        n.y()=0;
        n.z()=-10; //! Gives clockwise output.
        nl=n.norm();
        //! std::cout<<"arc waypoint created, assumming plane is xy, output is clockwise g2."<<std::endl;
    }

    //! Axis to arc's origin.
    Eigen::Vector3d axis=n/sqrt(nl);
    //! std::cout<<"axis trough arc origin x:"<<axis.x()<<" y:"<<axis.y()<<" z:"<<axis.z()<<" l:"<<nl<<std::endl;

    //! Axis to arc's origin.
    Eigen::Vector3d an=axis.normalized();
    //! std::cout<<"axis trough arc origin normalized x:"<<an.x()<<" y:"<<an.y()<<" z:"<<an.z()<<std::endl;

    //! This can be a negative angle if angle > 180 degrrees. Solution is below.
    double angle=acos(va.dot(vb));

    //! https://stackoverflow.com/questions/5188561/signed-angle-between-two-3d-vectors-with-same-origin-within-the-same-plane
    //! Without checking if dot<0, angles > 180 degrees will fail.
    //!
    //!     Determine the sign of the angle
    //!     Find vector V3 = cross product of Va, Vb. (the order is important)
    //!     If (dot product of V3, Vn) is negative, theta is negative. Otherwise, theta is positive.
    //!
    Eigen::Vector3d vab=va.cross(vb);
    double dot=vab.dot(an);
    //! std::cout<<"sign of the angle <0 or >0:"<<dot<<std::endl;

    arc.arcAngleNegative=false; //! Reset flag.
    if(dot<0){
        double diff=M_PI-angle;
        angle=M_PI+diff;
        arc.arcAngleNegative=true; //! Set flag so user can see there is something going on.
    }

    arc.arcAngleRad=angle;
    //! std::cout<<"arc angle in radians:"<<angle<<std::endl;
    //! std::cout<<"arc angle in degrees:"<<angle*to_degrees<<std::endl;

    //! Arc, circle circumfence pi*diameter.
    arc.arcCircumFence=(M_PI*(2*arc.radius));

    //! Arc lenght.
    arc.arcLenght=(arc.arcAngleRad/(2*M_PI))*arc.arcCircumFence;

    //! Point on arc center line. (Arc center + Axis vector)
    arc.pointOnArcAxis={vp1.x()+an.x(),vp1.y()+an.y(),vp1.z()+an.z()};


    double i=angle/2;

    //!         Point to rotate.             Arc center             Point on arc center line. (Arc center + Axis vector)
    pi=sc_rotate_point_around_line(p0,i,p1,{vp1.x()+an.x(),vp1.y()+an.y(),vp1.z()+an.z()});

}

extern "C" void sc_arc_get_mid_waypoint_c(sc_pnt start, sc_pnt center, sc_pnt end, sc_pnt *waypoint){
    sc_pnt pi;
    tp_arcs().sc_arc_get_mid_waypoint(start,center,end,pi);
    *waypoint=pi;
}

//! Arc way may be start or end point.
//! Returns arc radius from calculated line lenght.
extern "C" double arc_radius( struct sc_pnt arc_way, struct sc_pnt arc_center){

    return sqrt(pow(arc_way.x-arc_center.x,2)+pow(arc_way.y-arc_center.y,2)+pow(arc_way.z-arc_center.z,2));
}














