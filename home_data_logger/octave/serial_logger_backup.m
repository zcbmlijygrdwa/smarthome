
clear all;
close all;


% pkg load instrument-control

s1 = serial("/dev/ttyACM0", 115200);
srl_flush(s1);

a = [-0.14015  -0.15382   9.92578];

locked = false;
while(true)
data = srl_read(s1, 120);
%data
%continue
for i = 1:size(data,2)-7
  if(typecast(data(i:i+3),"uint32") == 16843009 && typecast(data(i+4:i+7),"uint32") == 16843009)
    %i
      for k = i:32:size(data,2)
        if(k+31 <= size(data,2))
          d1 = typecast((data(i+8:i+15)),"double");
          d2 = typecast((data(i+16:i+23)),"double");
          d3 = typecast((data(i+24:i+31)),"double");
          d = [d1, d2, d3]
          angle = acos(dot(a,d)/(norm(a) * norm(d))) * 180/pi
        end
      end
  end
end


%char(data)


end