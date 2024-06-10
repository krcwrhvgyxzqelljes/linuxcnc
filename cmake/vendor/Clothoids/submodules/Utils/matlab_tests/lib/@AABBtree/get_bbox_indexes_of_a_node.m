% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function inodes = get_bbox_indexes_of_a_node( self, id )
  num = self.num_nodes(id);
  if num > 0
    ptr    = self.ptr_nodes(id);
    inodes = self.id_nodes(ptr:ptr+num-1);
  else
    inodes = [];
  end
end
