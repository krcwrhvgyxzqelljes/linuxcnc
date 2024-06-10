#ifndef GCODE_PARSER_H
#define GCODE_PARSER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>

class gcode_parser
{
public:
    gcode_parser();

    //! Represent a 3d point in xyz space.
    struct POINT {
        double x,y,z;
    };

    //! Kdl euler angles. We do not use Yaw,Roll and Pitch values to avoid confusing.
    struct EULER {
        double x,y,z;
    };

    //! Gcode rapid, line, arc.
    //! Macro's. Add user defined macro's here and tell the gcode_interface what to do with it.
    enum BLOCKTYPE {
        G0, G1, G2, G3,
        //! For example a macro can carry a value : M3 P1100
        M,
        Comment
    };

    //! Structs not sharing data with kernelmodule (halmodule.h):
    //! Initialize values with 0!
    struct BLOCK {
        //! Identification.
        double G=0;
        //! Gcode Cartesian xyz.
        double X=0,Y=0,Z=0;
        //! Gcode Euler xyz (x=a, y=b, z=c)
        double A=0,B=0,C=0;
        //! Optional.
        double U=0,V=0,W=0;
        //! Gcode Arc center pos.
        double I=0,J=0,K=0;
        //! Feedrate & Spindlespeed.
        double F=0,S=0;
        //! Macro nr.
        double M=0;
        //! Macro carrying a value.
        double P=0;
        struct POINT start, center, end;
        struct EULER euler_start, euler_end;
        //! current blocklenght of arc, line.
        double blocklenght=0;


        //! The velocity optimizer function has a forward calculation and a backward calculation
        //! to ensure a optimal velocity when crossing waypoints.
        double vel_fwd_start=0, vel_fwd_end=0;
        double vel_bck_start=0, vel_bck_end=0;
        //! The result off above lowest values, to ensure optimal path velocity.
        double vel_start=0;
        double vel_end=0;

        //! Chunks
        std::vector<std::string> chunkvec;
        //! If there are gcode comments, they are stored here.
        std::string comment;
    };

    struct GCODE {
        //! Contains the gcode line data.
        std::vector<BLOCK> blockvec;
        //! Machine identification.
        //unsigned int machine_id;
        //! The complete block stringvec to load in a textpreview.
        std::string fullblockstring;
        //! The gcode file line by line.
        std::vector<std::string> blockstringvec;
    };
    //! For every machine instance :
    //! gcodevec.at(0)=mdi          Machine_0
    //! gcodevec.at(1)=program      Machine_0
    //! gcodevec.at(2)=mdi          Machine_1
    //! gcodevec.at(3)=program      Machine_1
    //! etc.

    struct FILTER{
        std::string brutoline;
        std::string nettoline;
        std::string comment;
        std::vector<std::string> chunkvec;
        std::vector<std::pair<std::string,double>> chunkvec_id_value;
    };

    FILTER filter_line_comments(FILTER f, bool debug);

    //! Main function.
    GCODE process_gcode(std::string filename);

    //! Read the textfile into a stringvec.
    std::vector<std::string> read_file(std::string filename);

    //! Parse a line into chunks between the white spaces.
    FILTER parse_a_line_into_chunks(FILTER f, bool debug);

    //! Retrieve chunck id + value.
    FILTER parse_chunck_into_id_and_value(FILTER f, bool debug);

    //! Parse clean chunks into a block.
    BLOCK chunck_id_value_to_block(FILTER f, BLOCK b, bool debug);

    //! Remember the previous G0 or G1 etc. But reset XYZABCUVWIJK values etc.
    BLOCK zero_some_block_values(BLOCK b);

    //! From a multiline to a single line.
    std::string get_fulltext(std::vector<std::string> strvec);

    //! Print results.
    void print_results(std::vector<BLOCK> blkvec, bool debug);

};

#endif // GCODE_PARSER_H
