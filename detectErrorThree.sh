# Usage: ./detectErrorThree.sh ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix} ${middle} ${suffix}
# set -x

path=`pwd`
cd ${path}
CC=gcc

uniqueLabel=${1} # unique number
x0Start=${2}
x0End=${3}
x1Start=${4}
x1End=${5}
x2Start=${6}
x2End=${7}
if [ $# -eq 13 ]; then
    x0Size=${8}
    x1Size=${9}
    x2Size=${10}
    prefix=${11} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${12} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${13} # different version. Eg: herbie daisy origin temp_0_3 final
elif [ $# -eq 10 ]; then
    x0Size=256
    x1Size=256
    x2Size=256
    prefix=${8}
    middle=${9}
    suffix=${10}
else
    echo "detectErrorThree: Invalid input parameters"
    exit
fi
testFileName=test3param

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix} ${middle} ${suffix}"

# echo "${CC} ${testFileName}.c ${prefix}_${suffix}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ${testFileName}.c ${prefix}_${suffix}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -o ${testFileName}.exe
# echo "./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix}__${middle}_${suffix}"
./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix}__${middle}_${suffix}
rm ${testFileName}.exe
echo "end detecting ${uniqueLabel}"
echo

