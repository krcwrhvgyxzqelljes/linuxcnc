close all

nc = 10000;
np = 50000;

%nc = 10000*4;
%np = 50000*4;

DL = DiskList();
DL.random( nc, 0.02 );
pnts = rand(2,np);

fprintf(1,'   "Slow" algorithm: \n');

start = tic;
id0_list = cell(np,1);
for k=1:np
  id0_list{k} = DL.intersect(pnts(:,k));
end
toc(start)

fprintf(1,'   "Fast" algorithm: \n');

start = tic;

tic

[bb_min,bb_max] = DL.get_bboxes();

%-- compute aabb-tree for circles
%tr = AABB_tree();
tr = AABBtree();
tr.set_max_num_objects_per_node(2);
tr.build(bb_min.',bb_max.');
%tr.info

%-- compute aabb-tree for points
%tr2 = AABB_tree();
tr2 = AABBtree();
tr2.build(pnts.',pnts.');
%tr2.info

toc

%id2_list = tr.intersect_and_refine( tr2, bb_min.', bb_max.', pnts.', pnts.' );
%id2_list = tr2.intersect_and_refine( tr, pnts.', pnts.' , bb_min.', bb_max.');

tic
id1_list = tr2.intersect( tr );
toc

tic
id_list  = cell(np,1);
for k=1:length(id1_list)
  idk = double(id1_list{k});
  if ~isempty(idk)
    idx = double(tr2.get_bbox_indexes_of_a_node(k));
    for kkk=1:length(idx)
      id_list{idx(kkk)} = [id_list{idx(kkk)} idk.'];
    end
  end
end

id3_list = cell(np,1);
for k=1:length(id_list)
  idx = id_list{k};
  if ~isempty(idx)
    idx2 = DL.intersect2(pnts(:,k),idx);
    id3_list{k} = idx(idx2);
  end
end
toc

toc( start )

fprintf(1,'   Equivalent results? \n');
fprintf('n.slow = %d n.fast = %d\n',length(id0_list),length(id3_list));

if size(id0_list) == size(id3_list)
  same = true ;
  for ii=1:length(id0_list)
    c1 = id0_list{ii};
    c2 = sort(id3_list{ii});
    if length(c1) == length(c2)
      if any(c1 ~= c2)
        same = false;
        break;
      end
    else
     fprintf('n.c1 = %d n.c2 = %d\n',length(c1),length(c2));
     same = false;
     break;
    end
  end
else
  same = false;
end
if same
  fprintf(1,'   TRUE \n');
else
  fprintf(1,'  FALSE \n');
end

figure;
nstep = 10;

subplot(1,2,1);
hold on;
DL.plot(10);
plot(pnts(1,1:nstep:end),pnts(2,1:nstep:end),'r.');

axis image off;
set(gca,'units','normalized','position',[0.01,0.05,.48,.90]);

subplot(1,2,2);
hold on;
tr.plot();
axis image off;
set(gca,'units','normalized','position',[0.51,0.05,.48,.90]);
