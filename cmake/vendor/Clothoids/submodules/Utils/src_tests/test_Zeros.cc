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

#include "Utils_zeros.hh"

using namespace std;

using Utils::Zeros;
using Utils::m_pi;

using real_type = double;

//static inline real_type power2( real_type x ) { return x*x; }
//static inline real_type power3( real_type x ) { return x*x*x; }
//static inline real_type power4( real_type x ) { return power2(power2(x)); }
//static inline real_type power5( real_type x ) { return power4(x)*x; }

static int ntest{0};

static
void
do_solve( real_type x_guess, Utils::Zeros_base_fun<real_type> * pf ) {
  Zeros<real_type> solver;
  real_type x = solver.solve_Newton( x_guess, pf );
  ++ntest;
  fmt::print(
    "[NEWTON]    #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  x = solver.solve_Halley( x_guess, pf );
  fmt::print(
    "[HALLEY]    #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  x = solver.solve_Chebyshev( x_guess, pf );
  fmt::print(
    "[CHEBYSHEV] #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  x = solver.solve_Order4( x_guess, pf );
  fmt::print(
    "[ORDER4]    #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  x = solver.solve_Order8( x_guess, pf );
  fmt::print(
    "[ORDER8]    #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  x = solver.solve_Order16( x_guess, pf );
  fmt::print(
    "[ORDER16]   #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  x = solver.solve_Order32( x_guess, pf );
  fmt::print(
    "[ORDER32]   #{:<3} iter = {:<3} #nfun = {:<3} converged={} x = {:12} f(x) = {}\n",
    ntest, solver.used_iter(), solver.num_fun_eval(), solver.converged(),
    fmt::format("{:.6}",x),
    fmt::format("{:.3}",pf->eval(x))
  );
  fmt::print("\n\n");
}

class class_fun0 : public Utils::Zeros_base_fun<real_type> {
public:
  real_type
  eval( real_type x ) const override {
    real_type t1 = x*x;
    return (x+4)*t1-10;
  };

  real_type
  eval_D( real_type x ) const override {
    return (3*x+8)*x;
  };

  real_type
  eval_DD( real_type x ) const override {
    return 6*x+8;
  };

  real_type
  eval_DDD( real_type ) const override {
    return 6;
  };
};


class class_fun1 : public Utils::Zeros_base_fun<real_type> {
public:
  real_type
  eval( real_type x ) const override
  { return log(1+x*x)+exp(x*(x-3))*sin(x); };

  real_type
  eval_D( real_type x ) const override {
    real_type t1 = x*x;
    real_type t2 = t1+1.0;
    real_type t4 = sin(x);
    real_type t6 = cos(x);
    real_type t12 = exp(x*(x-3.0));
    return 2.0/t2*(t12*(t4*(x-3.0/2.0)+t6/2.0)*t2+x);
  };

  real_type
  eval_DD( real_type x ) const override {
    real_type t1 = x*x;
    real_type t3 = pow(t1+1.0,2.0);
    real_type t6 = sin(x);
    real_type t8 = cos(x);
    real_type t15 = exp(x*(x-3.0));
    return 1/t3*(4.0*t15*(t6*(t1-3.0*x+5.0/2.0)+(x-3.0/2.0)*t8)*t3-2.0*t1+2.0);
  };

  real_type
  eval_DDD( real_type x ) const override {
    real_type t2 = x*x;
    real_type t3 = 3.0*x;
    real_type t6 = sin(x);
    real_type t9 = cos(x);
    real_type t13 = t2+1.0;
    real_type t14 = t13*t13;
    real_type t15 = t14*t13;
    real_type t19 = exp(x*(x-3.0));
    return 1/t15*(8.0*t19*t15*(t6*(t2-t3+3.0)*(x-3.0/2.0)+3.0/2.0*t9*(t2-t3+8.0/3.0))+4.0*t2*x-12.0*x);
  };
};

class class_fun2 : public Utils::Zeros_base_fun<real_type> {
public:
  real_type
  eval( real_type x ) const override {
    real_type t1 = x*x;
    real_type t3 = exp(-t1+x+2.0);
    real_type t6 = cos(x+1.0);
    return x*t1+t3-t6+1.0;
  };

  real_type
  eval_D( real_type x ) const override {
    real_type t3 = x*x;
    real_type t5 = exp(-t3+x+2.0);
    real_type t9 = sin(x+1.0);
    return t5*(-2.0*x+1.0)+3.0*t3+t9;
  };

  real_type
  eval_DD( real_type x ) const override {
    real_type t1 = x*x;
    real_type t3 = exp(-t1+x+2.0);
    real_type t7 = pow(-2.0*x+1.0,2.0);
    real_type t11 = cos(x+1.0);
    return t3*t7+t11-2.0*t3+6.0*x;
  };

  real_type
  eval_DDD( real_type x ) const override {
    real_type t2 = -2.0*x+1.0;
    real_type t3 = x*x;
    real_type t5 = exp(-t3+x+2.0);
    real_type t8 = t2*t2;
    real_type t12 = sin(x+1.0);
    return t5*t2*t8-6.0*t5*t2-t12+6.0;
  };
};

class class_fun3 : public Utils::Zeros_base_fun<real_type> {
public:
  real_type
  eval( real_type x ) const override {
    real_type t1 = x*x;
    real_type t2 = t1+1.0;
    real_type t5 = cos(m_pi*x/2.0);
    real_type t9 = log(t1+2.0*x+2.0);
    return t5*t2+1/t2*t9;
  };

  real_type
  eval_D( real_type x ) const override {
    real_type t2 = m_pi*x/2.0;
    real_type t3 = cos(t2);
    real_type t6 = x*x;
    real_type t7 = t6+1.0;
    real_type t9 = sin(t2);
    real_type t14 = t6+2.0*x+2.0;
    real_type t19 = log(t14);
    real_type t20 = t7*t7;
    return 2.0*t3*x-t9*m_pi*t7/2.0+2.0/t7/t14*(x+1.0)-2.0*x/t20*t19;
  };

  real_type
  eval_DD( real_type x ) const override {
    real_type t1 = m_pi*x;
    real_type t2 = t1/2.0;
    real_type t3 = cos(t2);
    real_type t5 = sin(t2);
    real_type t8 = x*x;
    real_type t9 = t8+1.0;
    real_type t10 = m_pi*m_pi;
    real_type t15 = t8+2.0*x+2.0;
    real_type t16 = 1/t15;
    real_type t17 = 1/t9;
    real_type t20 = x+1.0;
    real_type t22 = t15*t15;
    real_type t27 = t9*t9;
    real_type t28 = 1/t27;
    real_type t32 = log(t15);
    return 2.0*t3-2.0*t5*t1-t3*t10*t9/4.0+2.0*t17*t16-4.0*t17/t22*t20*t20-8.0*x*t28*t20*t16+8.0*t8/t9/t27*t32-2.0*t28*t32;
  };

  real_type
  eval_DDD( real_type x ) const override {
    real_type t2  = m_pi*x/2.0;
    real_type t3  = sin(t2);
    real_type t6  = m_pi*m_pi;
    real_type t8  = cos(t2);
    real_type t11 = x*x;
    real_type t12 = t11+1.0;
    real_type t18 = t11+2.0*x+2.0;
    real_type t19 = t18*t18;
    real_type t20 = 1/t19;
    real_type t21 = 1/t12;
    real_type t23 = x+1.0;
    real_type t26 = 1/t18;
    real_type t27 = t12*t12;
    real_type t28 = 1/t27;
    real_type t32 = 4.0*t23*t23;
    real_type t44 = 2.0*t26*t23;
    real_type t46 = 1/t12/t27;
    real_type t52 = log(t18);
    real_type t53 = t27*t27;
    return -3.0*t3*m_pi-3.0/2.0*t8*t6*x+t3*m_pi*t6*t12/8.0-12.0*t23*t21*t20-12.0*x*t28*t26+4.0*t21/t18/t19*t23*t32+6.0*x*t28*t20*t32+24.0*t11*t46*t44-6.0*t28*t44-48.0*x*t11/t53*t52+24.0*x*t46*t52;
  };
};

class class_fun4 : public Utils::Zeros_base_fun<real_type> {
public:
  real_type
  eval( real_type x ) const override {
    real_type t1 = x*x;
    real_type t2 = t1*t1;
    real_type t5 = sin(1/t1*m_pi);
    return t2+t5-5.0;
  };

  real_type
  eval_D( real_type x ) const override {
    real_type t1 = x*x;
    real_type t2 = x*t1;
    real_type t8 = cos(1/t1*m_pi);
    return 4.0*t2-2.0*t8/t2*m_pi;
  };

  real_type
  eval_DD( real_type x ) const override {
    real_type t1 = x*x;
    real_type t3 = t1*t1;
    real_type t7 = 1/t1*m_pi;
    real_type t8 = cos(t7);
    real_type t11 = m_pi*m_pi;
    real_type t15 = sin(t7);
    return 12.0*t1+6.0*t8/t3*m_pi-4.0*t15/t1/t3*t11;
  };

  real_type
  eval_DDD( real_type x ) const override {
    real_type t2 = x*x;
    real_type t3 = t2*t2;
    real_type t8 = 1/t2*m_pi;
    real_type t9 = cos(t8);
    real_type t12 = m_pi*m_pi;
    real_type t17 = sin(t8);
    real_type t21 = t3*t3;
    return 24.0*x-24.0*t9/x/t3*m_pi+36.0*t17/x/t2/t3*t12+8.0*t9/x/t21*m_pi*t12;
  };

};

class_fun0 fun0;
class_fun1 fun1;
class_fun2 fun2;
class_fun3 fun3;
class_fun4 fun4;

int
main() {

  do_solve( -0.3,    &fun0);
  do_solve( 0.35,    &fun1 );
  do_solve( -0.3-10, &fun2 );
  do_solve( -1.1,    &fun3 );
  do_solve(  1.5+4,  &fun4 );
  cout << "\nAll Done Folks!\n";

  return 0;
}
