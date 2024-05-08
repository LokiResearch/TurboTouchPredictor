% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [rt, ut, ux, uy] = tct_UnifyAllByEvents(t,x,y,ev,Fs )
    % ind of UP event
    up_ind=find(cellfun(@(x) strcmp(x,'UP'), ev));
    % ind of DOWN event
    dwn_ind=find(cellfun(@(x) strcmp(x,'DOWN'), ev));

    %clear 

    rt=[];
    % for each interval
    for i=1:numel(up_ind),
        % extract subinterval between DOWN and UP
        int_t=t(dwn_ind(i):up_ind(i));

        % new (refined) time, linearly spaced
%        ref_t=(linspace(int_t(1), int_t(end), numel(int_t)))';
        
        % new (refined) time, equally spaced
        ref_t=(0:1:numel(int_t)-1)'*1/Fs;
        
        %merge
        if i==1,
            rt=ref_t;
        else
            ref_t=ref_t-ref_t(1)+rt(end)+1/Fs;
            rt=[rt;ref_t];%#ok<AGROW>
        end

    end
    
    %unify it
    ut=(t(1):1/Fs:t(end))';
    ux=interp1(rt, x, ut);
    uy=interp1(rt, y, ut);    
    
end

