%=========================================================================%
%                                                                         %
%  Autors: Enrico Bertolazzi                                              %
%          Department of Industrial Engineering                           %
%          University of Trento                                           %
%          enrico.bertolazzi@unitn.it                                     %
%                                                                         %
%=========================================================================%

close all;

addpath('../lib');

NN = 400;

% check constructors
fprintf('Generate lines\n');
SEGS = {};
for k=1:NN
  x     = 10*rand;
  y     = 10*rand;
  theta = 2*pi*rand;
  len   = 0.1+3.9*rand;
  Pa    = [x;y];
  Pb    = Pa + len*[cos(theta);sin(theta)];
  SEGS{k} = Segment(Pa,Pb);
end

hold on;

%fprintf('Plot lines\n');
%hold on;
%for k=1:1000
%  SEGS{k}.plot();
%end

fprintf('Intersect\n');
S = Segment([10*rand;10*rand], [10*rand;10*rand]);
S.plot( 'o-r', 'LineWidth', 3, 'MarkerFaceColor', 'red' );
for k=1:NN
  [s,t,ok] = S.intersect( SEGS{k} );
  if ok
    PP = S.eval(s);
    plot( PP(1), PP(2), 'o-r', 'MarkerFaceColor', 'blue', 'MarkerSize', 10 );
    SEGS{k}.plot( '-k', 'LineWidth', 2 );
  else
    SEGS{k}.plot( '-g', 'LineWidth', 1 );
  end
  %SEGS{k} = Segment(Pa,Pb);
end
