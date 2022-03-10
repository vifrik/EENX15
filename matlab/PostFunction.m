%%% Plot script, prints how the vehicle moved   
%Fordonets bana
clf; close all;
figure('Name','Fordonets bana')%,'units','normalized','outerposition',[0 0 1 1])
plot(X2,Y2,'b');
hold on
plot(X1,Y1,'m');
plot(X_waypoints,Y_waypoints,'k:',X_waypointsUp,Y_waypointsUp,'k',X_waypointsLow,Y_waypointsLow,'k');
title('Fordonets bana');
hTitle=title(sprintf('Time = %0.2g',tout(1)));
axis equal;
xlim([min(X2) max(X1)])
ylim([min(Y2)-6.3 max(Y1)+1])
xlabel('X [m]')
ylabel('Y [m]')
%------creating the tractor and trailer-----------------------
%Tractor
lfc = (L1-b1) ; lrc = b1;
%Trailer 1
lft1 = 0 ; lrt1 = (b2 + L2) ;
%Index c as in Car
X1c =  lfc*cos(theta(:,1)) - W/2*sin(theta(:,1));
X2c =  lfc*cos(theta(:,1)) + W/2*sin(theta(:,1));
X3c = -lrc*cos(theta(:,1)) + W/2*sin(theta(:,1));
X4c = -lrc*cos(theta(:,1)) - W/2*sin(theta(:,1));

Y1c =  W/2*cos(theta(:,1)) + lfc*sin(theta(:,1));
Y2c = -W/2*cos(theta(:,1)) + lfc*sin(theta(:,1));
Y3c = -W/2*cos(theta(:,1)) - lrc*sin(theta(:,1));
Y4c =  W/2*cos(theta(:,1)) - lrc*sin(theta(:,1));

%Trailer 1
X1t1 =  lft1*cos(theta(:,2)) - W/2*sin(theta(:,2));
X2t1 =  lft1*cos(theta(:,2)) + W/2*sin(theta(:,2));
X3t1 =  lrt1*cos(theta(:,2)) + W/2*sin(theta(:,2));
X4t1 =  lrt1*cos(theta(:,2)) - W/2*sin(theta(:,2));

Y1t1 =  W/2*cos(theta(:,2)) + lft1*sin(theta(:,2));
Y2t1 = -W/2*cos(theta(:,2)) + lft1*sin(theta(:,2));
Y3t1 = -W/2*cos(theta(:,2)) + lrt1*sin(theta(:,2));
Y4t1 =  W/2*cos(theta(:,2)) + lrt1*sin(theta(:,2));

hCar = patch([X1c(1) X2c(1) X3c(1) X4c(1)], [Y1c(1) Y2c(1) Y3c(1) Y4c(1)],'red','facecolor','none','edgecolor','green','LineWidth',1.5);
ht1 = patch([X1t1(1) X2t1(1) X3t1(1) X4t1(1)], [Y1t1(1) Y2t1(1) Y3t1(1) Y4t1(1)],'red','facecolor','none','edgecolor','red','LineWidth',1.5);
%------------ld circle----------------------------------------
circle = linspace(0,2*pi,100); 
Xd  = LookDistance.*cos(circle);
Yd  = LookDistance.*sin(circle);
targetPts = patch(Xd,Yd,'black','facecolor','none','edgecolor','green');
[xy,distance,t_a]=distance2curve([X_waypoints' Y_waypoints'],[X2 Y2]);
%[xy,distance,t_a]=distance2curve([X_waypoints' Y_waypoints'],[X1 Y1]);
medel_dist=100*mean(distance);
rms_dist=100*sqrt(mean(distance.^2));
max_dist=100*max(distance);
median_dist=100*median(distance);
text(-4,-3,{['Släp'];['Medel:' num2str(round(medel_dist,2,'significant')) ' cm'];['Median:' num2str(round(median_dist,2,'significant')) ' cm'];['RMS:' num2str(round(rms_dist,2,'significant')) ' cm'];['Max:' num2str(round(max_dist,2,'significant')) ' cm']})
[xy,distance,t_a]=distance2curve([X_waypoints' Y_waypoints'],[X1 Y1]);
medel_dist=100*mean(distance);
rms_dist=100*sqrt(mean(distance.^2));
max_dist=100*max(distance);
median_dist=100*median(distance);
text(-8,-3,{['Dolly'];['Medel:' num2str(round(medel_dist,2,'significant')) ' cm'];['Median:' num2str(round(median_dist,2,'significant')) ' cm'];['RMS:' num2str(round(rms_dist,2,'significant')) ' cm'];['Max:' num2str(round(max_dist,2,'significant')) ' cm']})

%----------Wheels-------------------------------------------
Leftwheel = animatedline('LineWidth',3);
XwL1 = X1 + X1c(:,1) - lfc*0.1*cos(-theta(:,1)+delta(:,1));
XwL2 = X1 + X1c(:,1) + lfc*0.1*cos(-theta(:,1)+delta(:,1));
YwL1 = Y1 + Y1c(:,1) + lfc*0.1*sin(-theta(:,1)+delta(:,1));
YwL2 = Y1 + Y1c(:,1) - lfc*0.1*sin(-theta(:,1)+delta(:,1));

Rightwheel = animatedline('LineWidth',3);
XwR1 = X1 + X2c(:,1) - lfc*0.1*cos(-theta(:,1)+delta(:,1));
XwR2 = X1 + X2c(:,1) + lfc*0.1*cos(-theta(:,1)+delta(:,1));
YwR1 = Y1 + Y2c(:,1) + lfc*0.1*sin(-theta(:,1)+delta(:,1));
YwR2 = Y1 + Y2c(:,1) - lfc*0.1*sin(-theta(:,1)+delta(:,1));
%-----------------------------------------------------------
legend('Släpets bana','Dollyns bana','Mitt','Kant','Kant','Dolly','Trailer','Synhorisont','Hjul', 'Hjul','Location','best')
        title([path])%, ' med radie ', num2str(R),' m'])
for i = 1:length(tout)
    clearpoints(Leftwheel);
    clearpoints(Rightwheel);
    addpoints(Leftwheel,XwL1(i),YwL1(i));
    addpoints(Leftwheel,XwL2(i),YwL2(i));
    addpoints(Rightwheel,XwR1(i),YwR1(i));
    addpoints(Rightwheel,XwR2(i),YwR2(i));
    set(hCar,'Xdata',X1(i)+[X1c(i) X2c(i) X3c(i) X4c(i)], ...
             'Ydata',Y1(i)+[Y1c(i) Y2c(i) Y3c(i) Y4c(i)]);
    set(ht1,'Xdata',X2(i)+[X1t1(i) X2t1(i) X3t1(i) X4t1(i)], ...
            'Ydata',Y2(i)+[Y1t1(i) Y2t1(i) Y3t1(i) Y4t1(i)]);
    set(targetPts, 'Xdata',Xd+X2(i), 'Ydata',Yd+Y2(i));
    %set(hTitle,'String',sprintf('Time= %0.2g',tout(i)));
    drawnow;
end
