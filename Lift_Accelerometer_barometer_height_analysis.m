%Code for the Arduino project
%Importing data and storing it in variables
%Use your own data! Adapt some values according to your data!
ay=Accely';
t=timeStampR';
%Setting initial conditions
vy=0;
sy=0;
%Extracting the values obtained during the experiment
ay=ay(11000:length(ay)-1939);
t=t(11000:length(t)-1939);
%'Zeroing out' the values
t=t-245.537;
off=10.3156;
off3=0;
ay=ay-10.3156;
%Loops to calculate velocity and displacement
for k = 1:5000
off2 = 0;
for i=1:length(ay)-1
vy(i+1)=vy(i)+ay(i+1).*(t(i+1)-t(i));
sy(i+1)=sy(i)+vy(i).*(t(i+1)-t(i))+0.5.*ay(i+1).*(t(i+1)-t(i)).^2;
end
if sy(end)>0
off2=+0.00001;
off3=off3+0.0001;
else
off2=-0.00001;
off3=off3-0.0001;
end
ay = ay - off2;
end
%Smoothing the data and obtaining the maximum value
ay=smoothdata(ay);
maxsy=max(sy);
maxay=max(ay);
%Plotting graphs
figure;
subplot(4,1,2); plot(t,sy); xlabel("Time(s)"); ylabel("YPosition(m)");xlim([0 68]);
grid on
subplot(4,1,3); plot(t,vy); xlabel("Time(s)"); ylabel("YVelocity(m)"); xlim([0 68]);
grid on
subplot(4,1,4); plot(t,ay); xlabel("Time(s)"); ylabel("YAcceleration(m)"); xlim([0 68]);
grid on
%Code for the Arduino project
%Importing and storing data in these variables
al=pheight';
t=timeStampR';
%Extracting values obtained during the experiment
al=al(11000:length(al));
t=t(11000:length(t));
%'Zeroing out' the values
t=t-245.537;
al=al+34.53;
%Obtaining maximum value
maxal=max(al);
%Plotting graph
subplot(4,1,1); plot(t,al); xlabel("Time(s)"); ylabel("YPosition(m)");xlim([0 68]);
grid on