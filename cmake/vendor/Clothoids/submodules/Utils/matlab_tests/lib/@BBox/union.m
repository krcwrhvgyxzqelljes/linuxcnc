function union( self, B )
  self.Pmin = min( self.Pmin, B.Pmin );
  self.Pmax = max( self.Pmax, B.Pmax );
end
