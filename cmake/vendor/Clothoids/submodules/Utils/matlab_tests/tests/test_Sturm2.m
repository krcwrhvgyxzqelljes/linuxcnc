%% TEST STURM CLASS

clc;
clear all;
close all;

ell = 0.1;

CodeGen = [ ...
  0, 0, 1.311999999999999e+15, ...
  -9.599999999999992e+16, ...
   2.719999999999998e+18, ...
  -4.479999999999996e+19, ...
   4.479999999999996e+20, ...
  -2.559999999999997e+21, ...
   6.399999999999994e+21 ...
];

% Apply Sturm
P = Poly( CodeGen );
P.normalize

S = Sturm();
S.build( P );

grid on;
grid minor;

% --------------
subplot(2,2,1);
x = 0:0.001:ell;
y = P.eval(x);
plot(x,y);
title('P');

% --------------
subplot(2,2,3);
R1 = P.to01(0,ell);
R1.normalize

x = 0:0.001:1;
y = R1.eval(x);
plot(x,y);
title('P01');

% --------------
subplot(2,2,2);
x = 0:0.001:ell;
Q = S.get(1);
y = Q.eval(x);
plot(x,y);
title('Q');

% --------------
subplot(2,2,4);
R2 = Q.to01(0,ell);
x = 0:0.001:1;
z = R2.eval(x);
plot(x,z);
title('Q01');

S.separate_roots( 0, ell );
S.refine_roots();
S.print(true);

x_roots = P.real_roots_in_the_interval( 0, ell );
x_roots - S.roots
