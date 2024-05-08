% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [opi] = tct_GetOPI(events, Lsamples, transient_duration)

% compute optimization index based on events

    opi=ones(length(events),1);
    
    %ind of UP and DOWN events
    up_ind=find(cellfun(@(x) strcmp(x,'UP'), events));
    dwn_ind=find(cellfun(@(x) strcmp(x,'DOWN'), events));
    
    L_left=transient_duration; % number of samples to skip on the left, transient time
    L_right=Lsamples; % number of samples to skip on the right, number of samples to generate "target"
    
    for i=1:numel(up_ind),
        if up_ind(i)-dwn_ind(i)<L_left+L_right,
            opi(dwn_ind(i):up_ind(i))=0;
        else
            opi(dwn_ind(i):dwn_ind(i)+L_left)=0;
            opi(up_ind(i)-L_right:up_ind(i))=0;
        end
    end
    opi=find(opi);
end

