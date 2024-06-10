%=========================================================================%
%                                                                         %
%  Autors: Enrico Bertolazzi                                              %
%          Department of Industrial Engineering                           %
%          University of Trento                                           %
%          enrico.bertolazzi@unitn.it                                     %
%                                                                         %
%=========================================================================%
close all;

addpath('../lib');

%% Initial guess
X0 = [10,10];
lb = [-20,-20]
ub = [20,20];

conv_tolerance = 1e-8;

HJSolver = HJPatternSearch();

%% My solver no gradient
HJSolver.setup( @myfun, X0, lb, ub );

tic
x_sol = HJSolver.run;
mysolver_time = toc;

HJSolver.print_info(conv_tolerance)

function res = myfun(x)
  res = dot(x,x);
end
