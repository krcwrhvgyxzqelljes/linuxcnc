#ifndef SCURVE_RESET_H
#define SCURVE_RESET_H

#include "scurve_struct.h"

void zero_period(struct scurve_period *p){
    p->accbeg=0;
    p->accend=0;
    p->velini=0;
    p->velbeg=0;
    p->velend=0;
    p->disbeg=0;
    p->disend=0;
    p->timbeg=0;
    p->timend=0;
    p->jermax=0;
    p->accinf=0;
}

void  scurve_data_reset_position_data(struct scurve_data *axis){
    axis->pd.overshoot=0;
    axis->pd.stopdist=0;
    axis->pd.stopinit=0;
    axis->pd.stoptime=0;

    zero_period(&axis->c0);
    zero_period(&axis->c1);
    zero_period(&axis->c2);
    zero_period(&axis->c3);
    zero_period(&axis->c4);
}

void scurve_data_reset(struct scurve_data *axis){
    axis->intval=0;
    axis->jermax=0;
    axis->curtim=0;

    axis->endacc=0;
    axis->endvel=0;
    axis->maxvel=0;
    axis->maxacc=0;

    axis->guipos=0;
    axis->guivel=0;
    axis->guiacc=0;

    axis->finish=0;

    axis->incpos=0;
    axis->oldpos=0;

    axis->incpos=0;
    axis->oldpos=0;

    axis->jog_btn_dir=0;
    axis->jog_stop_first=0;

    axis->tarpos=0;

    axis->vr=0;
    axis->sr=0;
    axis->ar=0;

    axis->revers=0;
    axis->modpos=0;

    axis->jog.enable=0;
    axis->jog.dir=FORWARD_SC;
    axis->jog.mode=VELOCITY_SC;

    axis->pd.overshoot=0;
    axis->pd.stopdist=0;
    axis->pd.stopinit=0;
    axis->pd.stoptime=0;
    axis->pd.dist_remove_a_cycle=0;
    axis->pd.cycles=0;
    axis->pd.cycle_count=0;
    axis->pd.btn_fwd=0;
    axis->pd.btn_rev=0;

    axis->rcode=0;

    zero_period(&axis->c0);
    zero_period(&axis->c1);
    zero_period(&axis->c2);
    zero_period(&axis->c3);
    zero_period(&axis->c4);
}

void scurve_data_sample(struct scurve_data *axis){
    axis->intval=0.001;
    axis->jermax=8000;
    axis->maxvel=175;
    axis->maxacc=1000;
    axis->endacc=0;
    axis->endvel=0;
}

#endif // SCURVE_RESET_H
