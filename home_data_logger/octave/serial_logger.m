clear all;
close all;

filename = "temperature.log";

fid = fopen(filename, "w");

 pkg load instrument-control

s1 = serial("/dev/ttyACM0", 115200);
srl_flush(s1);

read_patch_bytei_size = 100; % bytes

while(true)
 data = srl_read(s1, read_patch_bytei_size);

 for i = 1:(read_patch_bytei_size-80)
   if(data(i)==43 && data(i+1)==0 && data(i+2)==0 && data(i+3)==0)
     byte_start = i;
     flag1 = data(byte_start: byte_start+7);

     % light
     byte_start = byte_start +8;
     sensor_info.light_mean = typecast(data(byte_start: byte_start+7),"double");
     byte_start = byte_start +8;
     sensor_info.light_var = typecast(data(byte_start: byte_start+7),"double");

     % sound
     byte_start = byte_start +8;
     sensor_info.sound_mean = typecast(data(byte_start: byte_start+7),"double");
     byte_start = byte_start +8;
     sensor_info.sound_var = typecast(data(byte_start: byte_start+7),"double");

     % temperature
     byte_start = byte_start +8;
     sensor_info.temperature_mean = typecast(data(byte_start: byte_start+7),"double");
     byte_start = byte_start +8;
     sensor_info.temperature_var =  typecast(data(byte_start: byte_start+7),"double");

     % acceleration
     byte_start = byte_start +8;
     sensor_info.a_x = typecast(data(byte_start: byte_start+7),"double");
     byte_start = byte_start +8;
     sensor_info.a_y = typecast(data(byte_start: byte_start+7),"double");
     byte_start = byte_start +8;
     sensor_info.a_z = typecast(data(byte_start: byte_start+7),"double");

     sensor_info

     out_str = [num2str(time(), "%5.5f") ", " num2str(sensor_info.light_mean, "%5.5f") ", " num2str(sensor_info.light_var, "%5.5f") ", " num2str(sensor_info.sound_mean, "%5.5f") ", " num2str(sensor_info.sound_var, "%5.5f") ", " num2str(sensor_info.temperature_mean, "%5.5f") ", " num2str(sensor_info.temperature_var, "%5.5f") ", " num2str(sensor_info.a_x, "%5.5f") ", " num2str(sensor_info.a_y, "%5.5f") ", " num2str(sensor_info.a_z, "%5.5f") ]

     fputs(fid, [out_str "\n"]);

     pause(30);
   end
 end
end

fclose(fid)

