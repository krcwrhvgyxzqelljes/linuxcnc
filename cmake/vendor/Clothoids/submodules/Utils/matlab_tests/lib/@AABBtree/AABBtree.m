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
%
%   Build an axis-aligned bounding-box (AABB) tree for a
%   collection of d-dimensional bounding-box.
%
%   Method `build` forms a binary search tree, such that
%   each "leaf" node in the tree encloses a maximum number
%   of rectangles.
%   The tree is formed by recursively subdividing the
%   bounding-box of the collection.
%
%   At each division, a simple heuristic is used to
%   determine a splitting axis and to position an
%   axis-aligned splitting (hyper-)plane.
%
%   The associated collection of rectangles is partitioned
%   between two new child nodes. The dimensions of each node
%   in the tree are selected to provide a minimal enclosure
%   of the rectangles in its associated sub-tree.
%   Tree nodes may overlap as a result.
%
%   The bounding-boxes are defined as two N-by-d array
%   of min and max coordinates associated with each
%   bounding-box.
%
%   The resulting tree with (M >= N nodes) is stored
%   in the class as a set of arrays containing
%
%   - bb_min, bb_max: M-by-d arrays of tree-node bounding-box
%
%   - father: M-by-1 array with the index of the "father" node.
%             for the root node father(root)is equal to 0.
%
%   - child:  M-by-1 array with the index of the "child" nodes.
%             For a leaf node child(node) is equal to 0.
%             The index of the two children are child(node)
%             and child(node)+1
%
%   - id_nodes: N-by-1 array with the index of the objects
%               (with bounding-box) used in the AABB tree
%               construction. This array is internally reordered
%               to match the partition with the associated node tree.
%
%   - ptr_nodes: N-by-1 array with the index of the objects
%                in id_nodes associated with the tree nodes.
%                For a node at index IDX the number ptr_nodes(IDX)
%                is the starting index in id_nodes for the objects
%                associated.
%
%   - num_nodes: N-by-1 array with the number of the objects
%                in id_nodes associated with the tree nodes.
%                For a node at index IDX the objects associated
%                are at indices: ptr_nodes(IDX:IDX+num_nodes(IDX)-1)
%
%   - max_num_objects_per_node:
%       (default 32) is the maximum allowable number
%       of bounding-boxes per tree-node.
%
%   - bbox_long_edge_ratio:
%       (default 0.8) is a relative length tolerance
%       for "long" rectangles, such that any rectangles with
%
%       RMAX(IX)-RMIN(IX) >= bbox_long_edge_ratio * (NMAX(IX)-NMIN(IX))
%
%       remain in the parent node.
%       Here RMIN, RMAX are the coordinates of the rectangle,
%       NMIN,NMAX are the coordinates of the enclosing node in
%       the tree, and IX is the splitting axis.
%       Nodes that become "full" of "long" items may exceed their
%       max_num_objects_per_node capacity.
%
%   - bbox_overlap_tolerance:
%       (default 0.1) is a "volume" splitting criteria,
%       designed to continue subdivision while the net
%       node volume is reducing.
%       Specifically, a node is split if
%
%       VO <= VU*bbox_overlap_tolerance
%
%       where VO is the volume of the overlapping
%       bonding-boxes of the two children and VU
%       is the volume of the union of the volumes
%       associated with its children.
%
%   REMARK:
%
%   The code is an (hopefully) improved object oriented version
%   of the original code of Darren Engwirda (de2363@columbia.edu)
%   devoloped in its PhD thesis
%
%   Darren Engwirda
%   "Locally-optimal Delaunay-refinement & optimisation-based mesh generation".
%   Ph.D. Thesis, School of Mathematics and Statistics,
%   Univ. of Sydney, 2014:
%   http://hdl.handle.net/2123/13148
%
%   and available on Mathworks: https://it.mathworks.com/matlabcentral/profile/authors/870403
%
classdef AABBtree < matlab.mixin.Copyable
  properties (SetAccess = private, Hidden = true)
    father;
    child;
    % --------
    ptr_nodes;
    num_nodes;
    id_nodes;
    % --------
    bb_min;
    bb_max;
    % --------
    max_num_objects_per_node;
    bbox_long_edge_ratio;
    bbox_overlap_tolerance;
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj                          = copyElement@matlab.mixin.Copyable(self);
      obj.father                   = self.father;
      obj.child                    = self.child;
      obj.ptr_nodes                = self.ptr_nodes;
      obj.num_nodes                = self.num_nodes;
      obj.id_nodes                 = self.id_nodes;
      obj.bb_min                   = self.bb_min;
      obj.bb_max                   = self.bb_max;
      obj.max_num_objects_per_node = self.max_num_objects_per_node;
      obj.bbox_long_edge_ratio     = self.bbox_long_edge_ratio;
      obj.bbox_overlap_tolerance   = self.bbox_overlap_tolerance;
    end
  end
  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = AABBtree( varargin )
      isoctave = exist('OCTAVE_VERSION','builtin') > 0;
      if (isoctave)
        %-- faster for OCTAVE with large tree block size; slower loop execution.
        max_num_objects_per_node = 1024;
      else
        %-- faster for MATLAB with small tree block size; better loop execution.
        max_num_objects_per_node = 32;
      end
      bbox_long_edge_ratio   = 0.8;
      bbox_overlap_tolerance = 0.1;
      if nargin > 0; max_num_objects_per_node = varargin{1}; end
      if nargin > 1; bbox_long_edge_ratio     = varargin{2}; end
      if nargin > 2; bbox_overlap_tolerance   = varargin{3}; end
      self.setup( max_num_objects_per_node, bbox_long_edge_ratio, bbox_overlap_tolerance );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'AABBtree';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_max_num_objects_per_node( self, max_num_objects_per_node )
      self.max_num_objects_per_node = max_num_objects_per_node;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_bbox_long_edge_ratio( self, bbox_long_edge_ratio )
      self.bbox_long_edge_ratio = bbox_long_edge_ratio;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_bbox_overlap_tolerance( self, bbox_overlap_tolerance )
      self.bbox_overlap_tolerance = bbox_overlap_tolerance;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    setup( self, max_num_objects_per_node, bbox_long_edge_ratio, bbox_overlap_tolerance )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    build( self, bb_min, bb_max, varargin )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ok = bbox_overlap( self, A_min, A_max, B_min, B_max )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [bb_min,bb_max] = get_bb_min_max( self )
      bb_min = self.bb_min;
      bb_max = self.bb_max;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function bb_min = get_bb_min( self )
      bb_min = self.bb_min;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function bb_max = get_bb_max( self )
      bb_max = self.bb_max;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [f,c,ptr,num,id] = get_tree_structure( self )
      f   = self.father;
      c   = self.child;
      ptr = self.ptr_nodes;
      num = self.num_nodes;
      id  = self.id_nodes;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    inodes = get_bbox_indexes_of_a_node( self, id )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function N = get_num_nodes( self )
      N = length(self.father);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    id_list = scan_points( self, pnts )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    id_list = scan_bbox( self, bb_min, bb_max )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    id_list = intersect( self, aabb )
    id_list = intersect2( self, aabb )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ok_list = intersect_with_one_bbox( self, bb_min, bb_max )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ok_list = min_distance_candidates( self, pnt )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [pmin,pmax] = pnt_bbox_minmax( self, pnt, bb_min, bb_max )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    plot( self, varargin )
    plot_bbox( self, mi, ma, fc, ec )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    info( self )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
