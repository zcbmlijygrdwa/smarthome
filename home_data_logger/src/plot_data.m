clear all
close all

data = csvread('sensor_data.txt');

size(data)
figure(1)
plot(data(:,2));
hold on;
plot(data(:,3));
hold off;
legend('arduino','phone');
title('temperature')
figure(2)
plot(data(:,4));
title('sound')