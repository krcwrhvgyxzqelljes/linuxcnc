%
%  Given a function and given a bracketing triplet of abscissas
%  ax, bx, cx (such that bx is between ax and cx, and f(bx)
%  is less than both f(ax) and f(cx)), this routine isolates
%  the minimum to a fractional precision of about tol using Brent's method.
%  The abscissa of the minimum is returned as xmin,
%  and the function value at the minimum is returned as min,
%  the returned function value.
%
%  R.P.Brent, Algorithms for Minimization without Derivatives
%  (Englewood Cliffs, NJ: Prentice- Hall); 1973 reprinted 2002 (New York: Dover),
%  Chapter 5.
%
%  G.E.Forsythe, M.A.Malcolm and C.B. Moler,
%  Computer Methods for Mathematical Computations
%  (Englewood Cliffs, NJ: Prentice-Hall), 1977, Chapter 8.2.
%
function [ x, fx ] = brent( self )
  ZEPS = eps*1e-3;

  d = 0;
  e = 0; % This will be the distance moved on the step before last.

  % a and b must be in ascending order, but input abscissas need not be.
  if self.m_ax < self.m_cx
    a = self.m_ax;
  else
    a = self.m_cx;
  end
  if self.m_ax > self.m_cx;
    b = self.m_ax;
  else
    b = self.m_cx;
  end

  x  = self.m_bx;
  w  = self.m_bx;
  v  = self.m_bx;
  fx = self.f_evaluate( x );
  fw = fx;
  fv = fx;

  self.m_num_iter_done = 0;
  while true
    self.m_num_iter_done = self.m_num_iter_done + 1;
    xm   = 0.5 * ( a + b );
    tol1 = self.m_tolerance*abs(x)+ZEPS;
    tol2 = 2*tol1;
    %%
    %%  Check the stopping criterion.
    %%
    if abs( x - xm ) <= tol2 - 0.5 * (b-a); return; end
    %%
    %%  Fit a parabola.
    %%
    if abs( e ) > tol1
      % Construct a trial parabolic fit.
      r = ( x - w ) * ( fx - fv );
      q = ( x - v ) * ( fx - fw );
      p = ( x - v ) * q - ( x - w ) * r;
      q = 2 * ( q - r );
      if q > 0; p = -p; end
      q     = abs(q);
      etemp = e;
      e     = d;
      if abs(p) >= abs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x)
        % The above conditions determine the acceptability of the parabolic fit.
        % Here we take the golden section step into the larger of the two segments.
        if x >= xm; e = a-x; else e = b-x; end
        d = self.m_CGOLD*e;
      else
        d = p/q;
        u = x+d;
        if u-a < tol2 || b-u < tol2
          if xm >= x; d = tol1; else d = -tol1; end
        end
      end
    else
      if x >= xm; e = a-x; else e = b-x; end
      d = self.m_CGOLD*e;
    end
    if abs(d) >= tol1
      u = x+d;
    else
      if d > 0
        u = x+tol1;
      else
        u = x-tol1;
      end
    end
    % This is the one function evaluation per iteration.
    fu = self.f_evaluate( u );
    if fu <= fx
      if u >= x;
        a = x;
      else
        b = x;
      end
      v  = w;  w  = x;  x  = u;
      fv = fw; fw = fx; fx = fu;
    else
      if u < x; a=u; else b=u; end
      if fu <= fw || w == x
        v  = w;  w  = u;
        fv = fw; fw = fu;
      elseif fu <= fv || v == x || v == w
        v  = u;
        fv = fu;
      end
    end
  end
end
