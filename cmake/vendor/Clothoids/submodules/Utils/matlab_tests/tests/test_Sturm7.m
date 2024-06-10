%% TEST STURM CLASS

clc;
clear all;
close all;

CodeGen = [1,2,3,4,5,6]; %1.001,2.001,3.001,4.001,5.001,6.1];

% Apply Sturm
P     = Poly( CodeGen );
roots = P.real_roots_in_the_interval( -0.7, -0.6 );

for r=roots
  fprintf('P(%g) = %g\n', r, P.eval(r) );
end

x = -0.7:0.0001:-0.6;
y = P.eval(x);
plot(x,y, 'LineWidth', 1.1); hold on;
xlabel('x'); ylabel('y');
xlim([-3.5,1.5]);
plot(roots, P.eval( roots ), 'o','MarkerFaceColor','red', 'MarkerSize', 20.0);
grid on; grid minor;

P.eval(roots)
