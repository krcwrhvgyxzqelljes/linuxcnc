function res = test_fun1_barrier(x,y)
  res = zeros(size(x));
                  idx = find( x > -1 );
  res(idx) = Inf; idx = find( x < -17.001 );
  res(idx) = Inf; idx = find( y > -1 );
  res(idx) = Inf; idx = find( y < -x./3-28 );
  res(idx) = Inf; idx = find( (y+20).^2-3*x < 51 );
  res(idx) = Inf; idx = find( abs(x+14.5)+(y+15).^2 < 3 );
  res(idx) = Inf; idx = find( (x+16).^2+abs(y+8).^(3/2) < 20 );
  res(idx) = Inf; idx = find( (x+9.2).^2+abs(y+12) < 7 );
  res(idx) = Inf; idx = find( (x+6).^2+(y+15).^2 < 29.8 );
  res(idx) = Inf; idx = find( (x+6).^2+abs(y+1).^(3/2) < 15 );
  res(idx) = Inf;
end
