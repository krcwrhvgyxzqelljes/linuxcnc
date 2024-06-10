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
X0 = [0,-1];
X1 = [0,1];
X2 = [1,0];

xx    = -0.2:0.1:1.2;
yy    = -2:0.1:2;
[X,Y] = meshgrid(xx,yy);
Z     = test_fun_han(X,Y);
hold off;
contour(haxhax,X,Y,Z,200);
hold on
contourf(haxhax,X,Y,Z,'LevelList',[1000]);
axis equal

solver = NelderMead();

fun = @(x) test_fun_han( x(:,1), x(:,2) );

%% My solver no gradient
solver.setup( fun, 2 );

tic
x_sol = solver.run( [X0;X1;X2] );
mysolver_time = toc;

plot( x_sol(1), x_sol(2),  'o', 'Color', 'blue', 'MarkerFaceColor', 'green', 'MarkerSize', 10 );
