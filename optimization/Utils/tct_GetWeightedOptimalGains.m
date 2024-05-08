% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [theta, fval] = tct_GetWeightedOptimalGains(T, Phi, opi, theta0, Ts, metrics)
%SWP_GETOPTIMALGAINS Computes theta given the target and the regreessor

    % define a handle to the cost function
    fh_ComupteWeightedCost = tct_ComputeWeightedCost(T,[],Phi(:,1),opi, Ts,metrics,'handle');

    % define a handle to the minimized function
    fun = @(x) fh_ComupteWeightedCost(Phi(:,1)+Phi(:,2:end)*x);
%     op=optimset('Display','off','TolX',1e-9,'TolFun',1e-6);
%     [theta,fval]=fminsearch(fun,theta0,op);            
    [theta,fval]=fminsearch(fun,theta0);                 
end

