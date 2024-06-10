% Adjust the degree of the polynomial in order to match the correct degree.
%
% For example
%
% p(x) = 1+x+0*x^2+x^3+0*x^4 of degree 4
%
% become
%
% p(x) = 1+x+0*x^2+x^3 of degree 3
%
function adjust_degree( self )
  order = self.m_order;
  while order > 0 && self.m_coeffs(order) == 0
    order = order-1;
  end
  if order < self.m_order
    if order > 0
      self.m_coeffs = self.m_coeffs(1:order)
    else
      self.m_coeffs = zeros(1,0);
    end
    self.m_order = order;
  end
end
