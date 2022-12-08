# !/bin/bash
# Usage: ./detectErrorTwo.sh ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${prefix} ${middle} ${suffix}
# set -x

path=`pwd`
cd ${path}
CC=gcc

uniqueLabel=${1} # unique number
x0Start=${2}
x0End=${3}
x1Start=${4}
x1End=${5}

if [ $# -eq 10 ]; then
    x0Size=${6}
    x1Size=${7}
    prefix=${8} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${9} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${10} # different version. Eg: herbie daisy origin temp_0_3 final
elif [ $# -eq 8 ]; then
    x0Size=1024
    x1Size=1024
    prefix=${6}
    middle=${7}
    suffix=${8}
else
    echo "detectErrorTwo: Invalid input parameters"
    exit
fi

testFileName=test2paramFPED

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${prefix} ${middle} ${suffix}"

${CC} ${testFileName}.c ${prefix}_${suffix}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -o ${testFileName}.exe

./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${prefix}__${middle}_${suffix}
rm ${testFileName}.exe
echo "end detecting ${uniqueLabel}"
echo
