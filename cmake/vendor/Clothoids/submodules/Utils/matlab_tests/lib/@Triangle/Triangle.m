classdef Triangle < matlab.mixin.Copyable
  properties (SetAccess = private, Hidden = true)
    Pa; Na;
    Pb; Nb;
    Pc; Nc;
    cc; % counterclockwise
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj    = copyElement@matlab.mixin.Copyable(self);
      obj.Pa = self.Pa;
      obj.Pb = self.Pb;
      obj.Pc = self.Pc;
      obj.Na = self.Na;
      obj.Nb = self.Nb;
      obj.Nc = self.Nc;
      obj.cc = self.cc;
    end
  end
  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = Triangle( varargin )
      if nargin > 0
        self.setup( varargin{1}, varargin{2}, varargin{3} );
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'Triangle';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function setup( self, Pa, Pb, Pc )
      self.Pa = Pa(:);
      self.Pb = Pb(:);
      self.Pc = Pc(:);
      self.Na = [Pc(2)-Pb(2);Pb(1)-Pc(1)]; self.Na = self.Na./norm(self.Na);
      self.Nb = [Pa(2)-Pc(2);Pc(1)-Pa(1)]; self.Nb = self.Nb./norm(self.Nb);
      self.Nc = [Pb(2)-Pa(2);Pa(1)-Pb(1)]; self.Nc = self.Nc./norm(self.Nc);
      D = 2*Pa(:)-(Pb(:)+Pc(:));
      if dot(D,self.Na) < 0
        self.cc = false;
      else
        self.cc = true;
        self.Na = -self.Na;
        self.Nb = -self.Nb;
        self.Nc = -self.Nc;
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [A,B,C] = get_points( self )
      A = self.Pa;
      B = self.Pb;
      C = self.Pc;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [Na,Nb,Nc,cc] = get_normals( self )
      Na = self.Na;
      Nb = self.Nb;
      Nc = self.Nc;
      cc = self.cc;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function S = get_segment_AB( self )
      S = Segment( self.Pa, self.Pb );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function S = get_segment_BC( self )
      S = Segment( self.Pb, self.Pc );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function S = get_segment_CA( self )
      S = Segment( self.Pc, self.Pa );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    B = bbox( self, id )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [ s, nseg, ok ] = intersect_with_segment( self, S )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function ok = collide_with_segment( self, S )
      [ ~, ~, ok ] = self.intersect_with_segment( S );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    plot( self, varargin )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
