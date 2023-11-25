%Code for pressure and temperature test
%Importing data and storing it in variables
%Use your own data! Adapt some values according to your data!
h=height';
p=pressure';
t=timeStampp';
%'Zeroing out' the values
t=t-29.15;
h=h+90;
%Plotting graphs
figure;
plot(t,p); xlabel('Time(s)'); ylabel('Pressure(hPa)'); xlim([0 360])
grid on
figure;
plot(t,h); xlabel('Time(s)'); ylabel('Altitude(m)'); xlim([0 360])
grid on
%Obtaining minimum and maximum values and their difference
maxp=max(p);
minp=min(p);
maxh=max(h);
minh=min(h);
diffp=maxp-minp;
diffh=maxh-minh;
%Importing data and storing it in variables
T=temperature';
tt=timeStampT';
%'Zeroing out' the values
tt=tt-11.197;
%Plotting graphs
figure;
plot(tt,T); xlabel('Time(s)'); ylabel('Temperature(°C)'); xlim([0 150])
grid on
%Obtaining minimum and maximum values and their difference
maxT=max(T);
minT=min(T);
diffT=maxT-minT;