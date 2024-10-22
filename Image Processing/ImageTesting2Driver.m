clear all;close all;%Load all images and resize.
R=imread("Red.jpg");
RR=imread("R1.jpg");
RRR=imread("R2.jpg");
O=imread("Orange.jpg");
OO=imread("O1.jpg");
OOO=imread("O2.jpg");
Y=imread("Yellow.jpg");
YY=imread("Y1.jpg");
YYY=imread("Y2.jpg");
G=imread("Green.jpg");
GG=imread("G1.jpg");
GGG=imread("G2.jpg");
P=imread("Purple.jpg");
PP=imread("P1.jpg");
PPP=imread("P2.jpg");
config=imread("Skittles Color Palette.png");

R=imresize(R,[480,640]);
RR=imresize(RR,[480,640]);
RRR=imresize(RRR,[480,640]);
O=imresize(O,[480,640]);
OO=imresize(OO,[480,640]);
OOO=imresize(OOO,[480,640]);
Y=imresize(Y,[480,640]);
YY=imresize(YY,[480,640]);
YYY=imresize(YYY,[480,640]);
G=imresize(G,[480,640]);
GG=imresize(GG,[480,640]);
GGG=imresize(GGG,[480,640]);
P=imresize(P,[480,640]);
PP=imresize(PP,[480,640]);
PPP=imresize(PPP,[480,640]);
%%
[c,cA]=Configuration(config);%initial configuration of algorithm as well as initial test to show that it isn't working
names=["Orange","Yellow","Green","Purple","Red"];
params=[
    0,0,0,0,0;%rad
    0,0,0,0,0;%rsub
    0,0,0,0,0;%gad
    0,0,0,0,0;%gsub
    0,0,0,0,0;%bad
    0,0,0,0,0];%bsub
"New Run"
[n,p,e]=SortColor(cA,O,names,params)
[n,p,e]=SortColor(cA,Y,names,params)
[n,p,e]=SortColor(cA,G,names,params)
[n,p,e]=SortColor(cA,P,names,params)
[n,p,e]=SortColor(cA,R,names,params)

%%
numrun=1;
while true
test=true;
[n,p,e]=SortColor(cA,OO,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(1))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(1))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end

[n,p,e]=SortColor(cA,OOO,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(1))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(1))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end


[n,p,e]=SortColor(cA,YY,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(2))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(2))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end

[n,p,e]=SortColor(cA,YYY,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(2))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(2))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end


[n,p,e]=SortColor(cA,GG,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(3))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(3))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end

[n,p,e]=SortColor(cA,GGG,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(3))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(3))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end


[n,p,e]=SortColor(cA,PP,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(4))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    params(2*i-1,k)=params(2*i-1,k)-1;
                else
                    params(2*i,k)=params(2*i,k)-1;
                end
                i=i+1;
            end
        elseif strcmp(m,names(4))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end

[n,p,e]=SortColor(cA,PPP,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(4))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(4))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end


[n,p,e]=SortColor(cA,RR,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(5))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(5))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end

[n,p,e]=SortColor(cA,RRR,names,params);%e(colors,RGB,LessThan GreateTthan)
if ~strcmp(n,names(5))
    test=false;
    k=1;%k is the second index
    for m=names
        if strcmp(m,n)
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%if there are more elements below than above then we decrease the upper range so more will fall outside from that side. Otherwise decrease the lower range.
                    
                        params(2*i-1,k)=params(2*i-1,k)-1;
                    
                else
                    
                        params(2*i,k)=params(2*i,k)-1;
                    
                end
                i=i+1;
            end
        elseif strcmp(m,names(5))
            i=1;
            while i<4
                if e(k,i,1)>e(k,i,2)%If more fall elements are above increase above so that more fall inside. Otherwise increase below.
                    params(2*i,k)=params(2*i,k)+1;
                else
                    params(2*i-1,k)=params(2*i-1,k)+1;
                end
                i=i+1;
            end
        end
        k=k+1;
    end
end

if test%If all colors a properly sorted stop modifying algorithm
    break;
end

if numrun==100%If 1000 modifications have been made stop modifying and look to see if something is wrong.
    break
end
numrun=numrun+1;
end
%%
%Testing section
"New Run"
[n,p,e]=SortColor(cA,O,names,params);
n
[n,p,e]=SortColor(cA,OO,names,params);
n
[n,p,e]=SortColor(cA,OOO,names,params);
n
[n,p,e]=SortColor(cA,Y,names,params);
n
[n,p,e]=SortColor(cA,YY,names,params);
n
[n,p,e]=SortColor(cA,YYY,names,params);
n 
[n,p,e]=SortColor(cA,G,names,params);
n
[n,p,e]=SortColor(cA,GG,names,params);
n
[n,p,e]=SortColor(cA,GGG,names,params);
n
[n,p,e]=SortColor(cA,P,names,params);
n
[n,p,e]=SortColor(cA,PP,names,params);
n
[n,p,e]=SortColor(cA,PPP,names,params);
n
[n,p,e]=SortColor(cA,R,names,params);
n
[n,p,e]=SortColor(cA,RR,names,params);
n
[n,p,e]=SortColor(cA,RRR,names,params);
n
%%
ed=0:1:257;
figure()
subplot(3,1,1);histogram(OO(:,:,1),ed,"Normalization",'probability')
subplot(3,1,2);histogram(OO(:,:,2),ed,"Normalization",'probability')
subplot(3,1,3);histogram(OO(:,:,3),ed,"Normalization",'probability')
