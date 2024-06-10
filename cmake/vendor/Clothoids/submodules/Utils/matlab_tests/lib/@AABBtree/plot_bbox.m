% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function plot_bbox( self, mi, ma, fc, ec )
  %----------------------------------------------- tree in R^p
  np  = size(mi,1);
  idx = (1:np)';
  %------------------------------------------------- nodes
  min_x = mi(:,1);
  min_y = mi(:,2);
  max_x = ma(:,1);
  max_y = ma(:,2);

  %-------------------------- draw all "leaf" nodes as patches
  switch(size(mi,2))
  case 2
    pp = [
      min_x, min_y,
      max_x, min_y,
      max_x, max_y,
      min_x, max_y
    ];
    %------------------------------------------------- faces
    bb  = [ idx, idx+np, idx+2*np, idx+3*np ];

  case 3
    min_z = mi(:,3);
    max_z = ma(:,3);
    pp = [
      min_x, min_y, min_z,
      max_x, min_y, min_z,
      max_x, max_y, min_z,
      min_x, max_y, min_z,
      min_x, min_y, max_z,
      max_x, min_y, max_z,
      max_x, max_y, max_z,
      min_x, max_y, max_z
    ];
    %------------------------------------------------- faces
    bb  = [
      idx,      idx+np,   idx+2*np, idx+3*np,
      idx+4*np, idx+5*np, idx+6*np, idx+7*np,
      idx,      idx+3*np, idx+7*np, idx+4*np,
      idx+3*np, idx+2*np, idx+6*np, idx+7*np,
      idx+2*np, idx+np,   idx+5*np, idx+6*np,
      idx+np,   idx,      idx+4*np, idx+5*np
    ];

  otherwise
    %--------------------------- what to do with a tree in R^d!?
    error('scantree:unsupportedDimension Unsupported d-dimensionality.') ;
  end

  patch( ...
    'faces',     bb, ...
    'vertices',  pp, ...
    'facecolor', fc, ...
    'edgecolor', ec, ...
    'facealpha', 0.2 ...
  );
end
