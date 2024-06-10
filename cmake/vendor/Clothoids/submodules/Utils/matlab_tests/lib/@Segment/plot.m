% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function plot( self, varargin )
  XX = [self.Pa(1), self.Pb(1)];
  YY = [self.Pa(2), self.Pb(2)];
  if nargin > 0
    plot( XX, YY, varargin{:} );
  else
    plot( XX, YY, 'o-b', 'LineWidth', 2, 'MarkerSize', 10, 'MarkerFaceColor', 'blue' );
  end
end
