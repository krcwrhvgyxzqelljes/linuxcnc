%% TEST STURM CLASS

clc;
clear all;
close all;

CodeGen = [-3,-1,+3,+1];

% Apply Sturm
P     = Poly( CodeGen );
roots = P.real_roots_in_the_interval( -4, 4 );

for r=roots
  fprintf('P(%g) = %g\n', r, P.eval(r) );
end

x = -4:0.001:4;
y = P.eval(x);
plot(x,y, 'LineWidth', 1.1); hold on;
xlabel('x'); ylabel('y');
xlim([-3.5,1.5]);
plot(roots, P.eval( roots ), 'o','MarkerFaceColor','red', 'MarkerSize', 20.0);
grid on; grid minor;
