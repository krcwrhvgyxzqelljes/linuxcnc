% Multiply the polynomial by (a+b*x)
function mult_by_monomial( self, a, b )
  %
  % Multiply by (a+b*x)
  %
  cfs           = zeros( 1, self.m_order+1 );
  cfs(1:end-1)  = a*self.m_coeffs;
  cfs(2:end)    = cfs(2:end) + b*self.m_coeffs;
  self.m_coeffs = cfs;
  self.m_order  = self.m_order+1;
end
