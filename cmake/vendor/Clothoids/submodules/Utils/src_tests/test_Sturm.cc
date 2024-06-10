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
using Utils::Poly;


static
void
test1() {

  Poly<double> P( 4 );
  P << 1, 2, -2, -4;

  Utils::Sturm<double> S;
  P.normalize();
  S.build( P );

  double a = -2;
  double b = 2;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}


static
void
test2() {

  Poly<double> P( 9 );
  P << 0, 0, 1.311999999999999e+15,
  -9.599999999999992e+16,
   2.719999999999998e+18,
  -4.479999999999996e+19,
   4.479999999999996e+20,
  -2.559999999999997e+21,
   6.399999999999994e+21;

  Utils::Sturm<double> S;
  P.normalize();
  S.build( P );

  double a = 0;
  double b = 0.1094;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}


static
void
test3() {

  Poly<double> P( 9 );
  P << 0, 0,
  88740968089804.640625,
  -4915676989941691.0,
  127526512469551888.0,
  -1920443367344816640.0,
  17554326940994666496.0,
  -91691443933113974784.0,
  2.0953255012137562931e+20;

  Utils::Sturm<double> S;
  P.normalize();
  S.build( P );

  double a = 0;
  double b = 0.1094;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}


static
void
test4() {

  Poly<double> P0( 9 ), P1( 9 ), P( 9 ), DP(8);

  P0 <<
    0.00000000000000e+000,
    0.00000000000000e+000,
    87.4724022594292e+012,
   -4.78533691263029e+015,
    123.408382292671e+015,
   -1.84830637948408e+018,
    16.8027852680371e+018,
   -87.2871961975953e+018,
    198.379991358171e+018;

  P1 <<
    0.00000000000000e+000,
    0.00000000000000e+000,
    46.8750000000000e-003,
    2.00000000000000e+000,
    256.000000000000e+000,
   -1.02400000000000e+003,
    6.14400000000000e+003,
   -49.1520000000000e+003,
    262.144000000000e+003;

  P = P0+P1;
  P.derivative(DP);

  fmt::print(
    "P(x)  = {}\n"
    "P'(x) = {}\n",
    P.to_string(),
    DP.to_string()
  );

  Utils::Sturm<double> S;
  //P.normalize();
  S.build( P );

  double a = 0;
  double b = 0.1100001;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}

static
void
test5() {

  Poly<double> P( 9 );

  P << 0, 0, -0.005134583085657, 0.211003985866756, -1.363267351305853, 0, 0, 0, 0;

  Utils::Sturm<double> S;
  //P.normalize();
  S.build( P );

  double a = 0;
  double b = 0.15;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}


static
void
test6() {

  Poly<double> P( 4 );

  P << -3,-1,+3,+1;

  Utils::Sturm<double> S;
  //P.normalize();
  S.build( P );

  double a = -4;
  double b = 4;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}



static
void
test7() {

  Poly<double> P( 6 );

  P << 1,2,3,4,5,6;//1.001, 2.001, 3.001, 4.001, 5.001, 6.1;

  Utils::Sturm<double> S;
  //P.normalize();
  S.build( P );

  double a = -2;
  double b = 0;

  fmt::print("p(x) = {}\n",P.to_string() );

  Utils::Sturm<double>::Integer n_roots = S.separate_roots( a, b );

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}



static
void
test8() {

  Poly<double> P( 9 ), P1(8);

  P << 2.5678136684000492,
       0.032475277665839394,
       -3.1571112216028204E-7,
       -0.0022292180160741193,
       -0.024136475795403782,
       -0.00015262768831012657,
       1.5016498823759532E-9,
       0.000010476904806128173,
       0.000056718431225789833;

  P1 << 0.032475277665839394,
       -3.1571112216028204E-7*2,
       -0.0022292180160741193*3,
       -0.024136475795403782*4,
       -0.00015262768831012657*5,
       1.5016498823759532E-9*6,
       0.000010476904806128173*7,
       0.000056718431225789833*8;

  Utils::Sturm<double> S;
  //P.normalize();
  S.build( P );

  fmt::print("p(x) = {}\n",P.to_string() );
  Utils::Sturm<double>::Integer n_roots = S.separate_roots();

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }

  S.build( P1 );

  fmt::print("p'(x) = {}\n",P1.to_string() );
  n_roots = S.separate_roots();

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P'({}) = {} P -> {}\n", x, P1.eval(x), P.eval(x) );
  }
}


static
void
test9() {

  Poly<double> P( 9 );

  P << -1.9246341400688909E-23,
       3.2265850503182306E-20,
       2.921307661294706E-18,
       -2.9886608424247512E-14,
       1.7571015246010046E-11,
       -7.6999700746564349E-9,
       -0.0000074884938680270862,
       0.0066490626485599226,
       -1;

  Utils::Sturm<double> S;
  //P.normalize();
  S.build( P );

  fmt::print("p(x) = {}\n",P.to_string() );
  Utils::Sturm<double>::Integer n_roots = S.separate_roots();

  S.refine_roots();
  cout << S;

  fmt::print( "N.roots = {}\nCheck\n", n_roots );
  for ( auto & x : S.roots() ) {
    fmt::print( "P({}) = {}\n", x, P.eval(x) );
  }
}


int
main() {

  test1();
  cout << "\n\n\n\n\n\n\n";
  test2();
  cout << "\n\n\n\n\n\n\n";
  test3();
  cout << "\n\n\n\n\n\n\n";
  test4();
  cout << "\n\n\n\n\n\n\n";
  test5();
  cout << "\n\n\n\n\n\n\n";
  test6();
  cout << "\n\n\n\n\n\n\n";
  test7();
  cout << "\n\n\n\n\n\n\n";
  test8();
  cout << "\n\n\n\n\n\n\n";
  test9();

  cout << "\nAll Done Folks!\n";

  return 0;
}
