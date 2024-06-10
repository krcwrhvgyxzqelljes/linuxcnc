% Division with remainder.
%
% Given a polynomial P(x) and Q(x) the operation
%
% [M,R] = P.divide(Q);
%
% return the polynomial M(x) and R(x) such that
%
% P(x) = Q(x) * M(x) + R(x)
%
function [Mpoly,Rpoly] = divide( self, QP )

  P = self.m_coeffs;
  Q = QP.coeffs();
  scaleP = max(max(abs(P))); if scaleP > 0; P = P./scaleP; end
  scaleQ = max(max(abs(P))); if scaleQ > 0; Q = Q./scaleQ; end
  %
  % P(x) = Q(x) * M(x) + R(x)
  %
  R       = P;
  lcQ     = Q(end);
  R_order = length(R);
  Q_order = length(Q);
  dd      = R_order - Q_order + 1;
  M       = zeros(1,dd);

  if lcQ == 0
    error( 'Poly::divide leading coefficient is 0!');
  end

  while dd > 0 && R_order > 0
    lcR        = R(R_order);
    bf         = lcR/lcQ;
    M(dd)      = bf;
    IDX        = dd:dd+Q_order-1;
    R(IDX)     = R(IDX) - bf*Q;
    R(R_order) = 0;
    R_order    = R_order-1;
    dd         = dd-1;
  end

  % adjust degree or remainder
  epsi   = eps^(3./4.)*max(1,max(max(abs(R))));
  IDX    = find( abs(R) <= epsi );
  R(IDX) = 0;
  while R_order > 0
    if abs(R(R_order)) > epsi; break; end
    R_order = R_order-1;
  end
  R = R(1:R_order);

  % scale back polinomials
  %
  % P(x) = Q(x) * M(x) + R(x)
  % p(x) / scaleP = q(x) / scaleQ * M(x) + R(x)
  % p(x) = q(x) * (scaleP/scaleQ) * M(x) + scaleP*R(x)
  %
  M = M*(scaleP/scaleQ);
  R = R*scaleP;

  Mpoly = Poly(M);
  Rpoly = Poly(R);
end
