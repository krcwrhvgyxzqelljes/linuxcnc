classdef BBox < matlab.mixin.Copyable
  properties (SetAccess = private, Hidden = true)
    Pmin;
    Pmax;
    id;
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj      = copyElement@matlab.mixin.Copyable(self);
      obj.Pmin = self.Pmin;
      obj.Pmax = self.Pmax;
      obj.id   = self.id;
    end
  end
  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = BBox( varargin )
      if nargin > 0
        self.setup( varargin{1}, varargin{2}, varargin{3} );
      else
        self.Pmin = zeros(2,1);
        self.Pmax = zeros(2,1);
        self.id   = 0;
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'BBox';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function setup( self, Pmin, Pmax, id )
      self.Pmin = Pmin(:);
      self.Pmax = Pmax(:);
      self.id   = id;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function ma = get_max( self )
      ma = self.Pmax;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function mi = get_min( self )
      mi = self.Pmin;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ma,mi] = get_max_min( self )
      ma = self.Pmax;
      mi = self.Pmin;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    dst = distance( self, P )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    dst = max_distance( self, P )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ok = collide( self, B )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    union( self, B )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    intersect( self, B )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    plot( self, varargin )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
