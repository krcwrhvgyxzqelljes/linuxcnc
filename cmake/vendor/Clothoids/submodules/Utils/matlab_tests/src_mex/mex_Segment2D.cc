/****************************************************************************\
  Copyright (c) Enrico Bertolazzi 2019
  All Rights Reserved.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the file license.txt for more details.
\****************************************************************************/

#include "Utils_mex.hh"
#include "Utils_GG2D.hh"

#define IN_OUT( IN, OUT ) \
  UTILS_MEX_ASSERT( nlhs == OUT, "{}: expected " #OUT " output, nlhs = {}\n", CMD, nlhs ); \
  UTILS_MEX_ASSERT( nrhs == IN,  "{}: expected " #IN  " input, nrhs = {}\n", CMD, nrhs )

namespace Utils {

  #define SEGMENT2D Segment2D<double>

  /*\
   *                      _____                 _   _
   *  _ __ ___   _____  _|  ___|   _ _ __   ___| |_(_) ___  _ __
   * | '_ ` _ \ / _ \ \/ / |_ | | | | '_ \ / __| __| |/ _ \| '_ \
   * | | | | | |  __/>  <|  _|| |_| | | | | (__| |_| | (_) | | | |
   * |_| |_| |_|\___/_/\_\_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
   *
  \*/

  static
  void
  do_new(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *[] // unused
  ) {
    #define MEX_ERROR_MESSAGE_1 "Segment2DMexWrapper('new')"
    #define CMD MEX_ERROR_MESSAGE_1
    IN_OUT(1,1);
    arg_out_0 = Utils::mex_convert_ptr_to_mx<SEGMENT2D>(new SEGMENT2D());
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_delete(
    int nlhs, mxArray       *[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_2 "Segment2DMexWrapper('delete',obj)"
    #define CMD MEX_ERROR_MESSAGE_2
    IN_OUT(2,0);
    SEGMENT2D * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    delete ptr;
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_copy(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_3 "Segment2DMexWrapper('copy',obj)"
    #define CMD MEX_ERROR_MESSAGE_3
    IN_OUT( 2, 1 );
    SEGMENT2D * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    arg_out_0 = Utils::mex_convert_ptr_to_mx<SEGMENT2D>(new SEGMENT2D(*ptr));
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_setup(
    int nlhs, mxArray       *[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_4 "Segment2DMexWrapper('setup',obj,A,B)"
    #define CMD MEX_ERROR_MESSAGE_4
    IN_OUT(4,0);
    SEGMENT2D * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    mwSize dim;
    double const * A = Utils::mex_vector_pointer( arg_in_2, dim, CMD ": parameter A" );
    UTILS_MEX_ASSERT( dim == 2, "{}: length(A) = {} must be equal to 2\n", dim );
    double const * B = Utils::mex_vector_pointer( arg_in_3, dim, CMD ": parameter B" );
    UTILS_MEX_ASSERT( dim == 2, "{}: length(B) = {} must be equal to 2\n", dim );
    ptr->setup( A, B );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_getAB(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_5 "[A,B]=Segment2DMexWrapper('getAB',obj)"
    #define CMD MEX_ERROR_MESSAGE_5
    IN_OUT(2,2);
    SEGMENT2D const * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    double * A = Utils::mex_create_matrix_value( arg_out_0, 2, 1 );
    double * B = Utils::mex_create_matrix_value( arg_out_1, 2, 1 );
    Point2D<double> const & PA = ptr->Pa();
    Point2D<double> const & PB = ptr->Pb();
    A[0] = PA.coeff(0); A[1] = PA.coeff(1);
    B[0] = PB.coeff(0); B[1] = PB.coeff(1);
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_evals(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_6 "P=Segment2DMexWrapper('evals',obj,s)"
    #define CMD MEX_ERROR_MESSAGE_6
    IN_OUT(3,1);
    SEGMENT2D const * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    double s = Utils::mex_get_scalar_value( arg_in_2, CMD ": parameter s" );
    Point2D<double> Ps = ptr->eval( s );
    double * P = Utils::mex_create_matrix_value( arg_out_0, 2, 1 );
    P[0] = Ps.coeff(0);
    P[1] = Ps.coeff(1);
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_evalst(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_7 "P=Segment2DMexWrapper('evalst',obj,s,t)"
    #define CMD MEX_ERROR_MESSAGE_7
    IN_OUT(4,1);
    SEGMENT2D const * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    double s = Utils::mex_get_scalar_value( arg_in_2, CMD ": parameter s" );
    double t = Utils::mex_get_scalar_value( arg_in_3, CMD ": parameter t" );
    Point2D<double> Ps = ptr->eval( s, t );
    double * P = Utils::mex_create_matrix_value( arg_out_0, 2, 1 );
    P[0] = Ps.coeff(0);
    P[1] = Ps.coeff(1);
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_point_coord(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_8 "[s,t,PP]=Segment2DMexWrapper('point_coord',obj,P)"
    #define CMD MEX_ERROR_MESSAGE_8
    IN_OUT(3,3);
    SEGMENT2D const * ptr = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    mwSize dim;
    double const * P = Utils::mex_vector_pointer( arg_in_2, dim, CMD ": parameter P" );
    UTILS_MEX_ASSERT( dim == 2, "{}: length(P) = {} must be equal to 2\n", dim );

    Point2D<double> P2D;
    P2D.coeffRef(0) = P[0];
    P2D.coeffRef(1) = P[1];

    double s, t;
    Point2D<double> PP = ptr->projection( P2D, s, t );
    Utils::mex_set_scalar_value( arg_out_0, s );
    Utils::mex_set_scalar_value( arg_out_1, t );
    double * Pout = Utils::mex_create_matrix_value( arg_out_2, 2, 1 );
    Pout[0] = PP.coeff(0);
    Pout[1] = PP.coeff(1);
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_intersect(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_9 "[s,t,ok]=Segment2DMexWrapper('intersect',obj,obj2)"
    #define CMD MEX_ERROR_MESSAGE_9
    IN_OUT(3,3);

    SEGMENT2D const * ptr  = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_1);
    SEGMENT2D const * ptr1 = Utils::mex_convert_mx_to_ptr<SEGMENT2D>(arg_in_2);

    double s, t;
    bool ok = ptr->intersect( *ptr1, s, t );
    Utils::mex_set_scalar_value( arg_out_0, s );
    Utils::mex_set_scalar_value( arg_out_1, t );
    Utils::mex_set_scalar_bool( arg_out_2, ok );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  typedef void (*DO_CMD)( int nlhs, mxArray *plhs[], int nrhs, mxArray const *prhs[] );

  static std::map<std::string,DO_CMD> cmd_to_fun = {
    {"new",do_new},
    {"delete",do_delete},
    {"copy",do_copy},
    {"setup",do_setup},
    {"getAB",do_getAB},
    {"evals",do_evals},
    {"evalst",do_evalst},
    {"point_coord",do_point_coord},
    {"intersect",do_intersect}
  };

#define MEX_ERROR_MESSAGE \
"=====================================================================================\n" \
"FiberMexWrapper: \n" \
"\n" \
"USAGE:\n" \
"  - Constructors:\n" \
"    OBJ = AABB_treeMexWrapper( 'new' );\n" \
"\n" \
"  On output:\n" \
"    OBJ = pointer to the internal object\n" \
"   " MEX_ERROR_MESSAGE_1  "\n" \
"   " MEX_ERROR_MESSAGE_2  "\n" \
"   " MEX_ERROR_MESSAGE_3  "\n" \
"   " MEX_ERROR_MESSAGE_4  "\n" \
"   " MEX_ERROR_MESSAGE_5  "\n" \
"   " MEX_ERROR_MESSAGE_6  "\n" \
"   " MEX_ERROR_MESSAGE_7  "\n" \
"   " MEX_ERROR_MESSAGE_8  "\n" \
"   " MEX_ERROR_MESSAGE_9  "\n" \
"=====================================================================================\n"

  extern "C"
  void
  mexFunction(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {

    // the first argument must be a string
    if ( nrhs == 0 ) { mexErrMsgTxt(MEX_ERROR_MESSAGE); return; }

    try {
      UTILS_MEX_ASSERT0( mxIsChar(arg_in_0), "First argument must be a string" );
      std::string cmd = mxArrayToString(arg_in_0);
      DO_CMD pfun = cmd_to_fun.at(cmd);
      pfun( nlhs, plhs, nrhs, prhs );
    } catch ( std::exception const & e ) {
      mexErrMsgTxt( fmt::format( "Segment2DMexWrapper Error: {}", e.what() ).c_str() );
    } catch (...) {
      mexErrMsgTxt( "Segment2DMexWrapper failed" );
    }
  }
}
