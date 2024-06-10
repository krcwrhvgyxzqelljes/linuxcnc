function res = test_fun2_barrier(x,y)
  res = zeros(size(x));
  idx = find( x > 100 );
  res(idx) = Inf;
  idx = find( x < -0.01 );
  res(idx) = Inf;
  idx = find( y > 101.01 );
  res(idx) = Inf;
  idx = find( y < -0.01 );
  res(idx) = Inf;
  for v=[5,19,33,47,61,75,89]
    idx = find( abs(x)+abs(y-v).^(7/2) < 99.9 );
    res(idx) = Inf;
  end
  for v=[12,26,40,54,68,82,96]
    idx = find( abs(x-100)+abs(y-v).^3 < 99.9 );
    res(idx) = Inf;
  end
end
