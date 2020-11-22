function [retIm] = zeroPad(I,b)
sz=size(I);
if size(sz)<3
    retIm=zeros(sz(1)+2*b,sz(2)+2*b);
else
    retIm=zeros(sz(1)+2*b,sz(2)+2*b,sz(3));
end
ii=1;
while ii<=sz(1)
    jj=1;
    while jj<=sz(2)
        if(size(sz)<3)
            retIm(ii+b,jj+b)=I(ii,jj);
        else
            retIm(ii+b,jj+b,:)=I(ii,jj,:);
        end
        jj=jj+1;
    end
    ii=ii+1;
end

end

