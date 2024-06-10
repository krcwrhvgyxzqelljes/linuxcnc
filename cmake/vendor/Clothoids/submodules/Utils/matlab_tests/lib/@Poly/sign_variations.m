% Compute sign variation of polynomial coefficients
%
function sign_var = sign_variations( self )
  sign_var  = 0;
  last_sign = 0;
  for i=1:length(self.m_coeffs)
    v = self.m_coeffs(i);
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
