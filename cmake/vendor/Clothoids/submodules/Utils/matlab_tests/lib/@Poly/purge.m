% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function purge( self, epsi )
  % Set to 0 the coefficients of p(x) = sum p(i) * x^(i-1) such that |p(i)| < epsi
  EPS = epsi*max(1,max(max(abs(self.m_coeffs))));
  IDX = find( abs(self.m_coeffs) <= EPS );
  self.m_coeffs(IDX) = 0;
  self.adjust_degree();
end
