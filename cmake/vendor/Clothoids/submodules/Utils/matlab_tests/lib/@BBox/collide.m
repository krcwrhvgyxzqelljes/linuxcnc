function ok = collide( self, B )
  ok = false;
  if self.Pmin(1) > B.Pmax(1); return; end;
  if self.Pmin(2) > B.Pmax(2); return; end;
  if self.Pmax(1) < B.Pmin(1); return; end;
  if self.Pmax(2) < B.Pmin(2); return; end;
  ok = true;
end
