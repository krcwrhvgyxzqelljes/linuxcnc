/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2017                                                      |
 |                                                                          |
 |         , __                 , __                                        |
 |        /|/  \               /|/  \                                       |
 |         | __/ _   ,_         | __/ _   ,_                                |
 |         |   \|/  /  |  |   | |   \|/  /  |  |   |                        |
 |         |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/                       |
 |                           /|                   /|                        |
 |                           \|                   \|                        |
 |                                                                          |
 |      Enrico Bertolazzi                                                   |
 |      Dipartimento di Ingegneria Industriale                              |
 |      Universita` degli Studi di Trento                                   |
 |      email: enrico.bertolazzi@unitn.it                                   |
 |                                                                          |
\*--------------------------------------------------------------------------*/

#include "Utils_AABB_tree.hh"
#include "Utils_GG2D.hh"
#include <random>

using namespace std;
using integer   = int;
using real_type = double;

static unsigned seed1 = 2;
static std::mt19937 generator(seed1);

using Utils::Point2D;
using Utils::Segment2D;
using Utils::AABBtree;
using Utils::TicToc;
using Utils::m_pi;

static
real_type
rand( real_type xmin, real_type xmax ) {
  real_type random = real_type(generator())/generator.max();
  return xmin + (xmax-xmin)*random;
}

int
main() {

  TicToc tm;

  integer const NS  = 10000;
  integer const dim = 2;
  real_type bb_min1[NS*dim];
  real_type bb_max1[NS*dim];
  real_type bb_min2[NS*dim];
  real_type bb_max2[NS*dim];

  // generate a set of random lines
  std::vector<Segment2D<real_type>> S_set_1, S_set_2;
  Point2D<real_type> Pa, Pb, DIR;

  for ( integer k = 0; k < NS; ++k ) {
    real_type x, y, theta=0;
    if ( k == 0 ) {
      x     = rand(0,10);
      y     = rand(0,10);
      theta = rand(0,2*m_pi);
    } else {
      x     = S_set_1.back().Pb().x() + rand(0,4);
      y     = S_set_1.back().Pb().y() + rand(0,4);
      if      ( x < 0  ) x += 10;
      else if ( x > 10 ) x -= 10;
      if      ( y < 0  ) y += 10;
      else if ( y > 10 ) y -= 10;
      theta += rand(0,0.1*m_pi);
    }
    real_type len = rand(0.01,2);
    Pa.coeffRef(0)  = x;
    Pa.coeffRef(1)  = y;
    DIR.coeffRef(0) = cos(theta);
    DIR.coeffRef(1) = sin(theta);
    Pb.noalias()    = Pa + len*DIR;
    S_set_1.emplace_back(Pa,Pb);

    Point2D<real_type> pmin, pmax;
    S_set_1.back().bbox( pmin, pmax );

    bb_min1[k*dim+0] = pmin.x();
    bb_min1[k*dim+1] = pmin.y();
    bb_max1[k*dim+0] = pmax.x();
    bb_max1[k*dim+1] = pmax.y();
  }

  for ( integer k = 0; k < NS; ++k ) {
    real_type x, y, theta=0;
    if ( k == 0 ) {
      x     = rand(0,10);
      y     = rand(0,10);
      theta = rand(0,2*m_pi);
    } else {
      x     = S_set_2.back().Pb().x() + rand(0,4);
      y     = S_set_2.back().Pb().y() + rand(0,4);
      if      ( x < 0  ) x += 10;
      else if ( x > 10 ) x -= 10;
      if      ( y < 0  ) y += 10;
      else if ( y > 10 ) y -= 10;
      theta += rand(0,0.1*m_pi);
    }
    real_type len = rand(0.01,2);
    Pa.coeffRef(0)  = x;
    Pa.coeffRef(1)  = y;
    DIR.coeffRef(0) = cos(theta);
    DIR.coeffRef(1) = sin(theta);
    Pb.noalias() = Pa + len*DIR;
    S_set_2.emplace_back(Pa,Pb);

    Point2D<real_type> pmin, pmax;
    S_set_2.back().bbox( pmin, pmax );

    bb_min2[k*dim+0] = pmin.x();
    bb_min2[k*dim+1] = pmin.y();
    bb_max2[k*dim+0] = pmax.x();
    bb_max2[k*dim+1] = pmax.y();
  }

  Utils::AABBtree<real_type> T1, T2;
  T1.set_max_num_objects_per_node( 16 );
  T2.set_max_num_objects_per_node( 16 );

  tm.tic();
  T1.build(
    bb_min1, dim,
    bb_max1, dim,
    NS, dim
  );
  T2.build(
    bb_min2, dim,
    bb_max2, dim,
    NS, dim
  );
  tm.toc();

  fmt::print("T1 T2 build elapsed {} ms\n", tm.elapsed_ms() );

  /*
  for ( integer i_pos = 0; i_pos < T1.num_tree_nodes(); ++i_pos ) {
    std::set<integer> bb_index;
    T1.get_nodes( i_pos, bb_index );
    fmt::print( "{} ->", i_pos );
    for ( auto & m : bb_index ) fmt::print( " {}", m );
    fmt::print( "\n" );
  }
  */

  std::set<integer> bb_index;
  real_type const pnt[2] = { 4, 4 };
  tm.tic();
  T1.intersect_with_one_point( pnt, bb_index );
  tm.toc();

  fmt::print( "intersect: ({} ms) -> {} ms \n", tm.elapsed_ms(), NS*tm.elapsed_ms() );
  for ( auto & m : bb_index ) fmt::print( " {}", m );
  fmt::print( "\n" );

  std::map<integer,std::set<integer>> bbb_index;
  tm.tic();
  T1.intersect( T2, bbb_index );
  tm.toc();
  fmt::print("T1 vs T2 elapsed {} ms\nsize = {}\n", tm.elapsed_ms(), bbb_index.size() );

  bbb_index.clear();

  tm.tic();
  T2.intersect( T1, bbb_index );
  tm.toc();
  fmt::print("T2 vs T1 elapsed {} ms\nsize = {}\n", tm.elapsed_ms(), bbb_index.size() );

  bbb_index.clear();

  tm.tic();
  T1.intersect_and_refine( T2, bbb_index );
  tm.toc();
  fmt::print("intersect_with_refine T1 vs T2 elapsed {} ms\nsize = {}\n", tm.elapsed_ms(), bbb_index.size() );


  fmt::print("T1\n{}\n", T1.info() );
  fmt::print("T2\n{}\n", T2.info() );

  fmt::print("\n\nAll done!\n");
  return 0;
}
