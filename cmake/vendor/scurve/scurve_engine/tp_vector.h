#ifndef TP_VECTOR_H
#define TP_VECTOR_H

//! Author  : Skynet Cyberdyne
//! Licence : MIT
//! Date    : 2023

#ifdef __cplusplus

#include <vector>
#include <iostream>
#include <cmath>
#include "tp_scurve.h"

class tp_vector
{
public:

    //! Constructor.
    tp_vector();

    void interpolate_traject(double traject_progress,
                             double traject_lenght,
                             double &curve_progress,
                             int &curve_nr);

    std::vector<tp_segment> pvec;

private:
};

//! Here it tells if this code is used in c, convert the class to a struct. This is handy!
#else
typedef struct tp_vector tp_vector;
#endif //! cplusplus

#endif




















