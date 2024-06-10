% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function build( self, pmin, pmax, varargin )
  % get dimensions
  dim = size(pmin,2); % 2D or 3D or more...
  nbb = size(pmin,1); % number of BBOX

  % speed up computation by pre-allocating vectors
  nmax      = 2*nbb; % estimate max memory usage
  bb_min    = zeros(nmax,dim);
  bb_max    = zeros(nmax,dim);
  father    = zeros(nmax,1);
  child     = zeros(nmax,1);
  ptr_nodes = zeros(nmax,1);
  num_nodes = zeros(nmax,1);
  stack     = zeros(nbb,1);

  % initialize id nodes, will be reordered during the tree build
  id_nodes  = (1:nbb).';

  % bbox centers and lengths
  dcenter = 0.5*(pmin+pmax);
  dlength = pmax-pmin;
  % setup root node
  father(1)    = 0;
  child(1)     = 0;
  ptr_nodes(1) = 1;
  num_nodes(1) = nbb;
  % root contains all rectangles, build its bbox
  bb_min(1,:) = min(pmin,[],1);
  bb_max(1,:) = max(pmax,[],1);

  % main loop: divide nodes until all constraints satisfied
  stack(1)  = 1;
  top_stack = 1;
  nn        = 1;
  while top_stack > 0

    % pop node from stack
    id_father = stack(top_stack);
    top_stack = top_stack - 1;

    % get rectangles id in parent
    num = num_nodes(id_father);

    % if few bbox stop splitting
    if num < self.max_num_objects_per_node; continue; end

    ptr    = ptr_nodes(id_father);
    inodes = id_nodes(ptr:ptr+num-1);

    % split plane on longest axis, use euristic
    dd = bb_max(id_father,:) - bb_min(id_father,:);
    [dd,ia] = sort(dd);

    for id = dim:-1:1
      ax = ia(id);
      mx = dd(id);

      il          = dlength(inodes,ax) > self.bbox_long_edge_ratio * mx;
      long_nodes  = inodes( il); % "long" rectangles
      short_nodes = inodes(~il); % "short" rectangles

      if length(long_nodes) < 0.5*min(length(short_nodes),self.max_num_objects_per_node)
        break;
      end
    end

    if length(short_nodes) < 2
      % Split rectangles do not improve search, stop split at this level
      continue;
    end

    % select the split position: take the mean of the set of
    % (non-"long") rectangle centres along axis AX
    sp = mean(dcenter(short_nodes,ax));

    % partition based on centers
    idx         = dcenter(short_nodes,ax) > sp;
    left_nodes  = short_nodes(~idx); % "left" rectangles
    right_nodes = short_nodes( idx); % "right" rectangles

    nlong  = length(long_nodes);
    nleft  = length(left_nodes);
    nright = length(right_nodes);

    if nleft == 0 || nright == 0
      % cannot improve bbox, stop split at this level!
      continue;
    end

    % child indexing
    id_left  = nn + 1;
    id_right = nn + 2;

    % compute bbox of left and right child
    bb_min(id_left,:)  = min(pmin(left_nodes,:),[],1) ;
    bb_max(id_left,:)  = max(pmax(left_nodes,:),[],1) ;
    bb_min(id_right,:) = min(pmin(right_nodes,:),[],1) ;
    bb_max(id_right,:) = max(pmax(right_nodes,:),[],1) ;

    % check again if split improve the AABBtree otherwise stop exploration
    if nleft < self.max_num_objects_per_node || nright < self.max_num_objects_per_node
      % few nodes, check if improve volume
      dvo = min(bb_max(id_left,:),bb_max(id_right,:)) - ...
            max(bb_min(id_left,:),bb_min(id_right,:));
      if all(dvo > 0)
        % not empty overlap
        vo = prod( dvo );
        v1 = prod( bb_max(id_left,:)  - bb_min(id_left,:) );
        v2 = prod( bb_max(id_right,:) - bb_min(id_right,:) );
        if vo > (v1+v2-vo)*self.bbox_overlap_tolerance^dim
          % do not improve volume, stop split at this level!
          continue;
        end
      end
    end

    % push child nodes onto stack
    father(id_left)  = id_father;
    father(id_right) = id_father;
    child(id_father) = id_left;

    % reorder nodes in id_nodes
    ptr1 = ptr+nlong;
    ptr2 = ptr1+nleft;
    ptr3 = ptr2+nright;
    id_nodes(ptr:ptr1-1)  = long_nodes;
    id_nodes(ptr1:ptr2-1) = left_nodes;
    id_nodes(ptr2:ptr3-1) = right_nodes;

    % ptr_nodes(id_father) = ptr; % already set
    num_nodes(id_father) = nlong;

    ptr_nodes(id_left)  = ptr+nlong;
    num_nodes(id_left)  = nleft;

    ptr_nodes(id_right) = ptr+nlong+nleft;
    num_nodes(id_right) = nright;

    % push on stack children
    stack(top_stack+1) = id_left;
    stack(top_stack+2) = id_right;
    top_stack          = top_stack+2;
    nn                 = nn+2;

  end
  % pack memory and store in class
  self.bb_min    = bb_min(1:nn,:);
  self.bb_max    = bb_max(1:nn,:);
  self.father    = father(1:nn);
  self.child     = child(1:nn);
  self.ptr_nodes = ptr_nodes(1:nn);
  self.num_nodes = num_nodes(1:nn);
  self.id_nodes  = id_nodes;
end
