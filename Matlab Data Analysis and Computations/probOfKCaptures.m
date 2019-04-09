function[fk] = probOfKCaptures(Nue,Np,theta,Nr,Nt)
%this is the round >1 calculation

%% initialization
fk = zeros(1,min(Nue-Np,Nr+1));

%% grabbing the colision probabilities and A data
%{
colProbLookup = csvread('collisionDataNew.csv');
pCol = ones(Nue-Np,Np+1);%the rows represent Na, and the columns Nb
for i = 1:size(pCol,1)-1
    for j = 1:size(pCol,2)%the reason Nb ranges from 0:Nu while Na ranges 1:Nu is that if Na = 0 Pdisk(k) = 0 for all k
        pCol(i+1,j) = tableLookup(colProbLookup,[i,j-1,gamma,alpha]);
    end
end

if macCol == 1
    pCol = ceil(pCol);%makes all multi-occupancy prb collisions, akin to mac collisions
end
%}
A = cell(Nue-1,Np+1,min(Nue-1,Nr));%why is this written in a wierd way? well there are 
%potentially Nu-1 undiscovered UE that can transmit, so thats why Nu(0:Nu-1). The
%reason that we have Nd+1+1 is that because Nd can be 0 we need a +1 for
%size purposes, basically we need there to always be at least 1 there, the
%other +1 represents the reference UE, which cant discover itself but still
%play discovery(0:Nd+1).
for i = 1:(Nue)
    for j = 0:min(i-1,Np+1)
        for k = 1:min(i,min(Nue,Nr))
            string = strcat('AData/Nu',num2str(i),',Np',num2str(j),',r',num2str(k),'.csv');
            A{i,j+1,k} = csvread(string);
        end
    end
end


%% the actual calculation
for Nut = 0:Nue-Np-1%number of undiscovered UE which transmit
    PNut = binopdf(Nue-Np-1,Nut,theta);
    if PNut > 0
        for Npt = 0:Np%number of discovered UE which transmit
            PNpt = binopdf(Np,Npt,theta);
            if PNpt > 0
                fk2 = zeros(1,min(Nue-Np,Nr+1));
                for Nud = 0:Nut%number of transmitting UE which experience duplex
                    
                    PNud = binopdf(Nud,Nut,1/Nt);
                    
                    if Nud < Nut
                        for Npd = 0:Npt
                            
                            PNpd = binopdf(Npd,Npt,1/Nt);
                            fkTemp = PDiscovery(Nut-Nud,Npt-Npd,Nr-(Nr/Nt),A(Nut + Npt - Nud - Npd,Npt - Npd + 1,:));
                            fkTemp = [fkTemp,zeros(1,min(Nue-Np,Nr+1)-length(fkTemp))];
                            fk2 = fk2 + PNud*PNpd*fkTemp;
                        end
                    elseif Nud == Nut
                        for Npd = 0:Npt
                            
                            PNpd = binopdf(Npd,Npt,1/Nt);
                            fkTemp = 1;
                            fkTemp = [fkTemp,zeros(1,min(Nue-Np,Nr+1)-length(fkTemp))];
                            fk2 = fk2 + PNud*PNpd*fkTemp;
                        end
                    end
                end
                
                if Nut > 0
                    fk1 = PDiscovery(Nut,Npt,Nr,A(Nut+Npt,Npt+1,:));
                    fk1 = [fk1,zeros(1,min(Nue-Np,Nr+1)-length(fk1))];
                    fk = fk + PNut*PNpt*(theta*fk2 + (1-theta)*fk1);
                else
                    fk1 = 1;
                    fk1 = [fk1,zeros(1,min(Nue-Np,Nr+1)-length(fk1))];
                    fk = fk + PNut*PNpt*(theta*fk2 + (1-theta)*fk1);
                end
                
                
            end
        end
    end
end
end


