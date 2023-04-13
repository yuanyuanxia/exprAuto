# Usage: ./detectErrorFourParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix} ${middle} ${suffix} ${errfile}
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
if [ $# -eq 29 ]; then
    x0Size=${10}
    x1Size=${11}
    x2Size=${12}
    x3Size=${13}
    x0startNowIdx=${14} # the index of the start point of the current interval.
    x1startNowIdx=${15}
    x2startNowIdx=${16}
    x3startNowIdx=${17}
    x0startOriginInterval=${18} # the value of the start point of the origin interval.
    x1startOriginInterval=${19}
    x2startOriginInterval=${20}
    x3startOriginInterval=${21}
    stepX0=${22} # the step for sampling points.
    stepX1=${23}
    stepX2=${24}
    stepX3=${25}
    prefix=${26} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${27} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${28} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=${29} # 1 or 0: TRUE or False
elif [ $# -eq 28 ]; then
    x0Size=${10}
    x1Size=${11}
    x2Size=${12}
    x3Size=${13}
    x0startNowIdx=${14} # the index of the start point of the current interval.
    x1startNowIdx=${15}
    x2startNowIdx=${16}
    x3startNowIdx=${17}
    x0startOriginInterval=${18} # the value of the start point of the origin interval.
    x1startOriginInterval=${19}
    x2startOriginInterval=${20}
    x3startOriginInterval=${21}
    stepX0=${22} # the step for sampling points.
    stepX1=${23}
    stepX2=${24}
    stepX3=${25}
    prefix=${26} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${27} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${28} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
elif [ $# -eq 24 ]; then
    x0Size=64
    x1Size=64
    x2Size=64
    x3Size=64
    x0startNowIdx=${10} # the index of the start point of the current interval.
    x1startNowIdx=${11}
    x2startNowIdx=${12}
    x3startNowIdx=${13}
    x0startOriginInterval=${14} # the value of the start point of the origin interval.
    x1startOriginInterval=${15}
    x2startOriginInterval=${16}
    x3startOriginInterval=${17}
    stepX0=${18} # the step for sampling points.
    stepX1=${19}
    stepX2=${20}
    stepX3=${21}
    prefix=${22} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${23} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${24} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
else
    echo "detectErrorFourParallel: Invalid input parameters"
    exit
fi
testFileName=test4paramFPEDParallel
numProcs=32

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x3End} ${x3Start} ${x0Size} ${x1Size} ${x2Size} ${x3Size} ${prefix} ${middle} ${suffix} ${errfile}"
directory="./srcTest"/${uniqueLabel}
sourceFile=${prefix}_${suffix}
fileNameKernel=${prefix}__${middle}_${suffix}

# echo "${CC} ${testFileName}.c ${sourceFile}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -DERRFILE=${errfile} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${fileNameKernel}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x3End} ${x3Start} ${x0Size} ${x1Size} ${x2Size} ${x3Size} ${x0startNowIdx} ${x1startNowIdx} ${x2startNowIdx} ${x3startNowIdx} ${x0startOriginInterval} ${x1startOriginInterval} ${x2startOriginInterval} ${x3startOriginInterval} ${stepX0} ${stepX1} ${stepX2} ${stepX3} ${fileNameKernel} ${uniqueLabel}
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
