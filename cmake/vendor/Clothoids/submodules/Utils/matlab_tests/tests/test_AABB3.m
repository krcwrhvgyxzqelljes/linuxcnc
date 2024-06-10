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

NN = 100;
MM = 100;

% check constructors
fprintf('Generate lines\n');
SEGS1 = {};
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
  SEGS1{k} = Segment2D(Pa,Pb);
  %SEGS1{k} = Segment(Pa,Pb);
end

SEGS2 = {};
OX    = 4;
OY    = 3;
for k=1:MM
  if k==1
    x     = OX+10*rand;
    y     = OY+10*rand;
    theta = 2*pi*rand;
  else
    x     = mod( Pb(1) + rand/4 - OX, 10 )+ OX;
    y     = mod( Pb(2) + rand/4 - OY, 10 )+ OY;
    theta = theta+pi/10*rand;
  end
  len      = 0.01+2*rand;
  Pa       = [x;y];
  Pb       = Pa + len*[cos(theta);sin(theta)];
  SEGS2{k} = Segment2D(Pa,Pb);
  %SEGS2{k} = Segment(Pa,Pb);
end

subplot(2,2,1);
hold on;
bb_max1 = zeros(NN,2);
bb_min1 = zeros(NN,2);
for k=1:NN
  SEGS1{k}.plot( '-r', 'LineWidth', 1 );
  B1 = SEGS1{k}.bbox(k);
  bb_min1(k,:) = B1.get_min().';
  bb_max1(k,:) = B1.get_max().';
end

nobj = 2;
long = 0.8;
vtol = 0.55;

tr1 = AABB_tree(nobj,long,vtol);
tr1.build( bb_min1, bb_max1, true );
tr1.plot_bbox( bb_min1, bb_max1, 'red', 'black' );

tr1.plot('red', 'black');
tr1.info
%xlim([0,20]);
%axis equal

%subplot(2,2,2);
%hold on;
bb_max2 = zeros(MM,2);
bb_min2 = zeros(MM,2);
for k=1:MM
  SEGS2{k}.plot( '-b', 'LineWidth', 1 );
  B2 = SEGS2{k}.bbox(k);
  bb_min2(k,:) = B2.get_min().';
  bb_max2(k,:) = B2.get_max().';
end

tr2 = AABB_tree(nobj,long,vtol);
tr2.build( bb_min2, bb_max2, true );
tr2.plot('blue', 'black');
tr2.info
%xlim([0,15]);
axis equal

%
% trovo candidati intersezioni
%

subplot(2,2,2);
hold on;

for ii=1:size(bb_min2,1)
  ok_list = tr1.intersect_with_one_bbox( bb_min2(ii,:), bb_max2(ii,:) );
  if ~isempty(ok_list)
    tr1.plot_bbox( bb_min2(ii,:),      bb_max2(ii,:),      'cyan',  'black' );
    tr1.plot_bbox( bb_min1(ok_list,:), bb_max1(ok_list,:), 'green', 'white' );
  end
end
for k=1:NN
  SEGS1{k}.plot( '-r', 'LineWidth', 1 );
end
for k=1:MM
  SEGS2{k}.plot( '-b', 'LineWidth', 1 );
end

axis equal

subplot(2,2,3);
hold on;

if false
  id_list = tr1.intersect(tr2);
  idx_all = false(size(bb_min2,1),1);
  for k=1:length(id_list)
    idx = id_list{k};
    if ~isempty(idx)
      idx_all(idx) = true;
      id_list2 = tr1.get_bbox_indexes_of_a_node( k );
      for kk=id_list2
        tr1.plot_bbox( bb_min1(kk,:), bb_max1(kk,:), 'red', 'black' );
      end
    end
  end
  tr2.plot_bbox( bb_min2(idx_all,:), bb_max2(idx_all,:), 'blue', 'black' );
else
  id_list = tr1.intersect_and_refine( tr2 );
  idx_all = false(size(bb_min2,1),1);
  for k=1:length(id_list)
    idx = id_list{k};
    if ~isempty(idx)
      idx_all(idx) = true;
      tr1.plot_bbox( bb_min1(k,:), bb_max1(k,:), 'red', 'black' );
    end
  end
  tr2.plot_bbox( bb_min2(idx_all,:), bb_max2(idx_all,:), 'blue', 'black' );

end

for k=1:NN
  SEGS1{k}.plot( '-r', 'LineWidth', 1 );
end
for k=1:MM
  SEGS2{k}.plot( '-b', 'LineWidth', 1 );
end

%xlim([0,15]);
axis equal

subplot(2,2,4);
hold on;

for k=1:NN
  SEGS1{k}.plot( '-r', 'LineWidth', 1 );
end
for k=1:MM
  SEGS2{k}.plot( '-b', 'LineWidth', 1 );
end

for k=1:length(id_list)
  idx = id_list{k};
  if length(idx)>0
    %SEGS1{k}.plot( '-r', 'LineWidth', 2 );
    for j=1:length(idx)
      id = idx(j);
      %SEGS2{id}.plot( '-b', 'LineWidth', 2 );
      [s,t,ok] = SEGS1{k}.intersect( SEGS2{id} );
      if ok
        P = SEGS1{k}.eval(s);
        plot( P(1), P(2), 'o', 'MarkerFaceColor', 'green', 'MarkerSize', 5 );
      end
    end
  end
end

%xlim([0,15]);
axis equal

