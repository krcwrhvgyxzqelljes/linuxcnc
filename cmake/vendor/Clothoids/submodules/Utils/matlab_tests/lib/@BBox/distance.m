function dst = distance( self, P )
  %
  %
  %  6          7          8
  %      +-------------+
  %      |             |
  %  3   |      4      |   5
  %      |             |
  %      +-------------+
  %  0          1          2
  %
  %
  x = P(1);
  y = P(2);
  icase = 4;
  if x < m_xmin
    icase = 3;
  elseif x > m_xmax
    icase = 5;
  end
  if y < m_ymin
    icase = icase - 3;
  elseif y > m_ymax
    icase = icase + 3;
  end
  dst = 0;
  switch icase
    case 0; dst = hypot( x-m_xmin, y-m_ymin );
    case 1; dst = m_ymin-y;
    case 2; dst = hypot( x-m_xmax, y-m_ymin );
    case 3; dst = m_xmin-x;
    case 4;
    case 5; dst = x-m_xmax;
    case 6; dst = hypot( x-m_xmin, y-m_ymax );
    case 7; dst = y-m_ymax;
    case 8; dst = hypot( x-m_xmax, y-m_ymax );
  end
end
