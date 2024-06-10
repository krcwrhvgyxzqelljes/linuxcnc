classdef HJPatternSearch < handle
  %% HJPatternSearch class decription
  %
  % This class implements the Hooke-Jeeves Pattern Search Algorithm.
  % The references can be found in:
  %
  % R. Hooke, T. A. Jeeves,
  % "Direct Search" Solution of Numerical and Statistical Problems,
  % Westinghouse Research Laboratories,
  % Pittsburg, Pennsylvania
  %
  % Arthur Kaupe,
  % Algorithm 178: Direct Search,
  % Communications of the ACM,
  % Volume 6, Number 6, June 1963, page 313.
  %
  % M Bell, Malcolm Pike,
  % Remark on Algorithm 178: Direct Search,
  % Communications of the ACM,
  % Volume 9, Number 9, September 1966, page 684.
  %
  % FK Tomlin, LB Smith,
  % Remark on Algorithm 178: Direct Search,
  % Communications of the ACM,
  % Volume 12, Number 11, November 1969, page 637-638.
  %
  %% Properties - all public
  properties
    m_rho                % stencil step decreasing factor (must be 0 < rho < 1)
    m_h                  % scale of the stencil (Remark 1966)

    m_fun                % handle to the value function to minimize
    m_plot               % handle to the value function to minimize

    m_dim                % problem dimension (number of variables)

    m_x_old
    m_f_old
    m_x_best               % base point
    m_f_best               % best value function (fun evaluated in x_best)
    m_stencil_failure

    m_Vmat                 % direction matrix for exploration
    m_tolerance            % tolerance on the scale h
    m_max_fun_evaluation   % max number of function evaluations
    m_max_iteration        % max number of iterations
    m_iteration_count      % explore iteration counter
    m_fun_evaluation_count %
    m_verbose              % flag to activate info printing
    m_search_sign          % vector to keep in memory the direction of function value descent from the previous iteration in each direction j
    m_max_num_stagnation   %
  end

  methods
    function default_plot( self, x0, x1, h )
      if self.m_dim == 2
        plot( [x0(1),x1(1)], [x0(2),x1(2)], '-', 'Color', 'black', 'LineWidth', 1 );
        plot( x1(1),  x1(2),  'o', 'Color', 'black', 'MarkerFaceColor', 'blue' );
        plot( [x0(1)-h,x0(1)+h], [x0(2),x0(2)], '-', 'Color', 'red', 'LineWidth', 2 );
        plot( [x0(1),x0(1)], [x0(2)-h,x0(2)+h], '-', 'Color', 'red', 'LineWidth', 2 );
        drawnow;
        if self.m_verbose > 2
          input('step');
        end
      end
    end
    function no_plot( self, x0, x1, h )
    end
  end

  %% Methods - all public
  methods
    % =================================================================
    % Class constructor
    % =================================================================
    function self = HJPatternSearch()
      self.m_max_num_stagnation = 100;
      self.m_rho                = 0.1;    % Initialize stencil step decreasing factor (must be 0 < rho < 1)
      self.m_tolerance          = 1e-15;  % initialize scale tolerance
      self.m_max_iteration      = 10000;  % Initialize max number of iterations
      self.m_max_fun_evaluation = 100000; % Initialize max number of function evaluations
      self.m_verbose            = 2;      % Initialize level to activate info printing
      %self.m_plot               = @(x0,x1,h) self.no_plot(x0,x1,h);
      self.m_plot               = @(x0,x1,h) self.default_plot(x0,x1,h);
    end

    % =================================================================
    % Setup
    % =================================================================
    setup( self, fun, lb, ub, varargin )

    % =================================================================
    % set verbose
    % =================================================================
    function set_verbose( self, tf )
      self.m_verbose = tf;
    end

    % =================================================================
    % set tolerance
    % =================================================================
    function set_tolerance( self, tol )
      if tol <= 0
        error( 'set_tolerance(%g) argument must be >0',tol);
      end
      self.m_tolerance = tol;
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
    % info
    % =================================================================
    print_info( self, varargin )
    message( self )

    % =================================================================
    % Evaluate function
    % =================================================================
    F = eval_function( self, x )

    % =================================================================
    % Best Nearby
    % =================================================================
    best_nearby( self )

    % =================================================================
    % Search
    % =================================================================
    search( self )

    % =================================================================
    % Run
    % =================================================================
    x_sol = run( self, X0, h0 )
  end
end
