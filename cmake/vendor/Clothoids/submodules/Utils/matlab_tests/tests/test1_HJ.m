function test1_HJ
  global haxhax;

  close all;

  addpath('../lib');
  haxhax=axes;

  with_bound = true;

  xx = -20:0.1:1;
  yy = -30:0.1:1;
  [X,Y] = meshgrid(xx,yy);
  if with_bound
    Z = test_fun1(X,Y)+test_fun1_barrier(X,Y);
  else
    Z = test_fun1(X,Y);
  end
  %surf(X,Y,Z);
  hold off;
  contour(haxhax,X,Y,Z,200);
  hold on
  contourf(haxhax,X,Y,Z,'LevelList',[1000]);
  axis equal

  X0 = [-1.1;-27];

  conv_tolerance = 1e-8;

  solver = HJPatternSearch();
  
  if with_bound
    myfun = @(x) test_fun1(x(1),x(2))+test_fun1_barrier(x(1),x(2));
  else
    myfun = @(x) test_fun1(x(1),x(2));
  end
  solver.setup( myfun, 2 );
  solver.set_tolerance( 1e-150 );

  tic
  x_sol = solver.run( X0.', 1 );
  mysolver_time = toc;

  %tic
  %x_sol = HJSolver.run( X0, 1 );
  %mysolver_time = toc;

  %HJSolver.print_info(conv_tolerance);

  %options = optimset('PlotFcns',@iter_plot2);
  %x = fminsearch(myfun,X0,options)

  %options = optimoptions('patternsearch','PlotFcns',@iter_plot3,'SearchFcn','searchga');
  %x = patternsearch(myfun,X0,[],[],[],[],lb,ub,[],options)

  %[lb.',ub.']
%
  %P.f = @funzione;
  %opts.maxevals   = 10000;
  %opts.maxits     = 200;
  %opts.maxdeep    = 100;
  %opts.testflag   = 0;
  %opts.globalmin  = 0;
  %opts.globalxmin = 0;
  %opts.dimension  = 2;
  %opts.showits    = 1;
  %opts.ep         = 0.5;
  %opts.tol        = 1e-8;
  %%[minval,xatmin,hist] = dDirect_GL(P,opts,[lb.',ub.']);
  %[minval,xatmin,hist] = dDirect_L(P,opts,[lb.',ub.']);
  %[minval,xatmin,hist] = dBIRMIN(P,opts,[lb.',ub.']);

end

function iter_plot( X0, X1, H )
  plot( [X0(1),X1(1)], [X0(2),X1(2)], '-', 'Color', 'black', 'LineWidth', 2 );
  plot( X1(1), X1(2), 'o', 'Color', 'green', 'MarkerFaceColor', 'blue' );
  %input('step');
end

function stop = iter_plot2( x, optimValues, state )
  global haxhax;
  plot( haxhax, x(1), x(2), 'o', 'Color', 'green', 'MarkerFaceColor', 'blue' );
  stop = false;
end

function stop = iter_plot3( x_in, state )
  global haxhax;
  xx = x_in.x(1);
  yy = x_in.x(2);
  plot( haxhax, xx, yy, 'o', 'Color', 'green', 'MarkerFaceColor', 'blue' );
  stop = false;
end

function res = funzione( x_in )
  global haxhax;
  X = x_in(1);
  Y = x_in(2);
  plot( haxhax, X, Y, 'o', 'Color', 'green', 'MarkerFaceColor', 'blue' );
  res = test_fun1(X,Y)+test_fun1_barrier(X,Y);
end
