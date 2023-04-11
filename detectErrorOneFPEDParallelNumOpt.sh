# Usage: ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${prefix} ${middle} ${suffix} ${errfile}
# set -x

path=`pwd`
cd ${path}
CC=gcc
CPP=g++

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

# compile the NumOpt version
${CPP} -c -IincludeNumOpt -o objs/points.o srcNumOpt/points.cpp
${CPP} -c -IincludeNumOpt -o objs/${sourceFile}.o ${directory}/${sourceFile}.cpp
${CC} -c -o objs/${prefix}_mpfr.o ${directory}/${prefix}_mpfr.c
${CC} -c -IincludeTEST -o objs/computeULP.o ./srcTest/computeULP.c 
mpic++ -c -IincludeNumOpt -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -DERRFILE=${errfile} -o objs/${testFileName}.o ./srcTest/${testFileName}.c
mpic++ objs/${testFileName}.o objs/${sourceFile}.o objs/${prefix}_mpfr.o objs/computeULP.o objs/points.o -Xlinker -rpath -Xlinker libs -Llibs -liRRAM -lgmp -lpthread -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe

# run the NumOpt version
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x0Size} ${fileNameKernel}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x0Size} ${x0startNowIdx} ${x0startOriginInterval} ${stepX0} ${fileNameKernel} ${uniqueLabel}
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

echo "end detecting error ${uniqueLabel}"
echo
