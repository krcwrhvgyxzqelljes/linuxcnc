% Return the derivative of the stored polynomial.
%
% For example given p(x) = 1+3*x+5*x^6
% return res = p'(x) = 3+30*x^5
%
function res = derivative( self )
  der = zeros( 1, self.m_order-1 );
  for i=1:self.m_order-1
    der(i) = i * self.m_coeffs(i+1);
  end
  res = Poly( der );
end
