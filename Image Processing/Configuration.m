function [numColors,colorArray] = Configuration(imConfig)
import java.util.ArrayList
eDetecth=[1,0,-1;1,0,-1;1,0,-1];
zIm=zeroPad(imConfig,1);
testh=zeros(size(imConfig));
sz=size(zIm);
beginflag=true;
endflag=false;
begin=ArrayList;
ending=ArrayList;
h=0;
hh=sz(2)/2;
if mod(hh,1)~=0
    hh=hh+.5;
end
jj=2;
while jj<sz(2)
    htest=sum(zIm(hh-1,jj-1,:)*eDetecth(1,1)+zIm(hh,jj-1,:)*eDetecth(2,1)+zIm(hh+1,jj-1,:)*eDetecth(3,1)+zIm(hh-1,jj,:)*eDetecth(1,2)+zIm(hh,jj,:)*eDetecth(2,2)+zIm(hh+1,jj,:)*eDetecth(3,2)+zIm(hh-1,jj+1,:)*eDetecth(1,3)+zIm(hh,jj+1,:)*eDetecth(2,3)+zIm(hh+1,jj+1,:)*eDetecth(3,3));
    if htest~=0
        if beginflag
            begin.add(jj);
            beginflag=false;
            endflag=true;
        elseif endflag
            ending.add(jj);
            beginflag=true;
            endflag=false;
        end
        testh(:,jj-1)=htest;
        h=h+1;
    end
    jj=jj+1;
end
numColors=h/2;
colorArray=zeros(numColors,3);
sz=size(imConfig);
ii=0;
while ii<numColors
    jj=begin.remove(0);
    theend=ending.remove(0);
    color=imConfig(1,jj,:);
    while jj<theend
        kk=1;
        while kk<sz(1)
            color=(double(color)/2+double(imConfig(kk,jj,:))/2);
            kk=kk+1;
        end
        jj=jj+1;
    end
    colorArray(ii+1,:)=color;
    ii=ii+1;
end
 colorArray=uint8(colorArray);       





end

