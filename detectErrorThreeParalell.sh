# Usage: ./detectErrorThreeParalell.sh ${func} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size}
# set -x

path=`pwd`
cd ${path}

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
    prefix=${11} # herbie daisy origin tmp final
    middle=${12} # herbie daisy origin tmp final
    suffix=${13} # herbie daisy origin tmp final
elif [ $# -eq 10 ]; then
    x0Size=256
    x1Size=256
    x2Size=256
    prefix=${8} # herbie daisy origin tmp final
    middle=${9} # herbie daisy origin tmp final
    suffix=${10} # herbie daisy origin tmp final
else
    echo "detectErrorThreeParalell: Invalid input parameters"
    exit
fi
numProcs=32

echo "Detecting error: ${uniqueLabel} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix} ${middle} ${suffix}"

# echo "mpicc test3paramParallel.c ${prefix}_${suffix}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -O3 -o test3paramParallel.exe"
mpicc test3paramParallel.c ${prefix}_${suffix}.c ${prefix}_mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix}_ -DSUFFIX=${suffix} -lmpfr -lm -o test3paramParallel.exe
# echo "mpirun -n ${numProcs} ./test3paramParallel.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix}__${middle}_${suffix}"
mpirun -n ${numProcs} ./test3paramParallel.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size} ${prefix}__${middle}_${suffix}
rm test3paramParallel.exe
echo "end detecting ${func}"
echo
