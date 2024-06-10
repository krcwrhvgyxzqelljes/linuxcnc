function S = normalize( self )
  % Scale polynomial p(x) = sum p(i) * x^(i-1) un such a way max |p(i)| = 1
  % return in S the scaling value
  S = max(max(abs(self.m_coeffs)));
  if S > 0
    self.m_coeffs = self.m_coeffs./S;
  end
end
