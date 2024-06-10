p = Poly();
q = Poly();
p.set_by_coeffs([1,-1,-1]);
q.set_by_coeffs([1,2,-2,-4]);

fprintf('p(x) = %s\n',p.to_string());
fprintf('q(x) = %s\n',q.to_string());
r = p+q;
fprintf('p(x)+q(x) = %s\n',r.to_string());

r = q+p;
fprintf('q(x)+p(x) = %s\n',r.to_string());

r = p-q;
fprintf('p(x)-q(x) = %s\n',r.to_string());

r = q-p;
fprintf('q(x)-p(x) = %s\n',r.to_string());

r = -q;
fprintf('-q(x) = %s\n',r.to_string());

r = +q;
fprintf('+q(x) = %s\n',r.to_string());

r = p*q;
fprintf('p(x)*q(x) = %s\n',r.to_string());

r = q*p;
fprintf('p(x)*p(x) = %s\n',r.to_string());

fprintf('p(x) = %s\n',p.to_string());
r = p.integral();
fprintf('I(p(x)) = %s\n',r.to_string());
r = r.derivative();
fprintf('D(I(p(x))) = %s\n',r.to_string());
