% Compute all the real roots in the interval [a,b] with tolerance epsi
%
function roots = real_roots_in_the_interval( self, a, b )
  S = Sturm();
  S.build( self );
  S.separate_roots( a, b );
  S.refine_roots();
  roots = S.roots();
end
