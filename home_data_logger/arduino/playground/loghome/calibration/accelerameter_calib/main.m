clear all;
close all;

data = csvread("data.txt");
 

%{
ax = ax*kx+bx;
ay = ay*ky+by;
az = az*kz+bz;
%}

state = [0.5,0.5,0.5,0.5,0.5,0.5];
delta = 0.00001;
max_iter = 2000;
for iter = 1:max_iter
  disp(["== iter [ " num2str(iter) " /" num2str(max_iter) " ]=="])
  jacobian_matrix = zeros(size(data,1), length(state));
  for i = 1:length(state)
    new_state = state;
    new_state(i) = new_state(i) + delta;
    error_up = getError(data, new_state);
    
    new_state = state;
    new_state(i) = new_state(i) - delta;
    error_down = getError(data, new_state);
    
    jacobian_matrix(:,i) = (error_up-error_down)/delta;
  endfor
  error = getError(data, state);
  error_sum = sum(abs(error))
  H = jacobian_matrix'*jacobian_matrix;
  delta_state = -1*inv(H)*jacobian_matrix'*error*0.1;
  state = state .+ delta_state';
  next_state = state
  
  %pause(0.1)
endfor
