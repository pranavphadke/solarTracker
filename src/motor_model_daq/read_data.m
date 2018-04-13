clear
clf
close all
clc
'Pausing...'
pause(5)

ardu=serial('COM6','BaudRate',9600);
fopen(ardu);
% disp('Connected!')
% disp(ardu)
% fprintf(ardu,'%s','1');% start running
formatS='%d,%f';
dataOutSize=[1 2];
c=0;
'Capture...'
while true
    c=c+1;
    %dataIn=fgets(ardu);
    dataOut(c,:)=fscanf(ardu,formatS,dataOutSize);
    dataOut(c,1)=dataOut(c,1)/1000;
    if dataOut(c,2)==0 && dataOut(c,1)>10
        break;
    end
end
% fprintf(ardu,'%s','0');% stop running
fclose(ardu);
% disp(ardu)
delete(ardu);
plot(dataOut(:,1),dataOut(:,2))
save dataOut.txt dataOut -ascii
clear ardu

