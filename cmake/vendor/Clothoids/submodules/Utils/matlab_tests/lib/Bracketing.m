%
%   _               _        _   _
%  | |__  _ __ __ _| | _____| |_(_)_ __   __ _
%  | '_ \| '__/ _` | |/ / _ \ __| | '_ \ / _` |
%  | |_) | | | (_| |   <  __/ |_| | | | | (_| |
%  |_.__/|_|  \__,_|_|\_\___|\__|_|_| |_|\__, |
%                                        |___/
%
% Given distinct initial points a and b, this routine searches
% in the downhill direction (defined by the function as evaluated
% at the initial points) and returns new points a, b, c which
% bracket a minimum of the function.
% Also returned are the function values at the three points, fa, fb and fc.
%
function [ a, b, c, fa, fb, fc, niter ] = Braketing( ax, fa, bx, fb, FUN )

  GOLD   = 1.618034;
  GLIMIT = 100;
  TINY   = 1e-20;
  ok     = false;

  if fb > fa
    % Switch roles of a and b so that we can go downhill in the direction from a to b.
    cx = ax; ax = bx; bx = cx;
    fc = fa; fa = fb; fb = fc;
  end
  % First guess for c
  cx = bx + GOLD*(bx-ax);
  fc = feval( FUN, cx );

  while fb > fc % Keep returning here until we bracket.
    % Compute u by parabolic extrapolation from a, b, c.
    % TINY is used to prevent any possible division by zero.
    ba = bx-ax;
    bc = bx-cx;
    r  = ba*(fb-fc);
    q  = bc*(fb-fa);
    t  = 2*max(abs(q-r),TINY);
    if q < r; t = -t; end
    u    = bx - (bc*q-ba*r) / t;
    ulim = bx - GLIMIT*bc;
    % We won't go farther than this. Test various possibilities:
    if (bx-u)*(u-cx) > 0
      % Parabolic u is between b and c: try it.
      fu = feval( FUN, u );
      if fu < fc
        ax = bx; bx = u;
        fa = fb; fb = fu;
        return;
      elseif fu > fb
        cx = u; fc = fu;
        return;
      end
      % Parabolic fit was no use. Use default magnification.
      u  = cx - GOLD*bc;
      fu = feval( FUN, u );
    elseif (cx-u)*(u-ulim) > 0
      % Parabolic fit is between c and its allowed limit.
      fu = feval( FUN, u );
      if fu < fc
        bx = cx; cx = u;  u  = cx - GOLD*bc;
        fb = fc; fc = fu; fu = feval( FUN, u );
      end
    elseif (u-ulim)*(ulim-*cx) >= 0
      % Limit parabolic u to maximum allowed value.
      u  = ulim;
      fu = feval( FUN, u );
    else
      % Reject parabolic u, use default magnification.
      u  = cx - GOLD*bc;
      fu = feval( FUN, u );
    end
    % Eliminate oldest point and continue.
    ax = bx; bx = cx; cx = u;
    fa = fb; fb = fc; fc = fu;
  end
end
