#!/bin/bash
# Usage: ./sampleTwoFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${prefix} ${middle} ${suffix}
# set -x

path=`pwd`
cd ${path}
CC=mpicc

uniqueLabel=${1} # unique number
x0Start=${2}
x0End=${3}
x1Start=${4}
x1End=${5}
x2Start=${6}
x2End=${7}
x3Start=${8}
x3End=${9}
x4Start=${10}
x4End=${11}

if [ $# -eq 16 ]; then
    drawNum=${12}
    findMaxNum=${13}
    prefix=${14} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${15} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${16} # different version. Eg: herbie daisy origin temp_0_3 final
elif [ $# -eq 1 ]; then
    drawNum=384
    findMaxNum=32
    prefix=${12}
    middle=${13}
    suffix=${14}
else
    echo "sampleFiveFPEDParallel: Invalid input parameters"
    exit
fi

testFileName=sample5paramFPEDParallel
numProcs=32

# echo "sampling: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${drawNum} ${findMaxNum} ${prefix} ${middle} ${suffix} ${errfile}"
directory="./srcTest"/${uniqueLabel}
sourceFile=${prefix}_${suffix}
fileNameKernel=${prefix}__${middle}_${suffix}

${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x3Start} ${x3End} ${x4Start} ${x4End} ${drawNum} ${findMaxNum} ${fileNameKernel} ${uniqueLabel}
rm ${testFileName}.exe

# combine files
cd ./outputs/${uniqueLabel}
## for sample_X
findWord="${fileNameKernel}_sample_X_*.txt"
# echo "For suffix = ${suffix}, Find and combine by shell command cat:  ${findWord}"
find . -name "${findWord}" | sort -h | xargs cat > sample_${uniqueLabel}_${suffix}_X.txt
rm ${findWord}
## for sample_Y
findWord="${fileNameKernel}_sample_Y_*.txt"
# echo "For suffix = ${suffix}, Find and combine by shell command cat:  ${findWord}"
find . -name "${findWord}" | sort -h | xargs cat > sample_${uniqueLabel}_${suffix}_Y.txt
rm ${findWord}
## for sample_Z
findWord="${fileNameKernel}_sample_Z_*.txt"
# echo "For suffix = ${suffix}, Find and combine by shell command cat:  ${findWord}"
find . -name "${findWord}" | sort -h | xargs cat > sample_${uniqueLabel}_${suffix}_Z.txt
rm ${findWord}
## for sample_U
findWord="${fileNameKernel}_sample_U_*.txt"
# echo "For suffix = ${suffix}, Find and combine by shell command cat:  ${findWord}"
find . -name "${findWord}" | sort -h | xargs cat > sample_${uniqueLabel}_${suffix}_U.txt
rm ${findWord}
## for sample_V
findWord="${fileNameKernel}_sample_V_*.txt"
# echo "For suffix = ${suffix}, Find and combine by shell command cat:  ${findWord}"
find . -name "${findWord}" | sort -h | xargs cat > sample_${uniqueLabel}_${suffix}_V.txt
rm ${findWord}
cd - > /dev/null

# echo "end sampling ${uniqueLabel}"
echo