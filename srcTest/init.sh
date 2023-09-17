func1=sin
func2=tan
path_TGen=/home/xyy/helloBro/TGen
path_func1=${path_TGen}/${func1}_gen
path_func2=${path_TGen}/${func2}_gen

# init for error detection and performance detection
uniqueLabel=NMSEproblem345
x0Start=0.01
x0End=100
x0Size=500000
x0startNowIdx=0
x0startOriginInterval=0.01
stepX0=0.00019998
prefix=expr_${uniqueLabel}
middle=${x0Start}
suffix=origin
errfile=0

# init for the expression code file
expr_file=${uniqueLabel}/${prefix}_${suffix}.c
expr_bkp_file=${uniqueLabel}/${prefix}_${suffix}_bkp.c
if [ -f "${expr_bkp_file}" ]; then
    cp ${expr_bkp_file} ${expr_file}
else
    echo "ERROR"
    exit
fi
sed -i "1a\#include \"${func1}_gen.c\"" ${expr_file}
sed -i "2a\#include \"${func2}_gen.c\"" ${expr_file}
sed -i "s@${func1}(@${func1}_gen(@g" ${expr_file}
sed -i "s@${func2}(@${func2}_gen(@g" ${expr_file}