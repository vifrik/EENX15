function out = OptProblem(points, x, u, rev, horiz)
    % Parameters and helper functions
    dz = 2;  % Resolution in discretised space
    % Initiate variables
    vx = [points(1,1), points(1,2), x(3), x(4)];
    %vx = x;
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
    options = optimoptions("fmincon",...
        "Algorithm","interior-point",...
        "EnableFeasibilityMode",true,...
        "SubproblemAlgorithm","cg");
    [out, fval] = fmincon(@(U) 0,...
        v0, A, b, [], [], [], [], @(U) NonlCond(U(1:4), U(5:6), points, rev,...
        dz, horiz, vx), options);
end