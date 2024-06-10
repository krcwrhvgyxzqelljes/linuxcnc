#ifndef RUCKIG_DEV_ONLINE_INTERFACE_H
#define RUCKIG_DEV_ONLINE_INTERFACE_H

#ifdef __cplusplus

#include "ruckig_dev_format.h"
#include <ruckig/ruckig.hpp>

class ruckig_dev_online_interface
{
public:
    ruckig_dev_online_interface();

    //! Used for tp jogging, updated & derived from older ruckig lib.
    //! Calculate a normal cycle.
    ruckig_c_data dofs(ruckig_c_data input);

};


//! Here it tells if this code is used in c, convert the class to a struct. This is handy!
#else
typedef struct ruckig_dev_interface ruckig_dev_interface;
#endif //! cplusplus

#endif // RUCKIG_DEV_ONLINE_INTERFACE_H


