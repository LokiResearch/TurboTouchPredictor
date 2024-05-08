% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function J = tct_ComputeCostGivenHOM(HOM_params, time, T, D, events, opi, L0, Ts, steps_per_sample, transients_duration, metrics)
%TCT_COMPUTECOSTGIVENHOM Given the HOM params, computes optimal cost value

% T- target, D - delayed (measured)
% events - array of events
% HOM_params = [alpha, lambda]
% opi - optimization and evaluation interval
% Ts, steps_per_sample, transient_steps -- also HOM parameters
% L0 - used as upper bound for initial guess for 'weighted'
% metrics - metrics used for optimization



    % if we do it as 'weighted', then this function can be accelerated by modyfing swp_GetWeightedOptimalGains such that it takes cost function 
    % handle as input and does not generate it each time it is called. However, in such a case we have return the handle, ot the value.

    % first we compute the regressor based on the given parameters
    Phi=tct_GetHOMRegressorByEvents(D, events, HOM_params(1), HOM_params(2), Ts, steps_per_sample, transients_duration);
    
    % compute optimal theta
    switch metrics.cost_fun
        case 'L1'
            % we compute L1-optimal for metrics.desired_L and evaluate it with respect to Target provided.
            desired_T = tct_GetLagAheadData(time, D, metrics.desired_L);
            theta = swp_GetL1OptimalGains(desired_T(opi), Phi(opi,:));
            est=Phi(:,1)+Phi(:,2:end)*theta;
            J =  norm(T(opi)-est(opi),1)/numel(opi);
       
        case 'L2'
            % we compute L2-optimal for metrics.desired_L and evaluate it with respect to Target provided.
            desired_T = tct_GetLagAheadData(time, D, metrics.desired_L);
            theta = swp_GetL2OptimalGains(desired_T(opi), Phi(opi,:));
            est=Phi(:,1)+Phi(:,2:end)*theta;
            J =  norm(T(opi)-est(opi))/sqrt(numel(opi));
       
        case 'weighted'
            % different initial conditions
            Ls=0.005:0.005:L0;
            BestVal=Inf;
            for ind=1:numel(Ls),
                % for each L in Ls we first compute L2 optimal, and then use it as initial guess for
                % optimization
                new_T= tct_GetLagAheadData(time, D, Ls(ind));
                theta0 = swp_GetL2OptimalGains(new_T(opi), Phi(opi,:));
                [~, fval] = tct_GetWeightedOptimalGains(T, Phi, opi, theta0, Ts, metrics);
                if fval<BestVal,
                    BestVal=fval;
                end
            end
            J=BestVal;          
          
        otherwise
            error('ComputeCostGivenHOM: unknown cost_fun in the given metrics structure.');
    end

end

