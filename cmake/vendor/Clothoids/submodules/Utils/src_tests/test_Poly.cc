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

#include "Utils_Poly.hh"

using namespace std;

using real_type = double;
using integer   = int;
using poly      = Utils::Poly<real_type>;
using sturm     = Utils::Sturm<real_type>;

static
void
test1() {
  poly      poly2solve(7);
  sturm     mySturm;
  real_type myT;
  poly2solve << -3240000, 2592000, -478800, -3360, -9, 0, +1;

  fmt::print( "{}\n", poly2solve );
  poly2solve.normalize();
  mySturm.build( poly2solve );
  integer n_roots = mySturm.separate_roots( 0, 50 );
  fmt::print( "Number of roots: {}\n", n_roots );
  mySturm.refine_roots();
  if ( n_roots > 0 ){
    fmt::print(
      "Roots to solve T\n"
      "{}\n"
      "Attempt to print roots\n",
      mySturm.roots()
    );

    fmt::print( "roots = {}\n", mySturm.roots() );

    myT = mySturm.roots().minCoeff();
    //cout << this->m_T << std::endl;
  } else {
    fmt::print( "No roots to solve T\n" );
    myT = 0;
  }
  fmt::print( "Solved T\nT = {}\n", myT );
}

static
void
test2() {

  poly p1( 4 );
  poly p2( 3 );
  poly p3( 10 );
  poly D_p1, I_p1;

  p1 << 2, -2, 2, 3;
  p2 << 4, 0, 1;

  p1.derivative(D_p1);
  p1.integral(I_p1);

  fmt::print(
    "Il grado di p1: {}\n"
    "Il grado di p2: {}\n\n"
    "p1    = {}\n"
    "p1(2) = {}\n"
    "p2    = {}\n"
    "p2(3) = {}\n\n"
    "derivata di p1 = {}\n"
    "integrale di p1 = {}\n",
    p1.degree(), p2.degree(),
    p1, p1.eval( 2 ), p2, p2.eval( 3 ),
    D_p1, I_p1
  );

  p3  = p1 + p2;
  fmt::print( "sum of p1 and p2 = {}\n\n", p3 );

  p3  = p1 * p2;
  fmt::print( "product of p1 and p2 = {}\n", p3 );

  fmt::print(
    "product of\n"
    "p3 = {}\n"
    "with\n"
    "p1 = {}\n",
    p3, p1
  );

  fmt::print( "p3 * p1 => {}\n\n", p3*p1 );
  p3 *= p1;
  fmt::print( "p3 *= p1 => {}\n\n", p3 );

  // test division
  poly P( 6 ), Q(3), S(3), R(2), G;
  Q << 1, 2, 3;
  S << 1, 0, 1;
  R << 3, 2;
  P = Q*S+R;

  fmt::print(
    "P = {}\n"
    "Q = {}\n"
    "S = {}\n"
    "R = {}\n\n",
    P, Q, S, R
  );

  Utils::divide( P, Q, S, R );
  fmt::print(
    "P = {}\n"
    "Q = {}\n"
    "S = {}\n"
    "R = {}\n\n",
    P, Q, S, R
  );

  P.set_degree( 4 );
  P << -3, -1, 0, 3, 1;

  Q.set_degree( 3 );
  Q << 3, -5, 1, 1;

  Utils::GCD( P, Q, G, 1e-20 );
  G.make_monic();

  fmt::print(
    "P   = {}\n"
    "Q   = {}\n"
    "GCD = {}\n\n",
    P, Q, G
  );

  Utils::divide( P, G, S, R );
  fmt::print(
    "TEST DIVIDE\n"
    "P   = {}\n"
    "G   = {}\n"
    "P/G = {}\n"
    "R   = {}\n\n",
    P, G, S, R
  );

  Utils::divide( Q, G, S, R );

  fmt::print(
    "TEST DIVIDE\n"
    "Q   = {}\n"
    "G   = {}\n"
    "Q/G = {}\n"
    "R   = {}\n\n",
    Q, G, S, R
  );

  Utils::divide( G, Q, S, R );

  fmt::print(
    "TEST DIVIDE\n"
    "Q   = {}\n"
    "G   = {}\n"
    "G/Q = {}\n"
    "R   = {}\n\n",
    Q, G, S, R
  );

  sturm STURM;

  P.set_order( 13 );
  // roots of
  // (x-1)^3*(x-2.5)*(x-1.345)^2*(x+5.4)^3*(x-Pi)^3
  //
  P << -22080.83411,
       116727.3033385665948243520,
       -256946.0673316927782141151,
       300144.5335915349392862061,
       -191931.3045536365545769490,
       55772.64332394731276079758,
       3985.892468682997492135005,
       -6973.325399061096968788543,
       1155.465698015199421043348,
       210.0853813805896047708528,
       -63.97763326249465566375384,
       -1.414777960769379715387929,
       1;

/*
  Poly<double> DP;
  P.derivative(DP);

  Utils::GCD( P, DP, G );
  G.make_monic();

  fmt::print(
    "TEST GCD(P,DP)\n"
    "P   = {}\n"
    "DP  = {}\n"
    "GCD = {}\n\n",
    P, DP, G
  );
*/

  STURM.build( P );
  Utils::Sturm<double>::Integer n_roots = STURM.separate_roots( -10, 20 );
  fmt::print( "N.roots {}\n", n_roots );

  Eigen::VectorXd x(10);
  x << -10, -8, -4, 0, 0.99, 1, 1.01, 2, 10, 20;

  fmt::print( "Sturm sequence\n{}\n", STURM );

  for ( auto const & xx : x ) {
    bool on_root;
    int s0 = STURM.sign_variations( xx, on_root );
    fmt::print( "x = {:<10}, sign var = {}\n", xx, s0 );
  }

  STURM.refine_roots();

  fmt::print( "ROOTS = {}\n", STURM.roots() );

  for ( auto const & xx : STURM.roots() ) {
    fmt::print( "P({}) = {}\n", xx, P.eval(xx) );
  }

  poly const & SP = STURM.get(0);

  for ( auto const & xx : STURM.roots() ) {
    fmt::print("P[sturm]({}) = {}\n", xx, SP.eval(xx) );
  }
}


int
main() {
  test1();
  test2();
  fmt::print( "\n\nAll Done Folks!\n\n" );
  return 0;
}
