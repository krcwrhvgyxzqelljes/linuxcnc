#include "rtapi.h"
#include "rtapi_ctype.h"
#include "rtapi_app.h"
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "rtapi_math64.h"
#include <rtapi_io.h>
#include "hal.h"
#include "stdio.h"

#include "emcpose.h"
#include "motion.h"
#include "tc.h"
#include "tp_scurve.h"
#include "tp_vector.h"
#include "tp_conversion.h"
#include "tp_arcs.h"
#include "tp_lines.h"
#include "tp_corners.h"

#include "ruckig_format.h"
#include "scurve_engine.h"
#include "scurve_reset.h"

/* module information */
MODULE_AUTHOR("Skynet_Cyberdyne");
MODULE_DESCRIPTION("tpmod_scurve_skynet");
MODULE_LICENSE("GPL2");

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *hal_guivel, *hal_guiacc, *hal_guipos, *hal_segment_progress;
//! Pins
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *hal_reverse_run;

typedef struct { //! Int.
    hal_s32_t *Pin;
} s32_data_t;

typedef struct { //! Param int.
    hal_s32_t Pin;
} param_s32_data_t;

typedef struct { //! Uint.
    hal_u32_t *Pin;
} u32_data_t;

typedef struct { //! Param Uint.
    hal_u32_t Pin;
} param_u32_data_t;

typedef struct {
    hal_port_t *Pin;
} port_data_t;

//! Params
typedef struct {
    hal_float_t Pin;
} param_float_data_t;

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;
param_bit_data_t *done;

static int comp_idx; /* component ID */

static void the_function();
static int setup_pins();

int rtapi_app_main(void) {

    int r = 0;
    comp_idx = hal_init("tpmod_scurve_skynet");
    if(comp_idx < 0) return comp_idx;
    r = hal_export_funct("tpmod_scurve_skynet", the_function, &skynet,0,0,comp_idx);

    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    return 0;
}


void rtapi_app_exit(void){
    hal_exit(comp_idx);
}

//! Perforn's every ms.
static void the_function(){

}

//! Setup hal pins.
static int setup_pins(){
    int r=0;

    //! Pins to be motitored by halscope.
    hal_guivel = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_skynet.hal_guivel",HAL_OUT,&(hal_guivel->Pin),comp_idx);

    hal_guiacc = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_skynet.hal_guiacc",HAL_OUT,&(hal_guiacc->Pin),comp_idx);

    hal_guipos = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_skynet.hal_guipos",HAL_OUT,&(hal_guipos->Pin),comp_idx);

    hal_reverse_run = (bit_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_skynet.hal_reverse_run",HAL_IN,&(hal_reverse_run->Pin),comp_idx);

    return r;
}

//! Status and config from motion.h
static emcmot_status_t *emcmotStatus;
static emcmot_config_t *emcmotConfig;

//==========================================================
// tp module interface
// motmod function ptrs for functions called by tp:
static void(  *_DioWrite)(int,char);
static void(  *_AioWrite)(int,double);
static void(  *_SetRotaryUnlock)(int,int);
static int (  *_GetRotaryIsUnlocked)(int);
static double(*_axis_get_vel_limit)(int);
static double(*_axis_get_acc_limit)(int);

void tpMotFunctions(void(  *pDioWrite)(int,char)
                    ,void(  *pAioWrite)(int,double)
                    ,void(  *pSetRotaryUnlock)(int,int)
                    ,int (  *pGetRotaryIsUnlocked)(int)
                    ,double(*paxis_get_vel_limit)(int)
                    ,double(*paxis_get_acc_limit)(int)
                    )
{
    _DioWrite            = *pDioWrite;
    _AioWrite            = *pAioWrite;
    _SetRotaryUnlock     = *pSetRotaryUnlock;
    _GetRotaryIsUnlocked = *pGetRotaryIsUnlocked;
    _axis_get_vel_limit  = *paxis_get_vel_limit;
    _axis_get_acc_limit  = *paxis_get_acc_limit;
}

void tpMotData(emcmot_status_t *pstatus
               ,emcmot_config_t *pconfig
               )
{
    emcmotStatus = pstatus;
    emcmotConfig = pconfig;
}

//! To use functions from tp_vector.cpp we need to declare them here:
extern struct tp_vector* vector_init_ptr();
extern int vector_size_c(struct tp_vector *ptr);
extern void vector_clear(struct tp_vector *ptr);
extern int vector_at_id(struct tp_vector *ptr, int n);
extern struct tp_segment vector_at(struct tp_vector *ptr, int index);
extern void vector_add_segment(struct tp_vector *ptr, struct tp_segment b);
extern void vector_set_end_angle(tp_vector *ptr, int index, double angle_deg);
extern double vector_traject_lenght(tp_vector *ptr);

extern double arc_lenght_c(struct sc_pnt start, struct sc_pnt way, struct sc_pnt end);
extern double line_lenght_c(struct sc_pnt start, struct sc_pnt end);
extern void interpolate_line_c(struct sc_pnt p0, struct sc_pnt p1, double progress, struct  sc_pnt *pi);
extern void interpolate_dir_c(struct sc_dir p0, struct sc_dir p1, double progress, struct sc_dir *pi);
extern void interpolate_ext_c(struct sc_ext p0, struct sc_ext p1, double progress, struct sc_ext *pi);
extern void interpolate_arc_c(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2, double progress, struct sc_pnt *pi);
extern void sc_arc_get_mid_waypoint_c(struct sc_pnt start, struct sc_pnt center, struct sc_pnt end, struct sc_pnt *waypoint);
extern void vector_interpolate_traject_c(struct tp_vector *ptr, double traject_progress, double traject_lenght, double *curve_progress, int *curve_nr);

extern double line_line_angle(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2);
extern double line_arc_angle(struct sc_pnt p0,struct sc_pnt p1, struct sc_pnt p2, struct sc_pnt p3);
extern double arc_line_angle(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2, struct sc_pnt p3);
extern double arc_arc_angle(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2, struct sc_pnt p3, struct sc_pnt p4);
extern double segment_angle(struct tp_segment s0, struct tp_segment s1);


//! Gcode vector dynamic.
struct tp_vector *vector_ptr;

//! Grotius scurve.
//! // For each scurve axis we store the data in here.
struct scurve_data sc, xaxis;

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

void update_gui(TP_STRUCT * const tp);
void update_scurve(TP_STRUCT * const tp);
void update_hal(TP_STRUCT * const tp);
double tpGetVelMax(TP_STRUCT * const tp);

// Gbrl corner velocity algoritme. Use a deviation of 0.001
void get_corner_velocity(double corner_degree,      // Input in degree.
                         double deviation,          // Input in mm.
                         double max_acc,            // Input in mm/s^2
                         double max_vel,            // Input in mm/s, used as corner velocity when corner is 180 degree or colinear or to limit the corner vel output.
                         double *radius,            // Output in mm.
                         double *corner_velocity);  // Output in mm/sec.

struct sc_pnt xyz;
struct sc_dir abc;
struct sc_ext uvw;

//! Create a empty queue.
int tpInit(TP_STRUCT * const tp)
{
    printf("tpInit. \n");

    return 0;
}

int tpRunCycle(TP_STRUCT * const tp, long period)
{
    //! printf("tpRunCycle. \n");
    tp->cycleTime=period;
    tp->cycleTime*=0.000000001; //! Convert to ms.

    update_hal(tp);
    update_scurve(tp);
    update_gui(tp);
    return 0;
}

//! The first function call.
int tpCreate(TP_STRUCT * const tp, int _queueSize,int id)
{
    if (_queueSize <= 0) {
        tp->queueSize = TP_DEFAULT_QUEUE_SIZE;
    } else {
        tp->queueSize = _queueSize;
    }

    //! Set the queue size to the c++ vector.
    vector_ptr=vector_init_ptr();

    printf("tpCreate. set tp->queuesize to: %i \n", tp->queueSize);
    return 0;
}

int tpSetMaxJerk(TP_STRUCT * const tp, double max_jerk)
{
    if (!tp || max_jerk <= 0.0) {
        return -1;
    }

    tp->max_jerk=max_jerk;

    printf("tpSetMaxJerk to: %f \n",max_jerk);
    return 0;
}

int tpClear(TP_STRUCT * const tp)
{
    printf("tpClear. \n");
    tp->traject_lenght=0;
    tp->gcode_lastPos=tp->currentPos;
    vector_clear(vector_ptr);
    vector_ptr=NULL;

    return 0;
}

int tpSetCycleTime(TP_STRUCT * const tp, double secs)
{
    if (!tp || secs <= 0.0) {
        return -1;
    }

    tp->cycleTime = secs;
    printf("tpSetCycleTime to: %f \n",tp->cycleTime);
    return 0;
}

int tpSetVmax(TP_STRUCT * const tp, double vMax, double ini_maxvel)
{
    if (!tp || vMax <= 0.0 || ini_maxvel <= 0.0) {
        return -1;
    }

    tp->vMax = vMax;
    tp->ini_maxvel = ini_maxvel;

    printf("tpSetVmax to: %f ",tp->vMax);
    printf(" , ini_maxvel to: %f \n",tp->ini_maxvel);
    return 0;
}

int tpSetVlimit(TP_STRUCT * const tp, double vLimit)
{
    if(!tp){ return -1;}

    if (vLimit < 0.0){
        tp->vLimit = 0.;
    } else {
        tp->vLimit = vLimit;
    }

    printf("tpSetVlimit. to: %f \n",tp->vLimit);
    return 0;
}

int tpSetAmax(TP_STRUCT * const tp, double aMax)
{
    if (!tp || aMax <= 0.0) {
        return -1;
    }

    tp->aMax=aMax;
    printf("tpSetAmax to: %f \n",tp->aMax);
    return 0;
}

int tpSetId(TP_STRUCT * const tp, int id)
{
    if (!tp) {
        return -1;
    }

    //! printf("tpSetId. \n");

    //! Set gcode line nr for upcoming new line, arc.
    tp->gcode_upcoming_line_nr=id;

    return 0;
}

int tpGetExecId(TP_STRUCT * const tp)
{
    //! printf("tpGetExecId. \n");

    //! This is the executed gcode line nr. The gui's gcode preview
    //! uses this to set the line.

    return tp->gcode_current_executed_line_nr;
}

int tpSetTermCond(TP_STRUCT * const tp, int cond, double tolerance)
{
    return 0;
}

/**
 * Used to tell the tp the initial position.
 * It sets the current position AND the goal position to be the same.  Used
 * only at TP initialization and when switching modes.
 */
int tpSetPos(TP_STRUCT * const tp, EmcPose const * const pos)
{
    if (0 == tp) {
        return -1;
    }

    // printf("tpSetPos x: %f y: %f z: %f \n",pos->tran.x,pos->tran.y,pos->tran.z);

    tp->currentPos=*pos;

    // printf("vector size: %i \n",vector_size_c(vector_ptr));
    // printf("tpCurrentPos x: %f y: %f z: %f \n",tp->currentPos.tran.x,tp->currentPos.tran.y,tp->currentPos.tran.z);

    return 0;
}

int tpGetPos(TP_STRUCT const * const tp, EmcPose * const pos)
{
    if (0 == tp) {
        return -1;
    }

    //! The gui toolposition tp is updated from here.
    *pos = tp->currentPos;
    // printf("tpGetPos x: %f y: %f z: %f \n",pos->tran.x,pos->tran.y,pos->tran.z);
    return 0;
}

int tpErrorCheck(TP_STRUCT const * const tp) {

    return 0;
}

int tpSetSpindleSync(TP_STRUCT * const tp, int spindle, double sync, int mode) {
    return 0;
}

int tpPause(TP_STRUCT * const tp)
{
    printf("tpPause. \n");

    tp->pausing=1;

    return 0;
}

int tpResume(TP_STRUCT * const tp)
{
    printf("tpResume. \n");

    tp->pausing=0;

    return 0;
}

int tpAbort(TP_STRUCT * const tp)
{
    printf("tpAbort. \n");
    vector_clear(vector_ptr);
    tp->traject_lenght=0;
    tp->gcode_lastPos=tp->currentPos;
    return 0;
}

int tpGetMotionType(TP_STRUCT * const tp)
{
    return tp->motionType;
}

int tpIsDone(TP_STRUCT * const tp)
{
    if(vector_size_c(vector_ptr)==0){
        return 1;
    }
    return 0;
}

int tpQueueDepth(TP_STRUCT * const tp)
{
    return 0;
}

int tpActiveDepth(TP_STRUCT * const tp)
{
    return 0;
}

int tpSetAout(TP_STRUCT * const tp, unsigned char index, double start, double end) {
    return 0;
}

int tpSetDout(TP_STRUCT * const tp, int index, unsigned char start, unsigned char end) {
    return 0;
}

int tpSetRunDir(TP_STRUCT * const tp, tc_direction_t dir){

    printf("tpSetRunDir, motion reverse : %i \n",dir);

    tp->reverse_run=dir;
    return 0;
}

int tpAddRigidTap(TP_STRUCT * const tp,
                  EmcPose end,
                  double vel,
                  double ini_maxvel,
                  double acc,
                  unsigned char enables,
                  double scale,
                  struct state_tag_t tag) {

    printf("tpAddRigidTap \n");

    return 0;
}

int tpAddLine(TP_STRUCT *
              const tp,
              EmcPose end,
              int canon_motion_type,
              double vel,
              double ini_maxvel,
              double acc,
              unsigned char enables,
              char atspeed,
              int indexer_jnum,
              struct state_tag_t tag)


{
    printf("tpAddLine \n");

   // if(vector_size_c(vector_ptr)==0){
      //  tp->gcode_lastPos=tp->currentPos;
   // }

    struct tp_segment b;
    b.primitive_id=sc_line;
    b.type=canon_motion_type;
    b.pnt_s=emc_pose_to_sc_pnt(tp->gcode_lastPos);
    b.pnt_w.x=0;
    b.pnt_w.y=0;
    b.pnt_w.z=0;
    b.pnt_c.x=0;
    b.pnt_c.y=0;
    b.pnt_c.z=0;
    b.angle_begin=0;
    b.angle_end=0;
    b.path_begin=0;
    b.path_end=0;

    b.pnt_e=emc_pose_to_sc_pnt(end);

    b.dir_s=emc_pose_to_sc_dir(tp->gcode_lastPos);
    b.dir_e=emc_pose_to_sc_dir(end);

    b.ext_s=emc_pose_to_sc_ext(tp->gcode_lastPos);
    b.ext_e=emc_pose_to_sc_ext(end);

    b.gcode_line_nr=tp->gcode_upcoming_line_nr;

    b.path_lenght=line_lenght_c(b.pnt_s,b.pnt_e);

    b.path_begin=tp->traject_lenght;
    tp->traject_lenght+=b.path_lenght;
    b.path_end=tp->traject_lenght;
    printf("lenght begin: %f \n",b.path_begin);
    printf("lenght end: %f \n",b.path_end);

    b.vo=0;
    b.vm=vel;
    b.ve=0;

    //! Calculate previous segment to current segment path transition corners in degrees.
    if(vector_size_c(vector_ptr)>0){
        struct tp_segment previous=vector_at(vector_ptr,vector_size_c(vector_ptr)-1);
        double angle_deg=segment_angle(previous,b);

        b.angle_begin=angle_deg;
        vector_set_end_angle(vector_ptr,vector_size_c(vector_ptr)-1,angle_deg);
    }

    vector_add_segment(vector_ptr,b);
    printf("vector size: %d \n",vector_size_c(vector_ptr));

    //! Update last pose to end of gcode block.
    tp->gcode_lastPos=end;

    //! Clear.
    tp->vector_current_exec=0;

    scurve_data_reset(&sc);

    return 0;
}

int tpAddCircle(TP_STRUCT * const tp,
                EmcPose end,
                PmCartesian center,
                PmCartesian normal,
                int turn,
                int canon_motion_type, //! arc_3->lin_2->GO_1
                double vel,
                double ini_maxvel,
                double acc,
                unsigned char enables,
                char atspeed,
                struct state_tag_t tag)
{
    printf("tpAddCircle. \n");

   // if(vector_size_c(vector_ptr)==0){
   //     tp->gcode_lastPos=tp->currentPos;
   // }

    struct tp_segment b;
    b.primitive_id=sc_arc;
    b.path_begin=0;
    b.path_end=0;
    b.type=canon_motion_type;
    b.pnt_s=emc_pose_to_sc_pnt(tp->gcode_lastPos);
    b.pnt_e= emc_pose_to_sc_pnt(end);

    b.dir_s=emc_pose_to_sc_dir(tp->gcode_lastPos);
    b.dir_e=emc_pose_to_sc_dir(end);

    b.ext_s=emc_pose_to_sc_ext(tp->gcode_lastPos);
    b.ext_e=emc_pose_to_sc_ext(end);

    b.pnt_c=emc_cart_to_sc_pnt(center);

    //! Create a 3d arc using waypoint technique.
    sc_arc_get_mid_waypoint_c(emc_pose_to_sc_pnt(tp->gcode_lastPos),
                              b.pnt_c,
                              b.pnt_e,
                              &b.pnt_w);

    b.angle_begin=0;
    b.angle_end=0;

    b.gcode_line_nr=tp->gcode_upcoming_line_nr;

    b.vo=0;
    b.vm=vel;
    b.ve=0;

    b.path_lenght=arc_lenght_c(b.pnt_s,b.pnt_w,b.pnt_e);

    b.path_begin=tp->traject_lenght;
    tp->traject_lenght+=b.path_lenght;
    b.path_end=tp->traject_lenght;
    printf("lenght begin: %f \n",b.path_begin);
    printf("lenght end: %f \n",b.path_end);

    //! Calculate previous segment to current segment path transition corners in degrees.
    if(vector_size_c(vector_ptr)>0){
        struct tp_segment previous=vector_at(vector_ptr,vector_size_c(vector_ptr)-1);
        double angle_deg=segment_angle(previous,b);

        b.angle_begin=angle_deg;
        vector_set_end_angle(vector_ptr,vector_size_c(vector_ptr)-1,angle_deg);
    }

    vector_add_segment(vector_ptr,b);
    printf("vector size: %d \n",vector_size_c(vector_ptr));

    //! Update last pose to end of gcode block.
    tp->gcode_lastPos=end;

    tp->vector_current_exec=0;

    scurve_data_reset(&sc);

    return 0;
}

void tpToggleDIOs(TC_STRUCT * const tc) {

}

struct state_tag_t tpGetExecTag(TP_STRUCT * const tp)
{
    if (0 == tp) {
        struct state_tag_t empty = {0};
        return empty;
    }
    return tp->execTag;
}

//! This function is responsible for long startup delay if return=1.
int tcqFull(TC_QUEUE_STRUCT const * const tcq)
{
    return 0;
}

//! A Inline functinn is compiled in between the upper-level function. So
//! its not called every time, but compiled inbetween. This makes it faster.
inline void update_gui(TP_STRUCT * const tp){

    if(vector_size_c(vector_ptr)>0){

        double segment_progress=(sc.guipos-vector_at(vector_ptr,tp->vector_current_exec).path_begin)/vector_at(vector_ptr,tp->vector_current_exec).path_lenght;
        int id=tp->vector_current_exec;

        if(vector_at_id(vector_ptr,id)==sc_line){
            interpolate_line_c(vector_at(vector_ptr,id).pnt_s,
                               vector_at(vector_ptr,id).pnt_e,
                               segment_progress,
                               &xyz);
        }
        if(vector_at_id(vector_ptr,id)==sc_arc){
            interpolate_arc_c(vector_at(vector_ptr,id).pnt_s,
                              vector_at(vector_ptr,id).pnt_w,
                              vector_at(vector_ptr,id).pnt_e,
                              segment_progress,
                              &xyz);
        }
        tp->currentPos.tran.x=xyz.x;
        tp->currentPos.tran.y=xyz.y;
        tp->currentPos.tran.z=xyz.z;

        interpolate_dir_c(vector_at(vector_ptr,id).dir_s,
                          vector_at(vector_ptr,id).dir_e,
                          segment_progress,
                          &abc);
        tp->currentPos.a=abc.a;
        tp->currentPos.b=abc.b;
        tp->currentPos.c=abc.c;

        interpolate_ext_c(vector_at(vector_ptr,id).ext_s,
                          vector_at(vector_ptr,id).ext_e,
                          segment_progress,
                          &uvw);
        tp->currentPos.u=uvw.u;
        tp->currentPos.v=uvw.v;
        tp->currentPos.w=uvw.w;

        //! Update emc with some values.
        emcmotConfig->trajCycleTime=tp->cycleTime;

        //! Dtg in this move.
        emcmotStatus->distance_to_go=sc.tarpos-sc.guipos;

        //! More info for lcnc.
        EmcPose pose;
        pose.tran.x=vector_at(vector_ptr,id).pnt_e.x-xyz.x;
        pose.tran.y=vector_at(vector_ptr,id).pnt_e.y-xyz.y;
        pose.tran.z=vector_at(vector_ptr,id).pnt_e.z-xyz.z;
        emcmotStatus->dtg=pose;

        //! Velocity for lcnc.
        emcmotStatus->current_vel=sc.guivel;
    }
}

inline double tpGetVelMax(TP_STRUCT * const tp){
    double vm=vector_at(vector_ptr,tp->vector_current_exec).vm;
    if(vm>tp->vLimit){
        vm=tp->vLimit;
    }
    if(vector_at(vector_ptr,tp->vector_current_exec).type==1){ //! G0
        vm*=emcmotStatus->rapid_scale;
    } else { //! It's a G1,G2,G3.
        vm*=emcmotStatus->net_feed_scale;
    }
    if(vm==0){ //! Avoid invalid velmax.
        vm=0.001;
    }
    return vm;
}

inline void get_corner_velocity(double corner_degree,           // Input in degree.
                                double deviation,          // Input in mm.
                                double max_acc,            // Input in mm/s^2
                                double max_vel,            // Input in mm/s, used as corner velocity when corner is 180 degree or colinear or to limit the corner vel output.
                                double *radius,            // Output in mm.
                                double *corner_velocity){  // Output in mm/sec.

    // Using grbl corner algo.
    // Formula : sin(corner/2)=radius/(radius+deviation);
    // sin(θ / 2) = r / (r + δ)
    // r = r sin(θ / 2) + δ sin(θ / 2)
    // r - r sin(θ / 2) = δ sin(θ / 2)
    // r (1 - sin(θ / 2)) = δ sin(θ / 2)
    // r = (δ sin(θ / 2)) / (1 - sin(θ / 2))

    double corner_radians=corner_degree*TO_RAD; // I think degree to radians.

    double c=corner_radians;
    double d=deviation;
    *radius = (d * sin(c / 2)) / (1 - sin(c / 2));
    *corner_velocity=sqrt(max_acc* *radius);

    if(*corner_velocity==INFINITY){ // When a corner of 180 degree is used the path is colinear, we return the max velocity value.
        *corner_velocity=max_vel;
    }
    if(*corner_velocity>max_vel){ // Limit the corner velocity to maxvel.
        *corner_velocity=max_vel;
    }
}

//! A Inline functinn is compiled in between the upper-level function. So
//! its not called every time, but compiled inbetween. This makes it faster.
inline void update_scurve(TP_STRUCT * const tp){

    if(vector_size_c(vector_ptr)>0){  //! Check if there is gcode content.

        tp->gcode_current_executed_line_nr=vector_at(vector_ptr,tp->vector_current_exec).gcode_line_nr;
        sc.tarpos=vector_at(vector_ptr,tp->vector_current_exec).path_end;

        double endangle=vector_at(vector_ptr,tp->vector_current_exec).angle_end;
        double radius=0;
        //get_corner_velocity(endangle,0.0001,sc.maxacc,sc.maxvel,&radius,&sc.endvel);
        //printf("end angle of this segment: %f \n",endangle);
        //printf("using endvel: %f \n",sc.endvel);

        if(endangle>175){
            sc.endvel=sc.maxvel;
        } else {
            sc.endvel=0;
        }

        sc.jog.mode=POSITION_SC;
        sc.jog.dir=FORWARD_SC;
        sc.jog.enable=1;
        sc.jermax=tp->max_jerk;
        sc.maxacc=tp->aMax;
        sc.maxvel=tpGetVelMax(tp);
        sc.intval=tp->cycleTime;

        if(tp->pausing){ //! Check for pause, resume.
            sc.jog.enable=0;
        }

        sc=axis_update(sc);

        if(sc.finish){

            printf("segment finished forward vector nr: %d \n",tp->vector_current_exec);
            printf("finished at pos x: %f y: %f z: %f \n",tp->currentPos.tran.x,tp->currentPos.tran.y,tp->currentPos.tran.z);

            //! Set next gcode segment.
            if(tp->vector_current_exec<vector_size_c(vector_ptr)-1){
                tp->vector_current_exec++;
                scurve_data_reset_position_data(&sc);
                tp->segment_progress=0; //! From here the gui update is done, reset value to zero.
            }
            //! We are finished and completed the last gcode segment. Traject is done !
            if(tp->vector_current_exec==vector_size_c(vector_ptr)-1){
                scurve_data_reset(&sc);
                vector_clear(vector_ptr);
                tp->gcode_lastPos=tp->currentPos;
                tp->traject_lenght=0;
            }
        }
    }
}

//! A Inline functinn is compiled in between the upper-level function. So
//! its not called every time, but compiled inbetween. This makes it faster.
inline void update_hal(TP_STRUCT * const tp){
    //! For info, a pin uses a * before, a parameter not.
    tp->reverse_run=*hal_reverse_run->Pin;

    *hal_guipos->Pin=sc.guipos;
    *hal_guivel->Pin=sc.guivel;
    *hal_guiacc->Pin=sc.guiacc;

    tp->max_jerk=500; // Use a value.
}

EXPORT_SYMBOL(tpMotFunctions);
EXPORT_SYMBOL(tpMotData);

EXPORT_SYMBOL(tpSetMaxJerk);
EXPORT_SYMBOL(tpAbort);
EXPORT_SYMBOL(tpActiveDepth);
EXPORT_SYMBOL(tpAddCircle);
EXPORT_SYMBOL(tpAddLine);
EXPORT_SYMBOL(tpAddRigidTap);
EXPORT_SYMBOL(tpClear);
EXPORT_SYMBOL(tpCreate);
EXPORT_SYMBOL(tpGetExecId);
EXPORT_SYMBOL(tpGetExecTag);
EXPORT_SYMBOL(tpGetMotionType);
EXPORT_SYMBOL(tpGetPos);
EXPORT_SYMBOL(tpIsDone);
EXPORT_SYMBOL(tpPause);
EXPORT_SYMBOL(tpQueueDepth);
EXPORT_SYMBOL(tpResume);
EXPORT_SYMBOL(tpRunCycle);
EXPORT_SYMBOL(tpSetAmax);
EXPORT_SYMBOL(tpSetAout);
EXPORT_SYMBOL(tpSetCycleTime);
EXPORT_SYMBOL(tpSetDout);
EXPORT_SYMBOL(tpSetId);
EXPORT_SYMBOL(tpSetPos);
EXPORT_SYMBOL(tpSetRunDir);
EXPORT_SYMBOL(tpSetSpindleSync);
EXPORT_SYMBOL(tpSetTermCond);
EXPORT_SYMBOL(tpSetVlimit);
EXPORT_SYMBOL(tpSetVmax);
EXPORT_SYMBOL(tcqFull);
































