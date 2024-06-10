% Given a Sturm sequence separate the real roots in the interval [a,b]
function n_roots = separate_roots( self, a, b )
  self.m_a = a;
  self.m_b = b;
  [va,m_root_a] = self.sign_variations(a);
  [vb,m_root_b] = self.sign_variations(b);
  while m_root_a
    % on root, move interval a
    a = a - 1e-8*(b-a);
    [va,m_root_a] = self.sign_variations(a);
  end
  while m_root_b
    % on root, move interval a
    b = b + 1e-8*(b-a);
    [vb,m_root_b] = self.sign_variations(b);
  end
  n_roots = abs( va - vb );

  if n_roots == 0; return; end

  self.m_intervals = cell(1,n_roots);
  I0.a = a; I0.va = va;
  I0.b = b; I0.vb = vb;
  self.m_intervals{1} = I0;

  if n_roots == 1; return; end

  % search intervals
  i_pos = 1;
  n_seg = 1;

  while i_pos <= n_roots
    I0 = self.m_intervals{i_pos};
    if (I0.b-I0.a) < eps*max(abs(I0.a),abs(I0.b))
      % intervallo troppo piccolo passo al successivo
      i_pos   = i_pos + 1;
      n_roots = n_roots - abs(I0.vb-I0.va)+1;
      continue;
    end
    % refine segment
    c = (I0.a+I0.b)/2;
    [vc,vroot] = self.sign_variations(c);
    if vroot
      for iter=2:10
        c = (I0.a*iter+I0.b)/(1+iter);
        [vc,vroot] = self.sign_variations(c);
        if vroot
          c = (I0.a+I0.b*iter)/(1+iter);
          [vc,vroot] = self.sign_variations(c);
        end
        if ~vroot; break; end
      end
    end
    if vroot
      error('separate_roots failed\n');
    end
    if I0.va == vc % LO <- c
      I0.a  = c;
      I0.va = vc;
      self.m_intervals{i_pos} = I0;
    elseif I0.vb == vc % HI <- c
      I0.b  = c;
      I0.vb = vc;
      self.m_intervals{i_pos} = I0;
    else % split interval!
      % second interval
      n_seg = n_seg+1;
      I1.a = c;    I1.va = vc;
      I1.b = I0.b; I1.vb = I0.vb;
      self.m_intervals{n_seg} = I1;

      % first interval
      I0.b = c; I0.vb = vc;
      self.m_intervals{i_pos} = I0;

      % skip interval with sign variation == 1
      while i_pos <= n_seg
        I2 = self.m_intervals{i_pos};
        if abs( I2.vb - I2.va ) > 1
          break; % found interval to be analysed
        end
        i_pos = i_pos + 1;
      end
    end
  end
  self.m_intervals = self.m_intervals(1:n_roots);
  % sort intervals
  [~, order]       = sort(squeeze([cell2mat(self.m_intervals).a]));
  self.m_intervals = self.m_intervals(order);
end
