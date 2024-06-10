%
%  Given a function f, and given a bracketing triplet of
%  abscissas ax, bx, cx (such that bx is between ax and cx,
%  and f(bx) is less than both f(ax) and f(cx)),
%  this routine performs a golden section search for the minimum,
%  isolating it to a fractional precision of about tol.
%  The abscissa of the minimum is returned as xmin, and
%  the minimum function value is returned as golden,
%  the returned function value.
%
function [ x, fx ] = Golden( self )

  R = 0.61803399; % The golden ratios.
  C = 1-R;

  % At any given time we will keep track of four points, x0,x1,x2,x3.
  x0 = self.m_ax;
  x3 = self.m_cx;

  % Make x0 to x1 the smaller segment, and fill in the new point to be tried.
  bc = self.m_bx - self.m_cx;
  ba = self.m_bx - self.m_ax;
  if abs(bc) > abs(ba)
    x1 = self.m_bx;
    x2 = self.m_bx-C*bc;
  else
    x2 = self.m_bx;
    x1 = self.m_bx-C*ba;
  end
  % The initial function evaluations.
  % Note that we never need to evaluate the function at the original endpoints.
  f1 = self.f_evaluate( x1 );
  f2 = self.f_evaluate( x2 );
  self.m_num_iter_done = 0;
  while abs(x3-x0) > self.m_tolerance*(abs(x1)+abs(x2))
    self.m_num_iter_done = self.m_num_iter_done+1;
    if f2 < f1
      % One possible outcome, its housekeeping and a new function evaluation.
      x0 = x1;
      x1 = x2; x2 = R * x1 + C * x3;
      f1 = f2; f2 = self.f_evaluate( x2 );
    else
      % The other outcome, and its new function evaluation.
      x3 = x2;
      x2 = x1; x1 = R * x2 + C * x0;
      f2 = f1; f1 = self.f_evaluate( x1 );
    end
    % Back to see if we are done.
  end
  % We are done. Output the best of the two current values.
  if f1 < f2
    x  = x1;
    fx = f1;
  else
    x  = x2;
    fx = f2;
  end
end
