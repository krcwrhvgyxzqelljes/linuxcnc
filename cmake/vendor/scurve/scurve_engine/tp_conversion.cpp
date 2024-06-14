#include "tp_conversion.h"
#include "gp_Pnt.hxx"

struct gp_Pose {
    gp_Pnt p;
    double a,b,c,u,v,w;
};

gp_Pnt emc_pose_to_gp_Pnt(struct EmcPose pose){
    return {pose.tran.x,pose.tran.y,pose.tran.z};
}


















