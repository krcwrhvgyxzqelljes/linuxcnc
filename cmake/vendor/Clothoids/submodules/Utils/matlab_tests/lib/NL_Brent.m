%  
%   ____                 _
%  | __ ) _ __ ___ _ __ | |_
%  |  _ \| '__/ _ \ '_ \| __|
%  | |_) | | |  __/ | | | |_
%  |____/|_|  \___|_| |_|\__|
%  
%  Given a function f, and given a bracketing triplet of abscissas
%  a, x, b (such that x is between a and b, and f(x) is less than
%  both f(a) and f(b)),
%  this routine isolates the minimum to a fractional precision of
%  about tol using Brent's method.
%  The abscissa of the minimum is returned as xmin, and the minimum
%  function value is returned as brent, the returned function value.
%
function [ x, fx, iter ] = NL_Brent( a, fa, x, fx, b, fb, FUN, m_tolerance, m_debug )

  m_max_iter   = 40;
  m_epsi       = 1e-10;
  m_abs_Dm0_c1 = 0.1;

  a_in = a-0.2*(b-a);
  b_in = b+0.2*(b-a);

  if a >= x || x >= b || fx > fa || fx > fb
    error( ...
      ['NL_Brent triple (a,f(a)), (b,f(b)), (x,f(x)) not consistent\n', ...
       'must be a < x < b and f(a) > f(x) <= f(b) but found\n', ...
       'a=%g, x=%g, b=%g\n', ...
       'f(a)=%g, f(x)=%g, f(b)=%g\n', ...
       'f(a)-f(x)=%g, f(b)-f(x)=%g\n'], ...
       a, x, b, fa, fx, fb, fa-fx, fb-fx ...
    );
  end

  for iter=1:m_max_iter
    if ~isfinite(a) && ~isfinite(b) && ~isfinite(c)
      fprintf('qualcosa è andato storto a=%g, b=%g, x=%g\n',a,b,x);
    end

    xm   = (a+b)/2;
    tol1 = m_tolerance*abs(x)+m_epsi;
    tol2 = 2*tol1;

    if abs(x-xm) <= (tol2-(b-a)/2)
      return; % true
    end

    % Construct a trial parabolic fit.
    [ok,u] = min3p( a, b, x, fa, fb, fx, tol2 );
    if ~ok
      fprintf('do golden search step\n');
    end
    % This is the one function evaluation per iteration.
    fu = feval( FUN, u );

    if ~isfinite(u)
      fprintf('qualcosa è andato storto a=%g, b=%g, x=%g, u=%g\n',a,b,x,u);
    end

    if m_debug
      xx = a_in:(b_in-a_in)/200:b_in;
      yy = zeros(size(xx));
      for kk=1:length(xx)
        yy(kk) = feval( FUN, xx(kk) );
      end
      hold off;
      plot( xx, yy, 'LineWidth', 2 );
      hold on;
      plot( x, fx, 'o', 'MarkerSize', 15, 'MarkerFaceColor', 'green' );
      plot( u, fu, 'o', 'MarkerSize', 20, 'MarkerFaceColor', 'red'   );
      plot( a, fa, 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'blue'  );
      plot( b, fb, 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'blue'  );
      fprintf('\niter=%d,a=%g,u=%g,x=%g,b=%g\nfa=%g,fu=%g,fx=%g,fb=%g\n',iter,a,u,x,b,fa,fu,fx,fb);
      xxx = input('pausa');
    end

    if fu <= fx
      % u improve --> x on the border
      if u > x
        a = x; fa = fx;
      else
        b = x; fb = fx;
      end
      x = u; fx = fu;
      if fx + m_abs_Dm0_c1 * x <= 0
        return; % true
      end
    else
      % u do not improve --> u on the border
      if u < x
        a = u; fa = fu;
      else
        b = u; fb = fu;
      end
    end
  end
  % Too many iterations in Brent
  iter = -1;
end
%
% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
%
function [ok,u] = min3p( a, b, c, fa, fb, fc, tol2 )
  ok = false;
  if ~isfinite(fc)
    u = (b+c)/2;
    return;
  end
  ac     = a-c;
  bc     = b-c;
  fac    = fa-fc; % >= 0
  fbc    = fb-fc; % >= 0
  ac_fbc = ac*fbc;
  bc_fac = bc*fac;
  BOT    = ac_fbc - bc_fac;
  TOP    = (ac*ac_fbc - bc*bc_fac)/2;
  delta  = TOP/BOT;
  u      = c + delta;
  % controlla che sia dentro il range
  ma = max(a,b)-tol2;
  mi = min(a,b)+tol2;
  if ~isfinite(u) || u >= ma || u <= mi || abs(delta) <= tol2
    % seleziona intervallo piu grande
    if abs(ac) > abs(bc)
      u = c + ac/2;
    else
      u = c + bc/2;
    end
  end
  ok = true;
end
