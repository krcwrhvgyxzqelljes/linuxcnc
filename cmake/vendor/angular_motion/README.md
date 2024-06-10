# Angular Motion Calculator

The `angular_motion` class is a C++ implementation designed to calculate angular motion parameters based on linear and arc velocities. This README provides a comprehensive overview of the class, its usage, and highlights its significance in determining maximum arc velocities for path planning.

## Usage

1. Include the `angular_motion.h` header file in your C++ project.
2. Create an instance of the `angular_motion` class.
3. Set the required parameters using the `set_values` function.
4. Call the `calculateLinearArcSpeed` function to determine the linear speed on the arc based on a specified radius.
5. Optionally, perform a unit test using the `perform_unit_test` function.

## Path Planning Utility

The `angular_motion` class is especially valuable for determining maximum arc velocities during path planning. 
By configuring the maximum linear velocity (v_max), arc velocity (v_arc), and arc radius (r_arc), the class facilitates the optimization of arc velocities.

## Calculating Angular Speed

The angular speed (omega) is determined by the formula:

omega = arc velocity / arc radius

Where:
- omega is the angular speed in radians per second.
- arc velocity is the linear velocity along the arc.
- arc radius is the radius of the circular path.

## Example Calculation

For instance, if the arc velocity is 1400 mm/min and the arc radius is 5.0 mm, the angular speed (omega) would be calculated as:

omega = 1400 mm/min / 5.0 mm

Then the omega is used as a constant for other arc's or circle's to calculate optimal speeds.

## Example Use Case

Consider a scenario where a CNC machine needs to follow a curved path. Leveraging the `angular_motion` class allows for the determination of the maximum permissible arc velocity for different radius. This ensures that the machine adheres to specified acceleration constraints, leading to precise motion control.

## Debug Mode

```cpp
    angular_motion *am=new angular_motion();
    am->set_debug(true);
```

## Example Usage

```cpp
#include "angular_motion.h"

int main() {
    angular_motion motion_calculator;
    motion_calculator.set_debug(true);

    double v_max = 3000.0;  // Maximum linear velocity in mm/min
    double v_arc = 1400.0;  // Arc velocity in mm/min
    double r_arc = 5.0;     // Arc radius in mm

    motion_calculator.set_values(v_max, v_arc, r_arc);

    double radius = 5.0;  // Replace with your specific radius
    double linear_speed_on_arc = motion_calculator.calculateLinearArcSpeed(radius);

    // Output the result
    std::cout << "Linear Speed on the Arc: " << linear_speed_on_arc << " mm/min" << std::endl;

    return 0;
}
```
## Clone & run project using cmake

```
	git clone https://github.com/grotius-cnc/angular_motion.git
	cd linear_motion
	mkdir build
	cd build
	cmake ..
	make
```

## Unit Test

```cpp
#include "angular_motion.h"

int main(int argc, char *argv[])
{
    angular_motion().perform_unit_test();
    return 0;
}
```

Input:

```cpp
#include "angular_motion.h"

int main(int argc, char *argv[])
{
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
```
Output:
```cpp
Terminal output:
Angular Speed: 4.66667 rad/s
Angular Speed: 267.38 deg/s
Linear Speed on the Arc with Radius :5 v:1400 mm/min
Linear Speed on the Arc with Radius :10 v:2800 mm/min
Linear Speed on the Arc with Radius :20 v:5600 mm/min
Linear Speed on the Arc with Radius :0.01 v:2.8 mm/min
Linear Speed on the Arc with Radius :0.1 v:28 mm/min

```
