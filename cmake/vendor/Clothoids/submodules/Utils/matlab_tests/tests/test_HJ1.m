%=========================================================================%
%                                                                         %
%  Autors: Enrico Bertolazzi                                              %
%          Department of Industrial Engineering                           %
%          University of Trento                                           %
%          enrico.bertolazzi@unitn.it                                     %
%                                                                         %
%=========================================================================%
close all;

global haxhax;
addpath('../lib');
haxhax=axes;

%% Initial guess
X0 = [-1,1];

conv_tolerance = 1e-8;

xx    = -2:0.1:2;
yy    = -2:0.1:2;
[X,Y] = meshgrid(xx,yy);
Z     = myfun2(X,Y);
hold off;
contour(haxhax,X,Y,Z,200);
hold on
contourf(haxhax,X,Y,Z,'LevelList',[1000]);
axis equal

HJSolver = HJPatternSearch();

%% My solver no gradient
HJSolver.setup( @myfun, 2 );

tic
x_sol         = HJSolver.run(X0,0.1);
mysolver_time = toc;

HJSolver.print_info(conv_tolerance);
x_sol

function res = myfun2(X,Y)
  res = (Y-X.^2).^2+(1-X).^2;
end

function res = myfun(XX)
  res = myfun2(XX(1),XX(2));
end
