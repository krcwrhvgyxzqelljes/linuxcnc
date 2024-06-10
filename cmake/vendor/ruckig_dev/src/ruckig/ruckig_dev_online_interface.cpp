#include "ruckig_dev_online_interface.h"

ruckig_dev_online_interface::ruckig_dev_online_interface()
{

}
//! Older updated jogging functions.

ruckig_c_data ruckig_dev_online_interface::dofs(ruckig_c_data input){

    //! Setup ruckig config.
    ruckig::Ruckig<1> otg {input.cycletime};
    ruckig::InputParameter<1> in;
    ruckig::OutputParameter<1> out;
    std::array<double, 1> vel, acc, pos;

    int i=0;

    in.max_velocity[i]=abs(input.maxvel);
    in.max_acceleration[i]=input.maxacc;
    in.max_jerk[i]=input.maxjerk;
    in.current_position[i]=input.curpos;
    in.current_velocity[i]=input.curvel;
    in.current_acceleration[i]=input.curacc;
    in.target_velocity[i]=input.tarvel;
    in.target_acceleration[i]=input.taracc;
    in.target_position[i]=input.tarpos;

    if(input.control_interfacetype==int(ruckig::ControlInterface::Position)){
        in.control_interface=ruckig::ControlInterface::Position;
    }
    if(input.control_interfacetype==int(ruckig::ControlInterface::Velocity)){
        in.control_interface=ruckig::ControlInterface::Velocity;
    }

    in.synchronization=ruckig::Synchronization::Time;

    in.enabled[i]=input.enable;

    input.function_return_code = otg.update(in,out);

    // One ms forward (Should it be input.period instead of 0.001?)
    out.trajectory.at_time(input.cycletime,pos, vel, acc);

    input.curpos=pos[i];
    input.curvel=vel[i];
    input.curacc=acc[i];

    return input;
}

extern "C" ruckig_c_data wrapper_get_pos(ruckig_c_data input){
    ruckig_c_data r=ruckig_dev_online_interface().dofs(input);
    return r;
}
