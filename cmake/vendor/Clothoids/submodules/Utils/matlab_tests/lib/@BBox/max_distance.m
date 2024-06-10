function dst = max_distance( self, P )
  x   = P(1);
  y   = P(2);
  dx  = max( abs(x-m_xmin), abs(x-m_xmax) );
  dy  = max( abs(y-m_ymin), abs(y-m_ymax) );
  dst = hypot(dx,dy);
end
