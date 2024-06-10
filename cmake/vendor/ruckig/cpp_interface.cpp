#include "cpp_interface.h"

Cpp_interface::Cpp_interface()
{
    // This is performed every ms
}

result Cpp_interface::dofs(result input){

    //! Setup ruckig config.
    ruckig::Ruckig<1> otg {input.period};
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

    if(input.interfacetype==int(ruckig::ControlInterface::Position)){
        in.control_interface=ruckig::ControlInterface::Position;
    }
    if(input.interfacetype==int(ruckig::ControlInterface::Velocity)){
         in.control_interface=ruckig::ControlInterface::Velocity;
    }

    in.synchronization=ruckig::Synchronization::Time;

    in.enabled[i]=input.enable;

    input.function_return_code = otg.update(in,out);

    if(input.function_return_code==1){
        input.finished=true;
        input.error=false;
    } else
    if(input.function_return_code==0){
        input.finished=false;
        input.error=false;
    } else {
        input.error=true;
    }

    // One ms forward (Should it be input.period instead of 0.001?)
    out.trajectory.at_time(input.period,pos, vel, acc);

    input.curpos=pos[i];
    input.curvel=vel[i];
    input.curacc=acc[i];

    return input;
}

extern "C" result wrapper_get_pos(result input){
    result r=Cpp_interface().dofs(input);
    return r;
}

//! Input parameters as normal, returns stop lenght.
//! Input current position as 0.
//! Input end_vel, end_acc, etc as 0.
double Cpp_interface::dofs_stop_lenght(result input){

    //! Setup ruckig config.
    ruckig::Ruckig<1> otg {input.period};
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

    if(input.interfacetype==int(ruckig::ControlInterface::Position)){
        in.control_interface=ruckig::ControlInterface::Position;
    }
    if(input.interfacetype==int(ruckig::ControlInterface::Velocity)){
         in.control_interface=ruckig::ControlInterface::Velocity;
    }

    in.synchronization=ruckig::Synchronization::Time;

    in.enabled[i]=input.enable;

    input.function_return_code = otg.update(in,out);

    if(input.function_return_code==1){
        input.finished=true;
        input.error=false;
    } else
    if(input.function_return_code==0){
        input.finished=false;
        input.error=false;
    } else {
        input.error=true;
    }


    out.trajectory.at_time( out.trajectory.get_duration() ,pos, vel, acc);

    input.curpos=pos[i];
    input.curvel=vel[i];
    input.curacc=acc[i];

    return pos[0];
}

extern "C" double wrapper_stop_lenght(struct result input){
    return Cpp_interface().dofs_stop_lenght(input);
}













