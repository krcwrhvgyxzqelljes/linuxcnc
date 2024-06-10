% Negate a polynomial
%
% Given p(x) = 2-x-x^2
% res = -p(x) = -2 + x + x^2
%
function res = uminus(self)
  res = Poly(-self.m_coeffs);
end