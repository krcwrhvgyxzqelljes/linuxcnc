% Add two polynomial
%
% Given p(x) = 1+x, q(x) = 2-x-x^2
% res = p(x) + q(x) = 3 - x^2
%
function res = minus(self,b)
  ok1 = isequal(class(self),'Poly');
  ok2 = isequal(class(b),'Poly');
  if ok1 && ok2
    if self.m_order < b.order
      cfs = b.coeffs();
      cfs(1,1:self.m_order) = self.m_coeffs + cfs(1,1:self.m_order);
    elseif self.m_order > b.order
      cfs = self.m_coeffs;
      cfs(1,1:b.order) = cfs(1,1:b.order) + b.coeffs();
    else
      cfs = self.m_coeffs + b.coeffs();
    end
    res = Poly(cfs);
  elseif ok1 && isnumeric(b) && length(b) == 1
    cfs = self.m_coeffs;
    if length(cfs) > 0
      cfs(1) = cfs(1) + b;
      res = Poly( cfs );
    else
      res = Poly( b );
    end
  elseif ok2 && isnumeric(self) && length(self) == 1
    cfs = b.coeffs;
    if length(cfs) > 0
      cfs(1) = cfs(1) + self;
      res = Poly( cfs );
    else
      res = Poly( self );
    end
  else
    error('bad type %s x %s for Polynomial summation\n',class(self),class(b));
  end
end