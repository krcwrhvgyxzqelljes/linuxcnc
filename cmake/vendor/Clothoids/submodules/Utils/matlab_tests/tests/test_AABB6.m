%=========================================================================%
%                                                                         %
%  Autors: Enrico Bertolazzi                                              %
%          Department of Industrial Engineering                           %
%          University of Trento                                           %
%          enrico.bertolazzi@unitn.it                                     %
%                                                                         %
%=========================================================================%

close all;

rng(1234);

addpath('../lib');

NN      = 200;
aabbnew = true;

% check constructors
fprintf('Generate lines\n');
SEGS = {};
for k=1:NN
  if k==1
    x     = 10*rand;
    y     = 10*rand;
    theta = 2*pi*rand;
  else
    x     = mod( Pb(1) + rand/4, 10 );
    y     = mod( Pb(2) + rand/4, 10 );
    theta = theta+pi/10*rand;
  end
  len      = 0.01+2*rand;
  Pa       = [x;y];
  Pb       = Pa + len*[cos(theta);sin(theta)];
  SEGS{k} = Segment(Pa,Pb);
end

subplot(1,2,1);
hold on;
bb_max1 = zeros(NN,2);
bb_min1 = zeros(NN,2);
for k=1:NN
  SEGS{k}.plot( '-r', 'LineWidth', 1 );
  B1 = SEGS{k}.bbox(k);
  bb_min1(k,:) = B1.get_min().';
  bb_max1(k,:) = B1.get_max().';
end

nobj = 2;
long = 0.8;
vtol = 0.55;

if aabbnew
  tr1 = AABB_tree(nobj,long,vtol);
else
  tr1 = AABBtree(nobj,long,vtol);
end

tr1.build(bb_min1,bb_max1,true);
tr1.plot('red', 'black');
tr1.info

pnt1 = [2,3];
plot( pnt1(1), pnt1(2), 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'blue');

pnt2 = [6,12];
plot( pnt2(1), pnt2(2), 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'green');

%xlim([0,20]);
%axis equal
axis equal

%
% trovo candidati minima distanza
%

subplot(1,2,2);

hold on;

for k=1:NN
  SEGS{k}.plot( '-r', 'LineWidth', 1 );
end

id_list = tr1.min_distance_candidates( pnt1 );
id_list = sort(id_list)
tr1.plot_bbox( bb_min1(id_list,:), bb_max1(id_list,:), 'blue', 'black' );
for kk=id_list.'
  SEGS{kk}.plot( '-b', 'LineWidth', 3 );
end

id_list = tr1.min_distance_candidates( pnt2 );
id_list = sort(id_list)
tr1.plot_bbox( bb_min1(id_list,:), bb_max1(id_list,:), 'green', 'black' );
for kk=id_list.'
  SEGS{kk}.plot( '-k', 'LineWidth', 3 );
end

pnt1 = [2,3];
plot( pnt1(1), pnt1(2), 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'blue');

pnt2 = [6,12];
plot( pnt2(1), pnt2(2), 'o', 'MarkerSize', 10, 'MarkerFaceColor', 'green');

axis equal
