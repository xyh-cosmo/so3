function [ f ] = inverse_for_fft( L, flm )
%INVERSE_FOR_FFT Inverse spherical harmonic transform using matlab's ifft
%and a factoring of rotations approach.
%.
%   This computes the inverse spherical harmonic transform, synthesising 
%   the function using factoring of rotations (as outlined in equations
%   (12) to (14) of the ssht paper). It also employs matlab's ifft to 
%   compute the DFT.
%
%   L   ... the band limit (maximum l is L-1)
%   flm ... the coefficients of the spherical harmonics
%           to avoid wasting memory, these should be supplied in an
%           unrolled array of the following format:
%           [(0,0) (1,-1) (1,0) (1,1) (2,-2) (2,-1) ... ]
%           of size L^2, where the first number corresponds to l
%           and the second to m.

if length(flm) ~= L^2
    error('Parameter flm has to contain L^2 coefficients.')
end

[thetas, phis] = ssht_sampling(L);

thetasExt = [thetas; 2*pi - thetas(end-1:-1:1)];

f = zeros(length(thetas), length(phis));
fmm = zeros(2*L-1, 2*L-1);
m0i = L;

dl = zeros(2*L-1,2*L-1);
for l=0:L-1,
    dl = ssht_dl(dl, L, l, pi/2);
    
    lm0i = l^2+1+l;
    for m=-l:l,
        sign = 1i^(-m);
        for mp=-l:l, % m' in the equation (14)
            fmm(m0i+m,m0i+mp) = fmm(m0i+m,m0i+mp) +...
                sign*sqrt((2*l+1)/(4*pi))*...
                dl(m0i+mp,m0i+m)*dl(m0i+mp,m0i)*flm(lm0i+m) *...
                 exp(1i*(mp)*pi/(2*L-1));
%                 exp(1i*(mp+L)*pi/(2*L-1));
        end
    end
end

% fm = zeros(2*L-1, length(thetasExt));
% 
% for m=-L+1:L-1,
%     fm(m0i+m,:) = ifft(fmm(m0i+m,:)).*(2*L-1).*exp(-1i.*((L-1).*(thetasExt.')+pi/(2*L-1)));
% end
% 
% for j=1:length(thetas),
%     f(j,:) = ifft(fm(:,j).').*(2*L-1).*exp(-1i.*(L-1).*phis.');
% end

f = ifft2(ifftshift(fmm.')).*(2*L-1)^2;
f = f(1:L,:);
