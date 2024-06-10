#ifndef RUCKIG_DEV_FORMAT_H
#define RUCKIG_DEV_FORMAT_H

#include "stdbool.h"

#define OK 1;
#define ERROR -1;

enum ruckig_c_control_interface {
    position,
    velocity
};

enum ruckig_c_synchronization {
    Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
    Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
    TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
    None, ///< Calculate every DoF independently
};

enum ruckig_c_durationdiscretization {
    Continuous, ///< Every trajectory duration is allowed (Default)
    Discrete, ///< The trajectory duration must be a multiple of the control cycle
};

//! Result type of Ruckig's update function
enum ruckig_c_function_return_message {
    Working = 0, ///< The trajectory is calculated normally
    Finished = 1, ///< Trajectory has reached its final position
    Error = -1, ///< Unclassified error
    ErrorInvalidInput = -100, ///< Error in the input parameter
    ErrorTrajectoryDuration = -101, ///< The trajectory duration exceeds its numerical limits
    ErrorPositionalLimits = -102, ///< The trajectory exceeds the given positional limits (only in Ruckig Pro)
    ErrorNoPhaseSynchronization = -103, ///< The trajectory cannot be phase synchronized
    ErrorExecutionTimeCalculation = -110, ///< Error during the extremel time calculation (Step 1)
    ErrorSynchronizationCalculation = -111, ///< Error during the synchronization calculation (Step 2)
};

struct ruckig_c_data {
    double cycletime; //! Servo cycletime 0.001 seconds.
    double curvel, curacc, curpos;
    double newvel, newacc, newpos;
    double tarvel, taracc, tarpos;
    double oldmaxvel, oldmaxacc, oldmaxjerk, oldtarvel, oldtaracc, oldtarpos; //! Used to trigger interupts.
    bool oldpause, oldreverse; //! Used to trigger interupts.
    double maxvel,maxacc,maxjerk;
    bool enable;    //! Enable ruckig.
    bool initialized;  //! New motion has to be initialized.
    bool pause; //! Pause the motion.
    bool reverse; //! Motion reverse direction.
    bool reset; //! Reset values to zero.
    enum ruckig_c_control_interface control_interfacetype;
    enum ruckig_c_synchronization synchronizationtype;
    enum ruckig_c_durationdiscretization durationdiscretizationtype;
    int function_return_code;   //! 1=finished, 0=busy, <0=error
    double duration;    //! Duration of traject.
    double at_time;     //! Request pos,acc.vel at time.
    double pos_extrema_min, pos_extrema_max; //! Min's and max's of the trajectory.
};

struct ruckig_c_waypoint {
    double vo;      //! Velocity begin.
    double ve;      //! Velocity end.
    double starpos; //! Position start.
    double goalpos; //! Position target.
};

#endif // RUCKIG_DEV_FORMAT_H













