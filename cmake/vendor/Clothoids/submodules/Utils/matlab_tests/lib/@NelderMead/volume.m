%
% https://www.mathpages.com/home/kmath664/kmath664.htm
%
function V = volume( self )
  M = [self.m_p ones(self.m_dim+1,1) ];
  V = abs( det( M ) / factorial(self.m_dim+1) );
end
