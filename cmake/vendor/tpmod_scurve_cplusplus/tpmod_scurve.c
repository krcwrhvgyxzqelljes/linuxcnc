#include "rtapi.h"
#include "rtapi_ctype.h"
#include "rtapi_app.h"
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "rtapi_math64.h"
#include <rtapi_io.h>
#include "stdio.h"

#include "hal_section.h"
#include "emc_section.h"
#include "deb_section.h"

struct vector *ptr; // Pointer to vector c++ class.
extern struct vector* vector_init_ptr();
extern void vector_pop_back(struct vector *ptr);
extern int vector_size(struct vector *ptr);
extern void vector_clear(TP_STRUCT * const tp, struct vector *ptr);
extern int vector_add_circle(TP_STRUCT * const tp,
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
extern int vector_run_cycle(TP_STRUCT * const tp, long period, struct vector *ptr);
extern int vector_interpolate_segment(TP_STRUCT * const tp, int vector_id, double progress, struct vector *ptr);

//! Create a empty queue.
int tpInit(TP_STRUCT * const tp){

    print("init. \n");
    return 0;
}

int tpRunCycle(TP_STRUCT * const tp, long period){
    return vector_run_cycle(tp, period,ptr);
}

//! The first function call.
int tpCreate(TP_STRUCT * const tp, int _queueSize,int id){
    // printf("tpCreate. set tp->queuesize to: %i \n", tp->queueSize);

    if (_queueSize <= 0) {
        tp->queueSize = TP_DEFAULT_QUEUE_SIZE;
    } else {
        tp->queueSize = _queueSize;
    }

    ptr=vector_init_ptr();
    return 0;
}

int tpClear(TP_STRUCT * const tp){

    print("clear. \n");
    vector_clear(tp,ptr);
    return 0;
}

int tpSetCycleTime(TP_STRUCT * const tp, double secs)
{
    if (!tp || secs <= 0.0) {
        return -1;
    }

    tp->cycleTime=secs;
    tp->cycleTime*=0.000000001; //! Convert to ms.
    print("set cycle time to: %f \n",tp->cycleTime);
    return 0;
}

int tpSetVmax(TP_STRUCT * const tp, double vMax, double ini_maxvel)
{
    if (!tp || vMax <= 0.0 || ini_maxvel <= 0.0) {
        return -1;
    }

    tp->vMax = vMax;
    tp->ini_maxvel = ini_maxvel;

    print("set maxvel to: %f ",tp->vMax);
    print(" , ini maxvel to: %f \n",tp->ini_maxvel);
    return 0;
}

int tpSetVlimit(TP_STRUCT * const tp, double vLimit)
{
    if(!tp){return -1;}

    if (vLimit < 0.0){
        tp->vLimit = 0.;
    } else {
        tp->vLimit = vLimit;
    }

    print("tpSetVlimit. to: %f \n",tp->vLimit);
    return 0;
}

int tpSetAmax(TP_STRUCT * const tp, double aMax)
{
    if (!tp || aMax <= 0.0) {
        return -1;
    }

    tp->aMax=aMax;
    print("set accmax to: %f \n",tp->aMax);
    return 0;
}

int tpSetId(TP_STRUCT * const tp, int id)
{
    if(!tp){return -1;}

    // Set gcode line nr for upcoming new line, arc.
    tp->nextId=id;
    return 0;
}

int tpGetExecId(TP_STRUCT * const tp)
{
    // This is the executed gcode line nr. The gui's gcode preview
    // uses this to set the line.
    return tp->execId;
}

int tpSetTermCond(TP_STRUCT * const tp, int cond, double tolerance)
{
    tp->termCond = cond;
    tp->tolerance = tolerance;
    print("set term cond, tollerance: %f termcond: %d \n",tp->tolerance,tp->termCond);

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
    if(!tp){return -1;}

    tp->currentPos=*pos;
    print("set pos x: %f y: %f z: %f \n",tp->currentPos.tran.x,tp->currentPos.tran.y,tp->currentPos.tran.z);

    return 0;
}

int tpGetPos(TP_STRUCT const * const tp, EmcPose * const pos)
{
    if(!tp){return -1;}

    //! The gui toolposition tp is updated from here.
    *pos = tp->currentPos;
    // print("tpGetPos x: %f y: %f z: %f \n",pos->tran.x,pos->tran.y,pos->tran.z);
    return 0;
}

int tpErrorCheck(TP_STRUCT const * const tp) {
    return 0;
}

int tpSetSpindleSync(TP_STRUCT * const tp, int spindle, double sync, int mode) {
    return 0;
}

int tpPause(TP_STRUCT * const tp){
    print("pause. \n");
    tp->pausing=1;
    return 0;
}

int tpResume(TP_STRUCT * const tp){
    print("resume. \n");
    tp->pausing=0;
    return 0;
}

int tpAbort(TP_STRUCT * const tp){
    print("abort. \n");
    vector_clear(tp,ptr);
    tp->last_pos=tp->currentPos;
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

    print("set run dir, motion reverse : %i \n",dir);
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

    print("add rigid tap, disabled. \n");
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
    print("add line \n");
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
    print("add circle. \n");
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






































