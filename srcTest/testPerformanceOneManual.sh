#example: cd path/to/srcTest; taskset -c 0 ./testPerformanceOneManual.sh Bsplines3 origin 0 100

if [ $# == 1 ]; then
	uniqueLabel=${1}
	suffix=origin
	start=0.01
	end=100
	runtime=10000
elif [ $# == 4 ]; then
	uniqueLabel=${1}
	suffix=${2}
	start=${3}
	end=${4}
	runtime=10000
elif [ $# == 5 ]; then
	uniqueLabel=${1}
	suffix=${2}
	start=${3}
	end=${4}
	runtime=${5}
else
	echo "please input 1, 4 or 5 parameters"
	exit
fi

resultFileName=result_${uniqueLabel}_${start}_${end}.txt
funcName=expr_${uniqueLabel}_${suffix}

# gcc testPerformanceOne.c binary.c ${uniqueLabel}/${funcName}.c -DFUNCNAME=${funcName} -DRUNTIME=${runtime} -I../includeTEST -lm -o testPerformanceOne_${uniqueLabel}.exe -O3

# make testPerformanceOne.o -s CFLAGS="-DFUNCNAME=${funcName} -DRUNTIME=${runtime}"
gcc testPerformanceOne.c -DFUNCNAME=${funcName} -DRUNTIME=${runtime} -I../includeTEST -c -O3
make binary.o -s # gcc binary.c -I../includeTEST -c -O3
gcc ${uniqueLabel}/${funcName}.c -I../includeDD -c -O3 -mfma
gcc testPerformanceOne.o binary.o ${funcName}.o ../libs/libTGen.so -lmpfr -lm -lqd -o testPerformanceOne_${uniqueLabel}.exe -O3
./testPerformanceOne_${uniqueLabel}.exe ${resultFileName} ${start} ${end}
# rm -rf testPerformanceOne_${uniqueLabel}.exe

### compute the cycles of the function ${uniqueLabel}
make dataClean.exe -s CFLAGS="-DRUNTIME=${runtime}" # gcc dataClean.c -DRUNTIME=${runtime} -o dataClean.exe
sort -n ${resultFileName} > ${resultFileName}.sort
./dataClean.exe ${resultFileName}.sort
rm -rf ${resultFileName} ${resultFileName}.sort
# rm dataClean.exe
