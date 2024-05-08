% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [est_x, est_y] = tct_GetEstimate(del_x, del_y, events, est_par_x, est_par_y, Ts, steps_per_sample, transients_duration, V0)

    N=numel(del_x);

    Phi_x=tct_GetHOMRegressorByEvents(del_x, events,est_par_x.alpha, est_par_x.lambda, Ts, steps_per_sample, transients_duration);
    Phi_y=tct_GetHOMRegressorByEvents(del_y, events,est_par_y.alpha, est_par_y.lambda, Ts, steps_per_sample, transients_duration);

    
    est_x=zeros(N,1);
    est_y=zeros(N,1);

    %compute V2D
    Vx=Phi_x(:,2);
    Vy=Phi_y(:,2);
    V2D=sqrt(Vx.^2+Vy.^2);

    %
    th_x=est_par_x.theta;
    th_y=est_par_y.theta;

    alpha_gain=0.04^(1/10);

    for i=1:N,
        % get measurements
        x_measure=Phi_x(i,1);
        y_measure=Phi_y(i,1);

        % compute predicted deltas
        delta_x=Phi_x(i,2:end)*th_x;
        delta_y=Phi_y(i,2:end)*th_y;

        if V2D(i)>=V0,
            alpha_target=1;
        else
            alpha_target=0;
        end

        % alpha block
        if i==1,
            alpha=alpha_target;
        else
            alpha=alpha_gain*alpha+(1-alpha_gain)*alpha_target;
        end

        % compute predicted values
        est_x(i)=x_measure + alpha*delta_x;
        est_y(i)=y_measure + alpha*delta_y;    
    end
end