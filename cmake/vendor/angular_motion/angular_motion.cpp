#include "angular_motion.h"

angular_motion::angular_motion()
{

}

void angular_motion::set_debug(bool state){
    debug=state;
}

//! Velocity max in mm/min, Arc_velocity and arc_radius are a set
//! wich are chosen to be optimal machine values to run a arc.
void angular_motion::set_values(double linear_velocity_max, double arc_velocity, double arc_radius){
    vm=mm_min_to_mm_s(linear_velocity_max);
    v_arc=mm_min_to_mm_s(arc_velocity);
    r_arc=arc_radius;

    calculateAngularSpeed();
}

// Function to calculate angular speed (in radians per second) based on linear velocity and radius
void angular_motion::calculateAngularSpeed() {
    // Check if the radius is non-zero to avoid division by zero
    if (r_arc == 0.0) {
        r_arc=0.1;
        std::cout << "arc radius set to:"<<r_arc<<std::endl;
    }

    // Calculate angular speed using the formula: angular speed = linear velocity / radius
    v_angular = v_arc / r_arc;

    if(debug){
        std::cout << "Angular Speed: " << v_angular<< " rad/s" << std::endl;
        std::cout << "Angular Speed: " << rad_to_deg( v_angular )<< " deg/s" << std::endl;
    }
}

// Function to calculate linear speed on the arc (in mm/s) based on angular speed and radius
double angular_motion::calculateLinearArcSpeed(double radius) {
    // Calculate linear speed using the formula: linear speed = angular speed * radius   ,in mm/s.
    double linearSpeed =  mm_s_to_mm_min( v_angular * radius );

    if(debug){
        std::cout << "Linear Speed on the Arc with Radius :"<<radius<<" v:" << linearSpeed << " mm/min" << std::endl;
    }
    return linearSpeed;
}

double angular_motion::mm_s_to_mm_min(double mm_s){
    return mm_s*60;
}

double angular_motion::mm_min_to_mm_s(double mm_min){
    return mm_min/60;
}

//! Function to convert radians to degrees
double angular_motion::rad_to_deg(double radians) {
    return radians * (180.0 / M_PI);
}

void angular_motion::perform_unit_test(){

    angular_motion *am=new angular_motion();
    am->set_debug(true);

    double v_max=3000;  // mm/min
    double v_arc=1400;  // mm/min
    double r_arc=5;     // mm

    am->set_values(v_max,v_arc,r_arc);

    double radius1 = 5.0;       // Radius in mm
    double linearSpeed1 = am->calculateLinearArcSpeed(radius1);

    double radius2 = 10.0;       // Radius in mm
    double linearSpeed2 = am->calculateLinearArcSpeed(radius2);

    double radius3 = 20.0;       // Radius in mm
    double linearSpeed3 = am->calculateLinearArcSpeed(radius3);

    double radius4 = 0.01;       // Radius in mm, blending G64P0.01 in LinuxCNC
    double linearSpeed4 = am->calculateLinearArcSpeed(radius4);

    double radius5 = 0.1;       // Radius in mm, blending G64P0.1 in LinuxCNC
    double linearSpeed5 = am->calculateLinearArcSpeed(radius5);
}
