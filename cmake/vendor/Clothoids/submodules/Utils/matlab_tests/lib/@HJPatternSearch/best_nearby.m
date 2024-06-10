% =================================================================
% Explore
% =================================================================

function best_nearby( self )
  % EXPLORE This method explore all points on the stencil center at
  % x_temporary = x_center and updates the current iteration x to the current
  % best point x_current_best. If the current best point x_current_best is worse than the
  % base point x_best, the current iteration x will remain constant
  % (x = x_best) and stencil failure flag stencil_failure will be set to zero.

  % Initialize
  self.m_stencil_failure = true;

  % ----------------------------------------------------------------------------------------
  % Cycle on all stencil directions

  for j = 1:self.m_dim
    s_dirh = (self.m_search_sign(j) * self.m_h) * self.m_Vmat(j,:);
    p      = self.m_x_best + s_dirh;
    fp     = self.eval_function(p);
    if fp >= self.m_f_best
      p1  = self.m_x_best - s_dirh; % try the opposite direction
      fp1 = self.eval_function(p1);
      if fp1 < fp
        p  = p1;
        fp = fp1;
        % change priority of search direction to the opposite verse
        self.m_search_sign(j) = - self.m_search_sign(j);
      end
    end

    % Update temporary and current best point before checking
    % the remaining directions j
    if fp < self.m_f_best
      self.m_plot( self.m_x_best, p, self.m_h );
      self.m_x_best          = p;     % new current best point
      self.m_f_best          = fp;    % new best value function
      self.m_stencil_failure = false; % update stencil failure flag
    end
  end
end
