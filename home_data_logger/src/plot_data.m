clear all
close all

data = csvread('all_sensor_data2.txt');

size(data)
plot(data(:,2));
hold on;
plot(data(:,3));
hold off;