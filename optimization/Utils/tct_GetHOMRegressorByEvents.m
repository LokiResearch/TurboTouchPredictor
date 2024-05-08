% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function Phi = tct_GetHOMRegressorByEvents(Y, events, alpha, lambda, Ts, integration_steps_per_sample, transient_steps)
%TCT_GETHOMREGRESSOR computes the HOM output regressor
    
    %params
    dt=Ts/integration_steps_per_sample;
    Sc = 1e4;
    a1 = 5*alpha; a2 = 10*alpha^2; a3 = 10*alpha^3; a4 = 5*alpha^4; a5 = 1*alpha^5;
    p1 = 1+lambda; p2= 1+2*lambda; p3= 1+3*lambda; p4= 1+4*lambda; p5=1+5*lambda;
    
    state = zeros(5,1); 

    % Initialize
    state(1)=Y(1)/Sc; % initial value

    Ders=zeros(length(Y),3);

    steps_counter=0;
    % loop all the measurements
    for i = 1:numel(Y),
        switch events{i}
            case 'DOWN' % first touch
                % initialize
                state(1)=Y(i)/Sc; % use this position
                state(2:end)=0; % reset velocities   
                steps_counter=transient_steps; %start transient
                
                % output zero derivatives
                Ders(i,:) = zeros(1,3);
            
            case 'UP' %end of a stroke
                %bypass the last known measurement
                state(1)=Y(i)/Sc;
                state(2:end)=0; % reset velocities  
                
                % output zero derivatives     
                Ders(i,:) = zeros(1,3);
            
            case 'MOVE'  %regular sample
                
                % We compute dx and update the HOM's state.
                for ind=1:integration_steps_per_sample 
                    e = state(1)-Y(i)/Sc;
                    if (e > 0)
                        dx = [-a1*(e)^p1+state(2); -a2*(e)^p2+state(3); -a3*(e)^p3+state(4); -a4*(e)^p4+state(5); -a5*(e)^p5];
                    else
                        dx = [a1*(-e)^p1+state(2); a2*(-e)^p2+state(3); a3*(-e)^p3+state(4); a4*(-e)^p4+state(5); a5*(-e)^p5];
                    end;        
                    state = state+dt*dx;
                end                
                
                Ders(i,:) = [state(2) state(3) state(4)]*Sc;
                
                %if we are in transients
                if(steps_counter>0),
                    Ders(i,:)=Ders(i,:)*max(1-steps_counter/10,0); %smoothing the output
                    steps_counter=steps_counter-1;
                end
                
                
            otherwise
                error('Unknown state at i=%d.',i);
        end

    end
    
    % form the output regressor
    Phi = [Y Ders];
end
