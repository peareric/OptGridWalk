% Eric Pearson
close all;
clear;
clc;

% Note: this script depends on the freely available third party
% implementation of Latin Hypercube sampling available at:
% https://www.mathworks.com/matlabcentral/fileexchange/4352-latin-hypercube-sampling
% The only modification of this source was to add 
%%% rng(16180339,'mt19937ar'); %%%
% to line 13 of lhs_empir.m for reproducability

%%
% Generate a empirical trucated rexponential PDF [0,1]
trunc_exp = @(lambda, x) lambda.*exp(-lambda.*x) ...
                            ./(1-exp(-lambda));
lambda = 5.0;
dist_values = repmat(trunc_exp(lambda, (0:0.01:1)),[8,1]);
% Sample from the truncated exponential distribution from the latin
% hypercube
samples = lhs_empir(dist_values', 292);
% Manually add strong preference to one direction for each direction
one_dir = ones(8);
for i = 1:8
    one_dir(i,i) = 63;
end
dir_probs = [one_dir; samples];
for i = 1:length(dir_probs)
    dir_probs(i,:) = dir_probs(i,:) ./ sum(dir_probs(i,:));
end
dir_probs
stats = [min(dir_probs,[],2), max(dir_probs,[],2), std(dir_probs,[],2)]
stats_stats = [min(stats(:,1)) max(stats(:,2)), mean(stats(:,3))]

% Generate PMF parameter input, repeat directional parameters for distance
% parameter of 1, 2, and 3
params = [[dir_probs, 1.*ones(300,1)]; ...
          [dir_probs, 2.*ones(300,1)]; ...
          [dir_probs, 3.*ones(300,1)]];
