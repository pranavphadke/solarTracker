clear
clf
close all
clc

load dataOut.txt
load dataOut_sim.mat
plot(dataOut(:,1),dataOut(:,2))
hold
plot(simD(1,:),simD(2,:),'r')
legend('Actual','Simulated')
hold off
G=tf(0.468,[0.051,1])
% step(255*G)
% hold off
sisotool(G)

% motor encoder pulse to shaft angle =0.3 * pulse