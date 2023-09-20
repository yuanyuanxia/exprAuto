# Usage: ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${prefix} ${middle} ${suffix} ${errfile}
# set -x

path=`pwd`
cd ${path}
CC=mpicc
ERRORMODEL=ULP

uniqueLabel=${1} # unique number
x0Start=${2}
x0End=${3}
if [ $# -eq 11 ]; then
    x0Size=${4}
    x0startNowIdx=${5} # the index of the start point of the current interval.
    x0startOriginInterval=${6} # the value of the start point of the origin interval.
    stepX0=${7} # the step for sampling points.
    prefix=${8} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${9} # intervalsInfo_sizes. Eg: 3.8_7.8_500000
    suffix=${10} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=${11} # 1 or 0: TRUE or False
elif [ $# -eq 10 ]; then
    x0Size=${4}
    x0startNowIdx=${5} # the index of the start point of the current interval.
    x0startOriginInterval=${6} # the value of the start point of the origin interval.
    stepX0=${7} # the step for sampling points.
    prefix=${8} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${9} # intervalsInfo_sizes. Eg: 3.8_7.8_500000
    suffix=${10} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0
elif [ $# -eq 9 ]; then
    x0Size=500000
    x0startNowIdx=${5} # the index of the start point of the current interval.
    x0startOriginInterval=${6} # the value of the start point of the origin interval.
    stepX0=${7} # the step for sampling points.
    prefix=${8}
    middle=${9}
    suffix=${10}
    errfile=0
else
    echo "detectErrorOneFPEDParallel: Invalid input parameters"
    exit
fi

testFileName=test1paramFPEDParallel
numProcs=32

# echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${prefix} ${middle} ${suffix} ${errfile}"
directory="./srcTest"/${uniqueLabel}
sourceFile=${prefix}_${suffix}
fileNameKernel=${prefix}__${middle}_${suffix}

# echo "${CC} ${testFileName}.c ${sourceFile}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -DERRFILE=${errfile} -D${ERRORMODEL} -Llibs -lTGen -lmpfr -lm -lqd -mfma -o ${testFileName}.exe
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x0Size} ${fileNameKernel}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x0Size} ${x0startNowIdx} ${x0startOriginInterval} ${stepX0} ${fileNameKernel} ${uniqueLabel}
# mv outputs/${fileNameKernel}_error.txt ./outputs/${uniqueLabel}/${fileNameKernel}_error.txt
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

# echo "end detecting error ${uniqueLabel}"
# echo
