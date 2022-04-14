function sum = ObjFcn(x, u, horiz, dz, rev, points)
    % Weights
    w1 = 1;
    w2 = 1e-2;
    w3 = 1;
    w4 = 1;
    % Helper functions
    params = vars();
    z = @(u)1/u(2);  % Current lethargy
    x2 = @(x) x(1) - params(1)*cos(x(3)) - params(3)*cos(x(3) - x(4));  % x-trailer
    y2 = @(x) x(2) - params(1)*sin(x(3)) - params(3)*sin(x(3) - x(4));  % y-trailer
    dR = @(x,r) (x2(x) - r(1))^2 + (y2(x) - r(2))^2;
    % Initiate loop variables
    xp = x;
    sum = 0;
    % Objective function
    for i=1:horiz
        sum = sum + w1*dR(xp,points(i,:)) + w2*z(u);
        xp = StateEstimate(xp,u,dz);
    end
end
