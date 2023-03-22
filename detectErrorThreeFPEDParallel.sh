# Usage: ./detectErrorThreeParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix} ${middle} ${suffix} ${errfile}
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
if [ $# -eq 14 ]; then
    x0Size=${8}
    x1Size=${9}
    x2Size=${10}
    prefix=${11} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${12} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${13} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=${14} # 1 or 0: TRUE or False
elif [ $# -eq 13 ]; then
    x0Size=${8}
    x1Size=${9}
    x2Size=${10}
    prefix=${11} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${12} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${13} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
elif [ $# -eq 10 ]; then
    x0Size=256
    x1Size=256
    x2Size=256
    prefix=${8}
    middle=${9}
    suffix=${10}
    errfile=0 # 1 or 0: TRUE or False
else
    echo "detectErrorThreeParallel: Invalid input parameters"
    exit
fi
testFileName=test3paramFPEDParallel
numProcs=32

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix} ${middle} ${suffix} ${errfile}"
directory="./srcTest"/${uniqueLabel}
sourceFile=${prefix}_${suffix}
fileNameKernel=${prefix}__${middle}_${suffix}

# echo "${CC} ${testFileName}.c ${sourceFile}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -DERRFILE=${errfile} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${fileNameKernel}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${fileNameKernel} ${uniqueLabel}
rm ${testFileName}.exe

# combine files
if [ ${errfile} -eq 1 ]; then
    cd ./outputs/${uniqueLabel}
    findWord="${fileNameKernel}_sample_*.txt"
    # echo "For suffix = ${suffix}, Find and combine by shell command cat:  ${findWord}"
    find . -name "${findWord}" | sort -h | xargs cat > sample_${uniqueLabel}_${suffix}.txt
    # echo "sample file: `pwd`/sample_${uniqueLabel}_${suffix}.txt"
    rm ${findWord}
    cd - > /dev/null
fi

echo "end detecting ${uniqueLabel}"
echo
