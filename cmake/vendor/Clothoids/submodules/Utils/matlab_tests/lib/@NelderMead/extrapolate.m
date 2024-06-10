%
% Extrapolates by a factor alpha through the face of the simplex
% across from the high point.
%
% pface   = (sum(p)-pfrom)/N
% pcenter = sum(p)/(N+1);
% pe      = pface + alpha*(pface-pfrom)
%         = pface*(1+alpha)-pfrom*alpha
%         = (sum(p)-pfrom)/N*(1+alpha)-pfrom*alpha
%         = sum(p)/N*(1+alpha)-pfrom*(alpha+(1+alpha)/N)
%         = sum(p)/(N+1) * (1+alpha)*((N+1)/N) - pfrom*[1/N+alpha*(N+1)/N]
%         = pcenter * (1+alpha)*((N+1)/N) - pfrom*(1/N-(N+1)/N+(1+alpha)*(N+1)/N)
%         = pcenter * (1+alpha)*((N+1)/N) - pfrom*(-1+(1+alpha)*(N+1)/N)
%
% let beta = (1+alpha)*((N+1)/N);
%
% pe = pcenter * beta + pfrom * (1-beta)
%
function [fe,pe] = extrapolate( self, alpha, pfrom )
  N    = self.m_dim;
  beta = (1+alpha)*((N+1)/N);
  pe   = (beta/(N+1)) * self.m_psum + (1-beta) * pfrom;
  fe   = self.eval_function( pe );
end
