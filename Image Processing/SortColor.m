function [ColorDetected, results, extended] = SortColor(ColorArray,Image,ColorNames, parameters)
ii=1;
sz=size(ColorArray);
testing=zeros(sz(1),1);
results=zeros(sz(1),3);
extended=zeros(sz(1),3,2);
numC=sz(1,1);
sz=size(Image);

while ii<=sz(1)
    jj=1;
    while jj<=sz(2)
        r=double(Image(ii,jj,1));
        g=double(Image(ii,jj,2));
        b=double(Image(ii,jj,3));
        kk=1;
        if r>30 && g>30 &&b>30
            while kk<=numC
                rr=double(ColorArray(kk,1));
                gg=double(ColorArray(kk,2));
                bb=double(ColorArray(kk,3));
                rad=parameters(1,kk);
                rsub=parameters(2,kk);
                gad=parameters(3,kk);
                gsub=parameters(4,kk);
                bad=parameters(5,kk);
                bsub=parameters(6,kk);
                if r<rr+rad && r>rr-rsub
                    results(kk,1)=results(kk,1)+1;
                else
                    if r<rr-rsub
                        extended(kk,1,1)=extended(kk,1,1)+1;
                    else
                        extended(kk,1,2)=extended(kk,1,2)+1;
                    end
                end
                if g<gg+gad && g>gg-gsub
                    results(kk,2)=results(kk,2)+1;
                else
                    if g<gg-gsub
                        extended(kk,2,1)=extended(kk,2,1)+1;
                    else
                        extended(kk,2,2)=extended(kk,2,2)+1;
                    end
                end
                if b<bb+bad && b>bb-bsub
                    results(kk,3)=results(kk,3)+1;
                else
                    if b<bb-bsub
                        extended(kk,3,1)=extended(kk,3,1)+1;
                    else
                        extended(kk,3,2)=extended(kk,3,2)+1;
                    end
                end
                if(r<rr+rad && r>rr-rsub && g<gg+gad && g>gg-gsub && b<bb+bad && b>bb-bsub)
                    testing(kk)=testing(kk)+1;
                end
                kk=kk+1;
            end
        end
        jj=jj+1;
    end
    ii=ii+1;
end
res=1;
ii=1;
val=0;
while ii<=numC
    if testing(res)<testing(ii)
        res=ii;
    end
    if testing(ii)==0
        val=val+1;
    end
    ii=ii+1;
end
ColorDetected=ColorNames(res);
if val==5
    ColorDetected="No Color Detected";
end
end

