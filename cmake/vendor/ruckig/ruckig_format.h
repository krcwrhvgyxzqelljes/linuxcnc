/********************************************************************
* Description: ruckig_format.h
*   A file for common use in c written programs.
*
* Author:
* License: GPL Version 2
* System: Linux
*
* Copyright (c) 2004 All rights reserved
********************************************************************/

/*
    This file gives you the ruckig data format's.
*/

#ifndef RUCKIG_FORMAT_H
#define RUCKIG_FORMAT_H

#include "stdbool.h"

enum interface {
    position,
    velocity
};

enum synchronization {
    Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
    Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
    TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
    None, ///< Calculate every DoF independently
};

enum durationdiscretization {
    Continuous, ///< Every trajectory duration is allowed (Default)
    Discrete, ///< The trajectory duration must be a multiple of the control cycle
};

//! Result type of Ruckig's update function
enum ruckigs_function_return_message {
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

struct result {
    double period;
    double curvel, curacc, curpos;
    double tarvel, taracc, tarpos;
    double maxvel,maxacc,maxjerk;
    bool enable;
    enum interface interfacetype;
    enum synchronization synchronizationtype;
    enum durationdiscretization durationdiscretizationtype;
    bool finished;
    bool error;
    int function_return_code;
};

#endif	/* RUCKIG_FORMAT_H */

