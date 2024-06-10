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

NN = 200;

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

TRI = Triangle( 10*rand(2,1),  10*rand(2,1), 10*rand(2,1) );

TRI.plot();
hold on;

bb = TRI.bbox(1);
bb.plot();

fprintf('Intersect\n');
for k=1:NN
  ok = TRI.collide_with_segment( SEGS{k} );
  if ok == 1
    SEGS{k}.plot( '-k', 'LineWidth', 2 );
  elseif ok == 2
    SEGS{k}.plot( '-b', 'LineWidth', 1 );
  elseif ok < 0
    SEGS{k}.plot( '-r', 'LineWidth', 2 );
  else
    SEGS{k}.plot( '-g', 'LineWidth', 1 );
  end
end
