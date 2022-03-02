%% %%% Test case definiton %%%%%%%%%%%%%%%%%%%%%%%% %%
    close all;clc;clear all;
    path = 'Rak bana';  %'Rak bana'  'Stor kurva' 'Skarp kurva' 'Sinusvåg' 
    desiredSpeed = -.5;  % Longitudinal Speed [m/s]
    LookDistance  = 0.4; %Look-ahead distance, bra värden; 0.375, 0.4, 0.4
%% %%% Vehicle parameters %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    mm = 1e-3; %mm to m conversion
    L1 = 220 * mm  ;        % Theoretical Wheelbase [m]
    b1 = 110 * mm  ;         % Distance from pivot 1 to tractor rear axle [m]
    L2 = 1200 * mm  ;        % Distance from pivot 1 to rear axle of trailer 1 [m]
    b2 = 100 * mm  ;        % Distance from rear axle of trailer 1 to pivot 2[m]
    W = 260 * mm  ;         % Track width [m]                  
    vehicleDynamics = [b1 L1 L2 b2];
%% %%%%%%%% Waypoints %%%%%%%%%%%%%
% creating the trajectory
switch path
case 'Rak bana'
%----------------Rak bana----------------
    n = 20000;
    roadWidth = 0.2;
    X_waypoints = zeros(1,n);
    Y_waypoints = zeros(1,n);
    X_waypointsUp = zeros(1,n);
    Y_waypointsUp = zeros(1,n);
    X_waypointsLow = zeros(1,n);
    Y_waypointsLow = zeros(1,n);

    for i=1:n
        X_waypoints(i)=-(i/1000);
        X_waypointsUp(i)=-(i/1000);
        X_waypointsLow(i)=-(i/1000);
    end

    Y_waypointsUp(:) = roadWidth;
    Y_waypointsLow(:) = -roadWidth;
%----------------Stor kurva----------------
case 'Stor kurva'

    roadWidth=0.2;
    n = 20000;
    X_waypoints = zeros(1,n);
    Y_waypoints = zeros(1,n);
    X_waypointsUp = zeros(1,n);
    Y_waypointsUp = zeros(1,n);
    X_waypointsLow = zeros(1,n);
    Y_waypointsLow = zeros(1,n);

    for i=1:8001
        X_waypoints(i)=-(i/1000)+5;
        X_waypointsUp(i)=-(i/1000)+5;
        X_waypointsLow(i)=-(i/1000)+5;
    end
    Y_waypointsUp(1:8001) = roadWidth;
    Y_waypointsLow(1:8001) = -roadWidth;

    R=5; %Tuning parameter
    angles = zeros(1,8000);
    angles(1)=-pi/2;
    for i=2:8000
        angles(i)= angles(i-1) - ((pi/2)/8000);
        X_waypoints(8000+i)=R*cos(angles(i))-3;
        Y_waypoints(8000+i)=R*sin(angles(i))+R;
        X_waypointsUp(8000+i)=(R-roadWidth)*cos(angles(i))-3;
        Y_waypointsUp(8000+i)=(R-roadWidth)*sin(angles(i))+R;
        X_waypointsLow(8000+i)=(R+roadWidth)*cos(angles(i))-3;
        Y_waypointsLow(8000+i)=(R+roadWidth)*sin(angles(i))+R;
    end

    for i=1:4000
        Y_waypoints(i+16000)=(i/1000)+R;
        Y_waypointsUp(i+16000)=(i/1000)+R;
        Y_waypointsLow(i+16000)=(i/1000)+R;
    end
    X_waypoints(16001:20000) = -(3+R);
    X_waypointsUp(16001:20000) = -(3+R-roadWidth);
    X_waypointsLow(16001:20000) = -(3+R+roadWidth);
%----------------Skarp kurva----------------
case 'Skarp kurva'

    roadWidth=0.2;
    n = 20000;
    X_waypoints = zeros(1,n);
    Y_waypoints = zeros(1,n);
    X_waypointsUp = zeros(1,n);
    Y_waypointsUp = zeros(1,n);
    X_waypointsLow = zeros(1,n);
    Y_waypointsLow = zeros(1,n);

    for i=1:8001
        X_waypoints(i)=-(i/1000)+5;
        X_waypointsUp(i)=-(i/1000)+5;
        X_waypointsLow(i)=-(i/1000)+5;
    end
    Y_waypointsUp(1:8001) = roadWidth;
    Y_waypointsLow(1:8001) = -roadWidth;

    R=0.3; %Tuning parameter
    angles = zeros(1,8000);
    angles(1)=pi/2;
    for i=2:8000
        angles(i)= angles(i-1) + ((pi/2)/8000);
        X_waypoints(8000+i)=R*cos(angles(i))-3;
        Y_waypoints(8000+i)=R*sin(angles(i))-R;
        X_waypointsUp(8000+i)=(R+roadWidth)*cos(angles(i))-3;
        Y_waypointsUp(8000+i)=(R+roadWidth)*sin(angles(i))-R;
        X_waypointsLow(8000+i)=(R-roadWidth)*cos(angles(i))-3;
        Y_waypointsLow(8000+i)=(R-roadWidth)*sin(angles(i))-R;
    end

    for i=1:4000
        Y_waypoints(i+16000)=-(i/1000)-R;
        Y_waypointsUp(i+16000)=-(i/1000)-R;
        Y_waypointsLow(i+16000)=-(i/1000)-R;
    end
    X_waypoints(16001:20000) = -(3+R);
    X_waypointsUp(16001:20000) = -(3+R+roadWidth);
    X_waypointsLow(16001:20000) = -(3+R-roadWidth);


%----------------Sinus----------------
case 'Sinusvåg'
    magnitude = 0.6;
    roadWidth=0.2;

    X_waypoints = linspace(-L2,-15,15000);
    
    Y_waypoints = (sin(X_waypoints+L2+pi/2+0.1)-1)*magnitude;

    X_waypointsUp=X_waypoints-(roadWidth*cos(X_waypoints))./(sqrt(1+(cos(X_waypoints)*magnitude).^2));
    Y_waypointsUp=(sin(X_waypoints+L2+pi/2+0.1)-1)*magnitude + roadWidth./(sqrt(1+(cos(X_waypoints)*magnitude).^2));

    X_waypointsLow = X_waypoints+((roadWidth*cos(X_waypoints)*magnitude))./(sqrt(1+(cos(X_waypoints)*magnitude).^2));
    Y_waypointsLow = (sin(X_waypoints+L2+pi/2+0.1)-1)*magnitude - roadWidth./(sqrt(1+(cos(X_waypoints)*magnitude).^2));

X_waypoints(end)=NaN;
Y_waypoints(end)=NaN;
end