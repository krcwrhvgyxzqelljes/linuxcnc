function id_list = scan_points( self, pnts )
  sz      = length(self.father);
  id_list = cell(sz,1);

  % quick return on empty inputs
  if sz == 0; return; end

  % allocation
  stack = zeros(sz,1);

  % descend tree from root
  stack(1)   = 1;
  top_stack  = 1;
  idx        = (1:size(pnts,1)).';
  bmin       = self.bb_min(1,:);
  bmax       = self.bb_max(1,:);
  oks        = ~(any( pnts.' < bmin.' ) | any( pnts.' > bmax.' ));
  id_list{1} = idx(oks);
  while top_stack > 0
    % pop node from stack
    id_father = stack(top_stack);
    top_stack = top_stack - 1;
    idx       = id_list{id_father};

    % list of points that may intersect
    if self.num_nodes(id_father) == 0
      id_list{id_father} = [];
    end

    if self.child(id_father) == 0
      % no children, nothing to do
      continue;
    end

    pp = pnts(idx,:).';

    for kkk=0:1
      id_lr  = self.child(id_father)+kkk;
      lr_min = self.bb_min(id_lr,:).';
      lr_max = self.bb_max(id_lr,:).';

      % check intersetion
      oks = ~( any( pp < lr_min ) | any( pp > lr_max ) );
      if any(oks)
        % push nonempty node onto stack
        top_stack        = top_stack + 1;
        stack(top_stack) = id_lr;
        id_list{id_lr}   = idx(oks);
      end
    end
  end
end
