function [pmin,pmax] = pnt_bbox_minmax( self, pnt, bb_min, bb_max )
  % check overlap
  idx1 = pnt > bb_max;
  idx2 = pnt < bb_min;
  pmin = pnt;
  pmin(idx1) = bb_max(idx1);
  pmin(idx2) = bb_min(idx2);
  pmax = pnt;
  pmax(~idx1) = bb_max(~idx1);
  pmax(~idx2) = bb_min(~idx2);
end
