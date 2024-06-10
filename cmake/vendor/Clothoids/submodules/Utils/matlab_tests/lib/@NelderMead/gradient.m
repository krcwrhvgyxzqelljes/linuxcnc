%
% Nelder-Mead User's Manual
% Michael Baudin April 2010
%
function g = gradient( self )
  delta = zeros(self.m_dim,1);
  DT    = zeros(self.m_dim,self.m_dim);
  j     = 1;
  f0    = self.m_f(self.m_low);
  p0    = self.m_p(self.m_low,:);
  for i=1:self.m_dim+1
    if i ~= self.m_low
      DT(j,:)  = self.m_p(i,:)-p0;
      delta(j) = self.m_f(i)-f0;
      j        = j+1;
    end
  end
  g = DT\delta;
end
