% =================================================================
% Search
% =================================================================

function search(self)
  % SEARCH This method call the explore method on the first
  % iteration and then continue to call explore until a stencil
  % fails. In the case of a stencil failure, it tries once to go
  % back of half a step along the search direction by setting x_center
  % equal to the base point x_best.
  % If the stencil fails again, it exits the while loop and stencil_failure
  % is set to zero in order to signal that a reduction of h is necessary.

  self.m_iteration_count = self.m_iteration_count + 1; % augment counter
  % Print info
  self.message()

  % Explore the first iteration point
  self.best_nearby();
  while self.m_stencil_failure
    % reduce the scale
    self.m_h = self.m_h * self.m_rho;
    self.best_nearby();
    if self.m_h <= self.m_tolerance; return; end
    if self.m_fun_evaluation_count >= self.m_max_fun_evaluation; return; end
  end

  dir = self.m_x_best - self.m_x_old; % Compute search direction

  % Continue exploring of
  lambda  = 1;
  max_der = 0;
  ratio   = 1;
  for kk=1:4
    if self.m_fun_evaluation_count > self.m_max_fun_evaluation; break; end
    if lambda < 0.1; break; end;
    new_x = self.m_x_best + lambda*dir;
    new_f = self.eval_function(new_x);
    if new_f < self.m_f_best-0.25*max_der*lambda
      der = (self.m_f_best-new_f)/lambda;
      if der > max_der; max_der = der; end;
      self.m_plot( self.m_x_best, new_x, self.m_h );
      self.m_x_best = new_x;
      self.m_f_best = new_f;
      ratio  = max(ratio,lambda);
      lambda = lambda*2;
    else
      lambda = lambda*0.5;
    end
  end
  ratio
  self.m_fun_evaluation_count
  self.m_h = self.m_h*ratio;
end
