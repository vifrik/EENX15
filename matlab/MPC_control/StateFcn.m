function xdot = StateFcn(x, u)
    % Outputs the change in state variables given the current state
    % and control variables.

    % Input variables:
    %   x: the state of the system. x is a 7x1 vector with fields:
    %       x(1): x position of the dolly.
    %       x(2): y position of the dolly.
    %       x(3): heading angle relative to the x-axis.
    %       x(4): trailer angle.
    %   u: the control variables of the system. u is a 2x1 vector with
    %      fields:
    %       u(1): steering angle.
    %       u(2): velocity.
    params = vars();        % Load truck parameters.
    xdot = zeros(4,1);      % Initialize output variables.

    v1 = u(2)*cos(u(1));    % Velocity in the heading of the truck.
    w1 = u(2)*sin(u(1))/params(2);  % Angular velocity of the heading angle
    % Angular velocity of trailer angle (w2)
    w2 = (sin(x(4))*cos(u(1))-params(1)/params(2)*cos(x(4))*sin(u(1)))*u(2)/params(3);

    xdot(1) = v1*cos(x(3));
    xdot(2) = v1*sin(x(3));
    xdot(4) = w1 - w2;

    l = params(3)+params(1)*cos(x(4));  % Helper variable
    xdot(3) = sin(x(4))*u(2)/l + params(4)*xdot(4)/l;
end
