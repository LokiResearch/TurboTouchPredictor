% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [J1, J2, J3] = tct_ComputeCosts(T, P, opi, Ts, metrics, return_flag)
% SWP_COMPUTECOSTS Computes [J1, J2, J3] costs. Can return the values or a handle.

% it is assumed that T and P are uniformly measured with Ts. Thus, time stamps are not required

% defalut value
if ~exist('return_flag','var') || isempty(return_flag),
    return_flag = 'value'; % {value | 'handle'}
end


%parameters
FreqBnd_low = metrics.Fbnd;
FreqBnd_high = Inf;

% Freq part
Fs = 1/Ts;
NFFT = 2^nextpow2(length(T));
fft_freqs = Fs/2*linspace(0,1,NFFT/2+1);    
FreqBnd_ind = find(fft_freqs<FreqBnd_low | fft_freqs>FreqBnd_high);
    
% gate signal for smooth fft
gate = ones(size(T));
gate(1:50) = 0;
gate(end-50+1:end) = 0;
    
% define and normalize the kernel
gaus_kern = exp(-0.06*(-30:30).^2); %smoothing over approximately 10ms
gaus_kern = gaus_kern/sum(gaus_kern);
    
% constuct the smooth gate signal
gate = conv(gate,gaus_kern,'same');

% freq_gate signal for removing low freqs
freq_gate = ones(NFFT,1);
freq_gate(FreqBnd_ind) = 0; %#ok<FNDSB>

% return the values or the handle
switch return_flag
    case 'value'
        [J1, J2, J3] = ComputeCosts(P);
    case 'handle'
        handle_costs= @(P) ComputeCosts(P);
        J1 = handle_costs;
        J2=[];
        J3=[];
    otherwise 
        error('Wrong return flag in the ComputeCosts function.');
end


    function [J1_out, J2_out, J3_out] = ComputeCosts(P)
        
        if length(P)~=length(T)
            error('The lengthes are not equal in the ComputeCosts function.')
        end   
        % estimation error
        er=zeros(size(T)); % we simply fill everything that is not OPI with zeros
        er(opi) = T(opi)-P(opi); 
        
        % compute J1
        switch metrics.J1_norm
            case 'L2'
                J1_out = norm(er(opi),2)/sqrt(numel(opi));
            case 'L1'
                J1_out = norm(er(opi),1)/(numel(opi));
            otherwise 
                error('ComputeCosts: wrong J1_norm value in the given metrics structure.')
        end    
        
        % compute J2
        J2_out = norm(er(opi),inf);
        
        % freq part        
        
        % compute fft of the gated signal -- we simply fill everything that is not OPI with zeros
        fft_E=fft(er.*gate,NFFT)/length(er);

        % smooth the fft and make it symmetric
        fft_E=fft_E.*freq_gate;

        J3_out=norm(fft_E(1:NFFT/2+1))/sqrt(NFFT/2+1);

    end

end


