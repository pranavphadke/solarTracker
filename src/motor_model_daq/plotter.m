clear
clf
close all
clc

load dataOut_motor_w_gb_v2.txt
plot(dataOut_motor_w_gb_v2(:,1),dataOut_motor_w_gb_v2(:,2))
hold
% G=tf(15.235,[1,5.95])
% step(255*G)
% hold off
% sisotool(G)
load motor_w_gb_v2_sim.mat
plot(simD(1,:),simD(2,:),'r')
legend('Actual','Simulated')
% motor encoder pulse to shaft angle =0.3 * pulse