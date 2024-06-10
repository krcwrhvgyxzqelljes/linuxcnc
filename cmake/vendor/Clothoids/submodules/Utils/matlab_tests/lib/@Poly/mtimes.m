% Multiply two polynomial
function res = mtimes(self,b)
  ok1 = isequal(class(self),'Poly');
  ok2 = isequal(class(b),'Poly');
  if ok1 && ok2
    cfs = zeros(1,self.m_order+b.order()-1);
    bb  = b.coeffs;
    for i=1:self.m_order
      for j=1:b.order()
        cfs(1,i+j-1) = cfs(1,i+j-1) + self.m_coeffs(i) * bb(j);
      end
    end
    res = Poly( cfs );
  elseif ok1 && isnumeric(b) && length(b) == 1
    res = Poly( self.m_coeffs*b );
  elseif ok2 && isnumeric(self) && length(self) == 1
    res = Poly( b.coeffs*self );
  else
    error('bad type %s x %s for Polynomial multyiplication\n',class(self),class(b));
  end
end