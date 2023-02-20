# Usage: ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${prefix} ${middle} ${suffix}
# set -x

path=`pwd`
cd ${path}
CC=mpicc

uniqueLabel=${1} # unique number
x0Start=${2}
x0End=${3}
if [ $# -eq 7 ]; then
    x0Size=${4}
    prefix=${5} # expr_${uniqueLabel}. Eg: expr_20221030155958
    middle=${6} # intervalsInfo_sizes. Eg: 3.8_7.8_500000
    suffix=${7} # different version. Eg: herbie daisy origin temp_0_3 final
elif [ $# -eq 6 ]; then
    x0Size=500000
    prefix=${4}
    middle=${5}
    suffix=${6}
else
    echo "detectErrorOneFPEDParallel: Invalid input parameters"
    exit
fi
testFileName=test1paramFPEDParallel
numProcs=32

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${prefix} ${middle} ${suffix}"
directory="./srcTest"/${uniqueLabel}
# echo "${CC} ${testFileName}.c ${prefix}_${suffix}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o ${testFileName}.exe"
${CC} ./srcTest/${testFileName}.c ${directory}/${prefix}_${suffix}.c ${directory}/${prefix}_mpfr.c ./srcTest/computeULP.c -IincludeTEST -IincludeDD -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -Llibs -lTGen -lmpfr -lm -lqd -o ${testFileName}.exe
# echo "mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x0Size} ${prefix}__${middle}_${suffix}"
mpirun -n ${numProcs} ./${testFileName}.exe ${x0Start} ${x0End} ${x0Size} ${prefix}__${middle}_${suffix} ${uniqueLabel}
# mv outputs/${prefix}__${middle}_${suffix}_error.txt ./outputs/${uniqueLabel}/${prefix}__${middle}_${suffix}_error.txt
rm ${testFileName}.exe
echo "end detecting ${uniqueLabel}"
echo
