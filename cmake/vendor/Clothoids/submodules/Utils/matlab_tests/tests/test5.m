function test5
%-----------------------------------------------------------
    fprintf(1,[...
'   AABBTREE is a d-dimensional library, storing objects  \n',...
'   and performing search operations in R^d. AABBTREE sim-\n',...
'   ply requires an description of the d-dimensional boun-\n',...
'   ding-boxes of a given collection. It is not limited to\n',...
'   simplexes (triangles, tetrahedrons, etc).\n\n']);

  filename = mfilename('fullpath');
  filepath = fileparts( filename );

  addpath([filepath,'/mesh-file']);

  [geom] = loadmsh([filepath,'/test-data/veins.msh']);

  pp = geom.point.coord(:,1:3);
  tt = geom.tria3.index(:,1:3);

  bi = pp(tt(:,1),:);
  bj = pp(tt(:,1),:);
  for ii = 2 : size(tt,2)
    bi = min(bi,pp(tt(:,ii),:));
    bj = max(bj,pp(tt(:,ii),:));
  end

  tr = AABBtree(32,0.75,0.67);
  tr.build(bi,bj);

  fc = [.95,.95,.55];
  ec = [.25,.25,.25];

  figure;
  subplot(1,2,1); hold on;
  patch('faces',tt,'vertices',pp,'facecolor',fc,'edgecolor',ec,'facealpha',+1.);
  axis image off;
  set(gca,'units','normalized','position',[0.01,0.05,.48,.90]);
  view(80,15);
  light; camlight;
  subplot(1,2,2); hold on;
  tr.plot();
  axis image off;
  view(80,15);
  light; camlight;
  set(gca,'units','normalized','position',[0.51,0.05,.48,.90]);
end
