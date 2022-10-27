# !/bin/bash
# Usage: ./detectErrorOne.sh ${func} ${suffix} ${x0Start} ${x0End} ${x0Size}
# set -x
path=`pwd`
cd ${path}

func=${1}
suffix=${2} # herbie daisy origin tmp final
x0Start=${3}
x0End=${4}
x0Size=${5}
if [ $# -eq 6 ]; then
    x0Size=${6}
else
    x0Size=500000
fi
prefix=`echo ${func} | cut -d " " -f 1`
prefix=expr_${prefix}_

echo "detecting error: ${func} ${suffix} ${x0Start} ${x0End} ${x0Size}"
gcc test1param.c ${prefix}${suffix}.c ${prefix}mpfr.c computeULP.c -IincludeTEST -DEXPRESSION=${prefix} -DSUFFIX=${suffix} -lmpfr -lm -o test1param.exe
./test1param.exe ${x0Start} ${x0End} ${x0Size}
rm test1param.exe
echo -e "end detecting ${func}\n"

cd -
