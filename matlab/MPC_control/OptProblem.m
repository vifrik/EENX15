function vx = OptProblem(points, x, u, rev)
    % Parameters and helper functions
    params = vars();
    horiz = 5;  % Prediction horizon
    dz = 2;  % Resolution in discretised space
    % Initiate variables
    %vx = [r(1), r(2), x(3), x(4)];
    vx = x;
    %ux = u;
    ux = [1,0];
    v0 = [vx', ux'];
    % Optimization functions and parameters
    A = [0,0,0,0,1,0
        0,0,0,0,-1,0
        0,0,0,0,0,1
        0,0,0,0,0,-1
        0,0,0,1,0,0
        0,0,0,-1,0,0];
    b = [15*pi/180
        15*pi/180
        1
        1
        pi/2
        pi/2];
    % Helper functions
    z = @(x)1/x(4);  % Current lethargy
    x2 = @(x) x(1) - params(1)*cos(x(3)) - params(3)*cos(x(3) - x(4));  % x-trailer
    y2 = @(x) x(2) - params(1)*sin(x(3)) - params(3)*sin(x(3) - x(4));  % y-trailer
    dR = @(x,r) (x2(x) - r(1))^2 + (y2(x) - r(2))^2;
    px = @(r,rold,rev,dz) r(1) + (-1)^rev*(params(1)+params(3))/dz*(r(1)-rold(1));
    py = @(r,rold,rev,dz) r(1) + (-1)^rev*(params(1)+params(3))/dz*(r(2)-rold(2));
    dP = @(x,r,rold,rev,dz) (x(1) - px(r,rold,rev,dz))^2 + (x(2) - py(r,rold,rev,dz))^2;
    dr = 1;
    df = 1;
    c = @(x,u,points,rev,dz) [z(x) - 1
        -z(x) - 1
        dR(x,points(end,:)) - dr
        dP(x,points(end,:),points(end-1,:),rev,dz) - df];
    % Minimization function
    [vx, fval] = fmincon(@(U) ObjFcn(U(1:5), U(5:7), horiz, dz, rev, points), v0, A, b, [], [], [], [], @(U) c(U(1:5), U(5:7), points, rev, dz));
end