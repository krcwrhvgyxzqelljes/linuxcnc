%% TEST STURM CLASS

clc;
clear all;
close all;

ell     = 0.15;
CodeGen = [0 0 -0.005134583085657 0.211003985866756 -1.363267351305853 0 0 0 0];

% Apply Sturm
P     = Poly( CodeGen );
roots = P.real_roots_in_the_interval( 0, ell );

for r=roots
  fprintf('P(%g) = %g\n', r, P.eval(r) );
end

x = 0.0:0.001:ell;
y = P.eval(x);
plot(x,y, 'LineWidth', 1.1); hold on;
xlabel('x'); ylabel('y');
xlim([0.0, ell]);
plot(roots, P.eval( roots ), 'o','MarkerFaceColor','red', 'MarkerSize', 20.0);
grid on; grid minor;






