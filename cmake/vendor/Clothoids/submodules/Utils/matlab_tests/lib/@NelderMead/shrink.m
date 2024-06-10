%
% W. Spendley, G. R. Hext, and F. R. Himsworth.
% Sequential application of simplex designs in optimisation
% and evolutionary operation. Technometrics,
% 4(4):441-461, 1962.
%
function shrink( self, idx )
  c1 = 1-self.m_sigma;
  c2 = self.m_sigma;
  for i=1:self.m_dim+1
    if i ~= idx
      self.m_p(i,:) = c1 * self.m_p(i,:) + c2 * self.m_p(idx,:);
      self.m_f(i)   = self.eval_function( self.m_p(i,:) );
    end
  end
  self.m_psum           = sum(self.m_p);
  self.m_dist           = c1*self.m_dist;
  self.m_diameter       = c1*self.m_diameter;
  self.m_simplex_volume = c1^self.m_dim*self.m_simplex_volume;
end
