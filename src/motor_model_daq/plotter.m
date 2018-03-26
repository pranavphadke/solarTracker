clear
clf
close all
clc

load dataOut_motor_w_gb_v2.txt
load motor_w_gb_v2_sim.mat
plot(dataOut_motor_w_gb_v2(:,1),dataOut_motor_w_gb_v2(:,2))
hold
plot(simD(1,:),simD(2,:),'r')
legend('Actual','Simulated')
hold off
G=tf(30.95,[1,11.9,0])
% step(255*G)
% hold off
sisotool(G)

% motor encoder pulse to shaft angle =0.3 * pulse