%=========================================================================%
%                                                                         %
%  Autors: Enrico Bertolazzi                                              %
%          Department of Industrial Engineering                           %
%          University of Trento                                           %
%          enrico.bertolazzi@unitn.it                                     %
%          m.fregox@gmail.com                                             %
%                                                                         %
%=========================================================================%
% Driver test program to check Clothoids lib                              %
%=========================================================================%

DB = Dubins();
hold on;

%tiledlayout(2,2);%, 'Padding', 'none', 'TileSpacing', 'compact'); 
kk = 0;
DY = 4;
%for k_max=[1,2,6,20]
for k_max=[0.25,2]
%for k_max=[4,6]

  offs = [0, 0, 0 ];

  kk = kk+1;
  subplot(1,2,kk);

  fprintf('k = %d\n',k_max);

  %nexttile
  
  x0     = 0;
  y0     = 0;
  theta0 = 0;
  x3     = 1;
  y3     = 0;
  theta3 = 0;
  r_min  = 1/k_max;

  dubConnObj = dubinsConnection('MinTurningRadius',r_min);

  DB.build( x0, y0, theta0, x3, y3, theta3, k_max );
  DB.plot();

  startPose = [x0 y0 theta0]+offs;
  goalPose  = [x3 y3 theta3]+offs;
  [pathSegObj, pathCosts] = connect(dubConnObj,startPose,goalPose);
  show(pathSegObj{1})
 
  x0     = 0;
  y0     = DY;
  x3     = x0+1;
  y3     = y0;
  theta0 = -pi/2;
  theta3 = pi/2;
  DB.build( x0, y0, theta0, x3, y3, theta3, k_max );

  startPose = [x0 y0 theta0]+offs;
  goalPose  = [x3 y3 theta3]+offs;
  [pathSegObj, pathCosts] = connect(dubConnObj,startPose,goalPose);
  show(pathSegObj{1})

  DB.plot();


  x0     = 0;
  y0     = 2*DY;
  x3     = x0+1;
  y3     = y0;
  theta0 = pi/2;
  theta3 = -pi/2;
  DB.build( x0, y0, theta0, x3, y3, theta3, k_max );

  startPose = [x0 y0 theta0]+offs;
  goalPose  = [x3 y3 theta3]+offs;
  [pathSegObj, pathCosts] = connect(dubConnObj,startPose,goalPose);
  show(pathSegObj{1})

  DB.plot();

  x0     = 0;
  y0     = 3*DY;
  x3     = x0+1;
  y3     = y0;
  theta0 = pi/2;
  theta3 = pi/2;
  DB.build( x0, y0, theta0, x3, y3, theta3, k_max );

  startPose = [x0 y0 theta0]+offs;
  goalPose  = [x3 y3 theta3]+offs;
  [pathSegObj, pathCosts] = connect(dubConnObj,startPose,goalPose);
  show(pathSegObj{1})

  DB.plot();

  x0     = 0;
  y0     = 4*DY;
  x3     = x0+1;
  y3     = y0;
  theta0 = -pi/2;
  theta3 = -pi/2;
  DB.build( x0, y0, theta0, x3, y3, theta3, k_max );

  startPose = [x0 y0 theta0]+offs;
  goalPose  = [x3 y3 theta3]+offs;
  [pathSegObj, pathCosts] = connect(dubConnObj,startPose,goalPose);
  show(pathSegObj{1})

  DB.plot();

  x0     = 0;
  y0     = 5*DY;
  x3     = x0+1;
  y3     = y0;
  theta0 = -pi/2;
  theta3 = 0;
  DB.build( x0, y0, theta0, x3, y3, theta3, k_max );

  startPose = [x0 y0 theta0]+offs;
  goalPose  = [x3 y3 theta3]+offs;
  [pathSegObj, pathCosts] = connect(dubConnObj,startPose,goalPose);
  show(pathSegObj{1})
  hold on;

  DB.plot();

  axis equal;

end
