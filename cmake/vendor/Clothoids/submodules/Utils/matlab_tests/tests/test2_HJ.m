function test2_HJ
  xx = -10:0.1:110;
  yy = -10:0.1:110;
  [X,Y] = meshgrid(xx,yy);
  Z = test_fun2(X,Y)+test_fun2_barrier(X,Y);
  surf(X,Y,Z);
end
