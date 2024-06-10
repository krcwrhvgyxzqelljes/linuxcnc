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

/* module information */
MODULE_AUTHOR("Skynet_Cyberdyne");
MODULE_DESCRIPTION("tpmod_scurve_c++");
MODULE_LICENSE("GPL2");

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;

//! Pins
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;

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

static int comp_idx; /* component ID */

static void the_function();
static int setup_pins();

int rtapi_app_main(void) {

    int r = 0;
    comp_idx = hal_init("tpmod_scurve_cplusplus");
    if(comp_idx < 0) return comp_idx;
    r = hal_export_funct("tpmod_scurve_cplusplus", the_function, &skynet,0,0,comp_idx);

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

struct vector *ptr; // Pointer to vector c++ class.
extern struct vector* vector_init_ptr();
extern void vector_add_segment(struct vector *ptr, struct segment b);
extern void vector_pop_back(struct vector *ptr);
extern int vector_add_circle( TP_STRUCT * const tp,
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
                              struct state_tag_t tag,
                              struct vector *ptr);
extern int vector_add_line(TP_STRUCT *
                    const tp,
                    EmcPose end,
                    int canon_motion_type,
                    double vel,
                    double ini_maxvel,
                    double acc,
                    unsigned char enables,
                    char atspeed,
                    int indexer_jnum,
                    struct state_tag_t tag,
                    struct vector *ptr);




//! Create a empty queue.
int tpInit(TP_STRUCT * const tp){
    printf("tpInit. \n");

    return 0;
}

int tpRunCycle(TP_STRUCT * const tp, long period){
    //! printf("tpRunCycle. \n");
    tp->cycleTime=period;
    tp->cycleTime*=0.000000001; //! Convert to ms.

    return 0;
}

//! The first function call.
int tpCreate(TP_STRUCT * const tp, int _queueSize,int id){
    if (_queueSize <= 0) {
        tp->queueSize = TP_DEFAULT_QUEUE_SIZE;
    } else {
        tp->queueSize = _queueSize;
    }

    //! Set the queue size to the c++ vector.
    ptr=vector_init_ptr();

    printf("tpCreate. set tp->queuesize to: %i \n", tp->queueSize);
    return 0;
}

int tpClear(TP_STRUCT * const tp){

    printf("tpClear. \n");
    tp->traject_lenght=0;
    tp->gcode_lastPos=tp->currentPos;
    // vector_clear(vector_ptr);
    // vector_ptr=NULL;

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

int tpPause(TP_STRUCT * const tp){
    printf("tpPause. \n");
    tp->pausing=1;
    return 0;
}

int tpResume(TP_STRUCT * const tp){
    printf("tpResume. \n");
    tp->pausing=0;
    return 0;
}

int tpAbort(TP_STRUCT * const tp){
    printf("tpAbort. \n");
    vector_clear(ptr);
    tp->traject_lenght=0;
    tp->gcode_lastPos=tp->currentPos;
    return 0;
}

int tpGetMotionType(TP_STRUCT * const tp){
    return tp->motionType;
}

int tpIsDone(TP_STRUCT * const tp){
    if(vector_size(ptr)==0){
        return 1;
    }
    return 0;
}

int tpQueueDepth(TP_STRUCT * const tp){
    return 0;
}

int tpActiveDepth(TP_STRUCT * const tp){
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
    printf("tpA ddLine \n");
    return vector_add_line(tp,end,canon_motion_type,vel,ini_maxvel,acc,enables,atspeed,indexer_jnum,tag,ptr);
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
    return vector_add_circle(tp,end,center,normal,turn,canon_motion_type,vel,ini_maxvel,acc,enables,atspeed,tag,ptr);
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






































