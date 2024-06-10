% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function plot( self, varargin )
  % - - - - - - - - - - - - - - - - - - - - - - -  find non empty nodes
  lf = find(self.num_nodes>0);
  if nargin > 1
    fc = varargin{1};
  else
    fc = [.95,.95,.55];
  end
  if nargin > 2
    ec = varargin{2};
  else
    ec = [.15,.15,.15];
  end
  self.plot_bbox( self.bb_min(lf,:), self.bb_max(lf,:), fc, ec );
end
