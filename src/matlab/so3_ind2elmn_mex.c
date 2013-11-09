// SO3 package to perform Wigner transforms
// Copyright (C) 2013 Martin Büttner and Jason McEwen
// See LICENSE.txt for license details


#include <so3.h>
#include "so3_mex.h"
#include <string.h>
#include <stdlib.h>
#include "mex.h"


/**
 * Compute harmonic indices from array index.
 *
 * Usage:
 *   [el, m, n] = so3_ind2elmn(ind, L, N, order, storage, reality)
 *
 * \author Martin Büttner
 * \author Jason McEwen
 **/
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    int el, m, n, L, N, ind;
    int len, iin, iout = 0;
    char order[SO3_STRING_LEN], storage[SO3_STRING_LEN];
    int reality;

    so3_parameters_t parameters = {};

    /* Check number of arguments */
    if (nrhs != 6)
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:nrhs",
                          "Require six inputs.");
    if (nlhs != 3)
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:nlhs",
                          "Require three outputs.");

    /* Parse array index ind */
    iin = 0;
    if (!mxIsDouble(prhs[iin]) ||
        mxIsComplex(prhs[iin]) ||
        mxGetNumberOfElements(prhs[iin]) != 1)
    {
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:arrayIndex",
                          "Array index must be an integer.");
    }
    ind = (int)mxGetScalar(prhs[iin]);
    if (mxGetScalar(prhs[iin]) > (double)ind ||
        ind <= 0)
    {
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:arrayIndexNonInt",
                          "Array index must be a positive integer.");
    }

    /* Parse harmonic band-limit L */
    iin = 1;
    if (!mxIsDouble(prhs[iin]) ||
        mxIsComplex(prhs[iin]) ||
        mxGetNumberOfElements(prhs[iin]) != 1)
    {
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:harmonicBandLimit",
                          "Harmonic band-limit must be an integer.");
    }
    L = (int)mxGetScalar(prhs[iin]);
    if (mxGetScalar(prhs[iin]) > (double)L ||
        L <= 0)
    {
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:harmonicBandLimitNonInt",
                          "Harmonic band-limit must be a positive integer.");
    }

    /* Parse orientational band-limit N */
    iin = 2;
    if (!mxIsDouble(prhs[iin]) ||
        mxIsComplex(prhs[iin]) ||
        mxGetNumberOfElements(prhs[iin]) != 1)
    {
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:orientationalBandLimit",
                          "Orientational band-limit must be an integer.");
    }
    N = (int)mxGetScalar(prhs[iin]);
    if (mxGetScalar(prhs[iin]) > (double)N ||
        N <= 0)
    {
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:orientationalBandLimitNonInt",
                          "Orientational band-limit must be a positive integer.");
    }

    /* Parse storage order */
    iin = 3;
    if (!mxIsChar(prhs[iin]))
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:orderChar",
                          "Storage order must be a string.");

    len = mxGetM(prhs[iin]) * mxGetN(prhs[iin]) + 1;
    if (len >= SO3_STRING_LEN)
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:orderTooLong",
                          "Storage order exceeds maximum string length.");
    mxGetString(prhs[iin], order, len);

    /* Parse storage type */
    iin = 4;
    if (!mxIsChar(prhs[iin]))
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:storageChar",
                          "Storage type must be a string.");

    len = mxGetM(prhs[iin]) * mxGetN(prhs[iin]) + 1;
    if (len >= SO3_STRING_LEN)
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:storageTooLong",
                          "Storage type exceeds maximum string length.");
    mxGetString(prhs[iin], storage, len);

    /* Parse reality. */
    iin = 5;
    if( !mxIsLogicalScalar(prhs[iin]) )
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:reality",
                          "Reality flag must be logical.");
    reality = mxIsLogicalScalarTrue(prhs[iin]);

    parameters.L = L;
    parameters.N = N;

    if (strcmp(storage, SO3_STORAGE_PADDED) == 0)
    {
        if (reality && ind > N*L*L)
            mexErrMsgIdAndTxt("so3_ind2elmn:InvalidInput:indOutOfRange",
                              "The array index must lie between 1 and N*L*L.");
        if (!reality && ind > (2*N-1)*L*L)
            mexErrMsgIdAndTxt("so3_ind2elmn:InvalidInput:indOutOfRange",
                              "The array index must lie between 1 and (2*N-1)*L*L.");

        if (strcmp(order, SO3_ORDER_ZEROFIRST) == 0)
            parameters.storage = SO3_STORE_ZERO_FIRST_PAD;
        else if (strcmp(order, SO3_ORDER_NEGFIRST) == 0)
            parameters.storage = SO3_STORE_NEG_FIRST_PAD;
        else
            mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:order",
                              "Invalid storage order.");
    }
    else if (strcmp(storage, SO3_STORAGE_COMPACT) == 0)
    {
        if (reality && ind > N*(6*L*L-(N-1)*(2*N-1))/6)
            mexErrMsgIdAndTxt("so3_ind2elmn:InvalidInput:indOutOfRange",
                              "The array index must lie between 1 and N*(6*L*L-(N-1)*(2*N-1))/6.");
        if (!reality && ind > (2*N-1)*(3*L*L-N*(N-1))/3)
            mexErrMsgIdAndTxt("so3_ind2elmn:InvalidInput:indOutOfRange",
                              "The array index must lie between 1 and (2*N-1)*(3*L*L-N*(N-1))/3.");

        if (strcmp(order, SO3_ORDER_ZEROFIRST) == 0)
            parameters.storage = SO3_STORE_ZERO_FIRST_COMPACT;
        else if (strcmp(order, SO3_ORDER_NEGFIRST) == 0)
            parameters.storage = SO3_STORE_NEG_FIRST_COMPACT;
        else
            mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:order",
                              "Invalid storage order.");
    }
    else
        mexErrMsgIdAndTxt("so3_ind2elmn_mex:InvalidInput:storage",
                          "Invalid storage type.");

    --ind; // Adjust index from MATLAB-style 1-based to C-style 0-based
    if (reality)
        so3_sampling_ind2elmn_real(&el, &m, &n, ind, &parameters);
    else
        so3_sampling_ind2elmn(&el, &m, &n, ind, &parameters);

    plhs[iout++] = mxCreateDoubleScalar(el);
    plhs[iout++] = mxCreateDoubleScalar(m);
    plhs[iout++] = mxCreateDoubleScalar(n);
}
