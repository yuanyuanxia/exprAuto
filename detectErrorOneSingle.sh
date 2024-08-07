# Usage: ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${prefix} ${middle} ${suffix} ${errfile}
# set -x

path=`pwd`
cd ${path}
CC=gcc

uniqueLabel=${1} # unique number
if [ $# -eq 3 ]; then
    suffix=${2} # different version. Eg: herbie daisy origin temp_0_3 final
    x0=${3}
else
    echo "detectErrorOneSingle.sh: Invalid input parameters"
    exit
fi
prefix=expr_${uniqueLabel} # expr_${uniqueLabel}. Eg: expr_20221030155958

testFileName=test1paramSingle
numProcs=32

directory="./srcTest"/${uniqueLabel}
sourceFile=${prefix}_${suffix}

${CC} ./srcTest/${testFileName}.c ${directory}/${sourceFile}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
./${testFileName}.exe ${x0}
rm ${testFileName}.exe
