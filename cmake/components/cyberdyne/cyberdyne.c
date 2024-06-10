#include "rtapi.h"
#include "rtapi_ctype.h"
#include "rtapi_app.h"
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "rtapi_math64.h"
#include <rtapi_io.h>
#include "hal.h"
#include "ruckig_dev_format.h"
#include "ruckig_dev_interface.h"
#include "stdio.h"

/* module information */
MODULE_AUTHOR("Skynet");
MODULE_DESCRIPTION("Halmodule cyberdyne"
"Testing the ruckig-dev scurve library to use velocity end values.");
MODULE_LICENSE("GPL");

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *vel, *pos, *acc, *return_code;
float_data_t *maxjerk, *maxacc, *tarpos, *tarvel, *cycletime, *tarvel, *maxvel;
float_data_t *waypoint;

//! Pins
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *module, *pause_, *reverse, *setposzero;

typedef struct {
    hal_s32_t *Pin;
} s32_data_t;

typedef struct {
    hal_u32_t *Pin;
} u32_data_t;

typedef struct {
    hal_port_t *Pin;
} port_data_t;
port_data_t *port;

//! Params
typedef struct {
    hal_float_t Pin;
} param_float_data_t;

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;

static int comp_idx; /* component ID */

static void the_function();
static int setup_pins();
void set_ruckig_values();
void set_ruckig_waypoints();

struct ruckig_c_data r,s;
struct ruckig_dev_interface *ruckig_ptr;
extern ruckig_dev_interface* ruckig_init_ptr();
extern struct ruckig_c_data ruckig_calculate_c(struct ruckig_c_data in);
extern void ruckig_add_waypoint(ruckig_dev_interface *ptr, struct ruckig_c_waypoint point);
extern struct ruckig_c_waypoint ruckig_get_waypoint(ruckig_dev_interface *ptr, int index);
extern int ruckig_waypoint_vector_size(ruckig_dev_interface *ptr);

int waypoint_nr;
bool mode_auto;

int rtapi_app_main(void) {

    int r = 0;
    comp_idx = hal_init("cyberdyne");
    if(comp_idx < 0) return comp_idx;
    r = hal_export_funct("the_function", the_function, &skynet,0,0,comp_idx);

    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    printf("component %i ready.\n",comp_idx);

    ruckig_ptr=ruckig_init_ptr();

    set_ruckig_values();
    set_ruckig_waypoints();

    //! Return 0=ok.
    return r;
}

void set_ruckig_values(){

    //! Set some values if using in manual mode, set mode_auto=0;
    *maxvel->Pin=50;
    *maxjerk->Pin=1100;
    *maxacc->Pin=500;
    *tarpos->Pin=300;
    *tarvel->Pin=10;
    *cycletime->Pin=0.001;
}

void set_ruckig_waypoints(){

    //! A little trajectory using a few waypoints, using velocity end values.
    struct ruckig_c_waypoint waypoint;

    waypoint.starpos=0;
    waypoint.goalpos=200;
    waypoint.vo=0;
    waypoint.ve=25;
    ruckig_add_waypoint(ruckig_ptr,waypoint);

    waypoint.starpos=200;
    waypoint.goalpos=500;
    waypoint.vo=25;
    waypoint.ve=10;
    ruckig_add_waypoint(ruckig_ptr,waypoint);

    waypoint.starpos=500;
    waypoint.goalpos=1000;
    waypoint.vo=10;
    waypoint.ve=0;
    ruckig_add_waypoint(ruckig_ptr,waypoint);

    printf("waypoint vector size: %i \n",ruckig_waypoint_vector_size(ruckig_ptr));

    waypoint_nr=0;

    //! Turn off to use halpin values, see : set_ruckig_values();
    mode_auto=1;
}

void rtapi_app_exit(void){
    //! Free memory of the ruckig pointer.
    ruckig_ptr=NULL;
    hal_exit(comp_idx);
}

//! Perforn's every ms.
static void the_function(){

    *vel->Pin=r.newvel; //! Update actual tp value to show in halscope.
    *acc->Pin=r.newacc; //!
    *pos->Pin=r.newpos; //!

    //! Everything is ok.
    *return_code->Pin=r.function_return_code;

    r.enable=1; //! Enable ruckig.
    r.control_interfacetype=position; //! For normal usage, for pause we use type velocity.
    r.durationdiscretizationtype=Continuous; //! Every trajectory duration is allowed (Default)
    r.synchronizationtype=None; //! Calculate every DoF independently

    r.cycletime=*cycletime->Pin;    // 0.001
    r.maxacc=*maxacc->Pin;          // 500
    r.maxjerk=*maxjerk->Pin;        // 1100
    r.maxvel=*maxvel->Pin;          // 50
    r.pause=*pause_->Pin; //! Pause pin.
    r.reverse=*reverse->Pin; //! Motion reverse pin.

    //! If used for manual hal pin inputs.
    if(!mode_auto){
        r.tarpos=*tarpos->Pin;
        r.tarvel=*tarvel->Pin;
    } else {
        //! Use in auto mode, using waypoints.
        if(!r.reverse){ //! Motion forward, goto goalpos of segment.
            r.tarpos=ruckig_get_waypoint(ruckig_ptr,waypoint_nr).goalpos;
            r.tarvel=ruckig_get_waypoint(ruckig_ptr,waypoint_nr).ve;
        } else { //! Motion reverse, goto begin of segment.
            r.tarpos=ruckig_get_waypoint(ruckig_ptr,waypoint_nr).starpos;
            r.tarvel=ruckig_get_waypoint(ruckig_ptr,waypoint_nr).vo;
        }
    }

    //! Hard reset. Tp planner uses this to start from new queue.
    if(*setposzero->Pin){
        r.reset=1;
        waypoint_nr=0;
        *setposzero->Pin=0;
    }

    //! Perform the ruckig scurve traject calculation.
    r=ruckig_calculate_c(r);

    //! Returncode 1=finished.
    if(r.function_return_code==1){

        //! Size of the waypoint bucket.
        int size=ruckig_waypoint_vector_size(ruckig_ptr);

        //! Motion forward, increment to next segment.
        if(!r.reverse){
            if(waypoint_nr<size-1){
                waypoint_nr++;
                // printf("waypoint forward nr: %i \n",waypoint_nr);
            }
        } else { //! Motion reverse, decrement on segment.
            if(waypoint_nr>0){
                waypoint_nr--;
                // printf("waypoint reverse nr: %i \n",waypoint_nr);
            }
        }
    }
    //! Show the outside world wich waypoint we are.
    *waypoint->Pin=waypoint_nr;
}

//! Setup hal pins to interact outside the golden ring.
static int setup_pins(){
    int r=0;

    module = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("cyberdyne.enable",HAL_IN,&(module->Pin),comp_idx);

    pause_ = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("cyberdyne.pause",HAL_IN,&(pause_->Pin),comp_idx);

    reverse = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("cyberdyne.reverse",HAL_IN,&(reverse->Pin),comp_idx);

    vel = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.vel",HAL_OUT,&(vel->Pin),comp_idx);
    
    acc = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.acc",HAL_OUT,&(acc->Pin),comp_idx);

    pos = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.pos",HAL_OUT,&(pos->Pin),comp_idx);

    return_code = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.return_code",HAL_OUT,&(return_code->Pin),comp_idx);

    tarvel = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.tarvel_manual_mode",HAL_IN,&(tarvel->Pin),comp_idx);

    maxvel = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.maxvel",HAL_IN,&(maxvel->Pin),comp_idx);

    maxacc = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.maxacc",HAL_IN,&(maxacc->Pin),comp_idx);

    maxjerk = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.maxjerk",HAL_IN,&(maxjerk->Pin),comp_idx);

    tarpos = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.tarpos_manual_mode",HAL_IN,&(tarpos->Pin),comp_idx);

    cycletime = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.cycletime",HAL_IN,&(cycletime->Pin),comp_idx);

    waypoint = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("cyberdyne.waypoint",HAL_OUT,&(waypoint->Pin),comp_idx);

    setposzero = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("cyberdyne.setposzero",HAL_IN,&(setposzero->Pin),comp_idx);

    return r;
}







































