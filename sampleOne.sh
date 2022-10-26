#!/bin/bash
# Usage: ./sampleTwo.sh ${func} ${x1Start} ${x1End} ${findMaxNum}
# set -x
path=`pwd`
cd ${path}

func=${1}
x1Start=${2}
x1End=${3}
findMaxNum=${4}
funcName=`echo ${func} | cut -d " " -f 1`

echo "sampling ${funcName} ${x1Start} ${x1End} ${findMaxNum}"
gcc test1param.c expressionSingle.c computeULP.c -Iinclude -DEXPRESSION=${funcName} -lmpfr -lm -o sampleOne.exe
./sampleOne.exe ${x1Start} ${x1End} ${findMaxNum}
rm sampleOne.exe
echo -e "end sampinging ${funcName}\n"

cd -
