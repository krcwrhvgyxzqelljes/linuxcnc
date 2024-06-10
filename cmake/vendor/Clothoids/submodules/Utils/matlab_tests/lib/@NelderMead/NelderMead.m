%
%  J.A.Nelder and R.Mead
%  Computer Journal, vol. 7, pp. 308-313, 1965.
%
%  L.A.Yarbro and S.N.Deming
%  Analytica Chimica Acta, vol. 73, pp. 391-398, 1974.
%
%  S.L.S.Jacoby, J.S.Kowalik and J.T.Pizzo
%  Iterative Methods for Nonlinear Optimization Problems
%  (Englewood Cliffs, NJ: Prentice-Hall), 1972
%
classdef NelderMead < handle
  properties
    m_fun       % handle to the value function to minimize
    m_plot      % handle to the value function to minimize

    m_dim       % problem dimension (number of variables)
    m_tolerance % tolerance on the scale h

    m_low
    m_0high
    m_high

    m_f
    m_p
    m_psum
    m_dist
    m_min_dist
    m_diameter
    m_simplex_volume
    m_simplex_regular_volume

    m_rho
    m_gamma
    m_rho_chi
    m_rho_gamma
    m_sigma % shrink
    m_prob
    m_volume_tolerance

    m_max_fun_evaluation   % max number of function evaluations
    m_max_iteration        % max number of iterations

    m_iteration_count      % explore iteration counter
    m_fun_evaluation_count %

    m_which_step
    m_verbose              % flag to activate info printing
  end

  methods ( Access = private,  Hidden = true )
    function default_plot( self )
      if self.m_dim == 2
        x = [self.m_p(:,1);self.m_p(1,1)];
        y = [self.m_p(:,2);self.m_p(1,2)];
        plot( x, y, '-', 'Color', 'black', 'LineWidth', 1 );
        plot( x(self.m_low),  y(self.m_low),  'o', 'Color', 'black', 'MarkerFaceColor', 'blue' );
        plot( x(self.m_high), y(self.m_high), 'o', 'Color', 'black', 'MarkerFaceColor', 'red' );
        drawnow;
        if self.m_verbose > 2
          input('step');
        end
      end
    end
    [fe,pe] = extrapolate( self, alpha, pfrom )
    spendley( self, X0, len )
    diamond( self, X0, len )
    g = gradient( self )
    V = volume( self )
    shrink( self, idx )
    dist_init( self )
    dist_update( self, jpos )
    function [fr,pr] = reflect( self )
      rr      = self.m_rho;
      pp      = self.m_p(self.m_high,:);
      [fr,pr] = self.extrapolate( rr, pp );
    end
    function [fe,pe] = expand( self )
      rr      = self.m_rho_chi;
      pp      = self.m_p(self.m_high,:);
      [fe,pe] = self.extrapolate( rr, pp );
    end
    function [fo,po] = outside( self )
      rr      = self.m_rho_gamma;
      pp      = self.m_p(self.m_high,:);
      [fo,po] = self.extrapolate( rr, pp );
    end
    function [fi,pi] = inside( self )
      rr      = -self.m_gamma;
      pp      = self.m_p(self.m_high,:);
      [fi,pi] = self.extrapolate( rr, pp );
    end
    function replace_point( self, fj, pj, jpos )
      self.m_f(jpos)   = fj;
      self.m_psum      = self.m_psum + ( pj - self.m_p(jpos,:) );
      self.m_p(jpos,:) = pj;
      self.dist_update( jpos );
    end
  end

  %% Methods - all public
  methods
    % =================================================================
    % Class constructor
    % =================================================================
    function self = NelderMead()
      self.m_tolerance          = 1e-15;  % initialize scale tolerance
      self.m_max_iteration      = 5000;   % Initialize max number of iterations
      self.m_max_fun_evaluation = 10000;  % Initialize max number of function evaluations
      self.m_plot               = @( p, idx ) self.default_plot();
      self.m_verbose            = 2;

      self.m_rho       = 1;
      self.m_rho_chi   = 2;
      self.m_rho_gamma = 0.5;
      self.m_gamma     = 0.5;
      self.m_sigma     = 0.25;

      self.m_prob             = 0.5;
      self.m_volume_tolerance = 1e-4;
    end

    % =================================================================
    % Setup
    % =================================================================
    setup( self, fun, dim, varargin )

    % =================================================================
    % set verbose
    % =================================================================
    function set_verbose( self, tf )
      self.verbose = tf;
    end

    % =================================================================
    % set tolerance
    % =================================================================
    function set_tolerance( self, tol )
      if tol <= 0
        error( 'set_tolerance(%g) argument must be >0',tol);
      end
      self.tolerance = tol;
    end

    % =================================================================
    % set max_iteration
    % =================================================================
    function set_max_iteration( self, mit )
      if mit <= 0
        error( 'set_max_iteration(%g) argument must be >0',mit);
      end
      self.m_max_iteration = mit;
    end

    % =================================================================
    % set max_iteration
    % =================================================================
    function set_max_fun_evaluation( self, mfev )
      if mfev <= 0
        error( 'set_max_fun_evaluation(%g) argument must be >0',mfev);
      end
      self.m_max_fun_evaluation = mfev;
    end

    % =================================================================
    % regular_simplex_volume
    % =================================================================
    function res = regular_simplex_volume( self, ell )
      n   = self.m_dim;
      res = (sqrt(n+1)/factorial(n))*(ell/sqrt(2))^n;
    end

    % =================================================================
    % regular_simplex_edge
    % =================================================================
    function res = regular_simplex_edge( self, V )
      n   = self.m_dim;
      res = (factorial(n)/sqrt(n+1)*V)^(1/n) * sqrt(2);
    end

    % =================================================================
    % info
    % =================================================================
    print_info( self, varargin )
    message( self, rtol )

    % =================================================================
    % Evaluate function
    % =================================================================
    F = eval_function(self, x)

    % =================================================================
    % Search
    % =================================================================
    [pmin,fmin] = search(self)

    % =================================================================
    % Run
    % =================================================================
    [pmin,fmin] = run( self, X0, varargin )
  end
end
