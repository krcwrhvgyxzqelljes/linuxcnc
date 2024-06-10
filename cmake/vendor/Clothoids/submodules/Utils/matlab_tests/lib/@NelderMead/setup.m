% =================================================================
% Setup
% =================================================================
function setup( self, fun, dim, varargin )

  self.m_dim = dim;

  % Initialize parameters and class attributes (properties)
  if isa(fun,'function_handle')
    self.m_fun = fun; % store function handle
  else
    error('Provided function is not a function handle')
  end

  if nargin > 3
    self.m_plot = varargin{1};
  end
end
