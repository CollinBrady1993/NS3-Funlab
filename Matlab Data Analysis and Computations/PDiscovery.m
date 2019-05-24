function [fk] = PDiscovery(Nut,Npt,Nr,a)
%this is the Pdisc for the case where Na UE have not been discovered, Nb
%have been and Nu = Na + Nb are transmitting this round.

fk = zeros(1,min(Nut+1,Nr+1));

%% a catch for if no one transmits
if Nut == 0
    fk = 1;
    return
end

%% clculate Fk
prob = (1/Nr)^(Nut)*(1/Nr)^(Npt);
for r = 1:min(Nr,Nut+Npt)
    
    A = a{:,:,r};%finds all partitions of Nu-1, given r bins are used
    AR = real(A);%all real values, representing the undiscovered UE
    AI = imag(A);%all imaginary values, representing the previously discovered UE
    JR = makeJ(AR,Nut+Npt);%counts of AR
    
    for i = 1:size(A,1)
        %number of X that become the given A vector
        num = (factorial(Nr)/(factorial(r)*factorial(Nr-r)))*factorial(Nut)/prod(factorial(AR(i,:)))*factorial(Npt)/prod(factorial(AI(i,:)))*(factorial(r)/(factorial(length(AR(i,AR(i,:)>0)))*factorial(r - length(AR(i,AR(i,:)>0)))))*(factorial(length(AR(i,AR(i,:)>0)))/prod(factorial(JR(i,:))))*numWaysToDistributeDiscUsers(AR(i,:),AI(i,:),Nut+Npt);
        %number of discoveries
        k = sum(and(AR(i,:)==1,AI(i,:)==0));
        
        fk(k+1) = fk(k+1) + num*prob;
        
    end
end
end
