function [Out] = te(Image)
num=50000;
Image=double(Image);
sz=size(Image);
ii=1;
avg=0;
test=0;
while ii<sz(1)
    jj=1;
    while jj<sz(2)
        intensity=Image(ii,jj,1)^2+Image(ii,jj,2)^2+Image(ii,jj,3)^2;
        if intensity<num
            
        elseif avg==0
            avg=Image(ii,jj,:);
            test=test+1;
        else 
            avg=[(avg(1)+Image(ii,jj,1))/2,(avg(2)+Image(ii,jj,2))/2,(avg(3)+Image(ii,jj,3))/2];
            test=test+1;
        end
        jj=jj+1;
    end
    ii=ii+1;
end
Out=avg;


end

