% =================================================================
% Evaluate function
% =================================================================
function F = eval_function( self, x )
  F = self.m_fun(x);
  self.m_fun_evaluation_count = self.m_fun_evaluation_count + 1;
end
