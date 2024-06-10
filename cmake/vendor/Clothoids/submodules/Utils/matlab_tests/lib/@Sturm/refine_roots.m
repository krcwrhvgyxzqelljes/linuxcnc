% Given a Sturm sequence with roots separated approximate roots using Newton method.
function n_roots = refine_roots( self, tol )
  P            = self.m_sturm{1};
  NR           = length(self.m_intervals);
  self.m_roots = zeros(1,NR);
  fun          = @(x) P.eval(x);
  n_roots      = 0;
  for n=1:NR
    I0         = self.m_intervals{n};
    [sol,iter] = self.m_solver.eval( I0.a, I0.b, fun );
    if iter < 0
      warning('refine_roots FAILED for interval N.{}\n',NR);
    else
      n_roots = n_roots+1;
      self.m_roots(n_roots) = sol;
    end
  end
end
