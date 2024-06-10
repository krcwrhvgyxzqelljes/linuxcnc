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

    while(s==0)
    {
        s=pinterp->read();

        if(_sai.type==1){
            // printf("arc. \n");
        }
        if(_sai.type==0){
            // printf("line. \n");
        }

        // printf("pos x: %f, y: %f, z: % f \n",_sai.pose.tran.x,_sai.pose.tran.y,_sai.pose.tran.z);
        // printf("cx: %f cy: %f \n",_sai.cx,_sai.cy);
        // printf("canon plane: %i \n",_sai._active_plane);

        cad_data d;
        d.pose=_sai.pose;
        d.pose.tran.x*=_sai._length_unit_factor; //! If to inches G20 is on.
        d.pose.tran.y*=_sai._length_unit_factor;
        d.pose.tran.z*=_sai._length_unit_factor;

        d.cx=_sai.cx*_sai._length_unit_factor;
        d.cy=_sai.cy*_sai._length_unit_factor;
        d.type=_sai.type;
        d.plane=int(_sai._active_plane);
        std::cout<<"plane:"<<d.plane<<std::endl;
        d.rotation=_sai.rotation;

        cadvec.push_back(d);

        //! Reset values.
        _sai.type=-1;
        _sai.rotation=0;

        s=pinterp->execute(0);
    }

    pinterp->exit();

    free(pinterp);
    pinterp=0;

    return cadvec;
}












