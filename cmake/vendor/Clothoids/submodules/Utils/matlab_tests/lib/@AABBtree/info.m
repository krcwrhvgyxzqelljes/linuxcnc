function info( self )
  nleaf = sum(self.child == 0);
  nlong = sum(self.num_nodes > 0) - nleaf;
  fprintf('AABB tree info-----------------\n');
  fprintf('Dimension                %d\n',size(self.bb_min,2));
  fprintf('Number of nodes          %d\n',length(self.father));
  fprintf('Number of leaf           %d\n',nleaf);
  fprintf('Number of long node      %d\n',nlong);
  fprintf('Number of objects        %d\n',length(self.id_nodes));
  fprintf('max_num_objects_per_node %d\n',self.max_num_objects_per_node);
  fprintf('bbox_long_edge_ratio     %g\n',self.bbox_long_edge_ratio);
  fprintf('bbox_overlap_tolerance   %g\n',self.bbox_overlap_tolerance);
  fprintf('-------------------------------\n');
end
