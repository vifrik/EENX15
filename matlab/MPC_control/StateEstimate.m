function xp = StateEstimate(x,u,dz)
    % Runge-Kutta function for discretisation
    z = @(u)1/u(2);  % Current lethargy
    mu1 = 1/6;  % First order term
    l1 = StateFcn(x,u);
    mu2 = 1/3;  % Second order term
    l2 = StateFcn(z(u)*l1*dz/2 + x,u);
    mu3 = 1/3;  % Third order term
    l3 = StateFcn(z(u)*l2*dz/2 + x,u);
    mu4 = 1/6;  % Fourth order term
    l4 = StateFcn(z(u)*l3*dz + x,u);
    % Approximation of x(k+1)
    xp = x + dz*z(u)*(mu1*l1 + mu2*l2 + mu3*l3 + mu4*l4);
end
