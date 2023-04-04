# Usage: ./detectErrorTwoFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${prefix} ${middle} ${suffix} ${errfile}
# set -x

path=`pwd`
cd ${path}
CC=mpicc

uniqueLabel=${1} # unique number
x0Start=${2}
x0End=${3}
x1Start=${4}
x1End=${5}

if [ $# -eq 17 ]; then
    x0Size=${6}
    x1Size=${7}
    x0startNowIdx=${8} # the index of the start point of the current interval.
    x1startNowIdx=${9}
    x0startOriginInterval=${10} # the value of the start point of the origin interval.
    x1startOriginInterval=${11}
    stepX0=${12} # the step for sampling points.
    stepX1=${13}
    prefix=${14} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${15} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${16} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=${17} # 1 or 0: TRUE or False
elif [ $# -eq 16 ]; then
    x0Size=${6}
    x1Size=${7}
    x0startNowIdx=${8} # the index of the start point of the current interval.
    x1startNowIdx=${9}
    x0startOriginInterval=${10} # the value of the start point of the origin interval.
    x1startOriginInterval=${11}
    stepX0=${12} # the step for sampling points.
    stepX1=${13}
    prefix=${14} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${15} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${16} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
elif [ $# -eq 14 ]; then
    x0Size=1024
    x1Size=1024
    x0startNowIdx=${6} # the index of the start point of the current interval.
    x1startNowIdx=${7}
    x0startOriginInterval=${8} # the value of the start point of the origin interval.
    x1startOriginInterval=${9}
    stepX0=${10} # the step for sampling points.
    stepX1=${11}
    prefix=${12}
    middle=${13}
    suffix=${14}
    errfile=0 # 1 or 0: TRUE or False
else
    echo "detectErrorTwoFPEDParallel: Invalid input parameters"
    exit
fi

testFileName=test2paramFPEDParallel
numProcs=32

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${prefix} ${middle} ${suffix} ${errfile}"
directory="./srcTest"/${uniqueLabel}
suffixClean=`echo ${suffix} | sed 's@_x\|_y\|_z@@g'`
sourceFile=${prefix}_${suffixClean}
fileNameKernel=${prefix}__${middle}_${suffix}

# echo "${CC} ${testFileName}.c ${sourceFile}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffixClean} -DERRFILE=${errfile} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${fileNameKernel}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${x0startNowIdx} ${x1startNowIdx} ${x0startOriginInterval} ${x1startOriginInterval} ${stepX0} ${stepX1} ${fileNameKernel} ${uniqueLabel}
rm ${testFileName}.exe

# TODO: 进一步配适多参采样测试
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
