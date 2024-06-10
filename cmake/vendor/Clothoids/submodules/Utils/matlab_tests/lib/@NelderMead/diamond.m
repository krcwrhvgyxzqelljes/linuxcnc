%
%
function diamond( self, X0, len )
  n = self.m_dim;
  self.m_p = zeros( n+1, n );
  self.m_f = zeros( 1, n+1 );
  self.m_p(1,:) = X0;
  self.m_f(1)   = self.eval_function( X0 );
  for j=1:n
    p1    = X0;
    p2    = X0;
    p1(j) = X0(j)+len;
    p2(j) = X0(j)-len;
    f1    = self.eval_function( p1 );
    f2    = self.eval_function( p2 );
    if f1 < f2
      self.m_f(j+1)   = f1;
      self.m_p(j+1,:) = p1;
    else
      self.m_f(j+1)   = f2;
      self.m_p(j+1,:) = p2;
    end
  end
end
