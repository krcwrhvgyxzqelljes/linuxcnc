% Return the integral of the stored polynomial.
%
% For example given p(x) = 1+3*x+5*x^6
% return res = x+(3/2)*x^2+(5/7)*x^7
%
function res = integral( self )
  itg = zeros( 1, self.m_order+1 );
  for i=1:self.m_order
    itg(i+1) = self.m_coeffs(i)/i;
  end
  res = Poly(itg);
end
