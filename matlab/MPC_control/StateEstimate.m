function xp = StateEstimate(x,u,dz)
    % Runge-Kutta function for discretisation
    mu1 = 1/6;  % First order term
    l1 = @(x,u,dz) StateFcn(x,u);
    mu2 = 1/3;  % Second order term
    l2 = @(x,u,dz) StateFcn(z(x)*l1(x)*dz/2 + x,u);
    mu3 = 1/3;  % Third order term
    l3 = @(x,u,dz) SateFcn(z(x)*l2(x)*dz/2 + x,u);
    mu4 = 1/6;  % Fourth order term
    l4 = @(x,u,dz) SateFcn(z(x)*l3(x)*dz + x,u);
    % Approximation of x(k+1)
    xp = x + dz*z(x)*(mu1*l1(x,u,dz) + mu2*l2(x,u,dz) + mu3*l3(x,u,dz) + mu4*l4(x,u,dz));
end