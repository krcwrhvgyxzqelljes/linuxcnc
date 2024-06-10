%% TEST STURM CLASS

clc;
clear all;
close all;

ell = 0.1100001+0.01;

CodeGen1 = [ ...
    0.00000000000000e+000, ...
    0.00000000000000e+000, ...
    87.4724022594292e+012, ...
   -4.78533691263029e+015, ...
    123.408382292671e+015, ...
   -1.84830637948408e+018, ...
    16.8027852680371e+018, ...
   -87.2871961975953e+018, ...
    198.379991358171e+018 ...
];

CodeGen0 = [ ...
    0.00000000000000e+000, ...
    0.00000000000000e+000, ...
    46.8750000000000e-003, ...
    2.00000000000000e+000, ...
    256.000000000000e+000, ...
   -1.02400000000000e+003, ...
    6.14400000000000e+003, ...
   -49.1520000000000e+003, ...
    262.144000000000e+003, ...
];

P = Poly(CodeGen0+CodeGen1);

S = Sturm();
%P.normalize
S.build(P);
S.separate_roots( 0, ell );
S.refine_roots();
S.print(true);

x_roots = P.real_roots_in_the_interval( 0, ell )
y_roots = P.eval( x_roots );
y_roots

PP = S.get(1);
z_roots = PP.eval( x_roots );

x = 0.0:0.001:ell;
y = P.eval(x);
z = PP.eval(x);
subplot(2,1,1);
plot(x,y, 'LineWidth', 1.1); hold on;
xlabel('x'); xlabel('y');
xlim([0.0, ell]);
plot(x_roots, y_roots, 'o','MarkerFaceColor','red', 'MarkerSize', 20.0);
grid on; grid minor;
hold off;


subplot(2,1,2);
plot(x,z, 'LineWidth', 2); hold on;
xlabel('x'); xlabel('y');
xlim([0.0, ell]);
plot(x_roots, z_roots, 'o','MarkerFaceColor','red', 'MarkerSize', 20.0);
grid on; grid minor;

P.eval(x_roots)
PP.eval(x_roots)

