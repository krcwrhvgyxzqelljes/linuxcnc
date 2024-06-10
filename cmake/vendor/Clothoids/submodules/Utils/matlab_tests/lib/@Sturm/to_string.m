% Write the Sturm sequence and interval of separation in human readable way
function out = to_string( self )
  out = sprintf('Sturm sequence\n--------------\n');
  for i=1:length(self.m_sturm)
    out = sprintf( ...
      '%sP_%d(x) = %s\n', ...
      out, i-1, self.m_sturm{i}.to_string() ...
    );
  end
  n = length(self.m_intervals);
  if n > 0
    out = sprintf( ...
      '%s\nRoots separation for interval [%g,%g]\n', ...
      out, self.m_a, self.m_b ...
    );
    for i=1:n
      I = self.m_intervals{i};
      out = sprintf( ...
        '%sI%d = [%10g,%10g], V = [%2d,%2d]\n', ...
        out, i, I.a, I.b, I.va, I.vb ...
      );
    end
  end
  n = length(self.m_roots);
  if n > 0
    for i=1:n
      out = sprintf( '%sroot N.%d = %g\n', out, i, self.m_roots(i) );
    end
  end
end
