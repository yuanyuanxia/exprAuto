#!/bin/bash
# Description: generate date with multi dimension
# Usage: ./geneDataMulti.sh ${intervals[*]}
# Example: ./geneDataMulti.sh 12 42 53 64 75 8
# set -x

# ------initial
echo "start init geneDataMulti.sh"
path=$(pwd)
cd ${path}
rm inputData*txt
CC=gcc
let TESTNUM=2**19
let PARALLEL=8
let TESTNUM_LOCAL=${TESTNUM}/${PARALLEL}

# ------generate data
# echo ./geneDataMulti.exe $@ # $@ is ${intervals[*]}
echo "start generating data for $@"
# compile data generation function
${CC} geneDataMulti.c -DTESTNUM=${TESTNUM} -IincludeTEST -o geneDataMulti.exe
# the parameters $@ are intervals
./geneDataMulti.exe $@

# devide inputData.txt into many files
split -l ${TESTNUM_LOCAL} -d -a 2 inputData.txt inputData_
# add ".txt" to files
find . -name "inputData_*" | xargs -i mv {} {}.txt # TODO: add intervals information to inputData's filename
echo "end generating data for $@"
