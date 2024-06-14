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

#include "scurve_engine.h"
#include "scurve_reset.h"
#include "scurve_struct.h"

#include "ruckig_format.h"

#define DEFAULT_ENABLE_GROTIUS_SCURVE 1
#define DEFAULT_MAX_JERK_TP 8000
#define DEFAULT_ENABLE_RUCKIG_RUNNERS 0
#define DEFAULT_MAX_JERK_RUCKIG_RUNNERS 8000
#define DEFAULT_ENABLE_LOOK_AHEAD 1
#define DEFAULT_LOOK_AHEAD_LINES 20
#define DEFAULT_ARC_RADIUS_SAMPLE 5
#define DEFAULT_ARC_VELOCITY_SAMPLE 1400
#define DEFAULT_BLEND_RADIUS_G0 0
#define DEFAULT_BLEND_RADIUS_G1_G2_G3 0

/* module information */
MODULE_AUTHOR("Skynet_Cyberdyne");
MODULE_DESCRIPTION("tpmod_scurve_grotius");
MODULE_LICENSE("GPL2");

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t
*hal_guivel, *hal_guiacc, *hal_guipos, *hal_maxacc, *hal_maxvel, *hal_maxjerk,
*hal_ruckig_jerk, *hal_arc_radius_sample, *hal_arc_velocity_sample, *hal_blend_radius_G0, *hal_blend_radius_G1_G2_G3, *hal_segment_progress;
//! Pins
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t
*hal_reverse_run,
*hal_enable_look_ahead,
*hal_enable_grotius_scurve,
*hal_enable_runners,
*hal_enable_chamfer,
*hal_jog_x_plus, *hal_jog_x_min,
*hal_jog_y_plus, *hal_jog_y_min,
*hal_jog_z_plus, *hal_jog_z_min,
*hal_jog_a_plus, *hal_jog_a_min,
*hal_jog_b_plus, *hal_jog_b_min,
*hal_jog_c_plus, *hal_jog_c_min,
*hal_jog_u_plus, *hal_jog_u_min,
*hal_jog_v_plus, *hal_jog_v_min,
*hal_jog_w_plus, *hal_jog_w_min
;

typedef struct { //! Int.
    hal_s32_t *Pin;
} s32_data_t;

typedef struct { //! Param int.
    hal_s32_t Pin;
} param_s32_data_t;

typedef struct { //! Uint.
    hal_u32_t *Pin;
} u32_data_t;
u32_data_t *hal_look_ahead_lines, *hal_current_gcode_line;

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


struct sc_pnt emc_pose_to_sc_pnt(struct EmcPose pose){
    struct sc_pnt pnt;
    pnt.x=pose.tran.x;
    pnt.y=pose.tran.y;
    pnt.z=pose.tran.z;
    return pnt;
}

struct sc_dir emc_pose_to_sc_dir(struct EmcPose pose){
    struct sc_dir dir;
    dir.a=pose.a;
    dir.b=pose.b;
    dir.c=pose.c;
    return dir;
}

struct sc_ext emc_pose_to_sc_ext(struct EmcPose pose){
    struct sc_ext ext;
    ext.u=pose.u;
    ext.v=pose.v;
    ext.w=pose.w;
    return ext;
}

struct sc_pnt emc_cart_to_sc_pnt( PmCartesian pnt){
    struct sc_pnt p;
    p.x=pnt.x;
    p.y=pnt.y;
    p.z=pnt.z;
    return p;
}

PmCartesian sc_pnt_to_emc_cart(struct sc_pnt pnt){
    PmCartesian p;
    p.x=pnt.x;
    p.y=pnt.y;
    p.z=pnt.z;
    return p;
}

bool emc_pose_xyz_equal(struct EmcPose pose0, struct EmcPose pose1){

    if(pose0.tran.x==pose1.tran.x && pose0.tran.y==pose1.tran.y && pose0.tran.z==pose1.tran.z){
        return 1;
    }
    return 0;
}



int rtapi_app_main(void) {

    int r = 0;
    comp_idx = hal_init("tpmod_scurve_grotius");
    if(comp_idx < 0) return comp_idx;
    r = hal_export_funct("tpmod_scurve_grotius", the_function, &skynet,0,0,comp_idx);

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
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_guivel",HAL_OUT,&(hal_guivel->Pin),comp_idx);

    hal_guiacc = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_guiacc",HAL_OUT,&(hal_guiacc->Pin),comp_idx);

    hal_guipos = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_guipos",HAL_OUT,&(hal_guipos->Pin),comp_idx);

    hal_maxvel = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_maxvel",HAL_OUT,&(hal_maxvel->Pin),comp_idx);

    hal_maxacc = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_maxacc",HAL_OUT,&(hal_maxacc->Pin),comp_idx);

    hal_segment_progress = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_segment_progress",HAL_OUT,&(hal_segment_progress->Pin),comp_idx);

    hal_reverse_run = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_reverse_run",HAL_IN,&(hal_reverse_run->Pin),comp_idx);

    hal_enable_chamfer = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_enable_chamfer",HAL_IN,&(hal_enable_chamfer->Pin),comp_idx);

    hal_enable_runners = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_enable_runners",HAL_IN,&(hal_enable_runners->Pin),comp_idx);

    hal_enable_look_ahead = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_enable_look_ahead",HAL_IN,&(hal_enable_look_ahead->Pin),comp_idx);

    hal_look_ahead_lines = (u32_data_t*)hal_malloc(sizeof(u32_data_t));
    r+=hal_pin_u32_new("tpmod_scurve_grotius.hal_look_ahead_lines",HAL_IN,&(hal_look_ahead_lines->Pin),comp_idx);

    hal_current_gcode_line = (u32_data_t*)hal_malloc(sizeof(u32_data_t));
    r+=hal_pin_u32_new("tpmod_scurve_grotius.hal_current_gcode_line",HAL_OUT,&(hal_current_gcode_line->Pin),comp_idx);

    hal_enable_grotius_scurve = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_enable_grotius_scurve",HAL_IN,&(hal_enable_grotius_scurve->Pin),comp_idx);

    hal_maxjerk = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_maxjerk",HAL_IN,&(hal_maxjerk->Pin),comp_idx);

    hal_ruckig_jerk = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_ruckig_jerk",HAL_IN,&(hal_ruckig_jerk->Pin),comp_idx);

    hal_arc_radius_sample = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_arc_radius_sample",HAL_IN,&(hal_arc_radius_sample->Pin),comp_idx);

    hal_arc_velocity_sample = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_arc_velocity_sample",HAL_IN,&(hal_arc_velocity_sample->Pin),comp_idx);

    hal_jog_x_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_x_plus",HAL_IN,&(hal_jog_x_plus->Pin),comp_idx);
    hal_jog_x_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_x_min",HAL_IN,&(hal_jog_x_min->Pin),comp_idx);

    hal_jog_y_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_y_plus",HAL_IN,&(hal_jog_y_plus->Pin),comp_idx);
    hal_jog_y_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_y_min",HAL_IN,&(hal_jog_y_min->Pin),comp_idx);

    hal_jog_z_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_z_plus",HAL_IN,&(hal_jog_z_plus->Pin),comp_idx);
    hal_jog_z_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_z_min",HAL_IN,&(hal_jog_z_min->Pin),comp_idx);

    hal_jog_a_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_a_plus",HAL_IN,&(hal_jog_a_plus->Pin),comp_idx);
    hal_jog_a_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_a_min",HAL_IN,&(hal_jog_a_min->Pin),comp_idx);

    hal_jog_b_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_b_plus",HAL_IN,&(hal_jog_b_plus->Pin),comp_idx);
    hal_jog_b_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_b_min",HAL_IN,&(hal_jog_b_min->Pin),comp_idx);

    hal_jog_c_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_c_plus",HAL_IN,&(hal_jog_c_plus->Pin),comp_idx);
    hal_jog_c_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_c_min",HAL_IN,&(hal_jog_c_min->Pin),comp_idx);

    hal_jog_u_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_u_plus",HAL_IN,&(hal_jog_u_plus->Pin),comp_idx);
    hal_jog_u_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_u_min",HAL_IN,&(hal_jog_u_min->Pin),comp_idx);

    hal_jog_v_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_v_plus",HAL_IN,&(hal_jog_v_plus->Pin),comp_idx);
    hal_jog_v_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_v_min",HAL_IN,&(hal_jog_v_min->Pin),comp_idx);

    hal_jog_w_plus = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_w_plus",HAL_IN,&(hal_jog_w_plus->Pin),comp_idx);
    hal_jog_w_min = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("tpmod_scurve_grotius.hal_jog_w_min",HAL_IN,&(hal_jog_w_min->Pin),comp_idx);


    hal_blend_radius_G0 = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_blend_radius_G0",HAL_IN,&(hal_blend_radius_G0->Pin),comp_idx);

    hal_blend_radius_G1_G2_G3 = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("tpmod_scurve_grotius.hal_blend_radius_G1_G2_G3",HAL_IN,&(hal_blend_radius_G1_G2_G3->Pin),comp_idx);

    return r;
}

//! Status and config from motion.h
static emcmot_status_t *emcmotStatus;
static emcmot_config_t *emcmotConfig;
static emcmot_command_t *emcmotCommand;

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
               ,emcmot_command_t *cconfig
               )
{
    emcmotStatus = pstatus;
    emcmotConfig = pconfig;
    emcmotCommand = cconfig;
}

//! To use functions from tp_vector.cpp we need to declare them here:
extern struct tp_vector* vector_init_ptr();
extern int vector_size_c(struct tp_vector *ptr);
extern void vector_clear(struct tp_vector *ptr);
extern int vector_at_id(struct tp_vector *ptr, int n);
extern struct tp_segment vector_at(struct tp_vector *ptr, int index);
extern void vector_add_segment(struct tp_vector *ptr, struct tp_segment b);
extern void vector_pop_back(struct tp_vector *ptr);
extern void vector_set_end_angle(struct tp_vector *ptr, int index, double angle_deg);
extern double vector_traject_lenght(struct tp_vector *ptr);
extern void vector_set_tangent_or_colineair(tp_vector *ptr, int index, int value);
extern void vector_set_vo(struct tp_vector *ptr, int index, double value);
extern void vector_set_vm(struct tp_vector *ptr, int index, double value);
extern void vector_set_ve(struct tp_vector *ptr, int index, double value);
extern void vector_set_acs(struct tp_vector *ptr, int index, double value);
extern void vector_set_ace(struct tp_vector *ptr, int index, double value);
extern void vector_set_pnt_s(struct tp_vector *ptr, int index, struct sc_pnt p);
extern void vector_set_pnt_e(struct tp_vector *ptr, int index, struct sc_pnt p);
extern void vector_set_pnt_w(struct tp_vector *ptr, int index, struct sc_pnt p);
extern void vector_set_pnt_c(struct tp_vector *ptr, int index, struct sc_pnt p);

extern void vector_run_cavalier(struct tp_vector *ptr, int index, double blend_radius_G0, double blend_radius_G1_G2_G3, int enable_chamfer);
extern void vector_update_pathlenghts(struct tp_vector *ptr);

extern double arc_lenght_c(struct sc_pnt start, struct sc_pnt way, struct sc_pnt end);
extern double line_lenght_c(struct sc_pnt start, struct sc_pnt end);
extern int tp_lines_colinear_c(struct sc_pnt p0, struct sc_pnt p1, struct  sc_pnt p2);
extern int tp_lines_point_on_line_c(struct sc_pnt p0, struct sc_pnt lp1, struct  sc_pnt lp2);
extern int tp_lines_perpendicular_c(struct sc_pnt p0, struct sc_pnt p1, struct sc_pnt p2);
extern int tp_lines_are_points_equal_c(struct sc_pnt p1, struct sc_pnt p2, double tolerance);
extern int tp_lines_are_lengths_equal_c(double length1, double length2, double tolerance);
extern struct sc_pnt tp_lines_midpoint(struct sc_pnt p1, struct sc_pnt p2);
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
struct scurve_data sc, scx, scy, scz, sca, scb, scc, scu, scv, scw;

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
extern struct scurve_data forward_curve_build_c(struct scurve_data data);
extern double ttot_period_c(struct scurve_period p);

extern struct scurve_data jog_position_endve_enum_c(struct scurve_data data, int enable, double endvel, double endacc, double tarpos, enum jog_direction dir);

// STEP 2.
// Construct a curve respecting the given tarpos. STEP 2.
extern struct scurve_data jog_update_c(struct scurve_data data);
extern struct scurve_data jog_update_ve_c(struct scurve_data data);

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

struct scurve_data axis_update_endve(struct scurve_data data){
    if(data.jog.mode==POSITION_SC){
        data=jog_position_endve_enum_c(data,data.jog.enable,data.endvel,data.endacc,data.tarpos,data.jog.dir);
    }
    if(data.jog.mode==VELOCITY_SC){
        // data=jog_velocity_endve_enum_c(data,data.jog.enable,data.endvel,data.endacc,data.jog.dir);
    }
    data=jog_update_ve_c(data);
    return data;
}

void update_gui(TP_STRUCT * const tp);
void update_scurve_grotius(TP_STRUCT * const tp);
void update_scurve_ruckig(TP_STRUCT * const tp);
void update_hal(TP_STRUCT * const tp);
void update_pause_jog_x(TP_STRUCT * const tp);
void update_pause_jog_resume_x(TP_STRUCT * const tp);
void update_pause_jog_y(TP_STRUCT * const tp);
void update_pause_jog_resume_y(TP_STRUCT * const tp);
void update_pause_jog_z(TP_STRUCT * const tp);
void update_pause_jog_resume_z(TP_STRUCT * const tp);
void update_pause_jog_a(TP_STRUCT * const tp);
void update_pause_jog_resume_a(TP_STRUCT * const tp);
void update_pause_jog_b(TP_STRUCT * const tp);
void update_pause_jog_resume_b(TP_STRUCT * const tp);
void update_pause_jog_c(TP_STRUCT * const tp);
void update_pause_jog_resume_c(TP_STRUCT * const tp);
void update_pause_jog_u(TP_STRUCT * const tp);
void update_pause_jog_resume_u(TP_STRUCT * const tp);
void update_pause_jog_v(TP_STRUCT * const tp);
void update_pause_jog_resume_v(TP_STRUCT * const tp);
void update_pause_jog_w(TP_STRUCT * const tp);
void update_pause_jog_resume_w(TP_STRUCT * const tp);
void update_pause_grotius_scurve(TP_STRUCT * const tp);
double tpGetRapidVelMax(TP_STRUCT * const tp);
double tpGetLinearVelMaxId(TP_STRUCT * const tp, int index);
double tpGetArcSpeed(TP_STRUCT * const tp, int index);
void update_look_ahead(TP_STRUCT * const tp);
void look_ahead_optimizer(TP_STRUCT * const tp, int look_ahead_lines);
double endvel_scurve(TP_STRUCT * const tp, int index, double pathlenght, double vo);
double endvel_trapezium(TP_STRUCT * const tp, int index, double vo, double pathlenght);

struct sc_pnt xyz;
struct sc_dir abc;
struct sc_ext uvw;

struct result rx={},ry={},rz={},rxyz={}; //! xyz runners.
struct result rtp={}; //! Ruckig tp scurve.
void update_ruckig_runners(TP_STRUCT * const tp);
extern struct result wrapper_get_pos(struct result input);

//! Create a empty queue.
int tpInit(TP_STRUCT * const tp)
{
    printf("tpInit. \n");

    return 0;
}

// Test
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Global variables to share data between threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int calculation_done = 0;
int result = 0;
int calc_thread_running = 0;

// Function for extensive calculation
void* extensive_calculation(void* arg) {
    // Lock mutex to prevent others from accessing the vector
    sleep(3);  // Sleeps for 5,000,000 microseconds (5 seconds)

    vector_pop_back(vector_ptr); // Remove last empty fillet from vector, will trigger signal 11, core dump.


    // Simulate a long calculation
    // usleep(5000000);  // Sleeps for 5,000,000 microseconds (5 seconds)
    int calc_result = 42;  // Example result of the calculation

    // Lock mutex and update shared data
    pthread_mutex_lock(&mutex);
    result = calc_result;
    calculation_done = 1;

    printf("Calculation done, unlock mutex.\n");

    calc_thread_running = 0; // Indicate that calculation thread is no longer running
    pthread_cond_signal(&cond);  // Signal that calculation is done
    pthread_mutex_unlock(&mutex);

    return NULL;
}
// Function to start the calculation thread
void start_calculation_thread() {
    if (!calc_thread_running) {
        // Start the calculation thread
        printf("Start the calculation thread.\n");
        pthread_t calc_thread;
        calc_thread_running = 1; // Indicate that calculation thread is running
        pthread_create(&calc_thread, NULL, extensive_calculation, NULL);
        pthread_detach(calc_thread); // Detach to avoid needing to join
    }
}

// Function to check if the calculation is done and process the result
//void check_calculation_done() {
//    pthread_mutex_lock(&mutex);
//    if (calculation_done) {
//        // Process the result
//        printf("Calculation done, result = %d\n", result);
//        // calculation_done = 0;  // Reset flag
//        //tp->cavalier_ready = 1; // Set ready flag after processing the result
//        // vector_update_pathlenghts(vector_ptr);
//    }
//    pthread_mutex_unlock(&mutex);
//}


int tpRunCycle(TP_STRUCT * const tp, long period){

    //! printf("tpRunCycle. \n");
    tp->cycleTime=period;
    tp->cycleTime*=0.000000001; //! Convert to ms.

    //    if(tp->buffer_ready==0){
    //        tp->cavalier_ready=0;
    //        calculation_done=0;
    //    }

    //   if(tp->buffer_ready==1){

    //        if(!calculation_done){
    //            start_calculation_thread();
    //        }

    //        if(tp->cavalier_ready==0){
    //            vector_update_pathlenghts(vector_ptr);
    //            tp->cavalier_ready=1;
    //        }

    //        if(!calc_thread_running){ // This enables the calculation thread to modify the gcode buffer, without any others acces the data.
    update_hal(tp);
    //   if(tp->enable_look_ahead){
    //       update_look_ahead(tp);
    //   }
    // if(tp->enable_grotius_scurve){
    update_scurve_grotius(tp);
    //  } else {
    //     update_scurve_ruckig(tp);
    //  }
    update_gui(tp);
    //       }
    //    }
    //   tp->buffer_ready=1; // Will keep this value if no new segments are added.

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
    if(tp->aMax>10000){
        tp->aMax=10000;
        printf("Hard coded in tp_scurve_grotius.c : Limiting aMax to 10000.\n");
    }
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
    tp->termCond = cond;
    tp->tolerance = tolerance;
    printf("tpSetTermCond, tollerance: %f termcond: %d \n",tp->tolerance,tp->termCond);

    //Purposeful waterfall for now
    /*
        case TC_TERM_COND_PARABOLIC:
        case TC_TERM_COND_TANGENT:
        case TC_TERM_COND_EXACT:
        case TC_TERM_COND_STOP:*/

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

    rx.curpos=tp->currentPos.tran.x; // Update the runners to currentpos.
    ry.curpos=tp->currentPos.tran.y;
    rz.curpos=tp->currentPos.tran.z;

    tp->currentPos=*pos;
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

int tpIsTangent(struct tp_segment a, struct tp_segment b){

    int result=0;
    if(a.primitive_id==sc_line && b.primitive_id==sc_line){
        result=tp_lines_colinear_c(a.pnt_s,a.pnt_e,b.pnt_e);
    }
    if(a.primitive_id==sc_arc && b.primitive_id==sc_line){
        result=tp_lines_perpendicular_c(a.pnt_c, b.pnt_s, b.pnt_e);
    }
    if(a.primitive_id==sc_line && b.primitive_id==sc_arc){
        result=tp_lines_perpendicular_c(a.pnt_s, a.pnt_e, b.pnt_c);
    }
    if(a.primitive_id==sc_arc && b.primitive_id==sc_arc){
        result=tp_lines_point_on_line_c(a.pnt_e,a.pnt_c,b.pnt_c);
    }
    if (a.primitive_id == sc_arc && b.primitive_id == sc_arc) {
        int points_equal = tp_lines_are_points_equal_c(a.pnt_c, b.pnt_c, 1e-6);
        // printf("Center points are equal: %d\n", points_equal);

        int radius_equal = tp_lines_are_lengths_equal_c(a.radius, b.radius, 1e-6);
        // printf("Radii are equal: %d\n", radius_equal);

        if(points_equal && radius_equal){
            result=1;
        }
    }
    return result;
}

int tpAddGeneralMotion(TP_STRUCT *
                       const tp,
                       EmcPose end,
                       int canon_motion_type,
                       double vel,
                       double ini_maxvel,
                       double acc,
                       unsigned char enables,
                       char atspeed,
                       int indexer_jnum,
                       struct state_tag_t tag){

    printf("tpAddGeneralMotion \n");

    struct tp_segment b;
    b.primitive_id=sc_line;
    b.motion_type=canon_motion_type;
    b.pnt_s=emc_pose_to_sc_pnt(tp->gcode_lastPos);
    b.pnt_c.x=0;
    b.pnt_c.y=0;
    b.pnt_c.z=0;
    b.angle_begin=0;
    b.angle_end=0;
    b.acs=0;
    b.ace=0;
    b.radius=0;

    b.pnt_e=emc_pose_to_sc_pnt(end);

    b.pnt_w=tp_lines_midpoint(b.pnt_s,b.pnt_e);

    b.dir_s=emc_pose_to_sc_dir(tp->gcode_lastPos);
    b.dir_e=emc_pose_to_sc_dir(end);

    b.ext_s=emc_pose_to_sc_ext(tp->gcode_lastPos);
    b.ext_e=emc_pose_to_sc_ext(end);

    b.gcode_line_nr=tp->gcode_upcoming_line_nr;

    b.path_lenght=line_lenght_c(b.pnt_s,b.pnt_e);

    b.vo=0;
    b.vm=vel;
    b.vm_gcode=vel;
    b.ve=0;
    b.is_fillet_segment=0;

    //! Calculate previous segment to current segment path transition corners in degrees.
    if(vector_size_c(vector_ptr)>0){
        struct tp_segment previous=vector_at(vector_ptr,vector_size_c(vector_ptr)-1);
        double angle_deg=segment_angle(previous,b);

        b.angle_begin=angle_deg;
        vector_set_end_angle(vector_ptr,vector_size_c(vector_ptr)-1,angle_deg);

        int res=tpIsTangent(previous,b);
        vector_set_tangent_or_colineair(vector_ptr,vector_size_c(vector_ptr)-1,res);
    }

    vector_add_segment(vector_ptr,b);
    // printf("vector size: %d \n",vector_size_c(vector_ptr));

    //if(vector_size_c(vector_ptr)>2){ // Used for cavalier algo.
    //    int start=vector_size_c(vector_ptr)-3;
    //    vector_run_cavalier(vector_ptr,start,tp->blend_radius_G0,tp->blend_radius_G1_G2_G3);
    //}

    //! Update last pose to end of gcode block.
    tp->gcode_lastPos=end;

    //! Clear.
    tp->vector_current_exec=0;

    scurve_data_reset(&sc);

    tp->buffer_ready=0;

    // Test to add tails.
    if(vector_size_c(vector_ptr)>0){ // Used for cavalier algo.
        struct tp_segment fillet;
        fillet.pnt_s=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_e=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e; // Use same point.
        fillet.pnt_w=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_c=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.primitive_id=sc_arc;
        fillet.path_lenght=0;
        fillet.gcode_line_nr=tp->gcode_upcoming_line_nr;
        fillet.radius=0;
        fillet.vo=0;
        fillet.vm=vel;
        fillet.vm_gcode=vel;
        fillet.ve=0;
        fillet.is_fillet_segment=1;
        vector_add_segment(vector_ptr,fillet);
    }

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

    struct tp_segment b;
    b.primitive_id=sc_line;
    b.motion_type=canon_motion_type;
    b.pnt_s=emc_pose_to_sc_pnt(tp->gcode_lastPos);
    b.pnt_c.x=0;
    b.pnt_c.y=0;
    b.pnt_c.z=0;
    b.angle_begin=0;
    b.angle_end=0;
    b.acs=0;
    b.ace=0;
    b.radius=0;

    b.pnt_e=emc_pose_to_sc_pnt(end);

    b.pnt_w=tp_lines_midpoint(b.pnt_s,b.pnt_e);

    b.dir_s=emc_pose_to_sc_dir(tp->gcode_lastPos);
    b.dir_e=emc_pose_to_sc_dir(end);

    b.ext_s=emc_pose_to_sc_ext(tp->gcode_lastPos);
    b.ext_e=emc_pose_to_sc_ext(end);

    b.gcode_line_nr=tp->gcode_upcoming_line_nr;

    b.path_lenght=line_lenght_c(b.pnt_s,b.pnt_e);

    b.vo=0;
    b.vm=vel;
    b.vm_gcode=vel;
    b.ve=0;
    b.is_fillet_segment=0;

    //! Calculate previous segment to current segment path transition corners in degrees.
    if(vector_size_c(vector_ptr)>0){
        struct tp_segment previous=vector_at(vector_ptr,vector_size_c(vector_ptr)-1);
        double angle_deg=segment_angle(previous,b);

        b.angle_begin=angle_deg;
        vector_set_end_angle(vector_ptr,vector_size_c(vector_ptr)-1,angle_deg);

        int res=tpIsTangent(previous,b);
        vector_set_tangent_or_colineair(vector_ptr,vector_size_c(vector_ptr)-1,res);
    }

    vector_add_segment(vector_ptr,b);
    // printf("vector size: %d \n",vector_size_c(vector_ptr));

    //if(vector_size_c(vector_ptr)>2){ // Used for cavalier algo.
    //    int start=vector_size_c(vector_ptr)-3;
    //    vector_run_cavalier(vector_ptr,start,tp->blend_radius_G0,tp->blend_radius_G1_G2_G3);
    //}

    //! Update last pose to end of gcode block.
    tp->gcode_lastPos=end;

    //! Clear.
    tp->vector_current_exec=0;

    scurve_data_reset(&sc);

    tp->buffer_ready=0;

    // Test to add tails.
    if(vector_size_c(vector_ptr)>0){ // Used for cavalier algo.
        struct tp_segment fillet;
        fillet.pnt_s=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_e=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e; // Use same point.
        fillet.pnt_w=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_c=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.primitive_id=sc_arc;
        fillet.path_lenght=0;
        fillet.gcode_line_nr=tp->gcode_upcoming_line_nr;
        fillet.radius=0;
        fillet.vo=0;
        fillet.vm=vel;
        fillet.vm_gcode=vel;
        fillet.ve=0;
        fillet.is_fillet_segment=1;
        vector_add_segment(vector_ptr,fillet);
    }

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

    struct tp_segment b;
    b.primitive_id=sc_arc;
    b.motion_type=canon_motion_type;
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
    b.vm_gcode=vel;
    b.ve=0;
    b.is_fillet_segment=0;

    b.radius=line_lenght_c(b.pnt_c,b.pnt_e);

    b.path_lenght=arc_lenght_c(b.pnt_s,b.pnt_w,b.pnt_e);

    //! Calculate previous segment to current segment path transition corners in degrees.
    if(vector_size_c(vector_ptr)>0){
        struct tp_segment previous=vector_at(vector_ptr,vector_size_c(vector_ptr)-1);
        double angle_deg=segment_angle(previous,b);

        b.angle_begin=angle_deg;
        vector_set_end_angle(vector_ptr,vector_size_c(vector_ptr)-1,angle_deg);

        bool res=tpIsTangent(previous,b);
        vector_set_tangent_or_colineair(vector_ptr,vector_size_c(vector_ptr)-1,res);
    }

    vector_add_segment(vector_ptr,b);
    // printf("vector size: %d \n",vector_size_c(vector_ptr));

    // if(vector_size_c(vector_ptr)>2){ // Used for cavalier algo.
    //    int start=vector_size_c(vector_ptr)-3;
    //    vector_run_cavalier(vector_ptr,start,tp->blend_radius_G0,tp->blend_radius_G1_G2_G3);
    // }

    //! Update last pose to end of gcode block.
    tp->gcode_lastPos=end;

    tp->vector_current_exec=0;

    scurve_data_reset(&sc);

    tp->buffer_ready=0;

    if(vector_size_c(vector_ptr)>0){ // Used for cavalier algo.
        struct tp_segment fillet;
        fillet.pnt_s=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_e=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_w=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.pnt_c=vector_at(vector_ptr,vector_size_c(vector_ptr)-1).pnt_e;
        fillet.primitive_id=sc_arc;
        fillet.path_lenght=0;
        fillet.gcode_line_nr=tp->gcode_upcoming_line_nr;
        fillet.radius=0;
        fillet.vo=0;
        fillet.vm=vel;
        fillet.vm_gcode=vel;
        fillet.ve=0;
        fillet.is_fillet_segment=1;
        vector_add_segment(vector_ptr,fillet);
    }

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

inline void update_ruckig_runners(TP_STRUCT * const tp){

    // Start of ruckig runners.
    rxyz.maxacc=tp->aMax;
    rxyz.maxjerk=tp->ruckig_jerk;

    // Populate vector with current gui values for vm, set vo and vo to zero.
    if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_rapid){
        rxyz.maxvel=tpGetRapidVelMax(tp);
    }
    if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_linear){
        rxyz.maxvel=tpGetLinearVelMaxId(tp,tp->vector_current_exec);
    }
    if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_circle){
        rxyz.maxvel=tpGetArcSpeed(tp,tp->vector_current_exec);
    }

    if(rxyz.maxvel==0){ //! Ruckig's maxvel may not be zero. Invalid.
        rxyz.maxvel=0.001;
    }

    rxyz.enable=1;
    rxyz.durationdiscretizationtype=Discrete;
    rxyz.synchronizationtype=None;

    //! MENTION: tp->cycletime is not set to 0.001, or it has a long to double conversion error.
    //! We set it fixed for now.
    rxyz.period=tp->cycleTime;

    rxyz.taracc=0;
    rxyz.tarvel=0;

    //! When pausing, goto velocity 0. See the component motdot
    //! how a jog stop is done.
    // if(tp->pausing || rxyz.maxvel==0 ){
    //    rxyz.interfacetype=velocity;
    //} else {
    rxyz.interfacetype=position;
    // }

    //! Copy shared values.
    rx.maxacc=rxyz.maxacc;
    rx.maxjerk=rxyz.maxjerk;
    rx.maxvel=rxyz.maxvel;
    rx.enable=rxyz.enable;
    rx.durationdiscretizationtype=rxyz.durationdiscretizationtype;
    rx.synchronizationtype=rxyz.synchronizationtype;
    rx.period=rxyz.period;
    rx.taracc=rxyz.taracc;
    rx.tarvel=rxyz.tarvel;

    ry.maxacc=rxyz.maxacc;
    ry.maxjerk=rxyz.maxjerk;
    ry.maxvel=rxyz.maxvel;
    ry.enable=rxyz.enable;
    ry.durationdiscretizationtype=rxyz.durationdiscretizationtype;
    ry.synchronizationtype=rxyz.synchronizationtype;
    ry.period=rxyz.period;
    ry.taracc=rxyz.taracc;
    ry.tarvel=rxyz.tarvel;

    rz.maxacc=rxyz.maxacc;
    rz.maxjerk=rxyz.maxjerk;
    rz.maxvel=rxyz.maxvel;
    rz.enable=rxyz.enable;
    rz.durationdiscretizationtype=rxyz.durationdiscretizationtype;
    rz.synchronizationtype=rxyz.synchronizationtype;
    rz.period=rxyz.period;
    rz.taracc=rxyz.taracc;
    rz.tarvel=rxyz.tarvel;

    //! Calculate the x axis follower.
    rx.tarpos=xyz.x;
    rx=wrapper_get_pos(rx);
    if(rx.function_return_code==Working){
        tp->currentPos.tran.x=rx.curpos;
    }

    //! Calculate the y axis follower.
    ry.tarpos=xyz.y;
    ry=wrapper_get_pos(ry);
    if(ry.function_return_code==Working){
        tp->currentPos.tran.y=ry.curpos;
    }

    //! Calculate the z axis follower.
    rz.tarpos=xyz.z;
    rz=wrapper_get_pos(rz);
    if(rz.function_return_code==Working){
        tp->currentPos.tran.z=rz.curpos;
    }
    // End of ruckig runners.
}

//! A Inline functinn is compiled in between the upper-level function. So
//! its not called every time, but compiled inbetween. This makes it faster.
inline void update_gui(TP_STRUCT * const tp){

    if(vector_size_c(vector_ptr)>0 && !tp->jog_mode){

        tp->segment_progress=(sc.guipos-vector_at(vector_ptr,tp->vector_current_exec).path_begin)/vector_at(vector_ptr,tp->vector_current_exec).path_lenght;


        tp->segment_progress=fmin(tp->segment_progress,1); // Limit to 1.
        tp->segment_progress=fmax(tp->segment_progress,0); // Limit to 0.
        if(isnan(tp->segment_progress)){ // Zero segment lenght, or duplicate segment star & end points. Triggered by a empty tail segment.
            return;
        }

        // printf("segment progress %f \n",tp->segment_progress);
        int id=tp->vector_current_exec;

        if(vector_at_id(vector_ptr,id)==sc_line){
            interpolate_line_c(vector_at(vector_ptr,id).pnt_s,
                               vector_at(vector_ptr,id).pnt_e,
                               tp->segment_progress,
                               &xyz);
        }
        if(vector_at_id(vector_ptr,id)==sc_arc){
            interpolate_arc_c(vector_at(vector_ptr,id).pnt_s,
                              vector_at(vector_ptr,id).pnt_w,
                              vector_at(vector_ptr,id).pnt_e,
                              tp->segment_progress,
                              &xyz);
        }

        tp->currentPos.tran.x=xyz.x; // Set tarpos for xyz runners.
        tp->currentPos.tran.y=xyz.y;
        tp->currentPos.tran.z=xyz.z;

        interpolate_dir_c(vector_at(vector_ptr,id).dir_s,
                          vector_at(vector_ptr,id).dir_e,
                          tp->segment_progress,
                          &abc);
        tp->currentPos.a=abc.a;
        tp->currentPos.b=abc.b;
        tp->currentPos.c=abc.c;

        interpolate_ext_c(vector_at(vector_ptr,id).ext_s,
                          vector_at(vector_ptr,id).ext_e,
                          tp->segment_progress,
                          &uvw);
        tp->currentPos.u=uvw.u;
        tp->currentPos.v=uvw.v;
        tp->currentPos.w=uvw.w;

        if(tp->enable_backend_runners==1){
            update_ruckig_runners(tp);
        }

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

inline double tpGetRapidVelMax(TP_STRUCT * const tp){
    double vm=tp->vLimit;
    vm*=emcmotStatus->rapid_scale;
    if(vm==0){ //! Avoid invalid velmax.
        vm=0.001;
    }
    return vm;
}

inline double tpGetLinearVelMaxId(TP_STRUCT * const tp, int index){
    double vm=tp->vLimit;
    vm=fmin(vector_at(vector_ptr,index).vm_gcode,vm); // Limit to given gcode feedrate.
    vm*=emcmotStatus->feed_scale;
    if(vm==0){ //! Avoid invalid velmax.
        vm=0.001;
    }
    return vm;
}

inline double tpGetArcSpeed(TP_STRUCT * const tp, const int index) {
    double radius = fmax(vector_at(vector_ptr, index).radius,0.001);
    // printf("arc radius: %f\n", radius);
    double radius_arc_sample = tp->arc_radius_sample; // Input in mm
    double vel_arc_sample = tp->arc_velocity_sample /* mm/min */ / 60.0 /* convert to mm/s */; // Input in mm/s.

    // Calculate angular speed using the formula: angular speed = linear velocity / radius
    double vel_angular = vel_arc_sample / radius_arc_sample;

    // Calculate the resulting linear velocity
    double vel_arc_result = vel_angular * radius;

    // Limit the velocity to the maximum allowed velocity
    double vm = fmin(vel_arc_result, tp->vLimit);
    vm=fmin(vector_at(vector_ptr,index).vm_gcode,vm); // Limit to given gcode feedrate.
    vm*=emcmotStatus->net_feed_scale;
    if(vm==0){ //! Avoid invalid velmax.
        vm=0.001;
    }
    // printf("arc vm in mm/s: %.2f\n", vm);
    // printf("arc vm in mm/min: %.2f\n", vm * 60.0);
    return vm;
}

inline void update_pause_grotius_scurve(TP_STRUCT * const tp){
    if(tp->pausing){ //! Check for pause, resume.
        sc.jog.enable=0;

        if(sc.guivel==0 && !tp->jog_mode){
            tp->jog_saved_pose=tp->currentPos;

            scurve_data_reset(&scx);
            scurve_data_reset(&scy);
            scurve_data_reset(&scz);
            scurve_data_reset(&sca);
            scurve_data_reset(&scb);
            scurve_data_reset(&scc);
            scurve_data_reset(&scu);
            scurve_data_reset(&scv);
            scurve_data_reset(&scw);

            scx.guipos=tp->jog_saved_pose.tran.x;
            scy.guipos=tp->jog_saved_pose.tran.y;
            scz.guipos=tp->jog_saved_pose.tran.z;
            sca.guipos=tp->jog_saved_pose.a;
            scb.guipos=tp->jog_saved_pose.b;
            scc.guipos=tp->jog_saved_pose.c;
            scu.guipos=tp->jog_saved_pose.u;
            scv.guipos=tp->jog_saved_pose.v;
            scw.guipos=tp->jog_saved_pose.w;

            tp->jog_mode=1;
        }
        if(tp->jog_mode){
            update_pause_jog_x(tp);
            update_pause_jog_y(tp);
            update_pause_jog_z(tp);
            update_pause_jog_a(tp);
            update_pause_jog_b(tp);
            update_pause_jog_c(tp);
            update_pause_jog_u(tp);
            update_pause_jog_v(tp);
            update_pause_jog_w(tp);
        }
    }

    if(!tp->pausing && tp->jog_mode){
        sc.jog.enable=0;

        // run to saved interupt pos first, then release the pause.
        update_pause_jog_resume_x(tp);
        update_pause_jog_resume_y(tp);

        if(scx.finish && scy.finish){ // First the x & y back to path, then z to path.
            update_pause_jog_resume_z(tp);
            update_pause_jog_resume_a(tp);
            update_pause_jog_resume_b(tp);
            update_pause_jog_resume_c(tp);
            update_pause_jog_resume_u(tp);
            update_pause_jog_resume_v(tp);
            update_pause_jog_resume_w(tp);
        }

        if(scx.finish && scy.finish && scz.finish && sca.finish && scb.finish &&
                scc.finish && scu.finish && scv.finish && scw.finish){
            tp->jog_mode=0;
        }
    }
}

inline void update_pause_jog_resume_x(TP_STRUCT * const tp){
    scx.tarpos=tp->jog_saved_pose.tran.x;
    scx.jog.mode=POSITION_SC;
    scx.jog.enable=1;
    if(scx.guipos>scx.tarpos){
        scx.jog.dir=REVERSE_SC;
    }
    if(scx.guipos<=scx.tarpos){
        scx.jog.dir=FORWARD_SC;
    }
    scx=axis_update(scx);
    tp->currentPos.tran.x=scx.guipos;
}

inline void update_pause_jog_resume_y(TP_STRUCT * const tp){
    scy.tarpos=tp->jog_saved_pose.tran.y;
    scy.jog.mode=POSITION_SC;
    scy.jog.enable=1;
    if(scy.guipos>scy.tarpos){
        scy.jog.dir=REVERSE_SC;
    }
    if(scy.guipos<=scy.tarpos){
        scy.jog.dir=FORWARD_SC;
    }
    scy=axis_update(scy);
    tp->currentPos.tran.y=scy.guipos;
}

inline void update_pause_jog_resume_z(TP_STRUCT * const tp){
    scz.tarpos=tp->jog_saved_pose.tran.z;
    scz.jog.mode=POSITION_SC;
    scz.jog.enable=1;
    if(scz.guipos>scz.tarpos){
        scz.jog.dir=REVERSE_SC;
    }
    if(scz.guipos<=scz.tarpos){
        scz.jog.dir=FORWARD_SC;
    }
    scz=axis_update(scz);
    tp->currentPos.tran.z=scz.guipos;
}

inline void update_pause_jog_resume_a(TP_STRUCT * const tp){
    sca.tarpos=tp->jog_saved_pose.a;
    sca.jog.mode=POSITION_SC;
    sca.jog.enable=1;
    if(sca.guipos>sca.tarpos){
        sca.jog.dir=REVERSE_SC;
    }
    if(sca.guipos<=sca.tarpos){
        sca.jog.dir=FORWARD_SC;
    }
    sca=axis_update(sca);
    tp->currentPos.a=sca.guipos;
}

inline void update_pause_jog_resume_b(TP_STRUCT * const tp){
    scb.tarpos=tp->jog_saved_pose.b;
    scb.jog.mode=POSITION_SC;
    scb.jog.enable=1;
    if(scb.guipos>scb.tarpos){
        scb.jog.dir=REVERSE_SC;
    }
    if(scb.guipos<=scb.tarpos){
        scb.jog.dir=FORWARD_SC;
    }
    scb=axis_update(scb);
    tp->currentPos.b=scb.guipos;
}

inline void update_pause_jog_resume_c(TP_STRUCT * const tp){
    scc.tarpos=tp->jog_saved_pose.c;
    scc.jog.mode=POSITION_SC;
    scc.jog.enable=1;
    if(scc.guipos>scc.tarpos){
        scc.jog.dir=REVERSE_SC;
    }
    if(scc.guipos<=scc.tarpos){
        scc.jog.dir=FORWARD_SC;
    }
    scc=axis_update(scc);
    tp->currentPos.c=scc.guipos;
}

inline void update_pause_jog_resume_u(TP_STRUCT * const tp){
    scu.tarpos=tp->jog_saved_pose.u;
    scu.jog.mode=POSITION_SC;
    scu.jog.enable=1;
    if(scu.guipos>scu.tarpos){
        scu.jog.dir=REVERSE_SC;
    }
    if(scu.guipos<=scu.tarpos){
        scu.jog.dir=FORWARD_SC;
    }
    scu=axis_update(scu);
    tp->currentPos.u=scu.guipos;
}

inline void update_pause_jog_resume_v(TP_STRUCT * const tp){
    scv.tarpos=tp->jog_saved_pose.v;
    scv.jog.mode=POSITION_SC;
    scv.jog.enable=1;
    if(scv.guipos>scv.tarpos){
        scv.jog.dir=REVERSE_SC;
    }
    if(scv.guipos<=scv.tarpos){
        scv.jog.dir=FORWARD_SC;
    }
    scv=axis_update(scv);
    tp->currentPos.v=scv.guipos;
}

inline void update_pause_jog_resume_w(TP_STRUCT * const tp){
    scw.tarpos=tp->jog_saved_pose.w;
    scw.jog.mode=POSITION_SC;
    scw.jog.enable=1;
    if(scw.guipos>scw.tarpos){
        scw.jog.dir=REVERSE_SC;
    }
    if(scw.guipos<=scw.tarpos){
        scw.jog.dir=FORWARD_SC;
    }
    scw=axis_update(scw);
    tp->currentPos.w=scw.guipos;
}

inline void update_pause_jog_x(TP_STRUCT * const tp){

    // printf("jog update. \n");
    scx.jog.mode=VELOCITY_SC;
    scx.jog.enable=1;
    scx.jermax=tp->max_jerk;
    scx.maxacc=tp->aMax;
    scx.maxvel=tpGetRapidVelMax(tp);
    scx.intval=tp->cycleTime;
    scx.finish=0;
    scx.endvel=0;

    if(tp->jog_x_plus){
        // printf("jog x plus. \n");
        scx.jog.dir=FORWARD_SC;
        scx.tarpos=INFINITY;

    } else if (tp->jog_x_min){
        // printf("jog x min. \n");
        scx.jog.dir=REVERSE_SC;
        scx.tarpos=-INFINITY;
    } else {
        // printf("jog x stop. \n");
        scx.jog.enable=0;
    }
    scx=axis_update(scx);
    tp->currentPos.tran.x=scx.guipos;
}

inline void update_pause_jog_y(TP_STRUCT * const tp){
    scy.jog.mode=VELOCITY_SC;
    scy.jog.enable=1;
    scy.jermax=tp->max_jerk;
    scy.maxacc=tp->aMax;
    scy.maxvel=tpGetRapidVelMax(tp);
    scy.intval=tp->cycleTime;
    scy.finish=0;
    scy.endvel=0;

    if(tp->jog_y_plus){
        scy.jog.dir=FORWARD_SC;
        scy.tarpos=INFINITY;
    } else if (tp->jog_y_min){
        scy.jog.dir=REVERSE_SC;
        scy.tarpos=-INFINITY;
    } else {
        scy.jog.enable=0;
    }
    scy=axis_update(scy);
    tp->currentPos.tran.y=scy.guipos;
}

inline void update_pause_jog_z(TP_STRUCT * const tp){
    scz.jog.mode=VELOCITY_SC;
    scz.jog.enable=1;
    scz.jermax=tp->max_jerk;
    scz.maxacc=tp->aMax;
    scz.maxvel=tpGetRapidVelMax(tp);
    scz.intval=tp->cycleTime;
    scz.finish=0;
    scz.endvel=0;

    if(tp->jog_z_plus){
        scz.jog.dir=FORWARD_SC;
        scz.tarpos=INFINITY;
    } else if (tp->jog_z_min){
        scz.jog.dir=REVERSE_SC;
        scz.tarpos=-INFINITY;
    } else {
        scz.jog.enable=0;
    }
    scz=axis_update(scz);
    tp->currentPos.tran.z=scz.guipos;
}

inline void update_pause_jog_a(TP_STRUCT * const tp){
    sca.jog.mode=VELOCITY_SC;
    sca.jog.enable=1;
    sca.jermax=tp->max_jerk;
    sca.maxacc=tp->aMax;
    sca.maxvel=tpGetRapidVelMax(tp);
    sca.intval=tp->cycleTime;
    sca.finish=0;
    sca.endvel=0;

    if(tp->jog_a_plus){
        sca.jog.dir=FORWARD_SC;
        sca.tarpos=INFINITY;
    } else if (tp->jog_a_min){
        sca.jog.dir=REVERSE_SC;
        sca.tarpos=-INFINITY;
    } else {
        sca.jog.enable=0;
    }
    sca=axis_update(sca);
    tp->currentPos.a=sca.guipos;
}

inline void update_pause_jog_b(TP_STRUCT * const tp){
    scb.jog.mode=VELOCITY_SC;
    scb.jog.enable=1;
    scb.jermax=tp->max_jerk;
    scb.maxacc=tp->aMax;
    scb.maxvel=tpGetRapidVelMax(tp);
    scb.intval=tp->cycleTime;
    scb.finish=0;
    scb.endvel=0;

    if(tp->jog_b_plus){
        scb.jog.dir=FORWARD_SC;
        scb.tarpos=INFINITY;
    } else if (tp->jog_b_min){
        scb.jog.dir=REVERSE_SC;
        scb.tarpos=-INFINITY;
    } else {
        scb.jog.enable=0;
    }
    scb=axis_update(scb);
    tp->currentPos.b=scb.guipos;
}

inline void update_pause_jog_c(TP_STRUCT * const tp){
    scc.jog.mode=VELOCITY_SC;
    scc.jog.enable=1;
    scc.jermax=tp->max_jerk;
    scc.maxacc=tp->aMax;
    scc.maxvel=tpGetRapidVelMax(tp);
    scc.intval=tp->cycleTime;
    scc.finish=0;
    scc.endvel=0;

    if(tp->jog_c_plus){
        scc.jog.dir=FORWARD_SC;
        scc.tarpos=INFINITY;
    } else if (tp->jog_c_min){
        scc.jog.dir=REVERSE_SC;
        scc.tarpos=-INFINITY;
    } else {
        scc.jog.enable=0;
    }
    scc=axis_update(scc);
    tp->currentPos.c=scc.guipos;
}

inline void update_pause_jog_u(TP_STRUCT * const tp){
    scu.jog.mode=VELOCITY_SC;
    scu.jog.enable=1;
    scu.jermax=tp->max_jerk;
    scu.maxacc=tp->aMax;
    scu.maxvel=tpGetRapidVelMax(tp);
    scu.intval=tp->cycleTime;
    scu.finish=0;
    scu.endvel=0;

    if(tp->jog_u_plus){
        scu.jog.dir=FORWARD_SC;
        scu.tarpos=INFINITY;
    } else if (tp->jog_u_min){
        scu.jog.dir=REVERSE_SC;
        scu.tarpos=-INFINITY;
    } else {
        scu.jog.enable=0;
    }
    scu=axis_update(scu);
    tp->currentPos.u=scu.guipos;
}

inline void update_pause_jog_v(TP_STRUCT * const tp){
    scv.jog.mode=VELOCITY_SC;
    scv.jog.enable=1;
    scv.jermax=tp->max_jerk;
    scv.maxacc=tp->aMax;
    scv.maxvel=tpGetRapidVelMax(tp);
    scv.intval=tp->cycleTime;
    scv.finish=0;
    scv.endvel=0;

    if(tp->jog_v_plus){
        scv.jog.dir=FORWARD_SC;
        scv.tarpos=INFINITY;
    } else if (tp->jog_v_min){
        scv.jog.dir=REVERSE_SC;
        scv.tarpos=-INFINITY;
    } else {
        scv.jog.enable=0;
    }
    scv=axis_update(scv);
    tp->currentPos.v=scv.guipos;
}

inline void update_pause_jog_w(TP_STRUCT * const tp){
    scw.jog.mode=VELOCITY_SC;
    scw.jog.enable=1;
    scw.jermax=tp->max_jerk;
    scw.maxacc=tp->aMax;
    scw.maxvel=tpGetRapidVelMax(tp);
    scw.intval=tp->cycleTime;
    scw.finish=0;
    scw.endvel=0;

    if(tp->jog_w_plus){
        scw.jog.dir=FORWARD_SC;
        scw.tarpos=INFINITY;
    } else if (tp->jog_w_min){
        scw.jog.dir=REVERSE_SC;
        scw.tarpos=-INFINITY;
    } else {
        scw.jog.enable=0;
    }
    scw=axis_update(scw);
    tp->currentPos.w=scw.guipos;
}

int count=0;
int init_first_fillet=0;
//! A Inline functinn is compiled in between the upper-level function. So
//! its not called every time, but compiled inbetween. This makes it faster.
inline void update_scurve_grotius(TP_STRUCT * const tp){

    // printf("running scurve.\n");

    // printf("vector size %d \n",vector_size_c(vector_ptr));

    if(vector_size_c(vector_ptr)>2 && !init_first_fillet){
        // printf("setting first fillet. \n");
        vector_run_cavalier(vector_ptr,0,tp->blend_radius_G0,tp->blend_radius_G1_G2_G3,tp->enable_chamfer);
        vector_update_pathlenghts(vector_ptr);
        init_first_fillet=1;
    }

    if(vector_size_c(vector_ptr)>0){  //! Check if there is gcode content.

        tp->gcode_current_executed_line_nr=vector_at(vector_ptr,tp->vector_current_exec).gcode_line_nr;

        sc.jog.mode=POSITION_SC;
        sc.jog.enable=1;
        sc.jermax=tp->max_jerk;
        sc.maxacc=tp->aMax;

        sc.maxvel=fmax(vector_at(vector_ptr,tp->vector_current_exec).vm,0.001); // fmin(tpGetLinearVelMax(tp), vector_at(vector_ptr,tp->vector_current_exec).vm);
        sc.intval=tp->cycleTime;
        sc.finish=0;

        if(!tp->reverse_run){
            sc.jog.dir=FORWARD_SC;
            sc.tarpos=vector_at(vector_ptr,tp->vector_current_exec).path_end;
            sc.endvel=vector_at(vector_ptr,tp->vector_current_exec).ve;
        } else {
            sc.jog.dir=REVERSE_SC;
            sc.tarpos=vector_at(vector_ptr,tp->vector_current_exec).path_begin;
            sc.endvel=0;
        }

        // Overwrite above values if no look ahead is active.
        if(tp->enable_look_ahead==0){
            sc.endvel=0;
            sc.endacc=0;
            sc.maxvel=tp->vLimit;
            if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_rapid){
                sc.maxvel=tpGetRapidVelMax(tp);
            }
            if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_circle){
                sc.maxvel=tpGetArcSpeed(tp,tp->vector_current_exec);
            }
            if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_linear){
                sc.maxvel=tpGetLinearVelMaxId(tp,tp->vector_current_exec);
            }
        }

        update_pause_grotius_scurve(tp);

        // if(sc.endvel==0){
        sc=axis_update(sc);
        // } else {
        //     sc=axis_update_endve(sc); // Solved the spikes on joint[n] output when envel>0.
        // }

        /*
        if(count>1000){ // Lower the printf to once a second.
            printf("return code: %i \n",sc.rcode);
            printf("segment progress %f \n",tp->segment_progress);

            count=0;
        }
        count++; */

        if(sc.finish){

            //! We are finished and completed the last gcode segment. Traject is done !
            if(tp->vector_current_exec==vector_size_c(vector_ptr)-1){
                scurve_data_reset(&sc);
                vector_clear(vector_ptr);
                tp->gcode_lastPos=tp->currentPos;
                tp->traject_lenght=0;

                init_first_fillet=0;
            }
            //! Set next or previous gcode segment.
            if(tp->vector_current_exec<vector_size_c(vector_ptr)-1){

                if(!tp->reverse_run){
                    tp->vector_current_exec++;

                    if(!vector_at(vector_ptr,tp->vector_current_exec).is_fillet_segment){
                        vector_run_cavalier(vector_ptr,tp->vector_current_exec,tp->blend_radius_G0,tp->blend_radius_G1_G2_G3,tp->enable_chamfer);
                        vector_update_pathlenghts(vector_ptr);
                    }
                } else {
                    if(tp->vector_current_exec>0){
                        tp->vector_current_exec--;
                    }
                }

            }
        }
    }
}

inline void update_scurve_ruckig(TP_STRUCT * const tp){

    if(vector_size_c(vector_ptr)>0){  //! Check if there is gcode content.

        tp->gcode_current_executed_line_nr=vector_at(vector_ptr,tp->vector_current_exec).gcode_line_nr;

        if(tp->pausing || rtp.maxvel==0 ){
            rtp.interfacetype=velocity;
        } else {
            rtp.interfacetype=position;
        }

        rtp.enable=1;
        rtp.durationdiscretizationtype=Discrete;
        rtp.synchronizationtype=None;
        rtp.maxacc=tp->aMax;
        rtp.maxjerk=tp->max_jerk;

        rtp.maxvel=fmax(vector_at(vector_ptr,tp->vector_current_exec).vm,0.001);
        rtp.period=tp->cycleTime;

        rtp.taracc=0;
        rtp.tarvel=0;

        if(!tp->reverse_run){
            rtp.tarpos=vector_at(vector_ptr,tp->vector_current_exec+fmax(tp->look_ahead_buffer-1,0)).path_end;
            rtp.tarvel=0;
        } else {
            rtp.tarpos=vector_at(vector_ptr,tp->vector_current_exec).path_begin;
            rtp.tarvel=0;
        }

        // Overwrite above values if no look ahead is active.
        if(tp->enable_look_ahead==0){
            rtp.taracc=0;
            rtp.tarvel=0;
            rtp.maxvel=tp->vLimit;

            if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_rapid){
                rtp.tarvel=tpGetRapidVelMax(tp);
            }
            if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_circle){
                rtp.tarvel=tpGetArcSpeed(tp,tp->vector_current_exec);
            }
            if(vector_at(vector_ptr,tp->vector_current_exec).motion_type==sc_linear){
                rtp.tarvel=tpGetLinearVelMaxId(tp,tp->vector_current_exec);
            }
        }

        // update_pause_grotius_scurve(tp);

        rtp.taracc=0;
        rtp=wrapper_get_pos(rtp);
        sc.guipos=rtp.curpos;

        printf("rtp curpos: %f rtp tarpos: %f rtp return code: %d \n",rtp.curpos,rtp.tarpos,rtp.function_return_code);

        if(rtp.curpos>vector_at(vector_ptr,tp->vector_current_exec).path_end){
            rtp.function_return_code=Finished;
        }

        if(rtp.function_return_code==Finished){

            //! We are finished and completed the last gcode segment. Traject is done !
            if(tp->vector_current_exec==vector_size_c(vector_ptr)-1){
                vector_clear(vector_ptr);
                tp->gcode_lastPos=tp->currentPos;
                tp->traject_lenght=0;

                sc.guipos=0;
                rtp.curpos=0;
            }
            //! Set next or previous gcode segment.
            if(tp->vector_current_exec<vector_size_c(vector_ptr)-1){

                if(!tp->reverse_run){
                    tp->vector_current_exec++;
                } else {
                    if(tp->vector_current_exec>0){
                        tp->vector_current_exec--;
                    }
                }
            }
        }
    }
}

inline double endvel_scurve(TP_STRUCT * const tp, int index, double pathlenght, double vo){

    struct scurve_data d;
    scurve_data_reset(&d);

    d.jermax=tp->max_jerk;
    d.maxacc=tp->aMax;
    d.maxvel=tp->vLimit;
    d.endvel=tp->vLimit;
    d.intval=0.01;
    d.endacc=0;
    d.guiacc=0;
    d.guipos=0;
    d.guivel=vo;
    d.jog.enable=1;
    d.tarpos=INFINITY;
    d.jog.dir=FORWARD_SC;
    d.jog.mode=POSITION_SC;
    d.finish=0;

    // Construct a curve with zero endacc. This will produce the full curve end vel at end of curve.
    d=forward_curve_build_c(d);
    double ttot=ttot_period_c(d.c0)+ttot_period_c(d.c1)+ttot_period_c(d.c2)+ttot_period_c(d.c3);
    double ve=0, endacc=0;
    // printf("ttot: %f \n",ttot);

    for(double i=0; i<ttot; i+=d.intval){
        ve=d.guivel;
        endacc=d.guiacc;
        // printf("vel: %f, pos: %f, acc: %f\n",d.guivel, d.guipos, d.guiacc);
        d=jog_update_c(d);

        if(d.guipos>pathlenght){
            break;
        }
    }

    // Limit ve to gcode feedrate, gui velocity overide, motion type etc.
    if(vector_at(vector_ptr,index).motion_type==sc_rapid){
        ve=fmin(ve,tpGetRapidVelMax(tp));
    }
    if(vector_at(vector_ptr,index).motion_type==sc_linear){
        ve=fmin(ve,tpGetLinearVelMaxId(tp,index));
    }
    if(vector_at(vector_ptr,index).motion_type==sc_circle){
        ve=fmin(ve,tpGetArcSpeed(tp,index));
    }
    return ve;
}

inline double endvel_trapezium(TP_STRUCT * const tp, int index, double vo, double pathlength) {

    if (tp->aMax < 0 || pathlength < 0) {
        return 0;
    }

    // Calculate the end velocity using the kinematic equation
    double ve = sqrt(vo * vo + 2 * tp->aMax * (pathlength/1000));

    if(vector_at(vector_ptr,index).motion_type==sc_rapid){
        ve=fmin(ve,tpGetRapidVelMax(tp));
    }
    if(vector_at(vector_ptr,index).motion_type==sc_linear){
        ve=fmin(ve,tpGetLinearVelMaxId(tp,index));
    }
    if(vector_at(vector_ptr,index).motion_type==sc_circle){
        ve=fmin(ve,tpGetArcSpeed(tp,index));
    }
    return ve;
}

inline void update_look_ahead(TP_STRUCT * const tp){

    if(vector_size_c(vector_ptr)==0){
        return;
    }

    int vec_size=vector_size_c(vector_ptr);
    int look_ahead=tp->look_ahead_lines;
    int current_exec=tp->vector_current_exec;

    // We set the vm at runtime. Let the acceleration transitions be controlled by the scurve engine.

    // printf("current exec nr: %d \n",current_exec);

    tp->look_ahead_buffer=0;
    for(int i=current_exec; i<fmin(vec_size-1,current_exec+look_ahead); i++){

        tp->look_ahead_buffer++;
        // printf("look fwd, current seg looking at: %d \n",i);

        // Populate vector with current gui values for vm, set vo and vo to zero.
        if(vector_at(vector_ptr,i).motion_type==sc_rapid){
            // vector_set_vm(vector_ptr,i,tpGetRapidVelMax(tp));
            vector_set_vm(vector_ptr,i,tpGetLinearVelMaxId(tp,i));
        }
        if(vector_at(vector_ptr,i).motion_type==sc_linear){
            vector_set_vm(vector_ptr,i,tpGetLinearVelMaxId(tp,i));
        }
        if(vector_at(vector_ptr,i).motion_type==sc_circle){
            // vector_set_vm(vector_ptr,i,tpGetArcSpeed(tp,i));
            vector_set_vm(vector_ptr,i,tpGetLinearVelMaxId(tp,i));
        }

        if(vector_at(vector_ptr,i).motion_type==sc_rapid){
            break;
        }

        if(vector_at(vector_ptr,i).angle_end<2 && vector_at(vector_ptr,i).path_lenght<10){ // Look if this will improve motion for short segments.
            vector_set_tangent_or_colineair(vector_ptr,i,1);
        }

        if(!vector_at(vector_ptr,i).next_segment_is_tangent_or_colinear){
            break;
        }
    }

    // printf("buffer: %d \n",tp->look_ahead_buffer);
    look_ahead_optimizer(tp,tp->look_ahead_buffer);
}

// Derived from look_aheadskeleton.h sample code.
// This code run's forward & reverse over the n ammount of gcode segments.
// It set's the lowest value in the reverse cycle.
inline void look_ahead_optimizer(TP_STRUCT * const tp, int look_ahead_lines){

    int vsize = vector_size_c(vector_ptr);
    int start = tp->vector_current_exec; // Simulate current index.
    int buffer = look_ahead_lines; // The size to look fwd & rev.
    int maxend;
    int use_trapezium=1;

    maxend = fmin(start + buffer, vsize);
    // Forward loop over svec
    for (int i = start; i < maxend; ++i) {
        if(use_trapezium==1){
            double ve=endvel_trapezium(tp,i,vector_at(vector_ptr,i).vo,vector_at(vector_ptr,i).path_lenght);

            int vec_size=vector_size_c(vector_ptr);
            if(i<vec_size-1){
                if(vector_at(vector_ptr,i+1).motion_type==sc_circle){
                    ve=fmin(ve,tpGetArcSpeed(tp,i+1));
                }
            }

            vector_set_ve(vector_ptr,i, /*vector_at(vector_ptr,i).vo + 2*/ ve);
        } else {
            vector_set_ve(vector_ptr,i, endvel_scurve(tp,i,vector_at(vector_ptr,i).path_lenght,vector_at(vector_ptr, i).vo));
        }

        if (i < maxend - 1) {
            vector_set_vo(vector_ptr,i+1,vector_at(vector_ptr,i).ve);
        }
        // printf("FWD Segment %d: vo = %.2f, ve = %.2f, lenght = %.2f\n", i, vector_at(vector_ptr, i).vo, vector_at(vector_ptr, i).ve, vector_at(vector_ptr, i).path_lenght);
    }

    maxend = fmin(start + buffer -1, vsize -1);
    // Reverse loop over svec
    for (int i = maxend; i >= start && i >= 0; --i) {
        if (i == start + buffer - 1) {
            vector_set_ve(vector_ptr,i,0);
        }
        if (i > start) {
            double fwd = vector_at(vector_ptr, i).vo;
            double rev=0;
            if(use_trapezium==1){
                rev = vector_at(vector_ptr,i).ve + endvel_trapezium(tp,i,vector_at(vector_ptr,i).ve,vector_at(vector_ptr,i).path_lenght);
            } else {
                rev = vector_at(vector_ptr,i).ve + endvel_scurve(tp,i,vector_at(vector_ptr,i).path_lenght,vector_at(vector_ptr, i).ve);
            }

            vector_set_vo(vector_ptr,i,fmin(fwd, rev));
            vector_set_ve(vector_ptr,i-1,vector_at(vector_ptr,i).vo);
        }
        if (i == start) {
            double fwd = vector_at(vector_ptr, i).vo;
            double rev=0;
            if(use_trapezium==1){
                rev = vector_at(vector_ptr,i).ve + endvel_trapezium(tp,i,vector_at(vector_ptr,i).ve,vector_at(vector_ptr,i).path_lenght);
            } else {
                rev = vector_at(vector_ptr,i).ve + endvel_scurve(tp,i,vector_at(vector_ptr,i).path_lenght,vector_at(vector_ptr, i).ve);
            }
            vector_set_vo(vector_ptr,i,fmin(fwd, rev));

        }
        // printf("REV Segment %d: vo = %.2f, ve = %.2f, lenght = %.2f\n", i, vector_at(vector_ptr, i).vo, vector_at(vector_ptr, i).ve, vector_at(vector_ptr, i).path_lenght);
    }
    // printf("Segment %d: vo = %.2f, ve = %.2f, lenght = %.2f\n", start, vector_at(vector_ptr, start).vo, vector_at(vector_ptr, start).ve, vector_at(vector_ptr, start).path_lenght);
}

int startup_init=0;
//! A Inline functinn is compiled in between the upper-level function. So
//! its not called every time, but compiled inbetween. This makes it faster.
inline void update_hal(TP_STRUCT * const tp){

    //! For info, a pin uses a * before, a parameter not.
    tp->reverse_run=*hal_reverse_run->Pin;
    tp->enable_chamfer=*hal_enable_chamfer->Pin;
    *hal_current_gcode_line->Pin=tp->gcode_current_executed_line_nr;
    *hal_segment_progress->Pin=tp->segment_progress;

    if(tp->enable_grotius_scurve){
        *hal_guipos->Pin=sc.guipos;
        *hal_guivel->Pin=sc.guivel*60;
        *hal_guiacc->Pin=sc.guiacc;
    } else {
        *hal_guipos->Pin=rtp.curpos;
        *hal_guivel->Pin=rtp.curvel*60;
        *hal_guiacc->Pin=rtp.curacc;
    }

    if(*hal_enable_look_ahead->Pin==0 && !startup_init){
        *hal_enable_look_ahead->Pin=DEFAULT_ENABLE_LOOK_AHEAD;
    }
    tp->enable_look_ahead=*hal_enable_look_ahead->Pin;

    if(*hal_maxjerk->Pin==0 && !startup_init){
        *hal_maxjerk->Pin=DEFAULT_MAX_JERK_TP;
    }
    tp->max_jerk=*hal_maxjerk->Pin;

    if(*hal_look_ahead_lines->Pin==0 && !startup_init){
        *hal_look_ahead_lines->Pin=DEFAULT_LOOK_AHEAD_LINES;
    }
    tp->look_ahead_lines=*hal_look_ahead_lines->Pin;

    if(*hal_ruckig_jerk->Pin==0 && !startup_init){
        *hal_ruckig_jerk->Pin=DEFAULT_MAX_JERK_RUCKIG_RUNNERS;
    }
    tp->ruckig_jerk=*hal_ruckig_jerk->Pin;

    if(*hal_enable_runners->Pin==0 && !startup_init){
        *hal_enable_runners->Pin=DEFAULT_ENABLE_RUCKIG_RUNNERS;
    }
    tp->enable_backend_runners=*hal_enable_runners->Pin;

    if(*hal_arc_radius_sample->Pin==0 && !startup_init){
        *hal_arc_radius_sample->Pin=DEFAULT_ARC_RADIUS_SAMPLE;
    }
    tp->arc_radius_sample=*hal_arc_radius_sample->Pin;

    if(*hal_arc_velocity_sample->Pin==0 && !startup_init){
        *hal_arc_velocity_sample->Pin=DEFAULT_ARC_VELOCITY_SAMPLE;
    }
    tp->arc_velocity_sample=*hal_arc_velocity_sample->Pin;

    if(*hal_enable_grotius_scurve->Pin==0 && !startup_init){
        *hal_enable_grotius_scurve->Pin=DEFAULT_ENABLE_GROTIUS_SCURVE;
    }
    tp->enable_grotius_scurve=*hal_enable_grotius_scurve->Pin;


    if(*hal_blend_radius_G0->Pin==0 && !startup_init){
        *hal_blend_radius_G0->Pin=DEFAULT_BLEND_RADIUS_G0;
    }
    tp->blend_radius_G0=*hal_blend_radius_G0->Pin;

    if(*hal_blend_radius_G1_G2_G3->Pin==0 && !startup_init){
        *hal_blend_radius_G1_G2_G3->Pin=DEFAULT_BLEND_RADIUS_G1_G2_G3;
    }
    tp->blend_radius_G1_G2_G3=*hal_blend_radius_G1_G2_G3->Pin;


    tp->jog_x_plus=*hal_jog_x_plus->Pin;
    tp->jog_x_min=*hal_jog_x_min->Pin;

    tp->jog_y_plus=*hal_jog_y_plus->Pin;
    tp->jog_y_min=*hal_jog_y_min->Pin;

    tp->jog_z_plus=*hal_jog_z_plus->Pin;
    tp->jog_z_min=*hal_jog_z_min->Pin;

    tp->jog_a_plus=*hal_jog_a_plus->Pin;
    tp->jog_a_min=*hal_jog_a_min->Pin;

    tp->jog_b_plus=*hal_jog_b_plus->Pin;
    tp->jog_b_min=*hal_jog_b_min->Pin;

    tp->jog_c_plus=*hal_jog_c_plus->Pin;
    tp->jog_c_min=*hal_jog_c_min->Pin;

    tp->jog_u_plus=*hal_jog_u_plus->Pin;
    tp->jog_u_min=*hal_jog_u_min->Pin;

    tp->jog_v_plus=*hal_jog_v_plus->Pin;
    tp->jog_v_min=*hal_jog_v_min->Pin;

    tp->jog_w_plus=*hal_jog_w_plus->Pin;
    tp->jog_w_min=*hal_jog_w_min->Pin;

    startup_init=1;
}


EXPORT_SYMBOL(tpMotFunctions);
EXPORT_SYMBOL(tpMotData);

EXPORT_SYMBOL(tpSetMaxJerk);
EXPORT_SYMBOL(tpAbort);
EXPORT_SYMBOL(tpActiveDepth);
EXPORT_SYMBOL(tpAddCircle);
EXPORT_SYMBOL(tpAddLine);
EXPORT_SYMBOL(tpAddGeneralMotion);
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






































