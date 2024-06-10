%
% W. Spendley, G. R. Hext, and F. R. Himsworth.
% Sequential application of simplex designs in optimisation
% and evolutionary operation. Technometrics,
% 4(4):441-461, 1962.
%
function spendley( self, X0, len )
  n = self.m_dim;
  p = (n-1+sqrt(n+1))/(n*sqrt(2));
  q = (sqrt(n+1)-1)/(n*sqrt(2));
  self.m_p = zeros( self.m_dim+1, self.m_dim );
  self.m_p(1,:) = X0;
  self.m_f(1)   = self.eval_function( X0 );
  for j=2:n+1
    self.m_p(j,:)   = X0+len*p;
    self.m_p(j,j-1) = X0(j-1)+len*q;
    self.m_f(j)     = self.eval_function( self.m_p(j,:) );
  end
end
