function res = test_fun0(x,y)
  res = zeros(size(x));
  I1 = find( x <= 0 );
  I2 = find( x  > 0 );
  res(I1) = 2400.*abs(x(I1)).^3+y(I1)+y(I1).^2;
  res(I2) = 6*x(I2).^3+y(I2)+y(I2).^2;
end
