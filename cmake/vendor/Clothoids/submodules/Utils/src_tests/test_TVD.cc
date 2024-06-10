/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2023                                                      |
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

#include "Utils_TVD.hh"
#include "Utils_eigen.hh"

using namespace std;
using integer   = int;
using real_type = double;
using dmat_t    = Eigen::Matrix<real_type,Eigen::Dynamic,Eigen::Dynamic>;
using dvec_t    = Eigen::Matrix<real_type,Eigen::Dynamic,1>;

int
main() {

  integer N = 1000;

  // Generate test signal: piecewise smooth with noise corruption
  dvec_t t, y, x;
  t.resize(N);
  x.resize(N);
  y.resize(N);
  for ( integer i = 0; i < N; ++i ) t(i) = (i*4*3.1415)/N;
  y = sin(t.array())+(t.array()-floor(t.array())) + 0.1*sin(100*t.array());

  real_type lambda = 0.1;
  Utils::TVD<real_type>::denoise( N, y.data(), lambda, x.data() );

  ofstream file("TVD.txt");
  file << "t\ty\tx\n";
  for ( integer i = 0; i < N; ++i )
    file << t(i) << '\t'
         << y(i) << '\t'
         << x(i) << '\n';
  file.close();
  cout << "All done folks!\n\n";
  return 0;
}
