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
X0 = [-1.1;-27];

xx = -20:0.1:1;
yy = -30:0.1:1;
[X,Y] = meshgrid(xx,yy);
Z = test_fun1(X,Y)+test_fun1_barrier(X,Y);
hold off;
contour(haxhax,X,Y,Z,200);
hold on
contourf(haxhax,X,Y,Z,'LevelList',[1000]);
axis equal

solver = NelderMead();
myfun = @(x) test_fun1(x(1),x(2))+test_fun1_barrier(x(1),x(2));

solver.setup( myfun, 2 );

tic
x_sol = solver.run( X0, 1 );
mysolver_time = toc;

x_sol

plot( x_sol(1), x_sol(2),  'o', 'Color', 'blue', 'MarkerFaceColor', 'green', 'MarkerSize', 10 );
