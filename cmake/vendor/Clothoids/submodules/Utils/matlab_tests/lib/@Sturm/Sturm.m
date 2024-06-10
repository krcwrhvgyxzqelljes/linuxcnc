classdef Sturm < handle
  properties (SetAccess = private, Hidden = true)
    m_sturm;
    m_intervals;
    m_a;
    m_b;
    m_roots;
    m_solver;
  end

  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = Sturm()
      self.m_sturm     = {};
      self.m_intervals = {};
      self.m_a         = 0;
      self.m_b         = 0;
      self.m_roots     = zeros(0,0);
      self.m_solver    = Algo748();
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function delete( self )
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %function set_max_iter( self, miter )
    %  assert( ...
    %    miter > 0 && floor(miter) == miter && miter < 1000000, ...
    %    'miter must be a positive integer < 1000000' ...
    %  );
    %  self.m_max_iter = miter;
    %end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [a,b] = get_ab( self )
      % Return the interval [a,b] where real roots are searched
      a = self.m_a;
      b = self.m_b;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function n = n_intervals( self )
      % Return the number of interval containing a real root
      n = length(self.m_intervals);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function I = get_interval( self, ipos )
      % Get the i-th interval containing a real root
      I = self.m_intervals{ipos};
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function n = n_roots( self )
      % Return the number of real roots found
      n = length(self.m_roots);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function r = roots( self )
      % Return the vector with the real roots
      r = self.m_roots;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function P = get( self, i )
      % Get the i-th polynomial of the Sturm sequence
      P = self.m_sturm{i};
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function len = length( self )
      % Get the number of polynomial of the Sturm sequence
      len = length(self.m_sturm);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    build( self, poly );
    separate_roots( self, a, b );
    refine_roots( self );
    [s,on_roots] = sign_variations( self, x );
    str = to_string( self );
    print( self, check )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
