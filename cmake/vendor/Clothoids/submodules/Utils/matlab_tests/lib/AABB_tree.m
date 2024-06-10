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

classdef AABB_tree < matlab.mixin.Copyable

  properties (SetAccess = private, Hidden = true)
    objectHandle;
    bb_min;
    bb_max;
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj = copyElement@matlab.mixin.Copyable(self);
      obj.objectHandle = AABB_treeMexWrapper( 'copy', self.objectHandle );
    end
  end
  methods
    function self = AABB_tree( varargin )
      self.objectHandle = AABB_treeMexWrapper( 'new' );
      if nargin > 0
        self.setup( varargin{:} )
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'AABB_tree';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function handle = handle( self )
      handle = self.objectHandle;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function delete(self)
      AABB_treeMexWrapper( 'delete', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function setup( self, max_num_objects_per_node, bbox_long_edge_ratio, bbox_overlap_tolerance )
      self.set_max_num_objects_per_node( max_num_objects_per_node );
      self.set_bbox_long_edge_ratio( bbox_long_edge_ratio );
      self.set_bbox_overlap_tolerance( bbox_overlap_tolerance );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_max_num_objects_per_node( self, max_num_objects_per_node )
      AABB_treeMexWrapper( ...
        'set_max_num_objects_per_node', self.objectHandle, max_num_objects_per_node ...
      );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_bbox_long_edge_ratio( self, bbox_long_edge_ratio )
      AABB_treeMexWrapper( ...
        'set_bbox_long_edge_ratio', self.objectHandle, bbox_long_edge_ratio ...
      );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_bbox_overlap_tolerance( self, bbox_overlap_tolerance )
      AABB_treeMexWrapper( ...
        'set_bbox_overlap_tolerance', self.objectHandle, bbox_overlap_tolerance ...
      );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function build( self, bb_min, bb_max, varargin )
      do_transpose = false;
      if nargin > 3; do_transpose = varargin{1}; end
      if do_transpose
        self.bb_min = bb_min.';
        self.bb_max = bb_max.';
        AABB_treeMexWrapper( 'build', self.objectHandle, bb_min.', bb_max.' );
      else
        self.bb_min = bb_min.';
        self.bb_max = bb_max.';
        AABB_treeMexWrapper( 'build', self.objectHandle, bb_min, bb_max );
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [bb_min,bb_max] = get_bb_min_max( self, varargin )
      if nargin > 1 && varargin{1}
        bb_min = self.bb_min.';
        bb_max = self.bb_max.';
      else
        bb_min = self.bb_min;
        bb_max = self.bb_max;
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ bb_min, bb_max ] = get_bboxes_of_the_tree( self, varargin )
      nmin = 0;
      if nargin > 1; nmin = varargin{1}; end
      [bb_min,bb_max] = AABB_treeMexWrapper( 'get_bboxes_of_the_tree', self.objectHandle, nmin );
      if nargin > 2
        if varargin{2}
          bb_min = bb_min.';
          bb_max = bb_max.';
        end
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id = get_bbox_indexes_of_a_node( self, inode )
      id = AABB_treeMexWrapper( 'get_bbox_indexes_of_a_node', self.objectHandle, inode );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function N = get_num_tree_nodes( self, varargin )
      if nargin > 1
        N = AABB_treeMexWrapper( 'get_num_tree_nodes', self.objectHandle, varargin{1} );
      else
        N = AABB_treeMexWrapper( 'get_num_tree_nodes', self.objectHandle, 0 );
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id_list = scan_points( self, pnts )
      id_list = AABB_treeMexWrapper( 'scan_points', self.objectHandle, pnts );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id_list = scan_bbox( self, bb_min, bb_max )
      id_list = AABB_treeMexWrapper( 'scan_bbox', self.objectHandle, bb_min, bb_max );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id_list = intersect_with_one_bbox( self, bb_min, bb_max )
      id_list = AABB_treeMexWrapper( 'intersect_with_one_bbox', self.objectHandle, bb_min, bb_max );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id_list = intersect( self, aabb )
      id_list = AABB_treeMexWrapper( 'intersect', self.objectHandle, aabb.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id_list = intersect_and_refine( self, aabb )
      id_list = AABB_treeMexWrapper( 'intersect_and_refine', self.objectHandle, aabb.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function id_list = min_distance_candidates( self, pnt )
      id_list = AABB_treeMexWrapper( 'min_distance_candidates', self.objectHandle, pnt );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function plot( self, varargin )
      % find non empty nodes
      [ bb_min, bb_max ] = self.get_bboxes_of_the_tree( 1 );
      if nargin > 1
        fc = varargin{1};
      else
        fc = [.95,.95,.55];
      end
      if nargin > 2
        ec = varargin{2};
      else
        ec = [.15,.15,.15];
      end
      self.plot_bbox( bb_min, bb_max, fc, ec, true );
      %self.plot_bbox( bb_min, bb_max, fc, ec );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function plot_bbox( self, mi, ma, fc, ec, varargin )
      transpose = false;
      if nargin > 5
        transpose = varargin{1};
      end
      if transpose
        nr    = size(mi,1);
        np    = size(mi,2);
        min_x = mi(1,:).';
        min_y = mi(2,:).';
        max_x = ma(1,:).';
        max_y = ma(2,:).';
      else
        np    = size(mi,1);
        nr    = size(mi,2);
        min_x = mi(:,1);
        min_y = mi(:,2);
        max_x = ma(:,1);
        max_y = ma(:,2);
      end
      idx   = (1:np)';
      % draw all "leaf" nodes as patches
      switch nr
      case 2
        pp = [ min_x, min_y; max_x, min_y; max_x, max_y; min_x, max_y ];
        % faces
        bb = [ idx, idx+np, idx+2*np, idx+3*np ];
      case 3
        if transpose
          min_z = mi(3,:).';
          max_z = ma(3,:).';
        else
          min_z = mi(:,3);
          max_z = ma(:,3);
        end
        pp = [ ...
          min_x, min_y, min_z; ...
          max_x, min_y, min_z; ...
          max_x, max_y, min_z; ...
          min_x, max_y, min_z; ...
          min_x, min_y, max_z; ...
          max_x, min_y, max_z; ...
          max_x, max_y, max_z; ...
          min_x, max_y, max_z  ...
        ];
        % faces
        bb = [
          idx,      idx+np,   idx+2*np, idx+3*np,
          idx+4*np, idx+5*np, idx+6*np, idx+7*np,
          idx,      idx+3*np, idx+7*np, idx+4*np,
          idx+3*np, idx+2*np, idx+6*np, idx+7*np,
          idx+2*np, idx+np,   idx+5*np, idx+6*np,
          idx+np,   idx,      idx+4*np, idx+5*np
        ];
      otherwise
        % what to do with a tree in R^d!?
        error('Unsupported d-dimensionality.') ;
      end
      patch( ...
        'faces',     bb, ...
        'vertices',  pp, ...
        'facecolor', fc, ...
        'edgecolor', ec, ...
        'facealpha', 0.2 ...
      );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function info( self )
      AABB_treeMexWrapper( 'info', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
