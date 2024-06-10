function id_list = intersect_with_one_bbox( self, bb_min, bb_max )
  sz      = length(self.father);
  id_list = [];

  % quick return on empty inputs
  if sz == 0; return; end

  % allocation
  stack = zeros(sz,1);

  % descend tree from root
  root       = 1;
  stack(1)   = root;
  top_stack  = 1;
  while top_stack > 0
    % pop node from stack
    id_father = stack(top_stack);
    top_stack = top_stack - 1;

    % get BBOX
    father_min = self.bb_min(id_father,:);
    father_max = self.bb_max(id_father,:);

    if self.bbox_overlap( bb_min, bb_max, father_min, father_max )
      % get rectangles id in parent
      id_list = [ id_list; self.get_bbox_indexes_of_a_node( id_father ) ];

      nn = self.child(id_father);
      if nn > 0
        % push on stack children
        stack(top_stack+1) = nn;
        stack(top_stack+2) = nn+1;
        top_stack          = top_stack+2;
      end
    end
  end
end
