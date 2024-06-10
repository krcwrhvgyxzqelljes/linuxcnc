%
%
%
function dist_init( self )
  n = self.m_dim;
  self.m_dist = zeros(n+1,n+1);
  for i=1:n+1
    for j=i+1:n+1
      self.m_dist(i,j) = norm( self.m_p(i,:) - self.m_p(j,:) );
      self.m_dist(j,i) = self.m_dist(i,j);
    end
  end
  self.m_diameter       = max(max(self.m_dist));
  self.m_simplex_volume = abs( det( [self.m_p ones(self.m_dim+1,1) ] ) / factorial(self.m_dim+1) );
end
