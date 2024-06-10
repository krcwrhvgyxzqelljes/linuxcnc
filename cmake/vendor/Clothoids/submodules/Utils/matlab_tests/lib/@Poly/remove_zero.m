%
% Divive polynomial by x^k in order to have P(0) <> 0.
%
% For example
%
% p(x) = x+x^2+x^4 of degree 4
%
% become
%
% p(x) = 1+x+x^3 of degree 3
%
function nzero = remove_zero( self )
  nzero = 1;
  while nzero <= self.m_order && self.m_coeffs(nzero) == 0
    nzero = nzero+1;
  end
  nzero = nzero-1;
  if nzero > 0
    if nzero < self.m_order
      self.m_coeffs = self.m_coeffs(nzero+1:self.m_order)
    else
      self.m_coeffs = zeros(1,0);
    end
    self.m_order = self.m_order-nzero;
  end
end
