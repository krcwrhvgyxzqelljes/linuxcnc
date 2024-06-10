/****************************************************************************\
  Copyright (c) Enrico Bertolazzi 2019
  All Rights Reserved.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the file license.txt for more details.
\****************************************************************************/

#include "Utils_mex.hh"
#include "Utils_AABB_tree.hh"

#define IN_OUT( IN, OUT ) \
  UTILS_MEX_ASSERT( nlhs == OUT, "{}: expected " #OUT " output, nlhs = {}\n", CMD, nlhs ); \
  UTILS_MEX_ASSERT( nrhs == IN,  "{}: expected " #IN  " input, nrhs = {}\n", CMD, nrhs )

namespace Utils {

  #define AABB_TREE AABBtree<double>

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
    #define MEX_ERROR_MESSAGE_1 "AABB_treeMexWrapper('new')"
    #define CMD MEX_ERROR_MESSAGE_1
    IN_OUT(1,1);
    arg_out_0 = Utils::mex_convert_ptr_to_mx<AABB_TREE>(new AABB_TREE());
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_delete(
    int nlhs, mxArray       *[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_2 "AABB_treeMexWrapper('delete',obj)"
    #define CMD MEX_ERROR_MESSAGE_2
    IN_OUT(2,0);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
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
    #define MEX_ERROR_MESSAGE_3 "AABB_treeMexWrapper('copy',obj)"
    #define CMD MEX_ERROR_MESSAGE_3
    IN_OUT( 2, 1 );
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    arg_out_0 = Utils::mex_convert_ptr_to_mx<AABB_TREE>(new AABB_TREE(*ptr));
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_set_max_num_objects_per_node(
    int nlhs, mxArray       *[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_4 "AABB_treeMexWrapper('set_max_num_objects_per_node',obj,N)"
    #define CMD MEX_ERROR_MESSAGE_4
    IN_OUT(3,0);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    int64_t N = Utils::mex_get_int64( arg_in_2, CMD ": parameter N" );
    ptr->set_max_num_objects_per_node( N );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_set_bbox_long_edge_ratio(
    int nlhs, mxArray       *[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_5 "AABB_treeMexWrapper('set_bbox_long_edge_ratio',obj,V)"
    #define CMD MEX_ERROR_MESSAGE_5
    IN_OUT(3,0);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    double V = Utils::mex_get_scalar_value( arg_in_2, CMD ": parameter V" );
    ptr->set_bbox_long_edge_ratio( V );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_set_bbox_overlap_tolerance(
    int nlhs, mxArray       *[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_6 "AABB_treeMexWrapper('set_bbox_overlap_tolerance',obj,V)"
    #define CMD MEX_ERROR_MESSAGE_6
    IN_OUT(3,0);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    double V = Utils::mex_get_scalar_value( arg_in_2, CMD ": parameter V" );
    ptr->set_bbox_overlap_tolerance( V );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_build(
    int nlhs, mxArray       *[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_7 "AABB_treeMexWrapper( 'build', obj, bb_min, bb_max )"
    #define CMD MEX_ERROR_MESSAGE_7
    IN_OUT(4,0);
    mwSize ldim0, ncol0, ldim1, ncol1;
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    double const * bb_min = Utils::mex_matrix_pointer( arg_in_2, ldim0, ncol0, CMD ": parameter bb_min" );
    double const * bb_max = Utils::mex_matrix_pointer( arg_in_3, ldim1, ncol1, CMD ": parameter bb_max" );
    UTILS_MEX_ASSERT(
      ldim0 == ldim1 && ncol0 == ncol1,
      "{}: size(bb_min) = {} x {} must be equal to size(bb_max) = {} x {}\n",
      CMD, ldim0, ncol0, ldim1, ncol1
    );
    ptr->build(
      bb_min, ldim0,
      bb_max, ldim1,
      ncol0, ldim0 // nbox, dim
    );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_get_bboxes_of_the_tree(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_8 "[bb_min,bb_max]=AABB_treeMexWrapper('get_bboxes_of_the_tree',obj,nmin)"
    #define CMD MEX_ERROR_MESSAGE_8
    IN_OUT(3,2);

    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    int64_t nmin = Utils::mex_get_int64( arg_in_2, CMD ": parameter nmin" );

    mwSize dim = ptr->dim();
    mwSize ntn = ptr->num_tree_nodes( nmin );

    double * bb_min = Utils::mex_create_matrix_value( arg_out_0, dim, ntn );
    double * bb_max = Utils::mex_create_matrix_value( arg_out_1, dim, ntn );

    ptr->get_bboxes_of_the_tree( bb_min, dim, bb_max, dim, nmin );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_get_bbox_indexes_of_a_node(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_9 "id_list=AABB_treeMexWrapper('get_bbox_indexes_of_a_node',obj,inode)"
    #define CMD MEX_ERROR_MESSAGE_9
    IN_OUT(3,1);

    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    int64_t inode = Utils::mex_get_int64( arg_in_2, CMD ": parameter inode" );

    AABB_TREE::SET bb_index;
    ptr->get_bbox_indexes_of_a_node( inode-1, bb_index ); // C/C++ 0-based indexing

    int32_t * id_list = Utils::mex_create_matrix_int32( arg_out_0, bb_index.size(), 1 );
    for ( auto const & id : bb_index ) *id_list++ = id+1;

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_intersect(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_10 "id_list=AABB_treeMexWrapper('intersect',obj,aabb)"
    #define CMD MEX_ERROR_MESSAGE_10
    IN_OUT(3,1);

    AABB_TREE       * ptr  = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    AABB_TREE const * ptr2 = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_2);

    AABB_TREE::MAP bb_index;

    //mexPrintf( "call ptr->intersect\n" );
    ptr->intersect( *ptr2, bb_index );
    //mexPrintf( "do_intersect ncheck: %d\n", ptr->num_check() );
    //mexPrintf( "do_intersect num intersect: %d\n", bb_index.size() );

    // Create a nrhs x 1 cell mxArray.
    arg_out_0 = mxCreateCellMatrix( ptr->num_tree_nodes(), 1 );
    for ( auto const & S : bb_index ) {
      mxArray *tmp;
      int32_t * idx = Utils::mex_create_matrix_int32( tmp, S.second.size(), 1 );
      for ( auto const & v : S.second ) *idx++ = v+1;
      mxSetCell( arg_out_0, S.first, mxDuplicateArray(tmp) );
    }

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_intersect_and_refine(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_11 "id_list=AABB_treeMexWrapper( 'intersect_and_refine', obj, aabb )"
    #define CMD MEX_ERROR_MESSAGE_11
    IN_OUT(3,1);

    AABB_TREE       * ptr  = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    AABB_TREE const * ptr2 = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_2);

    AABB_TREE::MAP bb_index;

    ptr->intersect_and_refine( *ptr2, bb_index );

    // Create a nrhs x 1 cell mxArray.
    arg_out_0 = mxCreateCellMatrix( ptr->num_objects(), 1 );
    for ( auto const & S : bb_index ) {
      mxArray *tmp;
      int32_t * idx = Utils::mex_create_matrix_int32( tmp, S.second.size(), 1 );
      for ( auto const & v : S.second ) *idx++ = v+1;
      mxSetCell( arg_out_0, S.first, mxDuplicateArray(tmp) );
    }

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_intersect_with_one_bbox(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_12 "id_list=AABB_treeMexWrapper('intersect_with_one_bbox',obj,bb_min,bb_max)"
    #define CMD MEX_ERROR_MESSAGE_12
    IN_OUT(4,1);

    AABB_TREE * ptr  = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    mwSize dim0, dim1;
    double const * bb_min = Utils::mex_vector_pointer( arg_in_2, dim0, CMD ": parameter bb_min" );
    double const * bb_max = Utils::mex_vector_pointer( arg_in_3, dim1, CMD ": parameter bb_max" );

    UTILS_MEX_ASSERT(
      dim0 == dim1,
      "{}: size(bb_min) = {} must be equal to size(bb_max) = {}\n",
      dim0, dim1
    );

    std::vector<double> bbox;
    bbox.resize( 2*dim0 );
    std::copy_n( bb_min, dim0, bbox.data() );
    std::copy_n( bb_max, dim0, bbox.data()+dim0 );

    AABB_TREE::SET bb_index;
    ptr->intersect_with_one_bbox( bbox.data(), bb_index );

    //mexPrintf( "do_intersect_with_one_bbox ncheck: %d\n", ptr->num_check() );
    //mexPrintf( "do_intersect_with_one_bbox num intersect: %d\n", bb_index.size() );

    // Create a nrhs x 1 cell mxArray.
    int32_t * idx = Utils::mex_create_matrix_int32( arg_out_0, bb_index.size(), 1 );
    for ( auto const & v : bb_index ) *idx++ = v+1;

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_intersect_with_one_point(
    int nlhs, mxArray       *plhs[],
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_13 "id_list=AABB_treeMexWrapper('intersect_with_one_point',obj,pnt)"
    #define CMD MEX_ERROR_MESSAGE_13
    IN_OUT(3,1);

    AABB_TREE * ptr  = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    mwSize dim0;
    double const * pnt = Utils::mex_vector_pointer( arg_in_2, dim0, CMD ": parameter bb_min" );

    AABB_TREE::SET bb_index;
    ptr->intersect_with_one_point( pnt, bb_index );

    // Create a nrhs x 1 cell mxArray.
    int32_t * idx = Utils::mex_create_matrix_int32( arg_out_0, bb_index.size(), 1 );
    for ( auto const & v : bb_index ) *idx++ = v+1;

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_info(
    int nlhs, mxArray       *[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_14 "AABB_treeMexWrapper('info',obj)"
    #define CMD MEX_ERROR_MESSAGE_14
    IN_OUT(2,0);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    mexPrintf("%s\n",ptr->info().c_str());
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_get_dim(
    int nlhs, mxArray       *plhs[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_15 "res=AABB_treeMexWrapper('get_dim',obj)"
    #define CMD MEX_ERROR_MESSAGE_15
    IN_OUT(2,1);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    Utils::mex_set_scalar_int32( arg_out_0, ptr->dim() );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_get_num_bb(
    int nlhs, mxArray       *plhs[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_16 "res=AABB_treeMexWrapper('get_num_bb',obj)"
    #define CMD MEX_ERROR_MESSAGE_16
    IN_OUT(2,1);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    Utils::mex_set_scalar_int32( arg_out_0, ptr->num_objects() );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_get_num_tree_nodes(
    int nlhs, mxArray       *plhs[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_17 "res=AABB_treeMexWrapper('get_num_tree_nodes',obj,nmin)"
    #define CMD MEX_ERROR_MESSAGE_17
    IN_OUT(3,1);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    int64_t nmin = Utils::mex_get_int64( arg_in_2, CMD ": parameter nmin" );
    if ( nmin == 0 ) Utils::mex_set_scalar_int32( arg_out_0, ptr->num_tree_nodes() );
    else             Utils::mex_set_scalar_int32( arg_out_0, ptr->num_tree_nodes(nmin) );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_min_distance_candidates(
    int nlhs, mxArray       *plhs[], // unused
    int nrhs, mxArray const *prhs[]
  ) {
    #define MEX_ERROR_MESSAGE_18 "res=AABB_treeMexWrapper('min_distance_candidates',obj,pnt)"
    #define CMD MEX_ERROR_MESSAGE_18
    IN_OUT(3,1);
    AABB_TREE * ptr = Utils::mex_convert_mx_to_ptr<AABB_TREE>(arg_in_1);
    mwSize dim0;
    double const * pnt = Utils::mex_vector_pointer( arg_in_2, dim0, CMD ": pnt" );

    AABB_TREE::SET bb_index;
    ptr->min_distance_candidates( pnt, bb_index );

    // Create a nrhs x 1 cell mxArray.
    int32_t * idx = Utils::mex_create_matrix_int32( arg_out_0, bb_index.size(), 1 );
    for ( auto const & v : bb_index ) *idx++ = v+1;

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  typedef void (*DO_CMD)( int nlhs, mxArray *plhs[], int nrhs, mxArray const *prhs[] );

  static std::map<std::string,DO_CMD> cmd_to_fun = {
    {"new",do_new},
    {"delete",do_delete},
    {"copy",do_copy},
    {"set_max_num_objects_per_node",do_set_max_num_objects_per_node},
    {"set_bbox_long_edge_ratio",do_set_bbox_long_edge_ratio},
    {"set_bbox_overlap_tolerance",do_set_bbox_overlap_tolerance},
    {"build",do_build},
    {"get_bboxes_of_the_tree",do_get_bboxes_of_the_tree},
    {"get_bbox_indexes_of_a_node",do_get_bbox_indexes_of_a_node},
    {"intersect",do_intersect},
    {"intersect_and_refine",do_intersect_and_refine},
    {"intersect_with_one_point",do_intersect_with_one_point},
    {"intersect_with_one_bbox",do_intersect_with_one_bbox},
    {"info",do_info},
    {"get_dim",do_get_dim},
    {"get_num_bb",do_get_num_bb},
    {"get_num_tree_nodes",do_get_num_tree_nodes},
    {"min_distance_candidates",do_min_distance_candidates}
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
"   " MEX_ERROR_MESSAGE_10 "\n" \
"   " MEX_ERROR_MESSAGE_11 "\n" \
"   " MEX_ERROR_MESSAGE_12 "\n" \
"   " MEX_ERROR_MESSAGE_13 "\n" \
"   " MEX_ERROR_MESSAGE_14 "\n" \
"   " MEX_ERROR_MESSAGE_15 "\n" \
"   " MEX_ERROR_MESSAGE_16 "\n" \
"   " MEX_ERROR_MESSAGE_17 "\n" \
"   " MEX_ERROR_MESSAGE_18 "\n" \
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
      mexErrMsgTxt( fmt::format( "AABB_treeMexWrapper Error: {}", e.what() ).c_str() );
    } catch (...) {
      mexErrMsgTxt( "AABB_treeMexWrapper failed" );
    }
  }
}
