function [s,t,ok] = intersect( self, S )
  ok = false;
  s  = 0;
  t  = 0;
  % check if collinear
  D1 = self.Pb-self.Pa;
  D2 = S.Pb-S.Pa;
  CX = ( D1(1)*D2(2) - D1(2)*D2(1) ) / (norm(D1)*norm(D2));
  if abs(CX) <= 10*eps
    % collinear point check if overlap
    [s,t,PP] = self.point_coord( S.Pa );
    if s >= 0 && s <= 1
      ok = true;
      return
    end
    [s,t,PP] = self.point_coord( S.Pb );
    if s >= 0 && s <= 1
      ok = true;
      return
    end
    s = 0;
    t = 0;
    return;
  end
  % regular case, find intersection
  % Pa + s*D1 = A + t*D2
  % [D1,-D2]*[s;t] = A-Pa;
  res = [D1,-D2]\(S.Pa-self.Pa);
  s   = res(1);
  t   = res(2);
  if s >= 0 && s <= 1 && t >= 0 && t <= 1
    ok = true;
  end
end
