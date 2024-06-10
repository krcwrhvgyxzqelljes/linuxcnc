#ifndef LINEAR_CYCLE_H
#define LINEAR_CYCLE_H

#ifdef __cplusplus

#include <iostream>
#include <iomanip>
#include <cmath>

//! The `angular_motion` class is a powerful tool for determining maximum
//! arc velocities during path planning. By configuring the
//! maximum linear velocity (`v_max`), arc velocity (`v_arc`),
//! and arc radius (`r_arc`), the class facilitates the optimization of arc velocities,
//! ensuring efficient and controlled motion along curved paths.
class angular_motion
{
public:
    angular_motion();

    void set_debug(bool state);

    void set_values(double linear_velocity_max,
                    double arc_velocity,
                    double arc_radius);

    double calculateLinearArcSpeed(double radius);

    void perform_unit_test();

private:

    bool debug=0;
    double vm=0;
    double v_arc=0;
    double r_arc=0;
    double v_angular=0;

    void calculateAngularSpeed();

    double mm_s_to_mm_min(double mm_s);
    double mm_min_to_mm_s(double mm_min);

    double rad_to_deg(double radians);
};

//! Here it tells if this code is used in c, convert the class to a struct. This is handy!
#else
typedef struct angular_motion angular_motion;
#endif //! cplusplus

#endif // LINEAR_CYCLE_H
