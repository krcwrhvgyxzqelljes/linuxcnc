#include <iostream>

#include <ruckig/ruckig.hpp>


using namespace ruckig;

int main() {
    // Create input parameters
    InputParameter<1> input;
    input.current_position = {0.0};
    input.current_velocity = {0.0};
    input.current_acceleration = {0.0};

    input.target_position = {350.0};
    input.target_velocity = {10.0};
    input.target_acceleration = {0.0};

    input.max_velocity = {50.0};
    input.max_acceleration = {50.0};
    input.max_jerk = {1100.0};

    // Set different constraints for negative direction
    input.min_velocity = {-2.0};
    input.min_acceleration = {-2.0};

    // We don't need to pass the control rate (cycle time) when using only offline features
    Ruckig<1> otg;
    Trajectory<1> trajectory;

    // Calculate the trajectory in an offline manner (outside of the control loop)
    Result result = otg.calculate(input, trajectory);
    if (result == Result::ErrorInvalidInput) {
        std::cout << "Invalid input!" << std::endl;
        return -1;
    }

    // Get duration of the trajectory
    std::cout << "Trajectory duration: " << trajectory.get_duration() << " [s]." << std::endl;

    double new_time {trajectory.get_duration() };

    // Then, we can calculate the kinematic state at a given time
    std::array<double, 1> new_position, new_velocity, new_acceleration;
    trajectory.at_time(new_time, new_position, new_velocity, new_acceleration);

    std::cout << "Position at time "  << new_time << " [s]: " << new_position[0] << " vel:" << new_velocity[0] << " acc:" << new_acceleration[0] << std::endl;

    // Get some info about the position extrema of the trajectory
    std::array<PositionExtrema, 1> position_extrema = trajectory.get_position_extrema();
    std::cout << "Position extremas for DoF 0 are " << position_extrema[0].min << " (min) to " << position_extrema[0].max << " (max)" << std::endl;
}
