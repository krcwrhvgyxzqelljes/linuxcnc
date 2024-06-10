/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2022                                                      |
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

#include "Utils_HJPatternSearch.hh"
#include <cmath>

using namespace std;

using Utils::HJPatternSearch;
using Utils::m_pi;

using std::abs;
using std::pow;

using real_type = double;

static inline real_type power2( real_type x ) { return x*x; }
//static inline real_type power3( real_type x ) { return x*x*x; }
//static inline real_type power4( real_type x ) { return power2(power2(x)); }
//static inline real_type power5( real_type x ) { return power4(x)*x; }

#if 0
static
real_type
fun1( real_type const X[] ) {
  real_type x = X[0];
  real_type y = X[1];
  if ( x                              > -1      ) return Utils::Inf<real_type>();
  if ( x                              < -17.001 ) return Utils::Inf<real_type>();
  if ( y                              > -1      ) return Utils::Inf<real_type>();
  if ( y                              < -x/3-28 ) return Utils::Inf<real_type>();
  if ( power2(y+20)-3*x               < 51      ) return Utils::Inf<real_type>();
  if ( abs(x+14.5)+power2(y+15)       < 3       ) return Utils::Inf<real_type>();
  if ( power2(x+16)+pow(abs(y+8),1.5) < 20      ) return Utils::Inf<real_type>();
  if ( power2(x+9.2)+abs(y+12)        < 7       ) return Utils::Inf<real_type>();
  if ( power2(x+6)+power2(y+15)       < 29.8    ) return Utils::Inf<real_type>();
  if ( power2(x+6)+pow(abs(y+1),1.5)  < 15      ) return Utils::Inf<real_type>();
  return pow(abs(y-x),2.07) + pow(abs(x*y),1.07);
}
#endif

#if 0
static
real_type
fun2( real_type const X[] ) {
  real_type x = X[0];
  real_type y = X[1];
  if ( x > 100    ) return Utils::Inf<real_type>();
  if ( x < 0      ) return Utils::Inf<real_type>();
  if ( y > 101.01 ) return Utils::Inf<real_type>();
  if ( y < 0      ) return Utils::Inf<real_type>();
  {
    real_type const v[] = {5,19,33,47,61,75,89};
    for ( int i = 0; i < 7; ++i ) {
      if ( abs(x)+pow( abs(y-v[i]), 3.5 ) < 99.9 )
        return Utils::Inf<real_type>();
    }
  }
  {
    real_type const v[] = {12,26,40,54,68,82,96};
    for ( int i = 0; i < 7; ++i ) {
      if ( abs(x-100)+pow( abs(y-v[i]), 3 ) < 99.9 )
        return Utils::Inf<real_type>();
    }
  }
  return abs(x-100)/100 + abs(y-101);
}
#endif

static
real_type
fun3( real_type const X[] ) {
  real_type x = X[0];
  real_type y = X[1];
  real_type res = 100*power2(y-x*x)+power2(1-x);
  //fmt::print( "{} {} -> {}\n", x, y, res );
  return res;
}

template <typename FUN>
void
do_solve( FUN f, real_type const X0[], real_type delta ) {
  Utils::Console console(&cout,4);
  HJPatternSearch<real_type> solver("HJPatternSearch");
  solver.setup( 2, f, &console );
  solver.set_tolerance( 1e-20);
  solver.run( X0, delta );
  real_type X[2];
  solver.get_last_solution( X );
  fmt::print( "X={}, Y={}\n", X[0], X[1] );
}

int
main() {
  #if 0
    real_type X0[2]{-1.1,-27.0};
    real_type delta = 1;
    std::function<real_type(real_type const[])> F(fun1);
    do_solve( F, X0, delta );
  #endif
  #if 0
    real_type X0[2]{1,1};
    real_type delta = 1;
    std::function<real_type(real_type const[])> F(fun2);
    do_solve( F, X0, delta );
  #endif
  #if 1
    real_type X0[2]{-1,1};
    real_type delta = 0.1;
    std::function<real_type(real_type const[])> F(fun3);
    do_solve( F, X0, delta );
  #endif

  cout << "\nAll Done Folks!\n";

  return 0;
}
