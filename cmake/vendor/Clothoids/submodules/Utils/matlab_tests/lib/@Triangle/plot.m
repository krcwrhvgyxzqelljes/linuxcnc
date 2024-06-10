% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function plot( self, varargin )
  X = [self.Pa(1),self.Pb(1), self.Pc(1),self.Pa(1)];
  Y = [self.Pa(2),self.Pb(2), self.Pc(2),self.Pa(2)];
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
