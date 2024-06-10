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

solver1 = Minimize1D();
solver2 = Minimize1D();

funs = {
  {   2.7,  7.5, @(x) sin(x)+sin((10/3)*x) }, % [1] n.1
  {   -10,   10, @(x) sum((1:6).'.*sin(((1:6).'+1).*x+(1:6).')) },
  {    -2,    4, @(x) (16*x.^2-24*x+5).*exp(-x.^2) },
  {     0,  1.2, @(x) -(1.4-3*x).*sin(18*x) },
  {   -10,   10, @(x) -(x+sin(x)).*exp(-x.^2) },
  {     2,    8, @(x) sin(x)+sin( (10/3)*x ) + log(x) - 0.84*x + 3 },
  {   -10,   10, @(x) -sum((1:6).'.*cos(((1:6).'+1).*x+(1:6).')) },
  {     0,   25, @(x) sin(x) + sin( (2/3)*x ) },
  {     0,   10, @(x) -x.*sin(x) },
  {    -2,    7, @(x) 2*cos(x) + cos(2*x) },
  {     0,    7, @(x) sin(x).^3+cos(x).^3 },
  { 0.001, 0.99, @(x) -abs(x).^(2/3)-abs(1-x.^2).^(1/3) },
  {     0,    4, @(x) -exp(-x).*sin(2*pi*x) },
  {    -5,    5, @(x) (x.^2-5*x+6) ./ (x.^2+1) },
  {     0,    6, @(x) p18(x) },
  {   -10,   10, @(x) (sin(x)-x).*exp(-x.^2) },
  {     0,   10, @(x) x.*sin(x)+x.*cos(2*x) },
  {     0,   20, @(x) exp(-3*x) - sin(x).^3 },
};

for k=1:length(funs)
  a   = funs{k}{1};
  b   = funs{k}{2};
  fun = funs{k}{3};
  [x,fx]   = solver1.golden_search( a, b, fun );
  [x1,fx1] = solver2.brent_search( a, b, fun );
  %[x,fx] = solver.eval( a, b, fun );
  %[x,fx] = solver.eval_global( a, b, fun );

  if true
    kk = 1+mod(k-1,9);
    if kk == 1
      figure();
    end
    subplot(3,3,kk);
    xx = a:(b-a)/1000:b;
    yy = fun(xx);
    hold off;
    plot(xx,yy,'LineWidth',2);
    hold on;
    plot(x,fx,'ob','MarkerSize',10,'MarkerFaceColor','red');
    plot(x1,fx1,'ob','MarkerSize',5,'MarkerFaceColor','blue');
    title(sprintf('N.%d',k));
  end

  fprintf( ...
    '#%-3d iter = %-3d/%3d #nfun = %-3d/%3d x = %-12.6g f(x) = %-.3g\n', ...
    k, solver1.num_iter_done(), solver2.num_iter_done(), ...
    solver1.num_fun_eval(), solver2.num_fun_eval(), x, fx ...
  );
end

function res = p18( x )
  res = (x-2).^2;
  idx = find( x > 3 );
  res(idx) = 2*log(x(idx)-2)+1;
end
