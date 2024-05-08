% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [theta, fval] = swp_GetL2OptimalGains(T, Phi)
%SWP_GETOPTIMALGAINS Computes L2-optimal theta. Done using YALMIP+MOSEK

    % For a given linear regressor T=Phi(1)+Phi(2:end)*theta we construct an L2 optimal estimate of theta
    theta=Phi(:,2:end)\(T-Phi(:,1));
    
    % cost function value
    fval = norm(T-Phi(:,1)-Phi(:,2:end)*theta)/sqrt(numel(T));
end

