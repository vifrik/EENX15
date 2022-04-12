function out = OptProblem(points, x, u, rev)
    % Parameters and helper functions
    horiz = 5;  % Prediction horizon
    dz = 2;  % Resolution in discretised space
    % Initiate variables
    %vx = [r(1), r(2), x(3), x(4)];
    vx = x;
    ux = u;
    %ux = [1,0];
    v0 = [vx, ux];
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
    % Minimization function
    [out, fval] = fmincon(@(U) ObjFcn(U(1:4), U(5:6), horiz, dz, rev, points), v0, A, b, [], [], [], [], @(U) NonlCond(U(1:4), U(5:6), points, rev, dz));

end