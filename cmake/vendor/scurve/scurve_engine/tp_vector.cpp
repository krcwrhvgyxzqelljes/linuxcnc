#include "tp_vector.h"
#include "tp_scurve.h"
#include "cavalier_3d.h"
#include "cavalier_offset.h"
#include "tp_arcs.h"

tp_vector::tp_vector()
{

}

extern "C" struct tp_vector* vector_init_ptr(){
    return new tp_vector();
}

extern "C" void vector_add_segment(struct tp_vector *ptr, struct tp_segment b){
    ptr->pvec.push_back(b);
}

extern "C" void vector_pop_back(struct tp_vector *ptr){
    ptr->pvec.pop_back();
}

extern "C" void vector_remove_last_segment(struct tp_vector *ptr){
    ptr->pvec.pop_back();
}

extern "C" int vector_size_c(struct tp_vector *ptr){
    return ptr->pvec.size();
}

extern "C" void vector_clear(struct tp_vector *ptr){
    return ptr->pvec.clear();
}

extern "C" int vector_at_id(struct tp_vector *ptr, int n){
    return ptr->pvec.at(n).primitive_id;
}

extern "C" tp_segment vector_at(struct tp_vector *ptr, int index){
    return ptr->pvec.at(index);
}

extern "C" void vector_set_end_angle(struct tp_vector *ptr, int index, double angle_deg){
    ptr->pvec.at(index).angle_end=angle_deg;
}

extern "C" void vector_set_vo(struct tp_vector *ptr, int index, double value){
    ptr->pvec.at(index).vo=value;
}

extern "C" void vector_set_ve(struct tp_vector *ptr, int index, double value){
    ptr->pvec.at(index).ve=value;
}

extern "C" void vector_set_vm(struct tp_vector *ptr, int index, double value){
    ptr->pvec.at(index).vm=value;
}

extern "C" void vector_set_acs(struct tp_vector *ptr, int index, double value){
    ptr->pvec.at(index).acs=value;
}

extern "C" void vector_set_ace(struct tp_vector *ptr, int index, double value){
    ptr->pvec.at(index).ace=value;
}

extern "C" void vector_set_tangent_or_colineair(struct tp_vector *ptr, int index, int value){
    ptr->pvec.at(index).next_segment_is_tangent_or_colinear=value;
}

extern "C" double vector_traject_lenght(struct tp_vector *ptr){
    return ptr->pvec.back().path_end;
}

extern "C" void vector_set_pnt_s(struct tp_vector *ptr, int index, struct sc_pnt p){
    ptr->pvec.at(index).pnt_s=p;
}

extern "C" void vector_set_pnt_e(struct tp_vector *ptr, int index, struct sc_pnt p){
    ptr->pvec.at(index).pnt_e=p;
}

extern "C" void vector_set_pnt_w(struct tp_vector *ptr, int index, struct sc_pnt p){
    ptr->pvec.at(index).pnt_w=p;
}

extern "C" void vector_set_pnt_c(struct tp_vector *ptr, int index, struct sc_pnt p){
    ptr->pvec.at(index).pnt_c=p;
}

extern "C" void vector_update_pathlenghts(struct tp_vector *ptr) {

    for(uint i=0; i<ptr->pvec.size(); i++){
        if(i==0){
            ptr->pvec.at(i).path_begin=0;
        }
        ptr->pvec.at(i).path_end = ptr->pvec.at(i).path_begin + ptr->pvec.at(i).path_lenght;

        /*
        std::cout<<"path vector nr:"<< i<<std::endl;
        std::cout<<"path begin:"<< ptr->pvec.at(i).path_begin<<std::endl;
        std::cout<<"path end:"<< ptr->pvec.at(i).path_end<<std::endl;
        std::cout<<"path lenght:"<< ptr->pvec.at(i).path_lenght<<std::endl;
        std::cout<<"is fillet:"<< ptr->pvec.at(i).is_fillet_segment<<std::endl;
        std::cout<<std::endl;*/

        if(i<ptr->pvec.size()-1){
            ptr->pvec.at(i+1).path_begin= ptr->pvec.at(i).path_end;
        }
    }
}

extern "C" void vector_run_cavalier(struct tp_vector *ptr, int index, double blend_radius_G0, double blend_radius_G1_G2_G3, int enable_chamfer) {

    int in=index;

    if(blend_radius_G0==0 && blend_radius_G1_G2_G3==0){
        return;
    }

    if(in+3>=ptr->pvec.size()){
        return;
    }

    double blend_radius=0;

    // Check colinear.
    /*
    if(ptr->pvec.at(in).primitive_id==sc_line && ptr->pvec.at(in+2).primitive_id==sc_line){
        int r=draw_primitives().is_point_on_line({ptr->pvec.at(in).pnt_s.x,
                                            ptr->pvec.at(in).pnt_s.y,
                                            ptr->pvec.at(in).pnt_s.z},
                                           {ptr->pvec.at(in+2).pnt_e.x,
                                            ptr->pvec.at(in+2).pnt_e.y,
                                            ptr->pvec.at(in+2).pnt_e.z},
                                           {ptr->pvec.at(in).pnt_e.x,
                                            ptr->pvec.at(in).pnt_e.y,
                                            ptr->pvec.at(in).pnt_e.z});

        if(!r){
            return;
        }
    }*/

    if(ptr->pvec.at(in).motion_type==sc_rapid && ptr->pvec.at(in+2).motion_type==sc_rapid){
        blend_radius=blend_radius_G0;
    } else {
        blend_radius=blend_radius_G1_G2_G3;
    }

    if(!ptr->pvec.at(in).is_fillet_segment && ptr->pvec.at(in+1).is_fillet_segment && !ptr->pvec.at(in+2).is_fillet_segment){

    } else {
        return;
    }

    cavline l0, l1;
    l0.p0 = {ptr->pvec[in].pnt_s.x, ptr->pvec[in].pnt_s.y, ptr->pvec[in].pnt_s.z};
    l0.p1 = {ptr->pvec[in].pnt_e.x, ptr->pvec[in].pnt_e.y, ptr->pvec[in].pnt_e.z};
    l0.pw = {ptr->pvec[in].pnt_w.x, ptr->pvec[in].pnt_w.y, ptr->pvec[in].pnt_w.z};

    l1.p0 = {ptr->pvec[in+2].pnt_s.x, ptr->pvec[in+2].pnt_s.y, ptr->pvec[in+2].pnt_s.z};
    l1.p1 = {ptr->pvec[in+2].pnt_e.x, ptr->pvec[in+2].pnt_e.y, ptr->pvec[in+2].pnt_e.z};
    l1.pw = {ptr->pvec[in+2].pnt_w.x, ptr->pvec[in+2].pnt_w.y, ptr->pvec[in+2].pnt_w.z};

    std::vector<cavline> cavvec;

     if(!cavalier_3d().get_3d_fillet_path_2_segments(l0, l1, blend_radius, cavvec)){
        std::cerr << "Error in get_3d_fillet_path_2_segments: " << std::endl;
        return;
    }

    int cavvec_size=cavvec.size();
    std::cout<<"cavvec size:"<<cavvec.size()<<std::endl;
    if(cavvec_size<3){
        std::cout<<"no blend solutions, abort."<<std::endl;
        return;
    }

    if(cavvec.size()==3){

        if(cavvec[0].p0.Distance(cavvec[0].p1)==0){
            std::cout<<"first blend lenght zero, abort."<<std::endl;
            return;
        }
        if(cavvec[2].p0.Distance(cavvec[2].p1)==0){
            std::cout<<"second blend lenght zero, abort."<<std::endl;
            return;
        }

        // std::cout<<"p0 x:"<<cavvec[1].p0.X()<<" y:"<<cavvec[1].p0.Y()<<" z:"<<cavvec[1].p0.Z()<<std::endl;
        // std::cout<<"p1 x:"<<cavvec[1].p1.X()<<" y:"<<cavvec[1].p1.Y()<<" z:"<<cavvec[1].p1.Z()<<std::endl;

        ptr->pvec[in].pnt_s = {cavvec[0].p0.X(), cavvec[0].p0.Y(), cavvec[0].p0.Z()};
        ptr->pvec[in].pnt_w = {cavvec[0].pw.X(), cavvec[0].pw.Y(), cavvec[0].pw.Z()};
        ptr->pvec[in].pnt_e = {cavvec[0].p1.X(), cavvec[0].p1.Y(), cavvec[0].p1.Z()};
        ptr->pvec[in].pnt_c = {cavvec[0].pc.X(), cavvec[0].pc.Y(), cavvec[0].pc.Z()};
        ptr->pvec[in].path_lenght=cavvec[0].lenght;
        ptr->pvec[in].next_segment_is_tangent_or_colinear=1;
        if(ptr->pvec[in].primitive_id==sc_line){
            ptr->pvec[in].path_lenght=cavvec[0].p0.Distance(cavvec[0].p1);
        }

        ptr->pvec[in+1].pnt_s = {cavvec[1].p0.X(), cavvec[1].p0.Y(), cavvec[1].p0.Z()};
        ptr->pvec[in+1].pnt_w = {cavvec[1].pw.X(), cavvec[1].pw.Y(), cavvec[1].pw.Z()};
        ptr->pvec[in+1].pnt_e = {cavvec[1].p1.X(), cavvec[1].p1.Y(), cavvec[1].p1.Z()};
        ptr->pvec[in+1].pnt_c = {cavvec[1].pc.X(), cavvec[1].pc.Y(), cavvec[1].pc.Z()};
        ptr->pvec[in+1].path_lenght=cavvec[1].lenght;
        ptr->pvec[in+1].next_segment_is_tangent_or_colinear=1;
        ptr->pvec[in+1].primitive_id=sc_arc;
        if(enable_chamfer){
            ptr->pvec[in+1].primitive_id=sc_line;
            ptr->pvec[in+1].path_lenght=cavvec[1].p0.Distance(cavvec[1].p1);
        }

        ptr->pvec[in+2].pnt_s = {cavvec[2].p0.X(), cavvec[2].p0.Y(), cavvec[2].p0.Z()};
        ptr->pvec[in+2].pnt_w = {cavvec[2].pw.X(), cavvec[2].pw.Y(), cavvec[2].pw.Z()};
        ptr->pvec[in+2].pnt_e = {cavvec[2].p1.X(), cavvec[2].p1.Y(), cavvec[2].p1.Z()};
        ptr->pvec[in+2].pnt_c = {cavvec[2].pc.X(), cavvec[2].pc.Y(), cavvec[2].pc.Z()};
        ptr->pvec[in+2].path_lenght=cavvec[2].lenght;

        // std::cout<<"l0:"<< ptr->pvec[in].path_lenght<<" bulge:"<<cavvec[0].bulge<<std::endl;
        // std::cout<<"l1:"<< ptr->pvec[in+1].path_lenght<<" bulge:"<<cavvec[1].bulge<<std::endl;
        // std::cout<<"l2:"<< ptr->pvec[in+2].path_lenght<<" bulge:"<<cavvec[2].bulge<<std::endl;

    }
}

//! Interpolates traject progress 0-1. But will not give value 1. So its not usable at this moment.
void tp_vector::interpolate_traject(double traject_progress,
                                    double traject_lenght,
                                    double &curve_progress, int &curve_nr){

    double ltot=traject_lenght;
    double l=0;
    for(uint i=0; i<pvec.size(); i++){
        double blocklenght=pvec[i].path_lenght;
        if(traject_progress>=l/ltot && traject_progress<(l+blocklenght)/ltot){

            double low_pct=l/ltot;                                   //10%
            double high_pct=(l+blocklenght)/ltot;                    //25%
            double range=high_pct-low_pct;                           //25-10=15%
            double offset_low=traject_progress-low_pct;              //12-10=2%
            curve_progress=offset_low/range;
            curve_nr=i;
            return;
        }
        l+=blocklenght;
    }
}

extern "C" void vector_interpolate_traject_c(tp_vector *ptr,
                                             double traject_progress,
                                             double traject_lenght,
                                             double *curve_progress,
                                             int *curve_nr){

    double curve_progress_=0;
    int curve_nr_=0;
    ptr->interpolate_traject(traject_progress, traject_lenght, curve_progress_, curve_nr_);

    *curve_progress=curve_progress_;
    *curve_nr=curve_nr_;
}

