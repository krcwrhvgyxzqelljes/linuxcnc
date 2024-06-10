%
%
%
function dist_update( self, j )
  n = self.m_dim;
  for i=1:n+1
    if i ~= j
      self.m_dist(i,j) = norm( self.m_p(i,:) - self.m_p(j,:) );
      self.m_dist(j,i) = self.m_dist(i,j);
    end
  end
  M = [self.m_p ones(self.m_dim+1,1) ];
  self.m_diameter       = max(self.m_dist,[],'all');
  tmp                   = self.m_dist+eye(self.m_dim+1)*self.m_diameter;
  self.m_min_dist       = min(tmp,[],'all');
  self.m_simplex_volume = abs( det( M ) / factorial(self.m_dim+1) );
end
