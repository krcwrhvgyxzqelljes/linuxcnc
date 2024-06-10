function test4
%-----------------------------------------------------------
    fprintf(1,[...
'   AABBTREE offers d-dimensional aabb-tree construction &\n',...
'   search for collections of spatial objects. These trees\n',...
'   are useful when seeking to implement efficient spatial\n',...
'   queries -- determining intersections between collecti-\n',...
'   ons of spatial objects. \n\n',...
'   Given a collection of spatial objects, an aabb-tree p-\n',...
'   artitions the bounding-boxes of the elements in the c-\n',...
'   ollection (the aabb''s) into a "tree" (hierarchy) of  \n',...
'   rectangular "nodes". In contrast to other types of ge-\n',...
'   ometric trees (quadtrees, kd-trees, etc) the nodes in \n',...
'   an aabb-tree enclose aabb''s -- not points -- and may \n',...
'   overlap as a result. Objects in the collection are co-\n',...
'   ntained in a single node only. \n\n']);

    filename = mfilename('fullpath');
    filepath = fileparts( filename );

    addpath([filepath,'/mesh-file']);

    [geom] = loadmsh([filepath,'/test-data/airfoil.msh']);

    pp = geom.point.coord(:,1:2);
    tt = geom.tria3.index(:,1:3);

    bi = pp(tt(:,1),:); bj = pp(tt(:,1),:);
    for ii = 2 : size(tt,2)
      bi = min(bi,pp(tt(:,ii),:));
      bj = max(bj,pp(tt(:,ii),:));
    end

    aabb = AABBtree();

    aabb.build(bi,bj);

    fc = [.95,.95,.55];
    ec = [.25,.25,.25];

    figure;
    subplot(1,2,1); hold on;
    patch('faces',tt,'vertices',pp,'facecolor',fc,...
        'edgecolor',ec,'facealpha',+.3);
    axis image off;
    set(gca,'units','normalized','position',[0.01,0.05,.48,.90]);
    subplot(1,2,2); hold on;

    aabb.plot();

    axis image off;
    set(gca,'units','normalized','position',[0.51,0.05,.48,.90]);

end
