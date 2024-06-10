% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function setup( self, max_num_objects_per_node, bbox_long_edge_ratio, bbox_overlap_tolerance )
  % bound population count
  if max_num_objects_per_node <= 0
    error('max_num_objects_per_node must be > 0, found %g', max_num_objects_per_node );
  end
  % bound "long" tolerance
  if bbox_long_edge_ratio < 0 || bbox_long_edge_ratio > 1
    error('bbox_long_edge_ratio must be in (0,1) found %g', bbox_long_edge_ratio );
  end
  % bound "bbox_overlap_tolerance" tolerance
  if bbox_overlap_tolerance < 0 || bbox_overlap_tolerance > 1
    error('bbox_overlap_tolerance must be in (0,1) found %g', bbox_overlap_tolerance );
  end
  self.max_num_objects_per_node = max_num_objects_per_node;
  self.bbox_long_edge_ratio     = bbox_long_edge_ratio;
  self.bbox_overlap_tolerance   = bbox_overlap_tolerance;
end
