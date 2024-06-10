classdef Poly < matlab.mixin.Copyable
  %%
  % Class storing a polynomial
  %
  %%% References
  %
  % * pippo
  %
  properties (SetAccess = private, Hidden = true)
    m_order;  % order of the polynomial
    m_coeffs; % coefficients of the polynomial
  end

  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj           = copyElement@matlab.mixin.Copyable(self);
      obj.m_order   = self.m_order;
      obj.m_coeffs  = self.m_coeffs;
    end
  end

  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = Poly( coeffs )
      % Construct a polynomial
      %
      % P = Poly()        % build an empty polynomial
      % P = Poly([1,2,3]) % build the polynomial 1+2*x+3*x^2
      %
      if nargin == 0
        self.m_coeffs = zeros(1,0);
      elseif nargin == 1
        self.m_coeffs = coeffs(:).';
      else
        error('Poly constructor, bad number of arguments');
      end
      self.m_order = length(self.m_coeffs);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function delete( self )
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = order( self )
      % Return the order (degree-1) of the polynomial
      res = self.m_order;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = degree( self )
      % Return the degree of the polynomial
      res = self.m_order-1;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function cfs = coeffs( self )
      % Return a vector with the coefficients of the polynomial
      cfs = self.m_coeffs;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = set_order( self, order )
      % Set a polynomial of a specified order with all the coefficients set to 0
      self.m_coeffs = zeros(1,order);
      self.m_order  = order;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = set_degree( self, degree )
      % Set a polynomial of a specified degree with all the coefficients set to 0
      self.m_coeffs = zeros(1,degree+1);
      self.m_order  = degree+1;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = set_scalar( self, a )
      % Set the polynomial of degree 0 (a constants) p(x) = a
      self.m_coeffs = a;
      self.m_order  = 1;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = set_monomial( self, a )
      % Set the monomial p(x) = x - a
      self.m_coeffs = [a,1];
      self.m_order  = 2;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = set_by_coeffs( self, coeffs )
      % Set the polynomial p(x) = sum coeffs(i)*x^(i-1)
      self.m_coeffs = coeffs;
      self.m_order  = length(coeffs);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function p = eval( self, x )
      % Evaluate the polynomial p(x) at point(s) x
      p = self.m_coeffs(end)*ones(size(x));
      for n=self.m_order-1:-1:1
        p = p.*x+self.m_coeffs(n);
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function dp = eval_D( self, x )
      % Evaluate the derivative of polynomial p(x) at point(s) x
      dp = self.m_coeffs(end)*ones(size(x))*(self.m_order-1);
      for n=self.m_order-1:-1:2
        dp = dp.*x+self.m_coeffs(n)*(n-1);
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    purge( self, epsi )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    S = normalize( self )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    res = plus(self,b)
    res = minus(self,b)
    res = uplus(self,b)
    res = uminus(self,b)
    res = mtimes(self,b)
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    out = to_string( self )
    res = derivative( self )
    res = integral( self )
    res = sign_variations( self )
    adjust_degree( self )
    nzero = remove_zero( self )
    [M,R] = divide( self, q )
    G = GCD( self, q )
    roots = real_roots_in_the_interval( self, a, b )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    mult_by_monomial( self, a, b ) % a * x + b
    P = to01( self, a, b )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function print( self )
      % Print the polynomial in a human readable way
      fprintf('%s\n',self.to_string());
    end
  end
end
