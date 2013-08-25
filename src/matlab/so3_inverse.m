function [f] = so3_inverse(flmn, L, N, varargin)
% so3_inverse - Compute inverse Wigner transform
%
% Computes inverse Wigner transform.
%
% Default usage is given by
%
%   f = so3_inverse(flmn, L, N, <options>)
%
% where L and N are the harmonic and orientational band-limit,
% respectively, flmn is the vector of  harmonic coefficients and
% f is the sampled function values indexed by gamma, beta, alpha.
%
% Options consist of parameter type and value pairs.  Valid options
% include:
%
%   'Order'   = { 'ZeroFirst'     [el-em blocks are stored in en-order
%                                  0, -1, 1, -2, 2, ... (default)],
%                 'NegativeFirst' [el-em blocks are stored in en-order
%                                  ..., -2, -1, 0, 1, 2, ...] }
%   'Storage' = { 'Padded'        [indices for el < en are zero (default)],
%                 'Compact'       [indices for el < en are omitted] }
%
% Authors: Martin Buettner (m.buettner.d@gmail.com)
%          Jason McEwen (www.jasonmcewen.org)

% SO3 package to perform Wigner transforms
% Copyright (C) 2013 Martin Buettner and Jason McEwen
% See LICENSE.txt for license details

% Parse arguments.
p = inputParser;
p.addRequired('flmn', @isnumeric);
p.addRequired('L', @isnumeric);
p.addRequired('N', @isnumeric);
p.addParamValue('Order', 'ZeroFirst', @ischar);
p.addParamValue('Storage', 'Padded', @ischar);
p.parse(flmn, L, N, varargin{:});
args = p.Results;

% Computing inverse transform.
[f] = ...
    so3_inverse_mex(flmn, L, N, args.Order, args.Storage);