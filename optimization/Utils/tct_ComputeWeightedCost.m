% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function Jw = tct_ComputeWeightedCost(T, P, D, opi,  Ts, metrics, return_flag)
%SWP_COMPUTEWEIGHTEDCOST Computes weighted costs. Can return a value or a handle

%T - target, P -Prediction ( estimation), D - delayed (measurements)

% defalut value
if ~exist('return_flag','var') || isempty(return_flag),
    return_flag = 'value'; % {value | 'handle'}
end

% first we create a handle for costs computing.
fh_ComputeCosts = tct_ComputeCosts(T,[],opi, Ts, metrics,'handle');

% Next we compute costs with respect to no predicition. These values are further used as baselines for
% J1 and J2 scaling
[J1_scale, J2_scale, ~] = fh_ComputeCosts(D);

% define the weights as a column vector (metrics.weights can be either a row or a column)
weights=reshape(metrics.weights,[3 1]);

% what are we expected to return?
switch return_flag
    case 'value'
        % wecan return the value
        Jw = ComputeWeightedCost(P);
    case 'handle'
        % or we can return the handle
        Jw = @(P) ComputeWeightedCost(P);
    otherwise 
        error('Wrong return flag in the ComputeWeightedCost function.');
end

    function Jw_out = ComputeWeightedCost(P)
        
        % compute the costs
        [J1, J2, J3]= fh_ComputeCosts(P);
        
        % sccale J1 and J2
        J1_sc=J1/J1_scale;
        J2_sc=J2/J2_scale;
        
       	% scale J3
        J3_sc = (J3-metrics.J3_bounds(1))/(metrics.J3_bounds(2)-metrics.J3_bounds(1));
        J3_sc = max(0, min(J3_sc,1));
        
        Jw_out=[J1_sc J2_sc J3_sc]*weights;
       
    end

end

