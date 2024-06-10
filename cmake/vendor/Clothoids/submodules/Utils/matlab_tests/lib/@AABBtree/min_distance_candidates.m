function id_list = min_distance_candidates( self, pnt )
  sz      = length(self.father);
  id_list = [];

  % quick return on empty inputs
  if sz == 0; return; end

  min_max_distance = Inf;

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

    [pmin,pmax] = self.pnt_bbox_minmax( pnt, father_min, father_max );
    dst_min = norm(pmin-pnt);

    if dst_min <= min_max_distance
      if self.m_num_nodes(id_father) > 0
        min_max_distance = min( min_max_distance, norm(pmax-pnt) );
      end
      nn = self.child(id_father);
      if nn > 0
        % push on stack childrens
        stack(top_stack+1) = nn;
        stack(top_stack+2) = nn+1;
        top_stack          = top_stack+2;
      end
    end
  end

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

    [pmin,pmax] = self.pnt_bbox_minmax( pnt, father_min, father_max );
    dst_min = norm(pmin-pnt);

    if dst_min <= min_max_distance
      id_list = [ id_list; self.get_bbox_indexes_of_a_node( id_father ) ];
      nn      = self.child(id_father);
      if nn > 0
        % push on stack childrens
        stack(top_stack+1) = nn;
        stack(top_stack+2) = nn+1;
        top_stack          = top_stack+2;
      end
    end
  end

end
