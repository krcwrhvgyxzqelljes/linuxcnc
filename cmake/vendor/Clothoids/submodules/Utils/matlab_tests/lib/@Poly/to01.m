% return the polynomial P(z) = p( a + z * (b-a) )
%
function P = to01( self, a, b )
  cfs = zeros( 1, self.m_order );
  if self.m_order > 0
    cfs(1) = self.m_coeffs(1);
    if self.m_order > 1
      Q = Poly(1);
      for k=2:self.m_order
        Q.mult_by_monomial( a, b-a ); 
        cfs(1:k) = cfs(1:k) + self.m_coeffs(k) * Q.coeffs;
      end
    end
  end
  P = Poly( cfs );
end
