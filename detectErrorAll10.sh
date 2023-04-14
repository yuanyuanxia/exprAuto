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

./detectErrorFourFPEDParallel.sh polyIDX0 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX0 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 origin
./detectErrorFourFPEDParallel.sh polyIDX0 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX0 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbie
./detectErrorFourFPEDParallel.sh polyIDX0 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX0 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbiePlus
./detectErrorFourFPEDParallel.sh polyIDX0 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX0 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 final
./detectErrorFourFPEDParallel.sh polyIDX1 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX1 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 origin
./detectErrorFourFPEDParallel.sh polyIDX1 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX1 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbie
./detectErrorFourFPEDParallel.sh polyIDX1 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX1 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbiePlus
./detectErrorFourFPEDParallel.sh polyIDX1 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX1 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbieArfa
./detectErrorFourFPEDParallel.sh polyIDX1 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX1 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 daisy
./detectErrorFourFPEDParallel.sh polyIDX1 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX1 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 daisyArfa
./detectErrorFourFPEDParallel.sh polyIDX2 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX2 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 origin
./detectErrorFourFPEDParallel.sh polyIDX2 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX2 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbie
./detectErrorFourFPEDParallel.sh polyIDX2 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX2 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbiePlus
./detectErrorFourFPEDParallel.sh polyIDX2 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX2 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 herbieArfa
./detectErrorFourFPEDParallel.sh polyIDX2 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX2 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 daisy
./detectErrorFourFPEDParallel.sh polyIDX2 3 9 1.5 5 -2 10 -1 3.5 16 16 16 16 0 0 0 0 3 1.5 -2 -1 0.375 0.21875 0.75 0.28125 expr_polyIDX2 3_9_1.5_5_-2_10_-1_3.5_16_16_16_16 daisyArfa
./detectErrorFiveFPEDParallel.sh pov_ray -1 1 -3.141592653589793 3.141592653589793 -3.141592653589793 3.141592653589793 -1 1 -1 1 8 8 8 8 8 0 0 0 0 0 -1 -3.141592653589793 -3.141592653589793 -1 -1 0.25 0.7853981633974483 0.7853981633974483 0.25 0.25 expr_pov_ray -1_1_-3.141592653589793_3.141592653589793_-3.141592653589793_3.141592653589793_-1_1_-1_1_8_8_8_8_8 origin
./detectErrorFiveFPEDParallel.sh pov_ray -1 1 -3.141592653589793 3.141592653589793 -3.141592653589793 3.141592653589793 -1 1 -1 1 8 8 8 8 8 0 0 0 0 0 -1 -3.141592653589793 -3.141592653589793 -1 -1 0.25 0.7853981633974483 0.7853981633974483 0.25 0.25 expr_pov_ray -1_1_-3.141592653589793_3.141592653589793_-3.141592653589793_3.141592653589793_-1_1_-1_1_8_8_8_8_8 herbie
./detectErrorFiveFPEDParallel.sh pov_ray -1 1 -3.141592653589793 3.141592653589793 -3.141592653589793 3.141592653589793 -1 1 -1 1 8 8 8 8 8 0 0 0 0 0 -1 -3.141592653589793 -3.141592653589793 -1 -1 0.25 0.7853981633974483 0.7853981633974483 0.25 0.25 expr_pov_ray -1_1_-3.141592653589793_3.141592653589793_-3.141592653589793_3.141592653589793_-1_1_-1_1_8_8_8_8_8 herbiePlus
./detectErrorFiveFPEDParallel.sh pov_ray -1 1 -3.141592653589793 3.141592653589793 -3.141592653589793 3.141592653589793 -1 1 -1 1 8 8 8 8 8 0 0 0 0 0 -1 -3.141592653589793 -3.141592653589793 -1 -1 0.25 0.7853981633974483 0.7853981633974483 0.25 0.25 expr_pov_ray -1_1_-3.141592653589793_3.141592653589793_-3.141592653589793_3.141592653589793_-1_1_-1_1_8_8_8_8_8 herbieArfa
./detectErrorFiveFPEDParallel.sh pov_ray -1 1 -3.141592653589793 3.141592653589793 -3.141592653589793 3.141592653589793 -1 1 -1 1 8 8 8 8 8 0 0 0 0 0 -1 -3.141592653589793 -3.141592653589793 -1 -1 0.25 0.7853981633974483 0.7853981633974483 0.25 0.25 expr_pov_ray -1_1_-3.141592653589793_3.141592653589793_-3.141592653589793_3.141592653589793_-1_1_-1_1_8_8_8_8_8 final
./detectErrorOneFPEDParallel.sh squareRoot3_2 0 10 500000 0 0 2e-05 expr_squareRoot3_2 0_10_500000 origin
./detectErrorOneFPEDParallel.sh squareRoot3_2 0 10 500000 0 0 2e-05 expr_squareRoot3_2 0_10_500000 herbie
./detectErrorOneFPEDParallel.sh squareRoot3_2 0 10 500000 0 0 2e-05 expr_squareRoot3_2 0_10_500000 herbiePlus
./detectErrorOneFPEDParallel.sh squareRoot3_2 0 10 500000 0 0 2e-05 expr_squareRoot3_2 0_10_500000 herbieArfa
./detectErrorOneFPEDParallel.sh squareRoot3_2 0 10 500000 0 0 2e-05 expr_squareRoot3_2 0_10_500000 final
./detectErrorOneFPEDParallel.sh squareRoot3Invalid_2 0 10 500000 0 0 2e-05 expr_squareRoot3Invalid_2 0_10_500000 origin
./detectErrorOneFPEDParallel.sh squareRoot3Invalid_2 0 10 500000 0 0 2e-05 expr_squareRoot3Invalid_2 0_10_500000 herbie
./detectErrorOneFPEDParallel.sh squareRoot3Invalid_2 0 10 500000 0 0 2e-05 expr_squareRoot3Invalid_2 0_10_500000 herbiePlus
./detectErrorOneFPEDParallel.sh squareRoot3Invalid_2 0 10 500000 0 0 2e-05 expr_squareRoot3Invalid_2 0_10_500000 herbieArfa
./detectErrorOneFPEDParallel.sh squareRoot3Invalid_2 0 10 500000 0 0 2e-05 expr_squareRoot3Invalid_2 0_10_500000 herbie
./detectErrorOneFPEDParallel.sh cav10_2 0 10 500000 0 0 2e-05 expr_cav10_2 0_10_500000 origin
./detectErrorOneFPEDParallel.sh cav10_2 0 10 500000 0 0 2e-05 expr_cav10_2 0_10_500000 herbie
./detectErrorOneFPEDParallel.sh cav10_2 0 10 500000 0 0 2e-05 expr_cav10_2 0_10_500000 herbiePlus
./detectErrorOneFPEDParallel.sh cav10_2 0 10 500000 0 0 2e-05 expr_cav10_2 0_10_500000 herbieArfa
./detectErrorOneFPEDParallel.sh cav10_2 0 10 500000 0 0 2e-05 expr_cav10_2 0_10_500000 final
./detectErrorOneFPEDParallel.sh smartRoot_2 -2 2 500000 0 -2 8e-06 expr_smartRoot_2 -2_2_500000 origin
./detectErrorOneFPEDParallel.sh smartRoot_2 -2 2 500000 0 -2 8e-06 expr_smartRoot_2 -2_2_500000 herbie
./detectErrorOneFPEDParallel.sh smartRoot_2 -2 2 500000 0 -2 8e-06 expr_smartRoot_2 -2_2_500000 herbiePlus
./detectErrorOneFPEDParallel.sh smartRoot_2 -2 2 500000 0 -2 8e-06 expr_smartRoot_2 -2_2_500000 herbieArfa
./detectErrorOneFPEDParallel.sh smartRoot_2 -2 2 500000 0 -2 8e-06 expr_smartRoot_2 -2_2_500000 final
./detectErrorThreeFPEDParallel.sh triangleSorted_2 1 9 1 9 1 9 256 256 256 0 0 0 1 1 1 0.03125 0.03125 0.03125 expr_triangleSorted_2 1_9_1_9_1_9_256_256_256 origin
./detectErrorThreeFPEDParallel.sh triangleSorted_2 1 9 1 9 1 9 256 256 256 0 0 0 1 1 1 0.03125 0.03125 0.03125 expr_triangleSorted_2 1_9_1_9_1_9_256_256_256 herbie
./detectErrorThreeFPEDParallel.sh triangleSorted_2 1 9 1 9 1 9 256 256 256 0 0 0 1 1 1 0.03125 0.03125 0.03125 expr_triangleSorted_2 1_9_1_9_1_9_256_256_256 herbiePlus
