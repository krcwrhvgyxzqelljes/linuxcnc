%
%  Given a function and given distinct initial points a and b,
%  this routine searches in the downhill direction
%  (defined by the function as evaluated at the initial points)
%  and returns new points m_ax, m_bx, m_cx that bracket a minimum of the function.
%  Also returned are the function values at the three points, m_fa, m_fb, and m_fc.
%
function bracket( self, a, b )
  ax = a;
  bx = b;
  fa = self.f_evaluate(ax);
  fb = self.f_evaluate(bx);
  if fb > fa
    % Switch roles of a and b so that we can go downhill in the direction from a to b.
    t = ax; ax = bx; bx = t;
    t = fa; fa = fb; fb = t;
  end
  % First guess for c.
  cx = bx + self.m_GOLD*(bx-ax);
  fc = self.f_evaluate( cx );
  while fb > fc

    % Compute u by parabolic extrapolation from a, b, c.
    % TINY is used to prevent any possible division by zero.

    ba = bx-ax;
    bc = bx-cx;
    r  = ba*(fb-fc);
    q  = bc*(fb-fa);
    t  = q-r;
    if abs(t) <= self.m_TINY
      if t > 0; t = self.m_TINY; else t = -self.m_TINY; end
    end
    u    = bx - (bc*q-ba*r)/(2*t);
    ulim = bx - self.m_GLIMIT*bc;

    % We won't go farther than this.
    % Test various possibilities:
    if (bx-u)*(u-cx) > 0
      % Parabolic u is between b and c: try it.
      fu = self.f_evaluate( u );
      if fu < fc
        % Got a minimum between b and c.
        ax = bx; bx = u;
        fa = fb; fb = fu;
        break;
      elseif fu > fb
        % Got a minimum between between a and u.
        cx = u; fc = fu;
        break; % return;
      end
      % Parabolic fit was no use. Use default magnification.
      u  = cx + self.m_GOLD*(cx-bx);
      fu = self.f_evaluate( u );
    elseif (cx-u)*(u-ulim) > 0
      % Parabolic fit is between c and its allowed limit.
      fu = self.f_evaluate( u );
      if fu < fc
        newu = u + self.m_GOLD*(u-cx);
        bx = cx; cx = u;  u  = newu;
        fd = fc; fc = fu; fu = self.f_evaluate( u );
      end
    elseif (u-ulim)*(ulim-cx) >= 0
      % Limit parabolic u to maximum allowed value.
      u  = ulim;
      fu = self.f_evaluate( u );
    else
      % Reject parabolic u, use default magnification.
      u = cx + self.m_GOLD*(cx-bx);
      fu = self.f_evaluate( u );
    end
    % Eliminate oldest point and continue.
    ax = bx; bx = cx; cx = u;
    fa = fb; fb = fc; fc = fu;
  end
  %------------------------------
  self.m_ax = ax; self.m_fa = fa;
  self.m_bx = bx; self.m_fb = fb;
  self.m_cx = cx; self.m_fc = fc;
end
