# !/bin/bash
# Usage: ./detectError.sh ${func} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size}
# set -x
path=`pwd`
cd ${path}

func=${1}
suffix=${2} # herbie daisy origin tmp final
x0Start=${3}
x0End=${4}
x1Start=${5}
x1End=${6}
if [ $# -eq 8 ]; then
    x0Size=${7}
    x1Size=${8}
else
    x0Size=1024
    x1Size=1024
fi
prefix=`echo ${func} | cut -d " " -f 1`
prefix=expr_${prefix}_

echo "detecting error: ${func} ${suffix} ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size}"
gcc test2param.c ${prefix}${suffix}.c ${prefix}mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix} -DSUFFIX=${suffix} -lmpfr -lm -o test2param.exe
./test2param.exe ${x0Start} ${x0End} ${x1Start} ${x1End} ${x0Size} ${x1Size}
rm test2param.exe
echo -e "end detecting ${func}\n"
