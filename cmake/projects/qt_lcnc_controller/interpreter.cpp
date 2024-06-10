#include "rs274ngc.hh"
#include "rs274ngc_interp.hh"
#include "rs274ngc_return.hh"
#include "inifile.hh"		// INIFILE
#include "canon.hh"		// _parameter_file_name
#include "config.h"		// LINELEN
#include <stdio.h>    /* gets, etc. */
#include <stdlib.h>   /* exit       */
#include <string.h>   /* strcpy     */
#include <getopt.h>
#include <stdarg.h>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>
#include <glob.h>
#include <wordexp.h>
#include <rtapi_string.h>

#include <saicanon.hh>
#include "tooldata.hh"
#include "common.h"
#include "iostream"
#include "cmath"

InterpBase *pinterp;
int _task = 0; // control preview behaviour when remapping
bool init=0;

extern std::vector<cad_data> runInterpreter(std::string filename)
{
    if(!init){
#ifdef TOOL_NML //{
        tool_nml_register((CANON_TOOL_TABLE*)& _sai._tools);
#else //}{
        const int random_toolchanger = 0;
        tool_mmap_creator((EMC_TOOL_STAT*)NULL,random_toolchanger);
        /* Notes:
  **   1) sai does not use toolInSpindle,pocketPrepped
  **   2) sai does not distinguish changer type
  */
#endif //}
        init=1;
    }

    std::vector<cad_data> cadvec;

    pinterp = new Interp;

    int s=pinterp->init();
    s=pinterp->open(filename.c_str());

    while(1)
    {
        int ir=pinterp->read();
        // std::cout<<"interp read:"<<ir<<std::endl;

        // printf("pos x: %f, y: %f, z: % f \n",_sai.pose.tran.x,_sai.pose.tran.y,_sai.pose.tran.z);
        // printf("cx: %f cy: %f \n",_sai.cx,_sai.cy);
        // printf("canon plane: %i \n",_sai._active_plane);

        cad_data d;

        if(isnan(_sai.pose.tran.x)){
            _sai.pose.tran.x=0;
        }
        if(isnan(_sai.pose.tran.y)){
            _sai.pose.tran.y=0;
        }
        if(isnan(_sai.pose.tran.z)){
            _sai.pose.tran.z=0;
        }

        d.pose=_sai.pose;
        d.pose.tran.x*=_sai._length_unit_factor; //! If to inches G20 is on.
        d.pose.tran.y*=_sai._length_unit_factor;
        d.pose.tran.z*=_sai._length_unit_factor;

        d.cx=_sai.cx*_sai._length_unit_factor;
        d.cy=_sai.cy*_sai._length_unit_factor;
        d.type=_sai.type;

        if(d.type==1){
            // std::cout<<"type G1."<<std::endl;
        }
        if(d.type==2){
            // std::cout<<"type G2 G3."<<std::endl;


        }
        if(d.type==3){
            // std::cout<<"type G0."<<std::endl;
        }
        if(d.type==-1){
            // This is when no G1,G2,G3 is in the current gcode line.
            // Then use previous type.
            // std::cout<<"type none."<<std::endl;
        }

        d.plane=int(_sai._active_plane);
        // std::cout<<"plane:"<<d.plane<<std::endl;

        d.rotation=_sai.rotation; // Arc dir, G2 or G3.
        // std::cout<<"rotation:"<<d.rotation<<std::endl;

        cadvec.push_back(d);

        //! Reset values.
        //_sai.type=-1;
        //_sai.rotation=0;

        int ie=pinterp->execute(0); // If this return value = 1. Then all gcode is loaded.
        // std::cout<<"interp execute:"<<ie<<std::endl;


        if(ie==5){ // This is end of first gcode dump.
            pinterp->reset();  // Enable the next gcode dump.
        }

        if(ie!=0 && ie!=5){ // If ie==1, then break.
            break;
        }
    }

    std::cout<<"interpreter ready."<<std::endl;

    pinterp->exit();

    free(pinterp);
    pinterp=0;

    return cadvec;
}












