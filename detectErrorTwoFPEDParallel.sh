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

if [ $# -eq 11 ]; then
    x0Size=${6}
    x1Size=${7}
    prefix=${8} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${9} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${10} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=${11} # 1 or 0: TRUE or False
elif [ $# -eq 10 ]; then
    x0Size=${6}
    x1Size=${7}
    prefix=${8} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${9} # intervalsInfo_sizes. Eg: 3.8_7.8_-4.5_-0.3_0.4_0.9_256_256_256
    suffix=${10} # different version. Eg: herbie daisy origin temp_0_3 final
    errfile=0 # 1 or 0: TRUE or False
elif [ $# -eq 8 ]; then
    x0Size=1024
    x1Size=1024
    prefix=${6}
    middle=${7}
    suffix=${8}
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
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size} ${fileNameKernel} ${uniqueLabel}
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
