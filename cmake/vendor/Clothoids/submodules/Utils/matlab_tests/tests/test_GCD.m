p = Poly();
q = Poly();
gg = Poly();
p.set_by_coeffs([1,2,-2,-4]);
q.set_by_coeffs([1,-1,-1]);
gg.set_by_coeffs([1/3,1]);

p = p*gg;
q = q*gg;

fprintf('p(x) = %s\n',p.to_string());
fprintf('q(x) = %s\n',q.to_string());

[s,r] = p.divide(q);
fprintf('p(x)/q(x) => s(x)=%s, r(s)=%s\n\n',s.to_string(),r.to_string());

t = s*q+r;
fprintf('s(x)*q(x)+r(x) = %s\n\n',t.to_string());


[s,r] = q.divide(p);
fprintf('q(x)/p(x) => s(x)=%s, r(s)=%s\n\n',s.to_string(),r.to_string());

t = s*p+r;
fprintf('s(x)*p(x)+r(x) = %s\n\n',t.to_string());


g = p.GCD(q);
fprintf('p.GCD(q) = %s\n\n',g.to_string());

g = q.GCD(p);
fprintf('q.GCD(p) = %s\n\n',g.to_string());

[s,r] = p.divide(g);
fprintf('p(x)/g(x) => s(x)=%s, r(s)=%s\n\n',s.to_string(),r.to_string());
[s,r] = q.divide(g);
fprintf('q(x)/g(x) => s(x)=%s, r(s)=%s\n\n',s.to_string(),r.to_string());


