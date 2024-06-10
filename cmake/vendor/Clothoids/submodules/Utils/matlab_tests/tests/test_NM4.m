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
%X0 = [0.1,0.1];
X0 = [100,6];

xx = 0:0.5:120;
yy = 0:0.5:120;
[X,Y] = meshgrid(xx,yy);
Z = test_fun2(X,Y)+test_fun2_barrier(X,Y);
hold off;
contourf(haxhax,X,Y,Z,200);
hold on
axis equal

solver = NelderMead();
myfun = @(x) test_fun2(x(1),x(2))+test_fun2_barrier(x(1),x(2));

solver.setup( myfun, 2 );
solver.set_max_iteration(1000);

tic
x_sol = solver.run( X0, 1 );
mysolver_time = toc;

plot( x_sol(1), x_sol(2),  'o', 'Color', 'blue', 'MarkerFaceColor', 'green', 'MarkerSize', 10 );
