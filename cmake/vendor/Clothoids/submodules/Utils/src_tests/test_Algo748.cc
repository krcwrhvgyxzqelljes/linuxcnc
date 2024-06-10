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

#include "Utils_Algo748.hh"

using namespace std;

using Utils::Algo748;
using Utils::m_pi;

using real_type = double;

static inline real_type power2( real_type x ) { return x*x; }
static inline real_type power3( real_type x ) { return x*x*x; }
static inline real_type power4( real_type x ) { return power2(power2(x)); }
static inline real_type power5( real_type x ) { return power4(x)*x; }

static int ntest = 0;

static
real_type
fun1( real_type x ) {
  real_type res = 0;
  for ( int i = 1; i <= 20; ++i ) {
    res += power2(2*i-5)/power3( x - i*i );
  }
  return -2*res;
}

static
real_type
fun2( real_type x, real_type a, real_type b ) {
  return a*x*exp(b*x);
}

static
real_type
fun3( real_type x, real_type n, real_type a ) {
  return pow(x,n)-a;
}

static
real_type
fun4( real_type x, real_type n ) {
  return 2*x*exp(-n)-2*exp(-n*x)+1;
}

static
real_type
fun5( real_type x, real_type n ) {
  return (1+power2(1-n))*x-power2(1-n*x);
}

static
real_type
fun6( real_type x, real_type n ) {
  return power2(x)-pow(1-x,n);
}

static
real_type
fun7( real_type x, real_type n ) {
  return (1+power4(1-n))*x-power4(1-n*x);
}

static
real_type
fun8( real_type x, real_type n ) {
  return exp(-n*x)*(x-1)+pow(x,n);
}

static
real_type
fun9( real_type x, real_type n ) {
  return (n*x-1)/((n-1)*x);
}

static
real_type
fun10( real_type x, real_type n ) {
  return pow(x,1/n)-pow(n,1/n);
}

static
real_type
fun11( real_type x ) {
  if ( x == 0 ) return 0;
  return x*exp(-1/(x*x));
}

static
real_type
fun12( real_type x, real_type n ) {
  if ( x < 0 ) return -n/20;
  return (n/20)*(x/1.5+sin(x)-1);
}

static
real_type
fun13( real_type x, real_type n ) {
  if ( x > 2e-3/(1+n) ) return exp(1)-1.859;
  if ( x < 0          ) return -0.859;
  return exp( (n+1)*0.5e3*x )-1.859;
}

static
real_type
fun_penalty( real_type x_in, real_type RHS ) {
  real_type m_h       = 0.01;
  real_type m_epsilon = 0.01;
  real_type m_A  = 1/m_h;
  real_type m_A1 = (1-m_epsilon)*power2(m_h/(1-m_h));
  real_type x   = abs(x_in);
  real_type Xh  = x/m_h;
  real_type res = 2*m_epsilon*Xh;
  if ( Xh > 1 ) res += 2*m_A1 * (Xh-1);
  res /= m_h;
  if ( x > 1 ) res += 2*m_A * (x-1);
  return (x_in < 0 ? -res : res) - RHS;
}

template <typename FUN>
void
do_solve( real_type a, real_type b, FUN f ) {
  Algo748<real_type> solver;
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
do_solve2( real_type a, real_type b, real_type amin, real_type bmax, FUN f ) {
  Algo748<real_type> solver;
  real_type res = solver.eval2( a, b, amin, bmax, f );
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

  do_solve( m_pi/2, m_pi, [](real_type x) { return sin(x)-x/2; });
  for ( int i = 1; i <= 10; ++i )
    do_solve( power2(i)+1e-9, power2(i+1)-1e-9, fun1 );

  do_solve( -9, 31, []( real_type x ) { return fun2(x,-40,-1); } );
  do_solve( -9, 31, []( real_type x ) { return fun2(x,-100,-2); } );
  do_solve( -9, 31, []( real_type x ) { return fun2(x,-200,-3); } );

  do_solve( 0, 5, []( real_type x ) { return fun3(x,4,0.2); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,6,0.2); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,8,0.2); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,10,0.2); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,12,0.2); } );

  do_solve( 0, 5, []( real_type x ) { return fun3(x,8,1); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,10,1); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,12,1); } );
  do_solve( 0, 5, []( real_type x ) { return fun3(x,14,1); } );

  do_solve( 0, 1.5, []( real_type x ) { return sin(x)-0.5; } );

  for ( int i = 1; i <= 5; ++i )
    do_solve( 0, 1, [i]( real_type x ) { return fun4(x,i); } );
  for ( int i = 20; i <= 100; i += 20 )
    do_solve( 0, 1, [i]( real_type x ) { return fun4(x,i); } );

  do_solve( 0, 1, []( real_type x ) { return fun5(x,5); } );
  do_solve( 0, 1, []( real_type x ) { return fun5(x,10); } );
  do_solve( 0, 1, []( real_type x ) { return fun5(x,20); } );

  do_solve( 0, 1, []( real_type x ) { return fun6(x,2); } );
  do_solve( 0, 1, []( real_type x ) { return fun6(x,5); } );
  do_solve( 0, 1, []( real_type x ) { return fun6(x,10); } );
  do_solve( 0, 1, []( real_type x ) { return fun6(x,15); } );
  do_solve( 0, 1, []( real_type x ) { return fun6(x,20); } );

  do_solve( 0, 1, []( real_type x ) { return fun7(x,1); } );
  do_solve( 0, 1, []( real_type x ) { return fun7(x,2); } );
  do_solve( 0, 1, []( real_type x ) { return fun7(x,3); } );
  do_solve( 0, 1, []( real_type x ) { return fun7(x,5); } );
  do_solve( 0, 1, []( real_type x ) { return fun7(x,8); } );
  do_solve( 0, 1, []( real_type x ) { return fun7(x,15); } );
  do_solve( 0, 1, []( real_type x ) { return fun7(x,20); } );

  do_solve( 0, 1, []( real_type x ) { return fun8(x,1); } );
  do_solve( 0, 1, []( real_type x ) { return fun8(x,5); } );
  do_solve( 0, 1, []( real_type x ) { return fun8(x,10); } );
  do_solve( 0, 1, []( real_type x ) { return fun8(x,15); } );
  do_solve( 0, 1, []( real_type x ) { return fun8(x,20); } );

  do_solve( 0.01, 1, []( real_type x ) { return fun9(x,2); } );
  do_solve( 0.01, 1, []( real_type x ) { return fun9(x,5); } );
  do_solve( 0.01, 1, []( real_type x ) { return fun9(x,15); } );
  do_solve( 0.01, 1, []( real_type x ) { return fun9(x,20); } );

  for ( int i = 2; i <= 9; ++i )
    do_solve( 1, 100, [i]( real_type x ) { return fun10(x,i); } );
  for ( int i = 11; i <= 33; i += 2 )
    do_solve( 1, 100, [i]( real_type x ) { return fun10(x,i); } );

  do_solve( -1, 4, []( real_type x ) { return fun11(x); } );

  for ( int i = 1; i <= 8; ++i )
    do_solve( -1e4, m_pi/2, [i]( real_type x ) { return fun12(x,i); } );
  for ( int i = 0; i <= 40; i += 10 )
    do_solve( -1e4, m_pi/2, [i]( real_type x ) { return fun12(x,i); } );

  for ( int i = 20; i <= 40; ++i )
    do_solve( -1e4, 1e-4, [i]( real_type x ) { return fun13(x,i); } );
  for ( int i = 100; i <= 1000; i += 100 )
    do_solve( -1e4, 1e-4, [i]( real_type x ) { return fun13(x,i); } );

  do_solve( 0.5,  5,      [] ( real_type x ) { return log(x); } );
  do_solve( 0.5,  8,      [] ( real_type x ) { return (10-x)*exp(-10*x)-pow(x,10)+1; } );
  do_solve( 1,    4,      [] ( real_type x ) { return exp(sin(x))-x-1; } );
  do_solve( 0.5,  1,      [] ( real_type x ) { return 11*pow(x,11)-1; } );
  do_solve( 0.1,  m_pi/3, [] ( real_type x ) { return 2*sin(x)-1; } );
  do_solve( 0,    1,      [] ( real_type x ) { return power2(x)+sin(x/10)-0.25; } );
  do_solve( 0,    1.5,    [] ( real_type x ) { return (x-1)*exp(x); } );
  do_solve( 0,    1.7,    [] ( real_type x ) { return cos(x)-x; } );
  do_solve( 1.5,  3,      [] ( real_type x ) { return power3(x-1)-1; } );
  do_solve( 2.6,  3.5,    [] ( real_type x ) { return exp(x*x+7*x-30)-1; } );
  do_solve( 1,    8,      [] ( real_type x ) { return atan(x)-1; } );
  do_solve( 0.2,  3,      [] ( real_type x ) { return exp(x)-2*x-1; } );
  do_solve( 0,    0.5,    [] ( real_type x ) { return exp(-x)-x-sin(x); } );
  do_solve( 0.1,  1.5,    [] ( real_type x ) { return power3(x)-1; } );
  do_solve( -1,   2,      [] ( real_type x ) { return power2(x)-power2(sin(x))-1; } );
  do_solve( -0.5, 1/3.0,  [] ( real_type x ) { return power3(x); } );
  do_solve( -0.5, 1/3.0,  [] ( real_type x ) { return power5(x); } );
  do_solve( 0.0, 1.0, [] ( real_type x ) { return tan(m_pi*(x*x*x*x*x*x*x*x-0.5)); } );
  do_solve( -1.0, 1.0, [] ( real_type x ) { return fun_penalty(x,0); } );
  do_solve( -1.0, 1.0, [] ( real_type x ) { return fun_penalty(x,-10); } );
  do_solve2( -1, 1.1498547501802843, -100, 100, [] ( real_type x ) { return fun_penalty(x,-229.970950036057); } );

  cout << "\nAll Done Folks!\n";

  return 0;
}
