#ifndef LOOK_AHEAD_SKELETON_H
#define LOOK_AHEAD_SKELETON_H

#include "tp_scurve.h"
#include "tp_types.h"

// Working skeleton code for info to run a look ahead code forward, and reverse and
// set the lowest value when doing the reverse.
void look_ahead_skeleton(){
    const int vsize = 51;
    // Initialize the array of tp_segment
    tp_segment svec[vsize];

    for (int i = 0; i < vsize; ++i) {
        svec[i].vo = 0;
        svec[i].vm = 10;
        svec[i].ve = 0;
        svec[i].path_lenght = i;
    }

    const int start = 25 ; // Simulate current index.
    const int buffer = 5; // The size to look fwd & rev.
    int maxend;

    maxend = std::min(start + buffer, vsize);
    // Forward loop over svec
    for (int i = start; i < maxend; ++i) {
        svec[i].ve = svec[i].vo + getVe(i,svec[i].path_lenght,svec[i].vo);
        if (i < maxend - 1) {
            svec[i + 1].vo = svec[i].ve;
        }
        qDebug() << "Fwd Segment" << i << ": vo =" << svec[i].vo << ", ve =" << svec[i].ve << ", vm =" << svec[i].vm;
    }

    maxend = std::min(start + buffer -1, vsize -1);
    // Reverse loop over svec
    for (int i = maxend; i >= start && i >= 0; --i) {
        if (i == start + buffer - 1) {
            svec[i].ve = 0;
        }
        if (i > start) {
            double fwd = svec[i].vo;
            double rev = svec[i].ve + getVe(i, svec[i].path_lenght, svec[i].ve);
            svec[i].vo = fmin(fwd, rev);
            svec[i - 1].ve = svec[i].vo;
        }
        if (i == start) {
            double fwd = svec[i].vo;
            double rev = svec[i].ve + getVe(i, svec[i].path_lenght, svec[i].ve);
            svec[i].vo = fmin(fwd, rev);
        }
        qDebug() << "Rev Segment" << i << ": vo =" << svec[i].vo << ", ve =" << svec[i].ve << ", vm =" << svec[i].vm;
    }
}

/*
Output :

Fwd Segment 25 : vo = 0 , ve = 25 , vm = 10
Fwd Segment 26 : vo = 25 , ve = 76 , vm = 10
Fwd Segment 27 : vo = 76 , ve = 179 , vm = 10
Fwd Segment 28 : vo = 179 , ve = 386 , vm = 10
Fwd Segment 29 : vo = 386 , ve = 801 , vm = 10
Rev Segment 29 : vo = 29 , ve = 0 , vm = 10
Rev Segment 28 : vo = 86 , ve = 29 , vm = 10
Rev Segment 27 : vo = 76 , ve = 86 , vm = 10
Rev Segment 26 : vo = 25 , ve = 76 , vm = 10
Rev Segment 25 : vo = 0 , ve = 25 , vm = 10
*/

// Derived from look_aheadskeleton.h sample code.
// This code run's forward & reverse over the n ammount of gcode segments.
// It set's the lowest value in the reverse cycle.
//inline void look_ahead_optimizer(TP_STRUCT * const tp, int look_ahead_lines){

//    /* Todo add logic for types.
//    if (next.motion_type == sc_circle) {
//        next_vmax = fmin(tpGetLinearVelMaxId(tp, i + 1), tpGetArcSpeed(tp, i + 1));
//    }
//    if (next.motion_type == sc_linear) {
//        next_vmax = tpGetLinearVelMaxId(tp, i + 1);
//    }*/

//    int vsize = vector_size_c(vector_ptr);
//    int start = tp->vector_current_exec; // Simulate current index.
//    int buffer = look_ahead_lines; // The size to look fwd & rev.
//    int maxend;
//    double this_endvel=0;
//    double this_endacc=0;

//    maxend = fmin(start + buffer, vsize);
//    // Forward loop over svec
//    for (int i = start; i < maxend; ++i) {
//        vector_set_ve(vector_ptr,i, vector_at(vector_ptr,i).vo + 2);
//        if (i < maxend - 1) {
//            vector_set_vo(vector_ptr, i+1,vector_at(vector_ptr, i).ve);
//        }
//        printf("FWD Segment %d: vo = %.2f, ve = %.2f, lenght = %.2f\n", i, vector_at(vector_ptr, i).vo, vector_at(vector_ptr, i).ve, vector_at(vector_ptr, i).path_lenght);
//    }

//    maxend = fmin(start + buffer -1, vsize -1);
//    // Reverse loop over svec
//    for (int i = maxend; i >= start && i >= 0; --i) {
//        if (i == start + buffer - 1) {
//            vector_set_ve(vector_ptr,i,0);
//        }
//        if (i > start) {
//            double fwd = vector_at(vector_ptr, i).vo;
//            double rev = vector_at(vector_ptr,i).ve + (vector_at(vector_ptr,i).ve+2);
//            vector_set_vo(vector_ptr,i,fmin(tp->vLimit, fmin(fwd, rev)));
//            vector_set_ve(vector_ptr,i-1,fmin(tp->vLimit, vector_at(vector_ptr,i).vo));
//        }
//        if (i == start) {
//            double fwd = vector_at(vector_ptr, i).vo;
//            double rev = vector_at(vector_ptr,i).ve + (vector_at(vector_ptr,i).ve+2);
//            vector_set_vo(vector_ptr,i,fmin(tp->vLimit, fmin(fwd, rev)));

//        }
//        printf("REV Segment %d: vo = %.2f, ve = %.2f, lenght = %.2f\n", i, vector_at(vector_ptr, i).vo, vector_at(vector_ptr, i).ve, vector_at(vector_ptr, i).path_lenght);
//    }
//}



#endif // LOOK_AHEAD_SKELETON_H
