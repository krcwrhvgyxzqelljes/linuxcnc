% =================================================================
% Run
% =================================================================
function [pmin,fmin] = run( self, X0, varargin )
  n = self.m_dim;
  if size(X0) == [n+1,n]
    self.m_p = X0;
    for j=1:n+1
      self.m_f(j) = self.eval_function( self.m_p(j,:) );
    end
  else
    if nargin > 2
      delta = varargin{1};
    else
      delta = max(1e-4,0.1*max(abs(X0)))*ones(1,self.m_dim);
    end
    %self.spendley( X0, delta );
    self.diamond( X0, delta );
  end
  [pmin,fmin] = self.search();
end
