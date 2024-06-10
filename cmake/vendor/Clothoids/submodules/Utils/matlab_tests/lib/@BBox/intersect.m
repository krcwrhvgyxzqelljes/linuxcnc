function intersect( self, B )
  self.Pmin = max( self.Pmin, B.Pmin );
  self.Pmax = min( self.Pmax, B.Pmax );
  if any( self.Pmin > self.Pmax )
    error( 'BBox::intersect, empty intersection' );
  end
end
