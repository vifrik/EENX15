%% %%%
close all;clc;clear all;
path = 'Rak bana';
Horizon = 20;
Target_speed = 1;
Initial_State = [0,0,0,0];
%% %%% Vehicle Parameters %%%
params = vars();
a1 = params(1);  % Distance between articulation point and rear axle of dolly. 
b1 = params(2);  % Distance between front and rear axle of dolly.
b2 = params(3);  % Distance between front and rear axle of trailer.
%% %%% Waypoints %%%%%%%%%%%%%
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
        X_waypoints(i)=(i/1000);
        X_waypointsUp(i)=-(i/1000);
        X_waypointsLow(i)=(i/1000);
    end

    Y_waypointsUp(:) = roadWidth;
    Y_waypointsLow(:) = -roadWidth;
    plot(X_waypoints,Y_waypoints,'b--')
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
    
    X_waypoints = -1.*X_waypoints;

    plot(X_waypoints,Y_waypoints,'b--')
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

case 'Parkering_2'
    L1 = 10;
    L2 = 3;
    R1 = 2;
    L3 = 2;

    %roadWidth=0.4;
    roadWidth=0.2;
    n = 250; % Upplösning

    %% L1
    X_waypoints_1 = linspace(0,L1,n*3);
    X_waypointsUp = linspace(0,L1,n);
    X_waypointsLow = linspace(0,L1,n);

    Y_waypoints_1 = zeros(1,n*3);
    Y_waypointsUp = roadWidth.*ones(1,n);
    Y_waypointsLow = -roadWidth.*ones(1,n);

    %% L2
    X_waypoints_2 = linspace(L1, L1-L2, n);
    X_waypointsUp = [X_waypointsUp,linspace(L1, L1-L2, n)];
    X_waypointsLow = [X_waypointsLow,linspace(L1, L1-L2, n)];

    Y_waypoints_2 = zeros(1,n);
    Y_waypointsUp = [Y_waypointsUp, roadWidth.*ones(1,n)];
    Y_waypointsLow = [Y_waypointsLow, -roadWidth.*ones(1,n)];

    %% R1
    X_waypoints_2 = [X_waypoints_2, L1-L2 + R1.*cos(linspace(pi/2,pi,n))];
    X_waypointsUp = [X_waypointsUp, (L1-L2 + (R1+roadWidth).*cos(linspace(pi/2,pi,n)))];
    X_waypointsLow = [X_waypointsLow, L1-L2 + (R1-roadWidth).*cos(linspace(pi/2,pi,n))];

    Y_waypoints_2 = [Y_waypoints_2, R1.*sin(linspace(pi/2,pi,n))-R1];
    Y_waypointsUp = [Y_waypointsUp, (R1+roadWidth).*sin(linspace(pi/2,pi,n))-R1];
    Y_waypointsLow = [Y_waypointsLow, (R1-roadWidth).*sin(linspace(pi/2,pi,n))-R1];


    %% L3
    X_waypoints_2 = [X_waypoints_2, linspace(L1-L2-R1, L1-L2-R1, n)];
    X_waypointsUp = [X_waypointsUp, linspace(L1-L2-R1-roadWidth, L1-L2-R1-roadWidth, n)];
    X_waypointsLow = [X_waypointsLow, linspace(L1-L2-R1+roadWidth, L1-L2-R1+roadWidth, n)];

    Y_waypoints_2 = [Y_waypoints_2, linspace(-R1,-R1-L3,n)];
    Y_waypointsUp = [Y_waypointsUp, linspace(-R1,-R1-L3,n)];
    Y_waypointsLow = [Y_waypointsLow, linspace(-R1,-R1-L3,n)];

    %X_waypoints = -1.*X_waypoints;
    %X_waypointsUp = -1.*X_waypointsUp;
    %X_waypointsLow = -1.*X_waypointsLow;
    
    X_waypoints_1 = -1.*X_waypoints_1;
    X_waypoints_2 = -1.*X_waypoints_2;

    Y_waypoints_1 = -1.*Y_waypoints_1;
    Y_waypoints_2 = -1.*Y_waypoints_2;

    Index = 1;

    figure(1)
    plot(X_waypoints_1,Y_waypoints_1,'b--')
    title('Forward')
    figure(2)
    plot(X_waypoints_2,Y_waypoints_2,'b--')
    title('Reverse')
    %plot(X_waypointsUp, Y_waypointsUp,'k')
    %plot(X_waypointsLow, Y_waypointsLow,'k')
end