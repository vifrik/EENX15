function out = OptProblem(points, x, u, rev, horiz)
    % Parameters and helper functions
    dz = 1e-3;  % Resolution in discretised space
    % Initiate variables
    vx = [points(1,1), points(1,2), x(3), x(4)];
    %vx = x;
    ux = u;
    %ux = [1,0];
    dx = [1,1];  % Initial soft constraint
    v0 = [vx, ux, dx];
    % Optimization functions and parameters
    A = [0,0,0,0,1,0,0,0
        0,0,0,0,-1,0,0,0
        0,0,0,0,0,1,0,0
        0,0,0,0,0,-1,0,0
        0,0,0,1,0,0,0,0
        0,0,0,-1,0,0,0,0
        0,0,0,0,0,0,1,0
        0,0,0,0,0,0,-1,0
        0,0,0,0,0,0,0,1
        0,0,0,0,0,0,0,-1];
    b = [15*pi/180
        15*pi/180
        1
        1
        pi/2
        pi/2
        10
        0
        10
        0];
    % Minimization function
    options = optimoptions("fmincon",...
        "Algorithm","interior-point",...
        "EnableFeasibilityMode",true,...
        "SubproblemAlgorithm","cg",...
        "MaxFunctionEvaluations",3000,...
        "MaxIterations",1000);
    [res, fval] = fmincon(@(U) ObjFcn(U(1:4),U(5:6),horiz,dz,rev,points, U(7:8)),...
        v0, A, b, [], [], [], [], @(U) NonlCond(U(1:4), U(5:6), points, rev,...
        dz, horiz, U(7:8)), options);
    out = res(1:6);
end