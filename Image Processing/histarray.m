function [result] = histarray(Im, Bkgrnd)
sze=size(Im);
if(size(sze)~=2)
    E=MException("Input:ImproperFormat","Input must be a 2-D array");
    throw(E);
end
result=zeros(256,1);
ii=1;
while(ii<=sze(1))
    jj=1;
    while(jj<=sze(2))
        if(Im(ii,jj)==Bkgrnd(ii,jj))
        else
            result(Im(ii,jj)+1)=result(Im(ii,jj)+1)+1;
        end
        jj=jj+1;
    end
    ii=ii+1;
end

end

