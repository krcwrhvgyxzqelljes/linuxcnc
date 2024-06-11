#ifndef COMMON_H
#define COMMON_H

#include "emcpos.h"
#include "vector"



struct cad_data {
    EmcPose pose;
    double cx=0, cy=0, cz=0;
    int type=0;  //! G1=1, G2_G3=2, G0=3.
    int plane=0; //! Saiconon.cc plane of arc.
    int rotation=0;
    double lenght_unit_factor=0; //! mm or inches.

    double motion_tolerance;      // G64 P
    double naivecam_tolerance;    // G64 Q
};
//std::vector<cad_data> cadvec;

#endif // COMMON_H
