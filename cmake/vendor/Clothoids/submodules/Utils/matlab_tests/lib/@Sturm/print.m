function print( self, check )
  fprintf('%s\n',self.to_string);
  if nargin > 1 && check
    fprintf('check roots...\n');
    P = self.m_sturm{1};
    for r=self.m_roots
      fprintf( 'P(%g) = %g\n', r, P.eval(r) );
    end
    fprintf('...done\n');
  end
end
