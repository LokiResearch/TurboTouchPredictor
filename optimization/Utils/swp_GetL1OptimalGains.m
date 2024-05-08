% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [theta, fval] = swp_GetL1OptimalGains(T, Phi)
%SWP_GETOPTIMALGAINS Computes L1-optimal theta. Done using YALMIP+MOSEK

    % For a given linear regressor T=Phi(1)+Phi(2:end)*theta we construct an L1 optimal estimate of theta
    % YALMIP 
    op=sdpsettings('showprogress',0,'verbose',0);
    op.solver='mosek';
    % op.solver='sedumi';
    Q=sdpvar(size(Phi,2)-1,1);

    % L1
    optimize([],norm(T-Phi(:,1)-Phi(:,2:end)*Q,1),op);
    theta=value(Q);
    if any(isnan(theta)),
        error('L1 optimization failed.');
    end    
    
    % cost function value
    fval = norm(T-Phi(:,1)-Phi(:,2:end)*theta,1)/numel(T);
end

