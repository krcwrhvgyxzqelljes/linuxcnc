%
%  Given a function f and its derivative function df,
%  and given a bracketing triplet of abscissas ax, bx, cx
%  [such that bx is between ax and cx, and f(bx) is less
%  than both f(ax) and f(cx)], this routine isolates the
%  minimum to a fractional precision of about tol using a
%  modification of Brent's method that uses derivatives.
%  The abscissa of the minimum is returned as xmin, and
%  the minimum function value is returned as dbrent,
%  the returned function value.
%
%  F.S.Acton, Numerical Methods That Work; 1970 corrected edition 1990
%  (Washington, DC: Mathematical Association of America), pp. 454-458.
%
%  R.P.Brent, Algorithms for Minimization without Derivatives
%  (Englewood Cliffs, NJ: Prentice- Hall); 1973 reprinted 2002 (New York: Dover),
%
function [ x, fx, dx ] = dbrent( self )
  ZEPS  = 1e-10;
  ITMAX = 100;
  if self.m_ax < self.m_cx
    a = self.m_ax;
    b = self.m_cx;
  else
    a = self.m_cx;
    b = self.m_ax;
  end

  x = self.m_bx;
  w = self.m_bx;
  v = self.m_bx;

  [ fx, dx ] = self.fDf_evaluate( x );
  fw = fx; dw = dx;
  fv = fx; dv = dx;

  d = 0;
  e = 0;

  for iter=1:ITMAX
    xm   = 0.5*(a+b);
    tol1 = tol*abs(x)+ZEPS;
    tol2 = 2*tol1;
    if abs(x-xm) <= (tol2-0.5*(b-a)); return; end
    if abs(e) > tol1
      d1 = 2*(b-a);
      d2 = d1;
      if dw ~= dx; d1 = (w-x)*dx/(dx-dw); end
      if dv ~= dx; d2 = (v-x)*dx/(dx-dv); end
      u1  = x+d1;
      u2  = x+d2;
      ok1 = (a-u1)*(u1-b) > 0 && dx*d1 <= 0;
      ok2 = (a-u2)*(u2-b) > 0 && dx*d2 <= 0;
      % Movement on the step before last.
      olde = e;
      e    = d;
      if ok1 || ok2
        if ok1 && ok2
          if abs(d1) < abs(d2); d = d1; else d = d2; end
        else if ok1
          d = d1;
        else
          d = d2;
        end

        if abs(d) <= abs(0.5*olde)
          u = x+d;
          if u-a < tol2 || b-u < tol2
            if xm > x; d = tol1; else d = -tol1; end
          end
        else
          % Bisect, not golden section.
          if dx >= 0; e = a-x; else e = b-x; end
          d = 0.5 * e;
        end
      else
        if dx >= 0; e = a-x; else e = b-x; end
        d = 0.5 * e;
      end
    else
      if dx >= 0; e = a-x; else e = b-x; end
      d = 0.5 * e;
    end
    if abs(d) >= tol1
      u = x+d;
      [ fu, du ] = self.fDf_evaluate( u );
    else
      if d > 0; u = x+tol1; else u = x-tol1; end
      [ fu, du ] = self.fDf_evaluate( u );
      if fu > fx; return; end
    end
    if fu <= fx
      if u >= x; a = x; else b = x; end
      v  = w;  w  = x;  x  = u;
      fv = fw; fw = fx; fx = fu;
      dv = dw; dw = dx; dx = du;
    else
      if u < x; a = u; else b = u; end
      if fu <= fw || w == x
        v  = w;  w  = u;
        fv = fw; fw = fu;
        dv = dw; dw = du;
      elseif fu < fv || v == x || v == w
        v  = u;
        fv = fu;
        dv = du;
      end
    end
  end
  error("Too many iterations in routine dsearch");
end
