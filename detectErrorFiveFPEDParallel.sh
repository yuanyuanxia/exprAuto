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
x4Start=${10}
x4End=${11}
if [ $# -eq 35 ]; then
    x0Size=${12}
    x1Size=${13}
    x2Size=${14}
    x3Size=${15}
    x4Size=${16}
    x0startNowIdx=${17} # the index of the start point of the current interval.
    x1startNowIdx=${18}
    x2startNowIdx=${19}
    x3startNowIdx=${20}
    x4startNowIdx=${21}
    x0startOriginInterval=${22} # the value of the start point of the origin interval.
    x1startOriginInterval=${23}
    x2startOriginInterval=${24}
    x3startOriginInterval=${25}
    x4startOriginInterval=${26}
    stepX0=${27} # the step for sampling points.
    stepX1=${28}
    stepX2=${29}
    stepX3=${30}
    stepX4=${31}
    prefix=${32} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${33} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${34} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=${35} # 1 or 0: TRUE or False
elif [ $# -eq 34 ]; then
    x0Size=${12}
    x1Size=${13}
    x2Size=${14}
    x3Size=${15}
    x4Size=${16}
    x0startNowIdx=${17} # the index of the start point of the current interval.
    x1startNowIdx=${18}
    x2startNowIdx=${19}
    x3startNowIdx=${20}
    x4startNowIdx=${21}
    x0startOriginInterval=${22} # the value of the start point of the origin interval.
    x1startOriginInterval=${23}
    x2startOriginInterval=${24}
    x3startOriginInterval=${25}
    x4startOriginInterval=${26}
    stepX0=${27} # the step for sampling points.
    stepX1=${28}
    stepX2=${29}
    stepX3=${30}
    stepX4=${31}
    prefix=${32} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${33} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${34} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
elif [ $# -eq 29 ]; then
    x0Size=32
    x1Size=32
    x2Size=32
    x3Size=32
    x0startNowIdx=${12} # the index of the start point of the current interval.
    x1startNowIdx=${13}
    x2startNowIdx=${14}
    x3startNowIdx=${15}
    x4startNowIdx=${16}
    x0startOriginInterval=${17} # the value of the start point of the origin interval.
    x1startOriginInterval=${18}
    x2startOriginInterval=${19}
    x3startOriginInterval=${20}
    x4startOriginInterval=${21}
    stepX0=${22} # the step for sampling points.
    stepX1=${23}
    stepX2=${24}
    stepX3=${25}
    stepX4=${26}
    prefix=${27} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${28} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${29} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
else
    echo "detectErrorFiveParallel: Invalid input parameters"
    exit
fi
testFileName=test5paramFPEDParallel
numProcs=32

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x3End} ${x3Start} ${x4End} ${x4Start} ${x0Size} ${x1Size} ${x2Size} ${x3Size} ${x4Size} ${prefix} ${middle} ${suffix} ${errfile}"
directory="./srcTest"/${uniqueLabel}
sourceFile=${prefix}_${suffix}
fileNameKernel=${prefix}__${middle}_${suffix}

# echo "${CC} ${testFileName}.c ${sourceFile}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -DERRFILE=${errfile} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${fileNameKernel}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x3End} ${x3Start} ${x4End} ${x4Start} ${x0Size} ${x1Size} ${x2Size} ${x3Size} ${x4Size} ${x0startNowIdx} ${x1startNowIdx} ${x2startNowIdx} ${x3startNowIdx} ${x4startNowIdx} ${x0startOriginInterval} ${x1startOriginInterval} ${x2startOriginInterval} ${x3startOriginInterval} ${x4startOriginInterval} ${stepX0} ${stepX1} ${stepX2} ${stepX3} ${stepX4} ${fileNameKernel} ${uniqueLabel}
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
