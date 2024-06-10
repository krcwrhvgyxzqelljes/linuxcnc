function PP = eval( self, s, varargin )
  DD = self.Pb - self.Pa;
  PP = self.Pa + s*DD;
  if nargin > 2
    NN = [DD(2);-DD(1)]./norm(DD);
    PP = PP + varargin{1}*NN;
  end
end
