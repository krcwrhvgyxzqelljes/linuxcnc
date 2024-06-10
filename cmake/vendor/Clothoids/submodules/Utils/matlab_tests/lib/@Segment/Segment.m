classdef Segment < matlab.mixin.Copyable
  properties (SetAccess = private, Hidden = true)
    Pa;
    Pb;
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj    = copyElement@matlab.mixin.Copyable(self);
      obj.Pa = self.Pa;
      obj.Pb = self.Pb;
    end
  end
  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = Segment( varargin )
      if nargin > 0
        self.Pa = varargin{1};
        self.Pb = varargin{2};
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'Segment';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function setup( self, Pa, Pb )
      self.Pa = Pa(:);
      self.Pb = Pb(:);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [A,B] = getAB( self )
      A = self.Pa;
      B = self.Pb;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    B = bbox( self, id )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    PP = eval( self, s, varargin )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [s,t,PP] = point_coord( self, P )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [s,t,ok] = intersect( self, S )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function ok = collide( self, S )
      [~,~,ok] = self.intersect( S )
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    plot( self, varargin )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
