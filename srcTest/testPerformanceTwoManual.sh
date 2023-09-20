#!/bin/bash
#example: cd path/to/srcTest; taskset -c 0 ./testPerformanceTwoManual.sh i6 origin 0 100 0 100

if [ $# == 1 ]; then
	uniqueLabel=${1}
	suffix=origin
	x0Start=0.01
	x0End=100
	x1Start=0.01
	x1End=100
	runtime=10000
elif [ $# == 6 ]; then
	uniqueLabel=${1}
	suffix=${2}
	x0Start=${3}
	x0End=${4}
	x1Start=${5}
	x1End=${6}
	runtime=10000
elif [ $# == 7 ]; then
	uniqueLabel=${1}
	suffix=${2}
	x0Start=${3}
	x0End=${4}
	x1Start=${5}
	x1End=${6}
	runtime=${7}
else
	echo "please input 1, 6 or 7 parameters"
	exit
fi

resultFileName=result_${uniqueLabel}_${x0Start}_${x0End}_${x1Start}_${x1End}.txt
funcName=expr_${uniqueLabel}_${suffix}

# gcc testPerformanceTwo.c binary.c ${uniqueLabel}/${funcName}.c -DFUNCNAME=${funcName} -DRUNTIME=${runtime} -I../includeTEST -lm -o testPerformanceTwo_${uniqueLabel}.exe -O3

# make testPerformanceTwo.o -s CFLAGS="-DFUNCNAME=${funcName} -DRUNTIME=${runtime}"
gcc testPerformanceTwo.c -DFUNCNAME=${funcName} -DRUNTIME=${runtime} -I../includeTEST -c -O3
# gcc ${uniqueLabel}/expr_${uniqueLabel}_mpfr.c -c
make binary.o -s # gcc binary.c -I../includeTEST -c -O3
gcc ${uniqueLabel}/${funcName}.c -I../includeTEST -I../includeDD -c -O3 -mfma
gcc testPerformanceTwo.o binary.o ${funcName}.o -lm -lqd -lmpfr -o testPerformanceTwo_${uniqueLabel}.exe -O3
./testPerformanceTwo_${uniqueLabel}.exe ${resultFileName} ${x0Start} ${x0End} ${x1Start} ${x1End}
# rm -rf testPerformanceTwo_${uniqueLabel}.exe

### compute the cycles of the function ${uniqueLabel}
make dataClean.exe -s CFLAGS="-DRUNTIME=${runtime}" # gcc dataClean.c -DRUNTIME=${runtime} -o dataClean.exe
sort -n ${resultFileName} > ${resultFileName}.sort
./dataClean.exe ${resultFileName}.sort
rm -rf ${resultFileName} ${resultFileName}.sort
# rm dataClean.exe
