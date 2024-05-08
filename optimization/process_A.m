% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function process_A(dirpath, Fs_arg, L0_arg, chosen_axes, w1_arg, w2_arg, w3_arg)

% define parameters
Fs = str2num(Fs_arg)
L0 = str2num(L0_arg) / 1000
Ts = 1/Fs;
steps_per_sample=2;
transients_duration=40; %samples

% Cost function and metrics
metrics = struct ('cost_fun', 'weighted',...          % {weighted | L1 | L2} -- what do we minimize: either weighted costs, or L2 error norm
                  'desired_L', L0,...         % desired estimation horizon. Is ued for cost_fun = L1|L2
                  'weights', [1 1 1],...        % weights for weighted cost function computation
                  'J1_norm', 'L1',...           % {'L1','L2'} -- type of norm used for J1 compiutation
                  'Fbnd', 6,...                 % Lower frequency bound for J3 computation
                  'J3_bounds', [0.09 0.15]);          % Bounds for J3 scaling
              
if nargin > 4
    metrics.weights = [str2num(w1_arg) str2num(w2_arg) str2num(w3_arg)];
end

% HOM tunning
% should we tune HOM, or we just want to tune thetas for a predefined HOM?
HOM_tunning_type = 'tune'; %{const | grid | tune | pure_tune}

% grid size for HOM tunning
Nals = 25;
Nlams = 25;

alpha_bounds=[20 300];
lambda_bounds=[-0.1999 -0.001];

% some predefined values based on previouse optimization

HOM_const_params = struct('alpha_x', 57.45, 'lambda_x', -0.1162, ...
                          'alpha_y', 59.01, 'lambda_y', -0.1156);

%% load and preprocess
fprintf('Look for train file in %s for axis %s.\n', dirpath, chosen_axes);

% path to the utilites
addpath('Utils');

% Load train data (Provided by Python script)
filepath = strcat(dirpath, '/train.csv');
fileID = fopen(filepath, 'r');
data = textscan(fileID, '%f64;%f64;%f64;%s');
time = data{1};
del_x = data{2};
del_y = data{3};
events = data{4};

% refine and unify
[rtime,utime,udel_x,udel_y]=tct_UnifyAllByEvents(time, del_x, del_y, events, Fs);
%dlmwrite('refined_data.csv', [rtime del_x del_y], 'delimiter', ';', 'precision', '%.9f');

% use refined time and original measurements
% Choose an axes
switch chosen_axes
    case 'x'
        del = del_x;
    case 'y'
        del = del_y;
    otherwise
        error('Wrong chosen axes.');
end

% compute L0 in samples
L0_in_samples=ceil(L0/Ts); %integer above L0/Ts, that is L0 in samples

% compute indeces for optimization and performance evaluation
opi=tct_GetOPI(events,L0_in_samples, transients_duration);

% compute target for the given L0
targ_L0=tct_GetLagAheadData(rtime, del, L0);
%dlmwrite('lagahead_data.csv', targ_L0, 'delimiter', ';', 'precision', '%.6f');

% HOM optimization handle. Takes HOM_params =[alpha, lambda] as an input
fh_optHOM = @(HOM_params) tct_ComputeCostGivenHOM(HOM_params, rtime, targ_L0, del, events, opi, L0, Ts, steps_per_sample, transients_duration, metrics);
%% Compute HOM optimization

% choose predefined params
switch chosen_axes
    case 'x'
        HOM_params = [HOM_const_params.alpha_x, HOM_const_params.lambda_x];
    case 'y'
        HOM_params = [HOM_const_params.alpha_y, HOM_const_params.lambda_y];
    otherwise
        error('Wrong chosen axes.');
end

% choose optimization type
switch HOM_tunning_type
    case 'tune'
        HOM_grid_flag=1;
        HOM_opt_flag=1;
    case 'grid'
        HOM_grid_flag=1;
        HOM_opt_flag=0;
    case 'const'
        HOM_grid_flag=0;
        HOM_opt_flag=0;
    case 'pure_tune'
        HOM_grid_flag=0;
        HOM_opt_flag=1;
    otherwise
        error('Wrong HOM_tunning_type.');
end

% grid
if HOM_grid_flag,

    % running a grid
    fprintf('Computing a %dx%d grid... ',Nals,Nlams);
    tic;

    % linear alpha grid
    als=linspace(alpha_bounds(1),alpha_bounds(2),Nals);
    % log lambda grid
    lams=-0.2*(1-exp(-linspace(0.1,4,Nlams)));

    Z=zeros(numel(als),numel(lams));
    for i=1:numel(als),
        for j=1:numel(lams),
            Z(i,j)=fh_optHOM([als(i) lams(j)]);
            %if i == 1 && j == 1
            %    dlmwrite('computecostgivenhom_input_data.csv', [als(i);lams(j)], 'delimiter', ';', 'precision', '%.9f');
            %    dlmwrite('computecostgivenhom_output_data.csv', Z(i,j), 'precision', '%.9f');
            %end
        end
    end

    % take the best grid point
    [r,c]=find(Z==min(min(Z)));
    HOM_params=[als(r); lams(c)];

    exec_time=toc;
    fprintf('done in %.1f seconds.\n',exec_time);

end

if HOM_opt_flag

        % running optimization from the best grid node
        fprintf('Starting best-grid optimization... ');
        tic;

        initial_cost=fh_optHOM(HOM_params);

        % optimize hom params
        opt=optimset('Display','notify');
        x_lb=[alpha_bounds(1);lambda_bounds(1)];
        x_ub=[alpha_bounds(2);lambda_bounds(2)];

        [HOM_params, final_cost]=fmincon(fh_optHOM,HOM_params,[],[],[],[],x_lb,x_ub,[],opt);

        exec_time=toc;
        fprintf('done in %.1f seconds. Initial cost: %.3f, final cost %.3f.\n',exec_time,initial_cost,final_cost);
end
%% Compute prediciton given HOM
%fileID = fopen('gethomregressor_input_data.csv', 'w');
%cell_del = mat2cell(del, ones(1,size(del,1)));
%inputs = [cell_del events];
%format = '%.6f;%s\n';
%for row = 1:size(inputs,1)
%    fprintf(fileID, format, inputs{row,:});
%end
%fclose(fileID);
%dlmwrite('gethomregressor_homparams_input_data.csv', HOM_params, 'delimiter', ';', 'precision', '%.6f');
Phi=tct_GetHOMRegressorByEvents(del, events, HOM_params(1), HOM_params(2), Ts, steps_per_sample, transients_duration);
%dlmwrite('gethomregressor_output_data.csv', Phi, 'delimiter', ';', 'precision', '%.6f');

% compute optimal theta
switch metrics.cost_fun
    case 'L1'
        % we compute L1-optimal for metrics.desired_L
        desired_T = tct_GetLagAheadData(rtime, del, metrics.desired_L);
        theta = swp_GetL1OptimalGains(desired_T(opi), Phi(opi,:));
    case 'L2'
        % we compute L2-optimal for metrics.desired_L
        desired_T = tct_GetLagAheadData(rtime, del, metrics.desired_L);
        theta = swp_GetL2OptimalGains(desired_T(opi), Phi(opi,:));
    case 'weighted'
        % different initial conditions
        Ls=0.005:0.001:L0;
        BestVal=Inf;
        BestL=0;
        BestTh=[0;0;0];
        BestTh0=[0;0;0];
        for ind=1:numel(Ls),
            % for each L in Ls we first compute L2 optimal, and then use it as initial guess for
            % optimization
            new_T= tct_GetLagAheadData(rtime, del, Ls(ind));
            %if ind == 1
            %    dlmwrite('optimalgains_input_data.csv', [new_T(opi) Phi(opi,:)], 'delimiter', ';', 'precision', '%.f');
            %end
            theta0 = swp_GetL2OptimalGains(new_T(opi), Phi(opi,:));
            %if ind == 1
            %    dlmwrite('optimalgains_theta_input_data.csv', theta0, 'delimiter', ';', 'precision', '%.12f');
            %    dlmwrite('optimalgains_targl0_data.csv', targ_L0, 'delimiter', ';', 'precision', '%.12f');
            %    dlmwrite('optimalgains_opi_data.csv', opi, 'delimiter', ';');
            %end
            [theta, fval] = tct_GetWeightedOptimalGains(targ_L0, Phi, opi, theta0, Ts, metrics);
            %if ind == 1
            %    dlmwrite('optimalgains_output_data.csv', [theta;fval], 'delimiter', ';', 'precision', '%.9f');
            %end
            if fval<BestVal,
                BestVal=fval;
                BestL=Ls(ind);
                BestTh=theta;
                BestTh0=theta0;
            end
        end
    otherwise
        error('Wrong cost function.');
end

% compute and evaluate prediction
pred=Phi(:,1)+Phi(:,2:end)*theta;

%dlmwrite('computecosts_input_data.csv', [targ_L0 pred del], 'delimiter', ';', 'precision', '%.6f');
%dlmwrite('computecosts_opi_data.csv', opi, 'delimiter', ';');
[J1, J2, J3]=tct_ComputeCosts(targ_L0,pred,opi,Ts,metrics);
Jw=tct_ComputeWeightedCost(targ_L0,pred,del,opi,Ts,metrics);

%fprintf('Results, %s axes tunning: J1=%.1f, J2=%.1f, J3=%.3f. Jw=%.1f.\n',chosen_axes,J1,J2,J3, Jw);
% define estimator parameters
%estim_params=struct('alpha', HOM_params(1), 'lambda', HOM_params(2), 'theta', theta);
filepath = strcat(dirpath, '/optim.csv');
dlmwrite(filepath, [HOM_params; theta], 'delimiter', ';', 'precision', '%.12f');
end
