function ok = bbox_overlap( self, A_min, A_max, B_min, B_max )
  % check intersetion
  ok = false(size(A_min,1),1);
  for k=1:size(A_min,2)
    ok = ok | (A_max(:,k) < B_min(k));
    ok = ok | (B_max(:,k) < A_min(k));
  end
  ok = ~ok;
end
