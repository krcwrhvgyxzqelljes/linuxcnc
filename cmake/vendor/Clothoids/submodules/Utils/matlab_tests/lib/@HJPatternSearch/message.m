% =================================================================
% Search
% =================================================================

function message(self)
  if self.m_verbose > 1
    line = '-------------------------------------------------------------------------';
    if self.m_verbose > 2
      fprintf('%s\n',line');
    end
    fprintf( ...
      'Iteration=%-5d f(x)=%-10.5g scale=%-10.5g #f=%-5d |h|=%-10.5g\n',...
      self.m_iteration_count, self.m_f_best, norm(self.m_h,inf), ...
      self.m_fun_evaluation_count, ...
      self.m_h ...
    );
    if self.m_verbose > 2
      for ii=1:self.N
        fprintf( '%d\t%g\n', ii, self.x(ii) );
      end
      fprintf('%s\n',line');
    end
  end
end
