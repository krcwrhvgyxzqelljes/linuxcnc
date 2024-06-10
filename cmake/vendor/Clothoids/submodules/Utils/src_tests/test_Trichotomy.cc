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

#include "Utils_Trichotomy.hh"

using namespace std;

using Utils::Trichotomy;
using Utils::m_pi;

using real_type = double;

#if 0
static inline real_type power2( real_type x ) { return x*x; }
static inline real_type power3( real_type x ) { return x*x*x; }
static inline real_type power4( real_type x ) { return power2(power2(x)); }
static inline real_type power5( real_type x ) { return power4(x)*x; }
#endif

static int ntest = 0;

template <typename FUN>
void
do_solve( real_type a, real_type b, FUN f ) {
  Trichotomy<real_type> solver;
  real_type res = solver.eval2( a, b, f );
  ++ntest;
  fmt::print(
    "#{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",res),
    fmt::format("{:.3}",f(res))
  );
}

template <typename FUN>
void
do_solve1( real_type x0, real_type h, FUN f ) {
  Trichotomy<real_type> solver;
  real_type res = solver.search2( x0, h, f );
  ++ntest;
  fmt::print(
    "#{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",res),
    fmt::format("{:.3}",f(res))
  );
}

int
main() {

  do_solve(  0.5, 1.0, [](real_type x) { return exp(x)+1/x; });
  do_solve(  0.5, 2.0, [](real_type x) { return 5/x+x*x; });
  do_solve(  0.8, 2.0, [](real_type x) { return -5/(x*(x-2)+5); });
  do_solve(  0.0, 1.5, [](real_type x) { return exp(-2*x)+x*x/2; });
  do_solve(  0.0, 1.5, [](real_type x) { return exp(x-1)+1/x; });
  do_solve(  0.0, 1.0, [](real_type x) { return x*(x-exp(-x)); });
  do_solve(  0.0, 2.5, [](real_type x) { return 5*x*x+1/x; });
  do_solve( -3.0, 0.0, [](real_type x) { return exp(-x)+1/(1-x); });
  do_solve(  0.0, 2.0, [](real_type x) { return 2+x*(x-1); });
  do_solve(  0.0, 3.0, [](real_type x) { return -x*exp(-0.5*x); });
  do_solve(  0.0, 3.0, [](real_type x) { return -(0.2*x+sin(2*x)); });
  do_solve(  0.0, 0.5, [](real_type x) { return (exp(-x)-1/x); });
  do_solve( -1.0, 0.0, [](real_type x) { return exp(x)+x*x; });
  do_solve( -1.0, 0.0, [](real_type x) { return x*(4+x*(2+x*x)); });
  do_solve( -1.0, 0.0, [](real_type x) { return x*x+sin(x); });
  do_solve( -1.0, 1.0, [](real_type x) { return exp(x)+1/(x+2); });
  do_solve( -2.0, 0.0, [](real_type x) { return 2/(x*x); });
  do_solve(  2.0, 6.0, [](real_type x) { return -5*x*x*exp(-0.5*x); });
  do_solve(  4.0, 9.0, [](real_type x) { return -(0.1*x+cos(x)); });
  do_solve(  4.0, 9.0, [](real_type x) { return x*x-cos(1.5*x)/sin(1.5*x); });

  do_solve1(  4.0, 0.1, [](real_type x) { return x*x; });
  do_solve1( -4.0, 0.1, [](real_type x) { return x*x; });

  cout << "All done folks!\n\n";

  return 0;
}
