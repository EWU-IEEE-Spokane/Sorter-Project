function[result] = removeBackground(Bkgrndh,Imh)
result=zeros(256,1);
ii=1;
while ii<=256
    if(Imh(ii)-Bkgrndh(ii)>0)
        result(ii)=Imh(ii)-Bkgrndh(ii);
    end
    ii=ii+1;
end



end
