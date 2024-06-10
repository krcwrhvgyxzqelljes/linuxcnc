function B = bbox( self, id )
  Pmin = min( self.Pa, min( self.Pb, self.Pc ) );
  Pmax = max( self.Pa, max( self.Pb, self.Pc ) );
  B    = BBox( Pmin, Pmax, id );
end
