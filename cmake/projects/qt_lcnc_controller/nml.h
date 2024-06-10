#ifndef NML_H
#define NML_H

#include <config.h>
#include <emc.hh>
#include <emc_nml.hh>
#include <string.h>
#include <iostream>

//! For info about nml usage visit : ~/linuxcnc/cmake/halui_src/halui.cc
class Nml {
public:
    Nml(){}

    struct status {
        bool estop=0;
        bool machine_on=0;
        char task_active_gcodes_string[256];
        char task_active_mcodes_string[256];
        char task_active_fcodes_string[256];
        char task_active_scodes_string[256];
        //! 0=manual 1=mdi 2=auto.
        int mode;
        //! Coordinates.
        double x=0, y=0, z=0, a=0, b=0, c=0, u=0, v=0, w=0;
        double dtgx=0, dtgy=0, dtgz=0;

        //! Feed override, jog override.
        float feed_override=0;
        float rapid_override=0;
        float spindle_override=0;
        float max_velocity=0;
        float current_velocity=0;
        float current_rpm=0;
        float adatpive_feed=0;
        //! Extra.
        int spindle_direction=0;
        bool spindle_brake=0;
        int spindle_increasing=0;
        bool spindle_enabled=0;
        bool spindle_homed=0;
        double spindle_speed=0;
        float lube_level=0;
        bool flood=0;
        bool mist=0;
        bool lube=0;
        //! Gcode.
        int current_line=0;
        int motion_line=0;

        char command[256];
        char inifile[256];
        std::string filename;

        bool homed_x=0, homed_y=0, homed_z=0, homed_all=0;
        bool run=0, pause=0, stop=0, idle=0;

        bool adaptive_feed_enabled=0;

    } theStatus;

    void update(){
        for(int i=0; i<1; i++){
            usleep(1);
            if(!stat->valid()) continue;
            if(stat->peek() != EMC_STAT_TYPE) continue;
            emcStatus = static_cast<EMC_STAT*>(stat->get_address());
        }

        if(emcStatus->task.state== EMC_TASK_STATE::ESTOP){
            theStatus.estop=true;
        } else {
            theStatus.estop=false;
        }

        if(emcStatus->task.state== EMC_TASK_STATE::ON){
            theStatus.machine_on=true;
        } else {
            theStatus.machine_on=false;
        }

        if(emcStatus->task.mode==EMC_TASK_MODE::MANUAL){
            theStatus.mode=0;
        }
        if(emcStatus->task.mode==EMC_TASK_MODE::MDI){
            theStatus.mode=1;
        }
        if(emcStatus->task.mode==EMC_TASK_MODE::AUTO){
            theStatus.mode=2;
        }

        char string[256];
        int t;
        int code;

        //! Active G codes
        theStatus.task_active_gcodes_string[0] = 0;
        for (t = 1; t < ACTIVE_G_CODES; t++) {
            code = emcStatus->task.activeGCodes[t];
            if (code == -1) {
                continue;
            }
            if (code % 10) {
                sprintf(string, "G%.1f ", (double) code / 10.0);
            } else {
                sprintf(string, "G%d ", code / 10);
            }
            strcat(theStatus.task_active_gcodes_string, string);
        }

        //! Active M codes.
        theStatus.task_active_mcodes_string[0] = 0;
        for (t = 1; t < ACTIVE_M_CODES; t++) {
            code = emcStatus->task.activeMCodes[t];
            if (code == -1) {
                continue;
            }
            sprintf(string, "M%d ", code);
            strcat(theStatus.task_active_mcodes_string, string);
        }

        //! Active F and S codes.
        sprintf(string, "F%.0f ", emcStatus->task.activeSettings[1]);
        theStatus.task_active_fcodes_string[0] = 0;
        strcat(theStatus.task_active_fcodes_string, string);
        sprintf(string, "S%.0f", abs(emcStatus->task.activeSettings[2]));
        theStatus.task_active_scodes_string[0] = 0;
        strcat(theStatus.task_active_scodes_string, string);

        theStatus.x=emcStatus->motion.traj.position.tran.x;
        theStatus.y=emcStatus->motion.traj.position.tran.y;
        theStatus.z=emcStatus->motion.traj.position.tran.z;
        theStatus.a=emcStatus->motion.traj.position.a;
        theStatus.b=emcStatus->motion.traj.position.b;
        theStatus.c=emcStatus->motion.traj.position.c;
        theStatus.u=emcStatus->motion.traj.position.u;
        theStatus.v=emcStatus->motion.traj.position.v;
        theStatus.w=emcStatus->motion.traj.position.w;

        theStatus.dtgx=emcStatus->motion.traj.dtg.tran.x;
        theStatus.dtgy=emcStatus->motion.traj.dtg.tran.y;
        theStatus.dtgz=emcStatus->motion.traj.dtg.tran.z;

        theStatus.feed_override=emcStatus->motion.traj.scale; //! Velocity override scale.
        theStatus.rapid_override=emcStatus->motion.traj.rapid_scale; //! Rapic override scale.
        theStatus.max_velocity=emcStatus->motion.traj.maxVelocity;
        theStatus.current_velocity=emcStatus->motion.traj.current_vel;
        theStatus.current_rpm=emcStatus->motion.spindle->speed;

        //! Extra
        theStatus.spindle_direction=emcStatus->motion.spindle->direction;
        theStatus.spindle_brake=emcStatus->motion.spindle->brake;
        theStatus.spindle_increasing=emcStatus->motion.spindle->increasing;
        theStatus.spindle_enabled=emcStatus->motion.spindle->enabled;
        theStatus.spindle_homed=emcStatus->motion.spindle->homed;
        theStatus.spindle_override=emcStatus->motion.spindle->spindle_scale; //! Spindle override.
        theStatus.spindle_speed=emcStatus->motion.spindle->speed;

        // theStatus.lube_level=emcStatus->io.lube.level;
        theStatus.flood=emcStatus->io.coolant.flood;
        theStatus.mist=emcStatus->io.coolant.mist;
        // theStatus.lube=emcStatus->io.lube.on;

        theStatus.current_line=emcStatus->task.currentLine;
        theStatus.motion_line=emcStatus->task.motionLine; //! current executed gcode line

        strcpy(theStatus.command,emcStatus->task.command);
        strcpy(theStatus.inifile,emcStatus->task.ini_filename);
        theStatus.filename=emcStatus->task.file;

        theStatus.homed_x=emcStatus->motion.joint[0].homed;
        theStatus.homed_y=emcStatus->motion.joint[1].homed;
        theStatus.homed_z=emcStatus->motion.joint[2].homed;



        if(theStatus.homed_x && theStatus.homed_y && theStatus.homed_z){
            theStatus.homed_all=1;
        } else {
            theStatus.homed_all=0;
        }

        theStatus.pause = emcStatus->task.interpState == EMC_TASK_INTERP::PAUSED;
        theStatus.run= emcStatus->task.interpState == EMC_TASK_INTERP::READING ||
                emcStatus->task.interpState == EMC_TASK_INTERP::WAITING;
        theStatus.idle= emcStatus->task.interpState == EMC_TASK_INTERP::IDLE;

        theStatus.adaptive_feed_enabled=emcStatus->motion.traj.adaptive_feed_enabled;

        //! Todo : theStatus.adatpive_feed
    }
    void machine_on(){
        EMC_TASK_SET_STATE s;
        s.state=EMC_TASK_STATE::ON;
        cmd->write(&s);
    }
    void machine_off(){
        EMC_TASK_SET_STATE s;
        s.state=EMC_TASK_STATE::OFF;
        cmd->write(&s);
    }
    void estop_reset(){
        EMC_TASK_SET_STATE s;
        s.state=EMC_TASK_STATE::ESTOP_RESET;
        cmd->write(&s);
    }
    void estop(){
        EMC_TASK_SET_STATE s;
        s.state=EMC_TASK_STATE::ESTOP;
        cmd->write(&s);
    }
    void mode_teleop(){
        EMC_TRAJ_SET_MODE x;
        x.mode=EMC_TRAJ_MODE::TELEOP;
        cmd->write(&x);
    }
    void teleop_enable(){
        EMC_TRAJ_SET_TELEOP_ENABLE a;
        a.enable=1;
        cmd->write(&a);
    }

    void coord(){
        EMC_TRAJ_SET_MODE x;
        x.mode=EMC_TRAJ_MODE::COORD;
        cmd->write(&x);
    }
    void free(){
        EMC_TRAJ_SET_MODE x;
        x.mode=EMC_TRAJ_MODE::FREE;
        cmd->write(&x);
    }
    void home_x(){
        EMC_JOINT_HOME m;
        m.joint=0;
        cmd->write(&m);
    }
    void home_y(){
        EMC_JOINT_HOME m;
        m.joint=1;
        cmd->write(&m);
    }
    void home_z(){
        EMC_JOINT_HOME m;
        m.joint=2;
        cmd->write(&m);
    }
    void home_all(){
        EMC_JOINT_HOME m;
        m.joint=-1;
        cmd->write(&m);
    }
    void unhome_x(){
        EMC_JOINT_UNHOME m;
        m.joint=0;
        cmd->write(&m);
    }
    void unhome_y(){
        EMC_JOINT_UNHOME m;
        m.joint=1;
        cmd->write(&m);
    }
    void unhome_z(){
        EMC_JOINT_UNHOME m;
        m.joint=2;
        cmd->write(&m);
    }
    void unhome_all(){
        EMC_JOINT_UNHOME m;
        m.joint=-1;
        cmd->write(&m);
    }
    void mode_auto(){
        EMC_TASK_SET_MODE m;
        m.mode=EMC_TASK_MODE::AUTO;
        cmd->write(&m);
    }
    void mode_mdi(){
        EMC_TASK_SET_MODE m;
        m.mode=EMC_TASK_MODE::MDI;
        cmd->write(&m);
    }
    void mode_manual(){
        EMC_TASK_SET_MODE m;
        m.mode=EMC_TASK_MODE::MANUAL;
        cmd->write(&m);
    }
    void run(){
        EMC_TASK_PLAN_RUN r;
        cmd->write(&r);
    }
    void run(int theLine){
        EMC_TASK_PLAN_RUN r;
        r.line=theLine;
        cmd->write(&r);
    }
    void mdi_execute(){
        EMC_TASK_PLAN_EXECUTE r;
        cmd->write(&r);
    }
    void run_step(){
        EMC_TASK_PLAN_STEP r;
        cmd->write(&r);
    }
    void pause(){
        EMC_TASK_PLAN_PAUSE p;
        cmd->write(&p);
    }
    void resume(){
        EMC_TASK_PLAN_RESUME r;
        cmd->write(&r);
    }
    void reverse(){
        EMC_TASK_PLAN_REVERSE r;
        cmd->write(&r);
    }
    void forward(){
        EMC_TASK_PLAN_FORWARD f;
        cmd->write(&f);
    }
    void end(){
        EMC_TASK_PLAN_END e;
        cmd->write(&e);
    }
    void stop(){
        EMC_TASK_ABORT e;
        cmd->write(&e);
    }
    void load(std::string theFile){
        EMC_TASK_PLAN_OPEN o;
        if(theFile.size()==0){
            printf("no input file.\n");
            return;
        } else {
            strcpy(o.file, theFile.c_str());
        }

        cmd->write(&o);
    }
    void close(){
         EMC_TASK_PLAN_CLOSE c;
         cmd->write(&c);
    }
    void spindle_off(int theSpindle){
        EMC_SPINDLE_OFF m;
        m.spindle=theSpindle;
        cmd->write(&m);
    }
    void spindle_on(int theSpindle, float theRpm){
        EMC_SPINDLE_ON m;
        m.spindle=theSpindle;
        m.speed=theRpm;
        cmd->write(&m);
    }
    void jog(int axis, float speed){
        EMC_JOG_CONT j;
        j.jjogmode = 0;
        j.joint_or_axis = axis;
        j.vel = speed;
        cmd->write(&j);
    }
    void jog_stop(int axis){
        EMC_JOG_STOP s;
        s.jjogmode = 0;
        s.joint_or_axis=axis;
        cmd->write(&s);
    }
    //! Factor 1.0 - ...
    void setFeedOveride(float value){
        EMC_TRAJ_SET_SCALE o;
        o.scale=value;
        cmd->write(&o);
    }
    //! MM/min.
    void setMaxVelocity(float theVelocity){
        EMC_TRAJ_SET_MAX_VELOCITY o;
        o.velocity=theVelocity;
        cmd->write(&o);
    }
    //! Factor 1.0 - ...
    void setRapidOverride(float value){
        EMC_TRAJ_SET_RAPID_SCALE o;
        o.scale=value;
        cmd->write(&o);
    }
    //! Starting at spindle 0, Factor 1.0 - ...
    void setSpindleOverride(int theSpindle, int theScale){
        EMC_TRAJ_SET_SPINDLE_SCALE s;
        s.spindle=theSpindle;
        s.scale=theScale;
        cmd->write(&s);
    }
    void mdi(std::string theCommand){
        EMC_TASK_PLAN_EXECUTE mdi;
        strcpy(mdi.command,theCommand.c_str());
        cmd->write(&mdi);
    }
    void flood_on(){
        EMC_COOLANT_FLOOD_ON s;
        cmd->write(&s);
    }
    void flood_off(){
        EMC_COOLANT_FLOOD_OFF s;
        cmd->write(&s);
    }
    void mist_on(){
        EMC_COOLANT_MIST_ON s;
        cmd->write(&s);
    }
    void mist_off(){
        EMC_COOLANT_MIST_OFF s;
        cmd->write(&s);
    }

    RCS_CMD_CHANNEL *cmd = new RCS_CMD_CHANNEL(emcFormat, "emcCommand", "xemc", EMC2_DEFAULT_NMLFILE);
    RCS_STAT_CHANNEL *stat = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", EMC2_DEFAULT_NMLFILE);
    NML *err = new NML(emcFormat, "emcError", "xemc", EMC2_DEFAULT_NMLFILE);

    // Info for error channel:
    //        EMC_OPERATOR_ERROR e;
    //        char *error=e.error;

    //        EMC_OPERATOR_TEXT t;
    //        char *text=t.text;

    //        EMC_OPERATOR_DISPLAY d;
    //        char *display=d.display;

    EMC_STAT *emcStatus;
};
#endif



















