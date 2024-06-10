% Return a string with human readable polynomial representation.
%
function out = to_string( self )
  out   = '';
  empty = true; % true indica che i coefficienti finora sono nulli
  s     = '';   % segno
  c     = 0;    % coefficiente
  e     = '';   % esponente

  if self.m_order <= 0
    out = 'EMPTY!';
    return;
  end
  if self.m_order == 1
    out = sprintf('%g',self.m_coeffs(1));
    return;
  end;
  if max(max(abs(self.m_coeffs))) == 0
    out = '0';
    return;
  end
  % controlla se esiste il primo coefficiente (grado 0)
  if self.m_coeffs(1) ~= 0
    out   = sprintf('%g',self.m_coeffs(1));
    empty = false;
  end

  for i=2:self.m_order
    % se il coefficiente e` negativo...
    if self.m_coeffs(i) < 0
      % e se i coefficienti precenti erano nulli...
      if empty
        s     = ''; % ...non scrive il segno
        c     = self.m_coeffs(i);
        empty = false;
      else
        s = ' - ';           % ...altrimenti scrive il segno come separatore
        c = -self.m_coeffs(i); % e inverte il segno del coefficiente
      end

      % se il coefficiente e` positivo...
    elseif self.m_coeffs(i) > 0
      c = self.m_coeffs(i);
      % e se i coefficienti precenti erano nulli...
      if empty
        s     = ''; % ...non scrive il segno
        empty = false;
      else
        s = ' + '; % ...altrimenti scrive il segno come separatore
      end

      % se il coefficiente e` zero...
    else
      continue; % ...procede al prossimo
    end

    % se il grado e` 1 non scrive l'esponente
    if i == 2
      e = 'x';
    else
      e = sprintf('x^%d',i-1);
    end

    % se il coeff è 1 non lo stampo
    if c == 1
      out = sprintf('%s%s%s',out,s,e); % stampa
    else
      out = sprintf('%s%s%g %s',out,s,c,e); % stampa
    end
  end
end
