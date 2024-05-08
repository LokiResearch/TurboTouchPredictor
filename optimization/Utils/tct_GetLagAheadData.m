% This Source Code is subject to the terms of
% TURBOTOUCH PREDICTOR version 1.0 licence
% available in LICENCE.txt
% Author: Stanislav Aranovskiy

function [ data_ahead ] = tct_GetLagAheadData( time, data, L )
%GETLAGAHEADDATA Construct Lag-ahead data from the input
    data_ahead=interp1(time,data,time+L,'linear',data(end));
end

