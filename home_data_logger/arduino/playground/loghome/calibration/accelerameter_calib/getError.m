function error = getError(data, new_state)
  
  temp_data = data;
  temp_data(:,1) = temp_data(:,1) .* new_state(1) .+ new_state(2);
  temp_data(:,2) = temp_data(:,2) .* new_state(3) .+ new_state(4);
  temp_data(:,3) = temp_data(:,3) .* new_state(5) .+ new_state(6);
  
  
  error = sqrt(temp_data(:,1).^2 .+ temp_data(:,2).^2 .+ temp_data(:,3).^2) .- 9.8;
endfunction
