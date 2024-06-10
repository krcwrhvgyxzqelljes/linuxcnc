% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function plot( self, varargin )
  X = [ self.Pmin(1), self.Pmax(1), self.Pmax(1), self.Pmin(1), self.Pmin(1) ];
  Y = [ self.Pmin(2), self.Pmin(2), self.Pmax(2), self.Pmax(2), self.Pmin(2) ];
  if nargin > 1
    plot( X, Y, 'LineWidth', 2, 'Color', varargin{:} );
    hold on;
    fill( X, Y, varargin{:} );
  else
    plot( X, Y, 'LineWidth', 2, 'Color', 'blue' );
    hold on;
    fill( X, Y, 'blue', 'FaceAlpha', 0.2 );
  end
end
