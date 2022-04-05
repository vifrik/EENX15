function vx = ObjFcn(x, u, horiz)
    % Weights
    w1 = 0;
    w2 = 1;
    w3 = 1;
    w4 = 1;
    % Objective function
    z = @(x)1/x(4);  % Current lethargy
    x2 = @(x) x(1) - params(1)*cos(x(3)) - params(3)*cos(x(3) - x(4));  % x-trailer
    y2 = @(x) x(2) - params(1)*sin(x(3)) - params(3)*sin(x(3) - x(4));  % y-trailer
    dR = @(x,r) (x2(x) - r(1))^2 + (y2(x) - r(2))^2;
    px = @(r,rold,rev,dz) r(1) + (-1)^rev*(params(1)+params(3))/dz*(r(1)-rold(1));
    py = @(r,rold,rev,dz) r(1) + (-1)^rev*(params(1)+params(3))/dz*(r(2)-rold(2));
    dP = @(x,r,rold,rev,dz) (x(1) - px(r,rold,rev,dz))^2 + (x(2) - py(r,rold,rev,dz))^2;
    J = @(x, u)
    xp = x; % Initiate
    sum = 0;
    for i=1:horiz
        sum = sum + w1*dR(xp,r) + w2*dP(xp,r,rold,rev,dz) + w3*z(xp);
        xp = StateEstimate(xp,u,dz);
    end
end