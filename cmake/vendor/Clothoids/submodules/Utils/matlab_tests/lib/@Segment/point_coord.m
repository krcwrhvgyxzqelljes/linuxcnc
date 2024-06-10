function [s,t,PP] = point_coord( self, P )
  % controllo se collineari
  D = self.Pb-self.Pa;
  N = [-D(2);D(1)] ./ norm(D); % counterclockwise 90 degree rotation
  % calcolo proiezione (P-(Pa+s*D))^2 = (P-Pa)^2 + s^2 - 2*s*(P-Pa).D
  s = dot(P-self.Pa,D);
  % projection point
  PP = self.Pa + s * D;
  % normal coordinate
  t = dot( N, P - PP );
end
