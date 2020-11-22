function [output] = weightedAvg(input)
sz=size(input);
tot=sum(input);
i=1;
output=0;
while(i<sz(1)+1)
    output=(i-1)*input(i)/tot+output;
    
    i=i+1;
end



end

