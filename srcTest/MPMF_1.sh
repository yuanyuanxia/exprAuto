#!/bin/bash
# set -x
# 多精度版本函数的路径是TGen的路径，特定于北京服务器的固定路径。
# NMSEproblem345的单参特供版，仅能修改其中的sin函数，用来测试使用不同函数的表达式的性能和精度
#init for mix-precision
source funcMap.sh
func1=sin
path_TGen=/home/xyy/helloBro/TGen
path_func1=${path_TGen}/${func1}_gen
path_current=`pwd`
temp_file=${path_current}/temp.log # 临时文件，在循环中被刷新
log_file=${path_current}/hjw.log # 其中依次记录的是: 函数名, bit, degree, 平均误差, 最大误差, 性能节拍

if [ -f "${log_file}" ]; then
    rm "${log_file}"
fi

# init for error detection and performance detection
uniqueLabel=NMSEproblem345
x0Start=0.01
x0End=100
x0Size=500000
x0startNowIdx=0
x0startOriginInterval=0.01
stepX0=0.00019998
prefix=expr_${uniqueLabel}
middle=${x0Start}_${x0End}_${x0Size}
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
sed -i "s@${func1}(@${func1}_gen(@g" ${expr_file}

# loop to generate different version

# loop1,2 for func1
# for ((bit1=0; bit1<=1; bit1++))
for ((bit1=0; bit1<=${bit_range[$func1]}; bit1++))
do
    # for ((degree1=0; degree1<1; degree1++))
    for ((degree1=0; degree1<${degree_range[$func1]}; degree1++))
    do
        cp ${path_func1}/${func1}_gen_${bit1}_1_${degree1}.c ${uniqueLabel}/${func1}_gen.c
        echo "${func1}: bit = ${bit1} degree = ${degree1}"
        echo -e "${func1}\t${bit1}\t${degree1}:" > ${temp_file}
        cd ..
        ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${x0startNowIdx} ${x0startOriginInterval} ${stepX0} ${prefix} ${middle} ${suffix} ${errfile} | tee -a ${temp_file}
        cd - >/dev/null
        taskset -c 8 ./testPerformanceOneManual.sh ${uniqueLabel} ${suffix} ${x0Start} ${x0End} | tee -a ${temp_file}
        awk 'NR == 1 { printf "%s\t", $0 }; NR == 3 { printf "%s\t", $0 }; NR == 5 { print $0 }' ${temp_file} >> ${log_file}
        echo
    done
done