% Compute sign variations of Sturm sequence
% ok = false if p(x) = 0, i.e. we are on a root of the polynomial
%
function [sign_var,on_roots] = sign_variations( self, x )
  sign_var  = 0;
  last_sign = 0;
  on_roots  = false;
  v         = self.m_sturm{1}.eval(x);
  if v > 0
    last_sign = 1;
  elseif v < 0
    last_sign = -1;
  else
    on_roots  = true;
    last_sign = 0;
  end
  for i=2:length(self.m_sturm)
    v = self.m_sturm{i}.eval(x);
    if v > 0
      if last_sign == -1
        sign_var = sign_var+1;
      end
      last_sign = 1;
    elseif v < 0
      if last_sign == 1
        sign_var = sign_var+1;
      end
      last_sign = -1;
    end
  end
end
