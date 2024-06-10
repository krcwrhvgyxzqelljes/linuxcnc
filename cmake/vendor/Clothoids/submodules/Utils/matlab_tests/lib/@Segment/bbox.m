function B = bbox( self, id )
  Pmin = min( self.Pa, self.Pb );
  Pmax = max( self.Pa, self.Pb );
  B = BBox( Pmin, Pmax, id );
end
