#!/bin/bash
# set -x
if [ $# -eq 1 ]; then
    target=${1}
else
    target=NumOpt
fi

if [ $target == "origin" ]; then
    suffix=""
elif [ $target == "NumOpt" ]; then
    suffix="NumOpt"
else
    echo "Invalid target"
    exit
fi

# DONE
# ./detectErrorOneFPEDParallel${suffix}.sh Bsplines3 0 1 500000 0 0 2e-06 expr_Bsplines3 0_1_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh exp1x 0.01 0.5 500000 0 0.01 9.8e-07 expr_exp1x 0.01_0.5_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh logexp -8 8 500000 0 -8 3.2e-05 expr_logexp -8_8_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample31 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample31 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample34 0.01 3 500000 0 0.01 5.98e-06 expr_NMSEexample34 0.01_3_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample35 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample35 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample36 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample36 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample37 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample37 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample38 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample38 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample39 0.01 1 500000 0 0.01 1.98e-06 expr_NMSEexample39 0.01_1_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem331 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem331 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem333 0.01 0.99 500000 0 0.01 1.96e-06 expr_NMSEproblem333 0.01_0.99_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem336 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem336 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem337 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem337 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem341 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem341 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem343 -1 1 500000 0 -1 4e-06 expr_NMSEproblem343 -1_1_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem344 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem344 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEsection311 0.01 100 500000 0 0.01 0.00019998 expr_NMSEsection311 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh predatorPrey 0.1 0.3 500000 0 0.1 4e-07 expr_predatorPrey 0.1_0.3_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh sine -1.57079632679 1.57079632679 500000 0 -1.57079632679 6.28318530716e-06 expr_sine -1.57079632679_1.57079632679_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh sineorder3 -2 2 500000 0 -2 8e-06 expr_sineorder3 -2_2_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh sqroot 0 1 500000 0 0 2e-06 expr_sqroot 0_1_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh test05_nonlin1_r4 1.00001 2 500000 0 1.00001 1.9999799999999998e-06 expr_test05_nonlin1_r4 1.00001_2_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh test05_nonlin1_test2 1.00001 2 500000 0 1.00001 1.9999799999999998e-06 expr_test05_nonlin1_test2 1.00001_2_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh verhulst 0.1 0.3 500000 0 0.1 4e-07 expr_verhulst 0.1_0.3_500000 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh ComplexSinCos 0 1 0 1 1024 1024 0 0 0 0 0.0009765625 0.0009765625 expr_ComplexSinCos 0_1_0_1_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh ComplexSquareRoot 0 1 0 1 1024 1024 0 0 0 0 0.0009765625 0.0009765625 expr_ComplexSquareRoot 0_1_0_1_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh NMSEexample33 0 100 0 100 1024 1024 0 0 0 0 0.09765625 0.09765625 expr_NMSEexample33 0_100_0_100_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh NMSEproblem332 0 100 0 100 1024 1024 0 0 0 0 0.09765625 0.09765625 expr_NMSEproblem332 0_100_0_100_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh NMSEproblem335 0 100 0 100 1024 1024 0 0 0 0 0.09765625 0.09765625 expr_NMSEproblem335 0_100_0_100_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh NMSEsection35 0 1 0 1 1024 1024 0 0 0 0 0.0009765625 0.0009765625 expr_NMSEsection35 0_1_0_1_1024_1024 ${target}

# problem error are too big
# ./detectErrorOneFPEDParallel${suffix}.sh intro_example 0 999 500000 0 0 0.001998 expr_intro_example 0_999_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh sqrt_add 1 1000 500000 0 1 0.001998 expr_sqrt_add 1_1000_500000 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh polarToCarthesianX 1 10 0 360 1024 1024 0 0 1 0 0.0087890625 0.3515625 expr_polarToCarthesianX 1_10_0_360_1024_1024 ${target}
# ./detectErrorThreeFPEDParallel${suffix}.sh doppler1 -30 50 -100 100 20 20000 256 256 256 0 0 0 -30 -100 20 0.3125 0.78125 78.046875 expr_doppler1 -30_50_-100_100_20_20000_256_256_256 ${target}
# ./detectErrorThreeFPEDParallel${suffix}.sh doppler2 -40 60 -125 125 15 25000 256 256 256 0 0 0 -40 -125 15 0.390625 0.9765625 97.59765625 expr_doppler2 -40_60_-125_125_15_25000_256_256_256 ${target}
# ./detectErrorThreeFPEDParallel${suffix}.sh doppler3 -50 30 -30 120 320 20300 256 256 256 0 0 0 -50 -30 320 0.3125 0.5859375 78.046875 expr_doppler3 -50_30_-30_120_320_20300_256_256_256 ${target}

# problem compile error
# ./detectErrorTwoFPEDParallel${suffix}.sh i4 0.1 10 -5 5 1024 1024 0 0 0.1 -5 0.00966796875 0.009765625 expr_i4 0.1_10_-5_5_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh i6 0.1 10 -5 5 1024 1024 0 0 0.1 -5 0.00966796875 0.009765625 expr_i6 0.1_10_-5_5_1024_1024 ${target}

# TODO: NO FILE
# ./detectErrorOneFPEDParallel${suffix}.sh exp1x_log 0.01 0.5 500000 0 0.01 9.8e-07 expr_exp1x_log 0.01_0.5_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEexample310 -1 1 500000 0 -1 4e-06 expr_NMSEexample310 -1_1_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem334 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem334 0.01_100_500000 ${target}
# ./detectErrorOneFPEDParallel${suffix}.sh NMSEproblem345 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem345 0.01_100_500000 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh hypot32 1 100 1 100 1024 1024 0 0 1 1 0.0966796875 0.0966796875 expr_hypot32 1_100_1_100_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh NMSEproblem346 1 2 0 1 1024 1024 0 0 1 0 0.0009765625 0.0009765625 expr_NMSEproblem346 1_2_0_1_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh polarToCarthesianY 1 10 0 360 1024 1024 0 0 1 0 0.0087890625 0.3515625 expr_polarToCarthesianY 1_10_0_360_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh sec4example 1.001 2 1.001 2 1024 1024 0 0 1.001 1.001 0.0009755859375000001 0.0009755859375000001 expr_sec4example 1.001_2_1.001_2_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh test03_nonlin2 0 1 -1 -0.1 1024 1024 0 0 0 -1 0.0009765625 0.00087890625 expr_test03_nonlin2 0_1_-1_-0.1_1024_1024 ${target}
# ./detectErrorTwoFPEDParallel${suffix}.sh theta 1 100 1 100 1024 1024 0 0 1 1 0.0966796875 0.0966796875 expr_theta 1_100_1_100_1024_1024 ${target}
# ./detectErrorThreeFPEDParallel${suffix}.sh turbine1 3.8 7.8 -4.5 -0.3 0.4 0.9 256 256 256 0 0 0 3.8 -4.5 0.4 0.015625 0.01640625 0.001953125 expr_turbine1 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256 ${target}