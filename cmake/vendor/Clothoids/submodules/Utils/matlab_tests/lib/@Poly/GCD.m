% Greather Commond divisor
%
% Given a polynomial P(x) and Q(x) the operation
%
% G = P.GCD(Q);
%
% return the polynomial G(x) that divide both P(x) and Q(x), i.e.
%
% P(x) = G(x) * P1(x)
% Q(x) = G(x) * Q1(x)
%
% and if another G1(x) is such that 
%
% P(x) = G1(x) * P1(x)
% Q(x) = G1(x) * Q1(x)
%
% Then G(x) divide G1(x), i.e. G1(x) = G(x) * H(x)
%
function G = GCD( self, Q )
  if Q.order() > 0
    [M,R] = self.divide( Q );
    G = Q.GCD( R );
  else
    G = self;
  end
  G.normalize();
end