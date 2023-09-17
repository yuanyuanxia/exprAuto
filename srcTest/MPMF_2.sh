#!/bin/bash
# set -x
# 多精度版本函数的路径是TGen的路径，特定于北京服务器的固定路径。
# NMSEproblem345的双参特供版，能修改其中的tan, sin函数的精度版本，用来测试使用不同函数的表达式的性能和精度

# init for multi-precision func
source funcMap.sh
source init.sh

# init for current path
path_current=`pwd`
temp_file=${path_current}/temp.log # 临时文件，在循环中被刷新
log_file=${path_current}/hjw.log # 其中依次记录的是: 函数名, bit, degree, 平均误差, 最大误差, 性能节拍

if [ -f "${log_file}" ]; then
    rm "${log_file}"
fi

# loop to generate different version

# loop1,2 for func1
# for ((bit1=0; bit1<=1; bit1++))
for ((bit1=0; bit1<=${bit_range[$func1]}; bit1++))
do
    # for ((degree1=0; degree1<1; degree1++))
    for ((degree1=0; degree1<${degree_range[$func1]}; degree1++))
    do
        cp ${path_func1}/${func1}_gen_${bit1}_1_${degree1}.c ${uniqueLabel}/${func1}_gen.c

        # loop3,4 for func2
        # for ((bit2=0; bit2<=1; bit2++))
        for ((bit2=0; bit2<=${bit_range[$func2]}; bit2++))
        do
            # for ((degree2=0; degree2<1; degree2++))
            for ((degree2=0; degree2<${degree_range[$func2]}; degree2++))
            do
                cp ${path_func2}/${func2}_gen_${bit2}_1_${degree2}.c ${uniqueLabel}/${func2}_gen.c
                echo "${func1}: bit = ${bit1} degree = ${degree1}; ${func2}: bit2 = ${bit2} degree = ${degree2}"
                echo -e "${func1}\t${bit1}\t${degree1}\t${func2}\t${bit2}\t${degree2}:" > ${temp_file}
                cd ..
                ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${x0startNowIdx} ${x0startOriginInterval} ${stepX0} ${prefix} ${middle} ${suffix} ${errfile} | tee -a ${temp_file}
                cd - >/dev/null
                taskset -c 8 ./testPerformanceOneManual.sh ${uniqueLabel} ${suffix} ${x0Start} ${x0End} | tee -a ${temp_file}
                awk 'NR == 1 { printf "%s\t", $0 }; NR == 3 { printf "%s\t", $0 }; NR == 5 { print $0 }' ${temp_file} >> ${log_file}
                echo
            done
        done
    done
done