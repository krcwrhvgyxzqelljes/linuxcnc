#include <iostream>

#include <ruckig/ruckig.hpp>


using namespace ruckig;

int main() {
    // Create instances: the ruckig otg as well as input and output parameters
    Ruckig<1> otg {0.001};
    InputParameter<1> input;
    OutputParameter<1> output;

    // Set input parameters
    input.current_position = {0.0};
    input.current_velocity = {0.0};
    input.current_acceleration = {0.0};

    input.target_position = {200.0};
    input.target_velocity = {10.0};
    input.target_acceleration = {0.0};

    input.max_velocity = {50.0};
    input.max_acceleration = {500.0};
    input.max_jerk = {1100.0};

    // Generate the trajectory within the control loop
    std::cout << "t | p1 | p2 | p3" << std::endl;
    bool on_stop_trajectory {false};
    while (otg.update(input, output) == Result::Working) {

        std::cout << "time:" << output.time << " pos:" << output.new_position[0]<< " vel:" << output.new_velocity[0] << " acc:" << output.new_acceleration[0]<<std::endl;

        // Activate stop trajectory after 1s
//        if (output.time >= 1.0 && !on_stop_trajectory) {
//            std::cout << "Stop immediately." << std::endl;
//            on_stop_trajectory = true;

//            // Synchronization is disabled so that each DoF stops as fast as possible independently
//            input.control_interface = ControlInterface::Velocity;
//            input.synchronization = Synchronization::None;
//            input.target_velocity = {0.0, 0.0, 0.0};
//            input.target_acceleration = {0.0, 0.0, 0.0};
//            input.max_jerk = {12.0, 10.0, 8.0};
//        }

        output.pass_to_input(input);
    }

    std::cout << "Stop trajectory duration: " << output.trajectory.get_duration() << " [s]." << std::endl;
}
