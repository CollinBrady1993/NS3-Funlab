%% setting
%simulation parameters
Nue = 15;%number of UE
theta = 1;%transmission probability
Nr = 10;%number of PRB
Nt = 10;%number of timeslots

%settings to toggle actions
readData = 0;
findExperimentalPMF = 0;
findExperimentalCDF = 0;
calculatePMF = 1;
calculateCDF = 0;%if you want to calculate this, you also have to calculate the PMF

if readData
    run('dataReadScript.m');
end

%% determining experimental pmfs
if findExperimentalPMF
    %this matrix is going to hold all the PMF for the number of discoveries in a round
    %the ith row represents the number of discoveries with i-1 previous
    %discoveries. the jth column represents the probability of discovering j-1
    %UE in a round with i-1 previous discoveries.
    PMF = zeros(Nue-1,min(Nue,Nr));
    
    PMFRaw = cell(1,Nue+1);%this is going to hold the data that is used to form the PMFs
    for i = 1:(min(Nue,Nr)+1)
        PMFRaw{i} = zeros(1,size(data,1));%this is by definition too small, but it's the min we can preallocate
    end
    a = ones(1,size(PMFRaw,2));%itterator
    
    
    
    %% gathering round 1 discoveries PMF data
    disp('Gathering Round 1 Discovery Data')
    for i = 1:size(data,1)
        disp(strcat(num2str(100*(i-1)/size(data,1)),'% complete'))%progress bar
        discPeriods = size(data{i},1);
        for j = 1:(discPeriods-1)
            for k = 1:Nue
                if ~isempty(data{i}{j})
                    PMFRaw{1}(a(1)) = sum(data{i}{j}(:,2) == k);%find the number of discoveries UE k makes
                    a(1) = a(1)+1;
                else
                    PMFRaw{1}(a(1)) = 0;
                    a(1) = a(1)+1;
                end
            end
        end
    end
    disp(strcat('100% complete'))
    
    %% gathering round >1 discoveries PMF data
    disp('Gathering Round >1 Discovery Data')
    for i = 1:size(data,1)
        disp(strcat(num2str(100*(i-1)/size(data,1)),'% complete'))
        discPeriods = size(data{i},1);
        for j = 1:(discPeriods-2)
            for k = 1:Nue
                if ~isempty(data{i}{j})
                    preDisc = sum(data{i}{j}(:,2) == k);%number discovered in this round
                else
                    preDisc = 0;
                end
                if preDisc > 0
                    if ~isempty(data{i}{j})
                        UEDisc = data{i}{j}(data{i}{j}(:,2) == k,3);%UE that it actually discovered
                    else
                        UEDisc = [];
                    end
                    if ~isempty(data{i}{j+1})
                        nextUEDisc = data{i}{j+1}(data{i}{j+1}(:,2) == k,3);%UE that it discovers the next round
                    else
                        nextUEDisc = [];
                    end
                    PMFRaw{preDisc+1}(a(preDisc+1)) = (length(nextUEDisc) - sum(ismember(UEDisc,nextUEDisc)));%[PMFRaw{preDisc+1},(length(nextUEDisc) - sum(ismember(UEDisc,nextUEDisc)))];
                    a(preDisc+1) = a(preDisc+1) + 1;
                end
            end
        end
    end
    
    disp(strcat('100% complete'))
    
    %% calculating the experimental PMFs
    for i = 1:(Nue-1)
        disp(strcat('Determining experimental PMF: ',num2str(i),'/',num2str(Nue-1)))
        figure
        h = histogram(PMFRaw{i},-.5:1:(min(Nue,Nr)-.5),'normalization','pdf');
        PMF(i,:) = h.Values;
        close gca
    end
    
    PMF = [zeros(size(PMF,1),1),PMF];
    
    for i = 1:size(PMF,1)
        PMF(i,1) = length(PMFRaw{i});
    end
    
    clear('UEDisc','nextUEDisc','preDisc','h','i','j','k')
end

%% calculating the experimental CMF
if findExperimentalCDF
    
    CDFRaw = zeros(1,Nue*size(data,1));%represents the number of rounds it took each UE to complete
    a = 1;
    
    for i = 1:Nue%for each UE
        
        for j = 1:size(data,1)%for each trial
            discPeriods = size(data{j},1);
            discList = [];%a variable that holds the UE that have been discovered
            
            for k = 1:discPeriods%for each discovery period, hopefully we dont go through them all
                if ~isempty(data{j}{k}) && length(data{j}{k}(:,2)) > 1
                    discListTemp = data{j}{k}((data{j}{k}(:,2) == i),3)';
                else
                    discListTemp = [];
                end
                discList = unique([discList,discListTemp]);
                if length(discList) == (Nue-1)
                    break
                end
            end
            
            CDFRaw(a) = k;
            a = a+1;
        end
    end
    
    h = histogram(CDFRaw,-.5:1:(max(CDFRaw)+.5),'normalization','pdf');
    CDF = h.Values;
    close all
    
    CDF = cumsum(CDF);
end

%% calculating the empirical PMF/CDF
if calculatePMF
    
    %this is capable of calculating the PMF for both HD1 and HD2. To use
    %the HD2 assumption set Nr=Nt.
    PMFCalc = zeros(Nue,min(Nue,Nr+1));
    
    for i = 0:Nue-1
        disp(strcat('Calculating PMF: ',num2str(i+1),'/',num2str(Nue)))
        datestr(now)
        temp = probOfKCaptures(Nue,i,theta,Nr,Nt);
        PMFCalc(i+1,:) = [temp,zeros(1,size(PMFCalc,2) - length(temp))];
    end
    clear('temp','i')
    
    if calculateCDF
        
        T = zeros(Nue,Nue);%this is the state transition matrix
        T(1,:) = [PMFCalc(1,:),zeros(1,Nue-length(PMFCalc(1,:)))];
        T(end,end) = 1;
        for i = 2:(size(PMFCalc,1)-1)
            T(i,i:(i+length(PMFCalc(i,:))-1)) = PMFCalc(i,:);
        end
        T = T(:,1:Nue);
        
        CDFCalc = 0;
        i = 1;
        tempT = T;
        while tempT(1,end) < .9999
            tempT = T^i;
            CDFCalc = [CDFCalc,tempT(1,end)];
            i = i+1;
        end
        
    end
end

clear('readData','findExperimentalPMF','calculatePMF','plottingUtility','findExperimentalCMF','calculateCMF')