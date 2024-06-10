p = Poly();
S = Sturm();
p.set_by_coeffs([1,2,-2,-4]);

fprintf('p(x) = %s\n',p.to_string());

a = -2;
b = 2;
x = a:(b-a)/400:b;
y = p.eval(x);

plot(x,y,'LineWidth',2);
xL = xlim;
yL = ylim;
line([0 0], yL);  %x-axis
line(xL, [0 0]);  %y-axis

S.build( p );
S.separate_roots( a, b );
S.refine_roots();

hold on;
for ipos=1:S.n_intervals()
  I0 = S.get_interval(ipos);
  plot([I0.a,I0.b],[0,0],'LineWidth',4);
end
hold off;

S.print( true );
