#include "ruckig_dev_interface.h"

ruckig_dev_interface::ruckig_dev_interface()
{

}

extern "C" ruckig_dev_interface* ruckig_init_ptr(){
    return new ruckig_dev_interface();
}

extern "C" void ruckig_add_waypoint(ruckig_dev_interface *ptr, struct ruckig_c_waypoint point){
    ptr->pointvec.push_back(point);
}

extern "C" struct ruckig_c_waypoint ruckig_get_waypoint(ruckig_dev_interface *ptr, int index){
    return ptr->pointvec.at(index);
}

extern "C" int ruckig_waypoint_vector_size(ruckig_dev_interface *ptr){
    return ptr->pointvec.size();
}

extern "C" struct ruckig_c_data ruckig_calculate_c(struct ruckig_c_data in){
    return ruckig_dev_interface().ruckig_calculate(in);
}

extern "C" struct ruckig_c_data ruckig_calculate_c_ptr(ruckig_dev_interface *ruckig_ptr, struct ruckig_c_data in){
    return ruckig_ptr->ruckig_calculate(in);
}

//! Main ruckig trajectory function.
//! This function calculates the traject, and can handle interupts like pause etc.
ruckig_c_data ruckig_dev_interface::ruckig_calculate(ruckig_c_data in){

    in.at_time+=in.cycletime;

    //! Hard position zero reset, when you need a new trajectory.
    if(in.reset){
        in.newpos=0;
        in.newacc=0;
        // in.newvel=0;
        in.curpos=0;
        in.curacc=0;
        // in.curvel=0;
        in.initialized=0;
        in.at_time=0;
        in.reset=0;
        printf("Ruckig reset. \n");
    }

    //! Check for all kind of interupts, program stop, start is also interupt.
    if(in.maxvel!=in.oldmaxvel || in.maxacc!=in.oldmaxacc ||  in.maxjerk!=in.oldmaxjerk ||
            in.tarvel!=in.oldtarvel || in.taracc!=in.oldtaracc || in.tarpos!=in.oldtarpos ||
            in.pause!=in.oldpause || in.reverse!=in.oldreverse ){
        // printf("Ruckig interupt. \n");

        //! For new calculation, set actual positions.
        in.curpos=in.newpos;
        in.curacc=in.newacc;
        in.curvel=in.newvel;

        in.initialized=0;
    }
    //! Ruckig input data format.
    ruckig::InputParameter<1> input;
    //! Convert c++ to c struct.
    input=ruckig_c_data_to_cpp(in);

    //! Calculate new motion.
    if(!in.initialized){

        // printf("Ruckig calculate new motion. \n");

        if(in.pause){
            input.control_interface=ruckig::ControlInterface::Velocity;
            input.synchronization=ruckig::Synchronization::None;
            input.target_velocity[0]=0;
            input.target_acceleration[0]=0;
        }

        //! Calculate the trajectory in an offline manner (outside of the control loop)
        //! This is done to avoid a velocity end error when using the online trajectory.
        ruckig::Ruckig<1> otg_offline;
        ruckig::Result result = otg_offline.calculate(input,trajectory);

        //! Add code.
        in.function_return_code=result;

        in.duration=trajectory.get_duration();

        //! Update oldvel to trigger interupt next cycle.
        in.oldmaxvel=in.maxvel;
        in.oldmaxacc=in.maxacc;
        in.oldmaxjerk=in.maxjerk;
        in.oldtarvel=in.tarvel;
        in.oldtaracc=in.taracc;
        in.oldtarpos=in.tarpos;
        in.oldpause=in.pause;
        in.oldreverse=in.reverse;

        in.at_time=0;
        in.initialized=1;
    }

    //! Motion finished.
    if(in.at_time+0.001>in.duration){
        in.at_time=in.duration;
        in.function_return_code=1;

        //! Set to the given tar values.
        in.curpos=in.tarpos;
        in.curacc=in.taracc;
        in.curvel=in.tarvel;

        //! Return finished.
        return in;
    }

    //! Then, we can calculate the kinematic state at a given time
    std::array<double, 1> new_position, new_velocity, new_acceleration;
    trajectory.at_time(in.at_time, new_position, new_velocity, new_acceleration);

    //! Update out new pos, acc, vel.
    in.newpos=new_position[0];
    in.newvel=new_velocity[0];
    in.newacc=new_acceleration[0];

    return in;;
}

ruckig_c_data ruckig_dev_interface::ruckig_cpp_data_to_c(ruckig::InputParameter<1> input){

    ruckig_c_data in;

    int i=0;

    //! Is the DoF considered for calculation?
    in.enable=input.enabled[i];

    //! Position, ///< Position-control: Full control over the entire kinematic state (Default)
    //! Velocity, ///< Velocity-control: Ignores the current position, target position, and velocity limits
    if(input.control_interface==ruckig::ControlInterface::Position){
        in.control_interfacetype=position;
    } else {
        in.control_interfacetype=velocity;
    }

    //! Continuous, ///< Every trajectory synchronization duration is allowed (Default)
    //! Discrete, ///< The trajectory synchronization duration must be a multiple of the control cycle
    if(input.duration_discretization==ruckig::DurationDiscretization::Continuous){
        in.durationdiscretizationtype=Continuous;
    } else {
        in.durationdiscretizationtype=Discrete;
    }

    //! Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
    //! Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
    //! TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
    //! None, ///< Calculate every DoF independently
    if(input.synchronization==ruckig::Synchronization::None){
        in.synchronizationtype=None;
    }
    if(input.synchronization==ruckig::Synchronization::Phase){
        in.synchronizationtype=Phase;
    }
    if(input.synchronization==ruckig::Synchronization::Time){
        in.synchronizationtype=Time;
    }
    if(input.synchronization==ruckig::Synchronization::TimeIfNecessary){
        in.synchronizationtype=TimeIfNecessary;
    }

    //! Current state.
    in.curpos=input.current_position[i];
    in.curvel=input.current_velocity[i];
    in.curacc=input.current_acceleration[i];

    //! Target state
    in.tarpos=input.target_position[i];
    in.tarvel=input.target_velocity[i];
    in.taracc=input.target_acceleration[i];

    //! Kinematic constraints
    in.maxvel=input.max_velocity[i];
    in.maxacc=input.max_acceleration[i] ;
    in.maxjerk=input.max_jerk[i];

    return in;
}

ruckig::InputParameter<1> ruckig_dev_interface::ruckig_c_data_to_cpp(ruckig_c_data in){

    ruckig::InputParameter<1> input;

    int i=0;

    //! Is the DoF considered for calculation?
    input.enabled[i]=in.enable;

    //! Position, ///< Position-control: Full control over the entire kinematic state (Default)
    //! Velocity, ///< Velocity-control: Ignores the current position, target position, and velocity limits
    if(in.control_interfacetype==position){
        input.control_interface=ruckig::ControlInterface::Position;
    } else {
        input.control_interface=ruckig::ControlInterface::Velocity;
    }

    //! Continuous, ///< Every trajectory synchronization duration is allowed (Default)
    //! Discrete, ///< The trajectory synchronization duration must be a multiple of the control cycle
    if(in.durationdiscretizationtype==Continuous){
        input.duration_discretization=ruckig::DurationDiscretization::Continuous;
    } else {
        input.duration_discretization=ruckig::DurationDiscretization::Discrete;
    }

    //! Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
    //! Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
    //! TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
    //! None, ///< Calculate every DoF independently
    if(in.synchronizationtype==None){
        input.synchronization=ruckig::Synchronization::None;
    }
    if(in.synchronizationtype==Phase){
        input.synchronization=ruckig::Synchronization::Phase;
    }
    if(in.synchronizationtype==Time){
        input.synchronization=ruckig::Synchronization::Time;
    }
    if(in.synchronizationtype==TimeIfNecessary){
        input.synchronization=ruckig::Synchronization::TimeIfNecessary;
    }

    //! Current state.
    input.current_position[i] = in.curpos;
    input.current_velocity[i] = in.curvel;
    input.current_acceleration[i] = in.curacc;

    //! Target state
    input.target_position[i] = in.tarpos;
    input.target_velocity[i] = in.tarvel;
    input.target_acceleration[i] = in.taracc;

    //! Kinematic constraints
    input.max_velocity[i] = in.maxvel;
    input.max_acceleration[i] = in.maxacc;
    input.max_jerk[i] = in.maxjerk;

    return input;
}
