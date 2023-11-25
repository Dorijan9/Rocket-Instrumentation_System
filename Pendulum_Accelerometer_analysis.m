%Code for the analysis of the pendulum data
%Importing datasets and storing them in the variables
%Use your own data! Adapt some values according to your data!
ax=Accelx';
ay=Accely';
az=Accelz';
t=timeStampR';
%Extracting values that were obtained during the experiment
t=t(10480:12586);
ax=ax(10480:12586);
ay=ay(10480:12586);
az=az(10480:12586);
%'Zeroing out' time and vertical acceleration %9.18
t=t-241.62;
ax=ax-0.5565;
ay=ay-1.0271;
az=az-9.0754;
%Smoothing the data to reduce noise
ax=smoothdata(ax);
ay=smoothdata(ay);
az=smoothdata(az);
%Plotting graphs
figure;
subplot(3,1,1); plot(t,ax);xlabel('time(s)');ylabel('XAcceleration(m/s^2)'); xlim([0 20])
grid on
subplot(3,1,2); plot(t,ay);xlabel('time(s)');ylabel('YAcceleration(m/s^2)'); xlim([0 20])
grid on
subplot(3,1,3); plot(t,az);xlabel('time(s)');ylabel('ZAcceleration(m/s^2)')
grid on