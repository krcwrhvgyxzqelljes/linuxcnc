function id_list = intersect2( self, aabb )
  sz1     = length(self.father);
  sz2     = length(aabb.father);
  id_list = cell(sz1,1);

  % quick return on empty inputs
  if sz1 == 0 || sz2 == 0; return; end

  % allocation
  stack = zeros(4*max(sz1,sz2),2);

  % descend tree from root
  stack(1,:) = [1,1];
  top_stack  = 1;
  while top_stack > 0
    % pop node from stack
    root1     = stack(top_stack,1);
    root2     = stack(top_stack,2);
    top_stack = top_stack - 1;

    % check for intersection
    if self.bbox_overlap( self.bb_min(root1,:), self.bb_max(root1,:), ...
                          aabb.bb_min(root2,:), aabb.bb_max(root2,:) )

      % check if there are elements to check
      nn1 = self.num_nodes(root1);
      nn2 = aabb.num_nodes(root2);
      if nn1 > 0 && nn2 > 0
        id_list{root1} = [id_list{root1}; aabb.get_bbox_indexes_of_a_node(root2)];
      end

      id_lr1 = self.child(root1);
      id_lr2 = aabb.child(root2);

      % do recursion for subtree
      icase = 0;
      if id_lr1 > 0; icase = 1;         end
      if id_lr2 > 0; icase = icase + 2; end

      switch icase
      case 0
      case 1
        stack(top_stack+1,:) = [ id_lr1,   root2 ];
        stack(top_stack+2,:) = [ id_lr1+1, root2 ];
        top_stack = top_stack + 2;
      case 2
        stack(top_stack+1,:) = [ root1, id_lr2   ];
        stack(top_stack+2,:) = [ root1, id_lr2+1 ];
        top_stack = top_stack + 2;
      case 3
        stack(top_stack+1,:) = [ id_lr1,   id_lr2   ];
        stack(top_stack+2,:) = [ id_lr1+1, id_lr2   ];
        stack(top_stack+3,:) = [ id_lr1,   id_lr2+1 ];
        stack(top_stack+4,:) = [ id_lr1+1, id_lr2+1 ];
        top_stack = top_stack + 4;
        if nn1 > 0
          stack(top_stack+1,:) = [ root1, id_lr2   ];
          stack(top_stack+2,:) = [ root1, id_lr2+1 ];
          top_stack = top_stack + 2;
        end
        if nn2 > 0
          stack(top_stack+1,:) = [ id_lr1,   root2 ];
          stack(top_stack+2,:) = [ id_lr1+1, root2 ];
          top_stack = top_stack + 2;
        end
      end
    end
  end
end
