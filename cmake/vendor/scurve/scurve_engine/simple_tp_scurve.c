/********************************************************************
* Description: simple_tp.c
*   A simple single axis trajectory planner.  See simple_tp.h for API.
*
* Authors: Skynet Cyberdyne alias Grotius
* License: GPL Version 2
* Created on:
* System: Linux
*
* Copyright (c) 2004 All rights reserved.
********************************************************************/

#include "simple_tp.h"
#include "rtapi_math.h"
#include "stdio.h"
#include <time.h>
#include "scurve_struct.h"  // ** Note, a copy from this file is in /emc/motion.
#include "scurve_reset.h"   // ** Note, a copy from this file is in /emc/motion.

// C style functions to use in c and c++.
extern struct scurve_data set_init_values_c(double jermax,
                                            double accmax,
                                            double maxvel,
                                            double cyctim,
                                            struct scurve_data data);

// STEP 1.
// Check jog conditions for enable, direction, finished.
extern struct scurve_data jog_velocity_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos);
extern struct scurve_data jog_velocity_enum_c(struct scurve_data data, int enable, double endvel, double endacc, enum jog_direction dir);
extern struct scurve_data jog_position_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos, int jog_fwd, int jog_rev);
extern struct scurve_data jog_position_enum_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos, enum jog_direction dir);
extern void jog_results_c(struct scurve_data data, double *velocity, double *acceleration, double *position, int *finished);

// STEP 2.
// Construct a curve respecting the given tarpos. STEP 2.
extern struct scurve_data jog_update_c(struct scurve_data data);

struct scurve_data axis_update(struct scurve_data data){
    if(data.jog.mode==POSITION_SC){
        data=jog_position_enum_c(data,data.jog.enable,data.endvel,data.endacc,data.tarpos,data.jog.dir);
    }
    if(data.jog.mode==VELOCITY_SC){
        data=jog_velocity_enum_c(data,data.jog.enable,data.endvel,data.endacc,data.jog.dir);
    }
    data=jog_update_c(data);
    return data;
}

// In terminal : "xset r on", to enable keyboard cursor repeat.
int print_time=0;
struct timespec start_time, end_time;

//! For every joint this function is called.
void simple_tp_update(simple_tp_t *tp, double period)
{
    tp->max_jerk=tp->max_acc*2; // Just set a reasonable jerk value.

    if(print_time){
        // Record start time
        clock_gettime(CLOCK_MONOTONIC, &start_time);
    }

    tp->active=0;                   // Reset flag.

    if(tp->enable){                 // Jog buttons pressed.
        tp->data.jog.enable=1;      // Enable scurve motion.
        tp->active=1;               // Info for lcnc.
    }
    if(!tp->enable){                // Jog button released, but could still be moving to zero velocity.
        tp->data.jog.enable=0;      // Set scurve disabled, so it will stop the current move to zero velocity.
        tp->pos_cmd = tp->curr_pos; // Set command to present position to avoid movement when next enabled. see simple_tp.c for this comment.

        if(tp->curr_vel==0){        // No jog button pressed, and motion has stopped.
            tp->data.guipos=tp->pos_cmd; // When program stop is pressed in auto mode, avoid jumping to last know jog position.
        }
    }

    tp->data.jog.mode=POSITION_SC;      // We use position control for this.
    tp->data=set_init_values_c(tp->max_jerk,tp->max_acc,tp->max_vel,period,tp->data); // Set scurve values for maxvel, maxjerk, maxacc.

    if(tp->pos_cmd>tp->curr_pos){
        tp->data.jog.dir=FORWARD_SC;    // Set scurve jog direction.
        tp->data.tarpos=tp->pos_cmd;    // Set scurve tarpos.
    }
    if(tp->pos_cmd<tp->curr_pos){
        tp->data.jog.dir=REVERSE_SC;
        tp->data.tarpos=tp->pos_cmd;
    }

    tp->data=axis_update(tp->data);     // Update scurve.

    if(tp->data.guivel==0){             // Info for lcnc.
        tp->active=0;
    }

    tp->curr_vel=tp->data.guivel;       // Update lcnc values.
    tp->curr_acc=tp->data.guiacc;
    tp->curr_pos=tp->data.guipos;

    if(print_time){
        // Record end time
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        // Calculate duration in nanoseconds
        long long duration_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL +
                (end_time.tv_nsec - start_time.tv_nsec);

        // Print the duration
        // printf("Function duration: %lld nanoseconds\n", duration_ns);
    }

    // printf("curpos: %f \n",tp->curr_pos);
    // printf("poscmd: %f \n",tp->pos_cmd);
    // printf("finish: %d \n",tp->data.finish);
    // printf("enable: %i \n",tp->enable);
}

