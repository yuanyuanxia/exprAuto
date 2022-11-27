#!/bin/bash
# Description: error detecting with multi dimension
# Usage: ./detectErrorMulti.sh ${uniqueLabel} ${intervals[*]} ${prefix} ${middle} ${suffix}
# Example: ./detectErrorMulti.sh 20221127132126 12 42 53 64 75 8 expr_20221127132126 12_42_53_64_75_8 origin
# set -x


# ------initial
echo "start init ${uniqueLabel}"
path=$(pwd)
cd ${path}
CC=gcc
PARACC=mpicc
let TESTNUM=2**19
let PARALLEL=8
let TESTNUM_LOCAL=${TESTNUM}/${PARALLEL}

uniqueLabel=${1} # unique number
declare -a intervals
declare -a components
let NUM_BEFORE=1
let NUM_AFTER=3 # 3 means prefix middle suffix
let NUM_INTERVALS=$#-${NUM_BEFORE}-${NUM_AFTER}
idx=0
idx_components=0
for i in "$@"; do
    if [ $idx -ge ${NUM_BEFORE} ]; then
        if [ $idx -le ${NUM_INTERVALS} ]; then
            intervals[$idx - ${NUM_BEFORE}]=${i}
        else
            components[idx_components]=${i}
            let idx_components++
        fi
    fi
    let idx++
done
prefix=${components[0]}
middle=${components[1]}
suffix=${components[2]}
echo "end init ${uniqueLabel}"
# for ((i = 0; i < ${#intervals[*]}; i++)); do
#     echo intervals[$i] = "${intervals[$i]}"
# done
# for ((i = 0; i < ${#components[*]}; i++)); do
#     echo components[$i] = "${components[$i]}"
# done
# echo ./geneDataMulti.exe ${uniqueLabel} ${intervals[*]}


# ------generate data
echo "start generating data for ${uniqueLabel}"
# compile data generation function
${CC} geneDataMulti.c -DTESTNUM=${TESTNUM} -IincludeTEST -o geneDataMulti.exe
# the parameters are intervals
./geneDataMulti.exe ${intervals[*]}

# devide inputData.txt into many files
split -l ${TESTNUM_LOCAL} -d -a 2 inputData.txt inputData_
# add ".txt" to files
find . -name "inputData_*" | xargs -i mv {} {}.txt
echo "end generating data for ${uniqueLabel}"


# ------error detecting
echo "start detecting ${uniqueLabel}"
# compile error detecting function
let PARA_NUM=2
testFileName=testMultiParallel
${PARACC} ${testFileName}.c computeULP.c expressionMulti.c ${prefix}_${suffix}.c ${prefix}_mpfr.c -DPARA_NUM=${PARA_NUM} -IincludeTEST -lmpfr -lm -o ${testFileName}.exe

# parallel run
OUTPUTFILE=${prefix}__${middle}_${suffix}
mpirun -n ${PARALLEL} ${testFileName}.exe ${OUTPUTFILE} ${TESTNUM}

# end everything
rm inputData*txt
rm ${testFileName}.exe
echo "end detecting ${uniqueLabel}"
echo
