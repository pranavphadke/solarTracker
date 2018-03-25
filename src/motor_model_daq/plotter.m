clear
clf
close all
clc

load dataOut_motor_w_gb.txt
plot(dataOut_motor_w_gb(:,1),dataOut_motor_w_gb(:,2))
hold
G=tf(15.235,[1,5.95])
step(255*G)
hold off
sisotool(G)

figure()
load dataOut_motor_wo_gb.txt
plot(dataOut_motor_wo_gb(:,1),dataOut_motor_wo_gb(:,2))
% motor encoder pulse to shaft angle =0.3 * pulse