#!/bin/bash
set -x
target=NumOpt
./detectErrorOneFPEDParallel${target}.sh Bsplines3 0 1 500000 0 0 2e-06 expr_Bsplines3 0_1_500000 ${target}