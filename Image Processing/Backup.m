hR1=histarray(R(:,:,1),Background(:,:,1));
hR2=histarray(R(:,:,2),Background(:,:,2));
hR3=histarray(R(:,:,3),Background(:,:,3));

% hRS1=histarray(RS(:,:,1),Background(:,:,1));
% hRS2=histarray(RS(:,:,2),Background(:,:,2));
% hRS3=histarray(RS(:,:,3),Background(:,:,3));
% 
% hO1=histarray(O(:,:,1),Background(:,:,1));
% hO2=histarray(O(:,:,2),Background(:,:,2));
% hO3=histarray(O(:,:,3),Background(:,:,3));
% 
% hOS1=histarray(OS(:,:,1),Background(:,:,1));
% hOS2=histarray(OS(:,:,2),Background(:,:,2));
% hOS3=histarray(OS(:,:,3),Background(:,:,3));
% 
% hY1=histarray(Y(:,:,1),Background(:,:,1));
% hY2=histarray(Y(:,:,2),Background(:,:,2));
% hY3=histarray(Y(:,:,3),Background(:,:,3));
% 
% hYS1=histarray(YS(:,:,1),Background(:,:,1));
% hYS2=histarray(YS(:,:,2),Background(:,:,2));
% hYS3=histarray(YS(:,:,3),Background(:,:,3));
% 
% hG1=histarray(G(:,:,1),Background(:,:,1));
% hG2=histarray(G(:,:,2),Background(:,:,2));
% hG3=histarray(G(:,:,3),Background(:,:,3));
% 
% hGS1=histarray(GS(:,:,1),Background(:,:,1));
% hGS2=histarray(GS(:,:,2),Background(:,:,2));
% hGS3=histarray(GS(:,:,3),Background(:,:,3));
% 
% hP1=histarray(P(:,:,1),Background(:,:,1));
% hP2=histarray(P(:,:,2),Background(:,:,2));
% hP3=histarray(P(:,:,3),Background(:,:,3));
% 
% hPS1=histarray(PS(:,:,1),Background(:,:,1));
% hPS2=histarray(PS(:,:,2),Background(:,:,2));
% hPS3=histarray(PS(:,:,3),Background(:,:,3));

figure()
subplot(6,1,1);plot(hR1);
subplot(6,1,2);histogram(R(:,:,1));
subplot(6,1,3);plot(hR2);
subplot(6,1,4);histogram(R(:,:,2));
subplot(6,1,5);plot(hR3);
subplot(6,1,6);histogram(R(:,:,3));

rravg=weightedAvg(hR1);
rgavg=weightedAvg(hR2);
rbavg=weightedAvg(hR3);


% rRRS1=corr(hR1,hRS1);
% rRRS2=corr(hR2,hRS2);
% rRRS3=corr(hR3,hRS3);
% 
% rRO1=corr(hR1,hO1);
% rRO2=corr(hR2,hO2);
% rRO3=corr(hR3,hO3);
% 
% 
% rRY1=corr(hR1,hY1);
% rRY2=corr(hR2,hY2);
% rRY3=corr(hR3,hY3);
% 
% 
% rRG1=corr(hR1,hG1);
% rRG2=corr(hR2,hG2);
% rRG3=corr(hR3,hG3);
% 
% 
% rRP1=corr(hR1,hP1);
% rRP2=corr(hR2,hP2);
% rRP3=corr(hR3,hP3);
% 
% rOOS1=corr(hO1,hOS1);
% rOOS2=corr(hO2,hOS2);
% rOOS3=corr(hO3,hOS3);
% 
% rOY1=corr(hO1,hY1);
% rOY2=corr(hO2,hY2);
% rOY3=corr(hO3,hY3);
% 
% rOG1=corr(hO1,hG1);
% rOG2=corr(hO2,hG2);
% rOG3=corr(hO3,hG3);
% 
% rOP1=corr(hO1,hP1);
% rOP2=corr(hO2,hP2);
% rOP3=corr(hO3,hP3);
% 
% rYYS1=corr(hY1,hYS1);
% rYYS2=corr(hY2,hYS2);
% rYYS3=corr(hY3,hYS3);
% 
% rYG1=corr(hY1,hG1);
% rYG2=corr(hY2,hG2);
% rYG3=corr(hY3,hG3);
% 
% rYP1=corr(hY1,hP1);
% rYP2=corr(hY2,hP2);
% rYP3=corr(hY3,hP3);
% 
% rGGS1=corr(hG1,hGS1);
% rGGS2=corr(hG2,hGS2);
% rGGS3=corr(hG3,hGS3);
% 
% rGP1=corr(hG1,hP1);
% rGP2=corr(hG2,hP2);
% rGP3=corr(hG3,hP3);
% 
% rPPS1=corr(hP1,hPS1);
% rPPS2=corr(hP2,hPS2);
% rPPS3=corr(hP3,hPS3);