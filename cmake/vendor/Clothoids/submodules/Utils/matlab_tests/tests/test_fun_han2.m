function res = test_fun_han2(x,y)
  res = x.^2;
  I1 = find( y > 1  );
  I2 = find( y < -1 );
  res(I1) = res(I1)+y(I1)-1;
  res(I2) = res(I2)-y(I2)-1;
end
