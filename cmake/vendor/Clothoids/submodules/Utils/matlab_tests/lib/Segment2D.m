%-------------------------------------------------------+
%                                                       |
% Copyright (C) 2022                                    |
%                                                       |
%        , __                 , __                      |
%       /|/  \               /|/  \                     |
%        | __/ _   ,_         | __/ _   ,_              |
%        |   \|/  /  |  |   | |   \|/  /  |  |   |      |
%        |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/     |
%                          /|                   /|      |
%                          \|                   \|      |
%                                                       |
%     Enrico Bertolazzi                                 |
%     Dipartimento di Ingegneria Industriale            |
%     Universita` degli Studi di Trento                 |
%     email: enrico.bertolazzi@unitn.it                 |
%                                                       |
%-------------------------------------------------------+

classdef Segment2D < matlab.mixin.Copyable

  properties (SetAccess = private, Hidden = true)
    objectHandle;
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj = copyElement@matlab.mixin.Copyable(self);
      obj.objectHandle = Segment2DMexWrapper( 'copy', self.objectHandle );
    end
  end
  methods
    function self = Segment2D( varargin )
      self.objectHandle = Segment2DMexWrapper( 'new' );
      if nargin > 0
        self.setup( varargin{:} )
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'Segment2D';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function handle = handle( self )
      handle = self.objectHandle;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function delete(self)
      Segment2DMexWrapper( 'delete', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function setup( self, Pa, Pb )
      Segment2DMexWrapper( 'setup', self.objectHandle, Pa, Pb );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [A,B] = getAB( self )
      [A,B] = Segment2DMexWrapper( 'getAB', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function BB = bbox( self, id )
      [A,B] = Segment2DMexWrapper( 'getAB', self.objectHandle );
      BB = BBox( min( A, B ), max( A, B ), id );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function PP = eval( self, s, varargin )
      if nargin > 2
        PP = Segment2DMexWrapper( 'evalst', self.objectHandle, s, varargin{1} );
      else
        PP = Segment2DMexWrapper( 'evals', self.objectHandle, s );
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [s,t,PP] = point_coord( self, P )
      [s,t,PP] = Segment2DMexWrapper( 'point_coord', self.objectHandle, P );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [s,t,ok] = intersect( self, S )
      [s,t,ok] = Segment2DMexWrapper( 'intersect', self.objectHandle, S.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function ok = collide( self, S )
      [~,~,ok] = self.intersect( S )
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function plot( self, varargin )
      [A,B] = self.getAB();
      if nargin > 0
        plot( [A(1),B(1)], [A(2),B(2)], varargin{:} );
      else
        plot( ...
          [A(1),B(1)], [A(2),B(2)], ...
          'o-b', 'LineWidth', 2, 'MarkerSize', 10, 'MarkerFaceColor', 'blue' ...
        );
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
