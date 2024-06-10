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

global haxhax;
addpath('../lib');
haxhax=axes;

%% Initial guess
X0 = [0,0];
X1 = [1,1];
X2 = [(1+sqrt(33))/8,(1-sqrt(33))/8];

xx    = -1:0.1:1.5;
yy    = -1:0.1:1.5;
[X,Y] = meshgrid(xx,yy);
Z     = test_fun0(X,Y);
hold off;
contour(haxhax,X,Y,Z,200);
hold on
%contourf(haxhax,X,Y,Z,'LevelList',[1000]);
axis equal

solver = NelderMead();

fun = @(x) test_fun0( x(:,1), x(:,2) );

%% My solver no gradient
solver.setup( fun, 2 );

tic
x_sol = solver.run( [X0;X1;X2] );
mysolver_time = toc;
