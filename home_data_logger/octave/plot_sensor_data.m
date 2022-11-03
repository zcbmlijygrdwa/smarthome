
clear all;
close all;

data = csvread("temperature.log2");

subplot(3,1,1)
plot (data(:,1)-data(1,1), data(:,6));
title("temperature")

subplot(3,1,2)
plot (data(:,1)-data(1,1), data(:,2));
title("brightness")

subplot(3,1,3)
plot (data(:,1)-data(1,1), data(:,4));
title("sound")

