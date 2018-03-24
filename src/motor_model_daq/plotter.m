clear
clf
close all
clc

load dataOut_motor_w_gb.txt
plot(dataOut_motor_w_gb(:,1),dataOut_motor_w_gb(:,2))
hold
G=tf(15.5,[1,5.95])
step(255*G)
hold off
sisotool(G)