%this script populates the workspace with the relevant data set from NS3,
%it will only be called from mainScript

%% importing data

periodTime = .32;%s, this may need to be changed based on your data

%this is your matlab directory, change appropriately
home = 'C:\Users\collin''s PC\Google Drive\research\Code\Matlab\D2D\NS3 Validation';
%this is the top level directory of the NS-3 logs, change appropriately
path = strcat('F:\HD2\UE',num2str(Nu),'\theta',num2str(theta*100),'\Nr=',num2str(Nr),',Nt=',num2str(Nt),'\');


%% determining the number of data points to read in
cd(path)
list = ls;
cd(home)
list([1,2],:) = [];
tempfile = zeros(size(list,1),1);
for i = 1:size(list,1)
    a = strsplit(list(i,:),'-');
    tempfile(i,1) = str2double(a{9});
end

runs = max(tempfile(:,1));


%% reading in data
data = cell(runs,1);
a = 1;
c = strsplit(list(1,:),'-');

for i = 1:runs
    
    tStart = 2.201;
    
    file = strcat(path,c(1),'-',c(2),'-',c(3),'-',c(4),'-',c(5),'-',num2str(Nu),'-',num2str(Nr),'-',num2str(Nt),'-',num2str(i),'\discovery-out-monitoring.tr');
    tempfile = importdata(file{1});
    
    tempData = tempfile.data(:,[1,2,5]);
    b = 1;
    
    while size(tempData,1) > 0
        timeDif = 0;
        k = 1;
        periodSkipFlag = 0;
        while timeDif < periodTime%find the end of the period
            timeDif = tempData(k,1) - tStart;
            if k == 1 && timeDif > periodTime
                periodSkipFlag = 1;
            else
                k = k+1;
            end
            
            if k > size(tempData,1)
                k = k+1;%this is a catch for the last value, otherwise it wont be included
                break
            end
        end
        
        if periodSkipFlag
            data{a}{b,1} = [];
        else
            data{a}{b,1} = tempData(1:(k-2),:);
            tempData(1:(k-2),:) = [];
        end
        
        b = b + 1;
        tStart = tStart + periodTime;
        
    end
    
    a = a+1;
    disp(['Loading Data, ',num2str(100*(a-1)/(runs)),'% complete'])
    
end

%% here we remove all the temp variables

clear('a','b','c','i','j','tempFile','tempData','k','tStart','timeDif','file','path','runs','trials','list')