# Usage: ./detectError.sh ${func} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size}
# set -x
path=`pwd`
cd ${path}

func=${1}
suffix=${2} # herbie daisy origin tmp final
x0Start=${3}
x0End=${4}
x1Start=${5}
x1End=${6}
x2Start=${7}
x2End=${8}
if [ $# -eq 11 ]; then
    x0Size=${9}
    x1Size=${10}
    x2Size=${11}
elif [ $# -eq 8 ]; then
    x0Size=256
    x1Size=256
    x2Size=256
else
    echo "Invalid input parameters"
    exit
fi
prefix=`echo ${func} | cut -d " " -f 1`
prefix=expr_${prefix}_

echo "detecting error: ${func} ${suffix} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size}"
gcc test3param.c ${prefix}${suffix}.c ${prefix}mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix} -DSUFFIX=${suffix} -lmpfr -lm -o test3param.exe
./test3param.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x2Start} ${x2End} ${x0Size} ${x1Size} ${x2Size}
rm test3param.exe
echo -e "end detecting ${func}\n"
