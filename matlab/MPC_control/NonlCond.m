function [c,ceq] = NonlCond(x,u,points,rev,dz)
    % Helper functions
    params = vars();
    z = @(u)1/u(2);  % Current lethargy
    x2 = @(x) x(1) - params(1)*cos(x(3)) - params(3)*cos(x(3) - x(4));  % x-trailer
    y2 = @(x) x(2) - params(1)*sin(x(3)) - params(3)*sin(x(3) - x(4));  % y-trailer
    dR = @(x,r) (x2(x) - r(1))^2 + (y2(x) - r(2))^2;
    px = @(r,rold,rev,dz) r(1) + (-1)^rev*(params(1)+params(3))/dz*(r(1)-rold(1));
    py = @(r,rold,rev,dz) r(1) + (-1)^rev*(params(1)+params(3))/dz*(r(2)-rold(2));
    dP = @(x,r,rold,rev,dz) (x(1) - px(r,rold,rev,dz))^2 + (x(2) - py(r,rold,rev,dz))^2;
    dr = 1;
    df = 1;
    c = [z(u) - 1
        -z(u) - 1
        dR(x,points(end,:)) - dr
        dP(x,points(end,:),points(end-1,:),rev,dz) - df];
    ceq = [];
end