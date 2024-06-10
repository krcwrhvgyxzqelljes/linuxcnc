classdef DiskList < matlab.mixin.Copyable

  properties (SetAccess = private, Hidden = true)
    m_center;
    m_ray;
  end
  methods(Access = protected)
    % Override copyElement method:
    function obj = copyElement( self )
      obj = copyElement@matlab.mixin.Copyable(self);
      obj.objectHandle = TestClassMexWrapper( 'copy', self.objectHandle );
    end
  end
  methods
    function self = DiskList()
      self.m_center = zeros(2,0);
      self.m_ray    = zeros(1,0);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function str = is_type( ~ )
      str = 'DiskList';
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function random( self, nc, rr )
      % Set nc randomised circles in R^2 with mean radius rr.
      self.m_center = rand(2,nc);
      self.m_ray    = rr*rand(1,nc);
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [bb_min,bb_max] = get_bboxes( self )
      bb_min = self.m_center - [1;1] * self.m_ray;
      bb_max = self.m_center + [1;1] * self.m_ray;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = intersect( self, pnt )
      dst2 = [1,1] * (self.m_center - pnt * ones(1,size(self.m_center,2))).^2;
      res  = find( dst2 <= self.m_ray.^2 );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = intersect2( self, pnt, idx )
      dst2 = [1,1] * (self.m_center(:,idx) - pnt * ones(1,length(idx))).^2;
      res  = find( dst2 <= self.m_ray(idx).^2 );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function plot( self, nstep )
      N  = size(self.m_center,2);
      fc = [.95,.95,.55];
      ec = [.25,.25,.25];
      % circles
      tt = linspace(0,2*pi,24);
      xx = cos(tt)';
      yy = sin(tt)';
      nt = length(tt);
      ee = [(1:nt-1)',(2:nt-0)'];

      xc = cell(N,1);
      yc = cell(N,1);
      jc = cell(N,1);

      kkk = 0;
      for ic=1:nstep:N
        xc{ic} = self.m_center(1,ic) + self.m_ray(ic)*xx;
        yc{ic} = self.m_center(2,ic) + self.m_ray(ic)*yy;
        jc{ic} = ee + kkk*nt;
        kkk = kkk+1;
      end
      pp = [vertcat(xc{:}), vertcat(yc{:})];
      ff = vertcat(jc{:});
      patch( ...
        'faces',     ff, ...
        'vertices',  pp, ...
        'facecolor', fc, ...
        'edgecolor', ec, ...
        'facealpha', 0.3 ...
      );
    end
  end
end
