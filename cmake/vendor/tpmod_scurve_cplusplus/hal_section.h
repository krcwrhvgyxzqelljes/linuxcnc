#ifndef HAL_PINS_H
#define HAL_PINS_H

#include "hal.h"
#include "def_section.h"

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;

//! Pins
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;

typedef struct { //! Int.
    hal_s32_t *Pin;
} s32_data_t;

typedef struct { //! Param int.
    hal_s32_t Pin;
} param_s32_data_t;

typedef struct { //! Uint.
    hal_u32_t *Pin;
} u32_data_t;

typedef struct { //! Param Uint.
    hal_u32_t Pin;
} param_u32_data_t;

typedef struct {
    hal_port_t *Pin;
} port_data_t;

//! Params
typedef struct {
    hal_float_t Pin;
} param_float_data_t;

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;


/* module information */
MODULE_AUTHOR( COMP_AUTHOR );
MODULE_DESCRIPTION( COMP_DESCRIPTION );
MODULE_LICENSE( COMP_LICENCE );

static int comp_idx;
static int create_hal_pins();
static void the_function();

int rtapi_app_main(void) {

    int r = 0;
    comp_idx = hal_init( COMP_NAME );
    if(comp_idx < 0) return comp_idx;
    r = hal_export_funct( COMP_NAME , the_function, &skynet,0,0,comp_idx);

    r+=create_hal_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    return 0;
}

void rtapi_app_exit(void){
    hal_exit(comp_idx);
}

//! Perforn's every ms.
static void the_function(){

}

//! Setup hal pins.
static int create_hal_pins(){
    int r=0;



    return r;
}

#endif // HAL_PINS_H
