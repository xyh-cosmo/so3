function [ ] = test_inverse()
%TEST_INVERSE Tests prototype implementations of the inverse transform.
%   This compares all matlab prototypes of the inverse transform with the
%   ssht implementation and throws and error if the discrepancy exceeds
%   machine precision.

L = 8;
flm = random('unif', 0, 1, 1, L^2);

ssht = ssht_inverse(flm, L);
fine = 1;

result = inverse_direct(L, flm);
err = max(abs(ssht(:) - result(:)));
if err > 10^-14
    disp(['Implementation inverse_direct disagrees with ssht with errors up to ', num2str(err)]);
    fine = 0;
end

result = inverse_direct_v(L, flm);
err = max(abs(ssht(:) - result(:)));
if err > 10^-14
    disp(['Implementation inverse_direct_v disagrees with ssht with errors up to ', num2str(err)]);
    fine = 0;
end

result = inverse_direct_dl(L, flm);
err = max(abs(ssht(:) - result(:)));
if err > 10^-14
    disp(['Implementation inverse_direct_dl disagrees with ssht with errors up to ', num2str(err)]);
    fine = 0;
end

result = inverse_direct_dln(L, flm);
err = max(abs(ssht(:) - result(:)));
if err > 10^-14
    disp(['Implementation inverse_direct_dln disagrees with ssht with errors up to ', num2str(err)]);
    fine = 0;
end

if fine == 1
    disp('All implementations agree with ssht up to machine precision.');
end