#include "vector.h"
#include "tp_types.h"
#include "gp_Pnt.hxx"
#include "gp_Ax3.hxx"
#include "gp_Pln.hxx"
#include "draw_primitives.h"
#include "segment.h"

vector::vector()
{

}

extern "C" {

struct vector* vector_init_ptr(){
    return new vector();
}
void vector_add_segment(struct vector *ptr, struct segment b){
    ptr->segvec.push_back(b);
}
void vector_pop_back(struct vector *ptr){
    ptr->segvec.pop_back();
}
int vector_size(struct vector *ptr){
    ptr->segvec.size();
}
void vector_clear(struct vector *ptr){
    ptr->segvec.clear();
}

int vector_add_line(TP_STRUCT *
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
                    struct vector *ptr){
    segment s;

    s.turn=0;
    s.canon_motion_type=canon_motion_type;
    s.vel=vel;
    s.g64_p=tag.fields_float[3];
    s.g64_q=tag.fields_float[4];

    // Points.
    s.p0={tp->last_pos.tran.x,tp->last_pos.tran.y,tp->last_pos.tran.z};
    s.pc={0,0,0};
    s.p1={end.tran.x,end.tran.y,end.tran.z};
    //s.pw=draw_primitives::get_line_midpoint(s.p0,s.p1);

    // Extern.
    s.a0=tp->last_pos.a;
    s.b0=tp->last_pos.b;
    s.c0=tp->last_pos.c;

    s.u0=tp->last_pos.u;
    s.v0=tp->last_pos.v;
    s.w0=tp->last_pos.w;

    s.a1=end.a;
    s.b1=end.b;
    s.c1=end.c;

    s.u1=end.u;
    s.v1=end.v;
    s.w1=end.w;

    ptr->segvec.push_back(s);

    return 0;
}

int vector_add_circle( TP_STRUCT * const tp,
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
                       struct vector *ptr ){
    segment s;

    s.turn=turn;
    s.canon_motion_type=canon_motion_type;
    s.vel=vel;
    s.g64_p=tag.fields_float[3];
    s.g64_q=tag.fields_float[4];

    // Points.
    s.p0={tp->last_pos.tran.x,tp->last_pos.tran.y,tp->last_pos.tran.z};
    s.pc={center.x,center.y,center.z};
    s.p1={end.tran.x,end.tran.y,end.tran.z};

    // Normal, axis, plane.
    s.normal={normal.x,normal.y,normal.z};
    s.pn={s.pc.X()+s.normal.X(), s.pc.Y()+s.normal.Y(), s.pc.Z()+s.normal.Z()};
    s.axis={s.pc,s.normal};
    s.plane={s.axis};

    if(s.p0.Distance(s.p1)==0){ // Full circle.
        // s.pw=draw_primitives::rotate_3d_point_around_line(s.p0,M_PI,s.pc,s.pn);
    } else {
        if(s.turn==-1){
            //draw_primitives::draw_3d_pc_arc(s.p1,s.p0,s.pc,s.normal.X(),s.normal.Y(),s.normal.Z(),s.pw);
        } else {
            // draw_primitives::draw_3d_pc_arc(s.p0,s.p1,s.pc,s.normal.X(),s.normal.Y(),s.normal.Z(),s.pw);
        }
    }

    // Extern.
    s.a0=tp->last_pos.a;
    s.b0=tp->last_pos.b;
    s.c0=tp->last_pos.c;

    s.u0=tp->last_pos.u;
    s.v0=tp->last_pos.v;
    s.w0=tp->last_pos.w;

    s.a1=end.a;
    s.b1=end.b;
    s.c1=end.c;

    s.u1=end.u;
    s.v1=end.v;
    s.w1=end.w;

    ptr->segvec.push_back(s);

    return 0;
}


}





