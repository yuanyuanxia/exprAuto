#example: cd path/to/srcTest; taskset -c 0 ./testPerformanceOne.sh Bsplines3 origin 0 100

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

# gcc gccPerformanceTest.c binary.c ${uniqueLabel}/${funcName}.c -DFUNCNAME=${funcName} -DRUNTIME=${runtime} -I../includeTEST -lm -o gccPerformanceTest_${uniqueLabel}.exe -O3

# make gccPerformanceTest.o -s CFLAGS="-DFUNCNAME=${funcName} -DRUNTIME=${runtime}"
gcc gccPerformanceTest.c -DFUNCNAME=${funcName} -DRUNTIME=${runtime} -I../includeTEST -c -O3
make binary.o -s # gcc binary.c -I../includeTEST -c -O3
gcc ${uniqueLabel}/${funcName}.c -I../includeDD -c -O3
gcc gccPerformanceTest.o binary.c.o ${funcName}.o -lm -lqd -o gccPerformanceTest_${uniqueLabel}.exe -O3
./gccPerformanceTest_${uniqueLabel}.exe ${resultFileName} ${start} ${end} > /dev/null
# rm -rf gccPerformanceTest_${uniqueLabel}.exe

### compute the cycles of the function ${uniqueLabel}
make dataClean.exe -s CFLAGS="-DRUNTIME=${runtime}" # gcc dataClean.c -DRUNTIME=${runtime} -o dataClean.exe
sort -n ${resultFileName} > ${resultFileName}.sort
./dataClean.exe ${resultFileName}.sort > ../outputs/${uniqueLabel}/${funcName}_performance.txt
rm -rf ${resultFileName} ${resultFileName}.sort
# rm dataClean.exe
