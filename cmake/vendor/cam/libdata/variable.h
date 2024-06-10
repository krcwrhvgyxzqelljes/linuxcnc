#ifndef VARIABLE_H
#define VARIABLE_H

#include <gp_Pnt.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Compound.hxx>

enum primitive_type {
    point=0,
    line=1,
    lwpolyline=2, // Wire, linestrip
    circle=3,
    arc=4,
    spline=5,
    ellipse=6,
};

enum contour_type {
    none=0,
    single_open=1,
    single_closed=2,
    multi_open=3,
    multi_closed=4,
};

enum contour_dir {
    cw=0,
    ccw=1,
};

enum offset_action {
    offset_contour=0,
    lead_base_contour=1,
    lead_in_contour=2,
    lead_out_contour=3,
    pocket=4,
};

struct datas {
    Handle(AIS_Shape) ashape;
    primitive_type primitivetype;
    contour_type contourtype=contour_type::none;
    std::string acad_layer;
    //! Startpoint, endpoint
    gp_Pnt start{0,0,0}, end{0,0,0}, center{0,0,0}; // Center is used by cavalier functions.
    std::vector<gp_Pnt> control, arcmid; // Arcmid is used by cavalier function and is the point at half way arc circumfence.
    double radius=0; // Radius is used by cavalier functions.
    double bulge=0; // For arc's to define if the arc is [cw] or [ccw]. bulge<0=g2
    bool select=0; // Helper for contour algoritme.
};
//! Datavec contains the filtered cad_layers to cut, dxfvec is the complete dxf data.
extern std::vector<datas> datavec, dxfvec, rapidvec;

struct lead_in_out {
    //! The lead-in, lead-out shape (line, arc, etc).
    Handle(AIS_Shape) ashape;
    //! Choose one of the parallel Points to draw the lead-in, lead-out.
    std::vector<gp_Pnt> points;
};

struct contour {
    std::vector<datas> primitive_sequence;
    contour_dir dir;
    double area=0;

    //! Lead_base=base points circumfence contour, Lead_in=circumfence points at lead_in offset, Lead_out=circumfence points at lead_out offset.
    lead_in_out lead_in, lead_out, lead_base;
    //! Current lead_base point position. If the lead_in-out is shifted to another position, this value stores the current lead_in-out index position.
    // unsigned int lead_index=0;

    std::vector<datas> offset_sequence; // Primitives in cw order.

    // Contour depth.
    std::vector<unsigned int> childcontours;
    bool select=false;
    int depth=0;
};
extern std::vector<contour> contourvec, pocketvec;

struct gcode_setup{
    std::string filename="gcode.ngc";
    std::string linenumber_format="N";
    std::string z_axis_format="Z";
    std::string tool_on_macro="";

    bool print_linenumbers=true;
    double lead_in;
    double lead_out;
    int lead_start_i;
    double offset;
    double travelheight;
    double pierceheight;
    double piercespeed;
    double piercedelay;
    double cutheight;
    double power;
    double feedrate;
    double internal_pocket_offset;
    double tooloffset_x, tooloffset_y, tooloffset_z;


    std::string layer;
    std::vector<std::string> intro;
    std::vector<std::string> outtro;

    std::string operation_type;  // Offset, pocket, drill.
};
extern gcode_setup gc;
extern std::vector<gcode_setup> gcvec;
extern double last_x,last_y,last_z; // Used for displaying rapids. Can be used for toolhomepos.

extern int linenumber;

extern std::vector<unsigned int> kpt_sequence; // Keep parts together list.

extern int maxdepth;

class variable
{
public:
    variable();
};

#endif // VARIABLE_H

























