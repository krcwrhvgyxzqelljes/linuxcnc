function [ s, nseg, ok ] = intersect_with_segment( self, S )
  s    = [];
  nseg = [];
  ok   = 0;
  % \  6  |
  %   \   |
  %     \ |
  %       C
  %       | \     2
  %   4   |   \
  %       |  0  \
  % ------A------B-------
  %       |        \
  %   5   |   1      \  3
  %       |            \
  %
  [A,B] = S.getAB();
  cA = 0;
  cB = 0;

  if dot(self.Nc,A-self.Pa) > 0; cA = cA + 1; end
  if dot(self.Na,A-self.Pb) > 0; cA = cA + 2; end
  if dot(self.Nb,A-self.Pc) > 0; cA = cA + 4; end

  if dot(self.Nc,B-self.Pa) > 0; cB = cB + 1; end
  if dot(self.Na,B-self.Pb) > 0; cB = cB + 2; end
  if dot(self.Nb,B-self.Pc) > 0; cB = cB + 4; end

  if (cA == 0) && (cB == 0)
    % internal segment
    ok = -1;
    return;
  end

  if (cA == 0) || (cB == 0)
    % intersect
    switch cA+cB
    case 1
      [s,~,ok] = S.intersect( self.get_segment_AB() );
      nseg     = 1;
    case 2
      [s,~,ok] = S.intersect( self.get_segment_BC() );
      nseg     = 2;
    case 3
      [s,~,ok] = S.intersect( self.get_segment_AB() );
      nseg     = 1;
      if s < 0 || s > 1
        [s,~,ok] = S.intersect( self.get_segment_BC() );
        nseg     = 2;
      end
    case 4
      [s,~,ok] = S.intersect( self.get_segment_CA() );
      nseg     = 3;
    case 5
      [s,~,ok] = S.intersect( self.get_segment_AB() );
      nseg     = 1;
      if s < 0 || s > 1
        [s,~,ok] = S.intersect( self.get_segment_CA() );
        nseg     = 3;
      end
    case 6
      [s,~,ok] = S.intersect( self.get_segment_BC() );
      nseg     = 2;
      if ~ok
        [s,~,ok] = S.intersect( self.get_segment_CA() );
        nseg     = 3;
      end
    otherwise
      error('error');
    end
    ok = 1;
    return;
  end

  % discard sure non-intersection
  AB = bitand(cA,cB);
  if AB == 1 || AB == 2 || AB == 4
    % no intersection
    return;
  end

  AB = bitor(cA,cB);
  if bitand(AB,4) == 0
    [s1,~,ok] = S.intersect( self.get_segment_AB() );
    nseg1     = 1;
    if ok
      [s2,~,ok] = S.intersect( self.get_segment_BC() );
      nseg2     = 2;
    end
  elseif bitand(AB,2) == 0
    [s1,~,ok] = S.intersect( self.get_segment_AB() );
    nseg1     = 1;
    if ok
      [s2,~,ok] = S.intersect( self.get_segment_CA() );
      nseg2     = 2;
    end
  elseif bitand(AB,1) == 0
    [s1,~,ok] = S.intersect( self.get_segment_BC() );
    nseg1     = 1;
    if ok
      [s2,~,ok] = S.intersect( self.get_segment_CA() );
      nseg2     = 2;
    end
  else
    [s1,~,ok1] = S.intersect( self.get_segment_AB() );
    [s2,~,ok2] = S.intersect( self.get_segment_BC() );
    [s3,~,ok3] = S.intersect( self.get_segment_CA() );
    s    = [];
    nseg = [];
    ok   = 0;
    if ok1
      s    = s1;
      nseg = 1;
      ok   = 1;
    end
    if ok2
      s    = [s,s2];
      nseg = [nseg,2];
      ok   = ok+1;
    end
    if ok3
      s    = [s,s3];
      nseg = [nseg,3];
      ok   = ok+1;
    end
    return;
  end
  if ok
    s    = [s1,s2];
    nseg = [nseg1,nseg2];
    ok   = 2;
  else
    ok   = 0;
  end
end
