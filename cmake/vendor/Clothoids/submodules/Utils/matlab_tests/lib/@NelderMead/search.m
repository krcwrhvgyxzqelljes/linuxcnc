%
%  Multidimensional minimization of the function FUN(x) where x(1:n_dim)
%  is a vector in n_dim dimensions, by the downhill simplex method of
%  Nelder and Mead.
%  The matrix p(1:ndim+1,1:n_dim) is input.
%  Its ndim+1 rows are n_dim-dimensional vectors which are the
%  vertices of the starting simplex.
%  Also input is the vector y(1:ndim+1), whose components must be
%  pre-initialized to the values of FUN(x) evaluated at the n_dim+1
%  vertices (rows) of p; and ftol the fractional convergence
%  tolerance to be achieved in the function value (n.b.!).
%  On output, p and y will have been reset to n_dim+1 new points
%  all within ftol of a minimum function value.
%
function [pmin,fmin] = search( self )

  self.m_iteration_count      = 0;
  self.m_fun_evaluation_count = 0;

  self.dist_init();
  self.m_psum       = sum(self.m_p);
  self.m_which_step = 'INIT';

  for kkk=1:self.m_max_iteration
    self.m_iteration_count = self.m_iteration_count+1;

    if ( self.m_fun_evaluation_count >= self.m_max_fun_evaluation )
      error( ...
        '#fun evaluation = %d exceed %d\n', ...
        self.m_fun_evaluation_count, self.m_max_fun_evaluation ...
      );
    end

    %
    % Determine which point is the highest (worst), next-highest, and lowest (best).
    %
    [~,idx]      = sort( self.m_f );
    self.m_low   = idx(1);
    self.m_0high = idx(end-1);
    self.m_high  = idx(end);
    f0           = self.m_f(self.m_low);
    fn           = self.m_f(self.m_0high);
    fn1          = self.m_f(self.m_high);
    rtol         = abs( fn1 - f0 ) / ( abs(f0) + self.m_tolerance );
    self.m_plot( self.m_p, [self.m_low, self.m_high] );
    self.message( rtol );

    if rtol <= self.m_tolerance || self.m_diameter <= self.m_tolerance
      pmin = self.m_p(self.m_low,:);
      fmin = f0;
      disp('CONVERGED');
      return;
    end

    % check volume deformation
    Vr    = self.regular_simplex_volume( self.m_diameter );
    ratio = (self.m_simplex_volume/Vr)^(1/self.m_dim);
    if ratio <= self.m_volume_tolerance
      self.diamond( self.m_p(self.m_low,:), self.m_volume_tolerance * self.m_diameter );
      self.dist_init();
      self.m_psum = sum(self.m_p);
      self.m_which_step = 'RESTART';
      continue;
    end

    [fr,pr] = self.reflect();
    if fr < f0
      [fe,pe] = self.expand();
      if fe < fr % f0 in versione originale
        self.m_which_step = 'EXPAND_FE';
        self.replace_point( fe, pe, self.m_high );
      else
        self.m_which_step = 'EXPAND_FR';
        self.replace_point( fr, pr, self.m_high );
      end
    elseif fr < fn
      self.m_which_step = 'REFLECT';
      self.replace_point( fr, pr, self.m_high );
    elseif fr < fn1
      [fc,pc] = self.outside();
      if fc < fr
        self.m_which_step = 'CONTRACT_O';
        self.replace_point( fc, pc, self.m_high );
      else
        self.m_which_step = 'REFLECT';
        self.replace_point( fr, pr, self.m_high );
      end
    else
      [fc,pc] = self.inside();
      if fc < fn1
        self.m_which_step = 'CONTRACT_I';
        self.replace_point( fc, pc, self.m_high );
      else
        if false && rand < self.m_prob && isfinite(fn)
          self.m_which_step = 'WORSE';
          self.replace_point( fr, pr, self.m_high );
        else
          self.m_which_step = 'SHRINK';
          self.shrink( self.m_low );
        end
      end
    end
  end
end
