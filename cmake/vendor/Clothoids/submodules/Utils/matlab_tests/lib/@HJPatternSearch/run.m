% =================================================================
% Run
% =================================================================

function x_sol = run( self, X0, h0 )

  if length(X0) ~= self.m_dim
    error( 'length(X0) = %d must be %d\n', length(X0), self.m_dim );
  end

  self.m_Vmat        = eye(self.m_dim);    % select direction matrix as the identity matrix
  self.m_search_sign = ones(self.m_dim,1); % Initialize search verse vector to all ones (first verse will be positive for each direction)
  self.m_x_best      = X0;                 % initialize base point to guess for the first iteration
  self.m_f_best      = self.eval_function(X0);

  self.m_iteration_count      = 0; % initialize explore iteration counter
  self.m_fun_evaluation_count = 0; % initialize function evaluation counter

  % initialize scale
  self.m_h = h0;

  % RUN This method run the whole Hooke-Jeeves algorithm.
  % Search is repeated until it fails, then the scal h is reduced.
  % When h is less than a threshold, the method returns the solution.

  num_stagnation = 0;
  while self.m_h > self.m_tolerance && ...
        self.m_fun_evaluation_count < self.m_max_fun_evaluation

    self.m_x_old = self.m_x_best;
    self.m_f_old = self.m_f_best;

    self.search();
    if self.m_stencil_failure; break; end

    % If iteration limit is reached,stop.
    if self.m_iteration_count >= self.m_max_iteration; break; end

    % reduce the scale
    self.m_h = self.m_h * self.m_rho;

    % check stagnation
    if self.m_f_old <= self.m_f_best
      num_stagnation = num_stagnation+1;
      if num_stagnation > self.m_max_num_stagnation; break; end
    else
      num_stagnation = 0;
    end
  end

  if self.m_verbose > 0
    self.print_info();
  end

  x_sol = self.m_x_best;
end
