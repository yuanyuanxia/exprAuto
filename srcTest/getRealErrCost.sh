#!/bin/bash
# set -x

# 多精度版本函数的路径是TGen的路径，特定于北京服务器的固定路径。
# NMSEproblem345的双参特供版，能修改其中的tan, sin函数的精度版本，用来测试使用不同函数的表达式的性能和精度
# getRealErrCost脚本的测试对象是帕累托优化得到的所有混合精度方案，用来验证帕累托优化效果
# getRealErrCost脚本从output.csv中读取数据。该数据是利用pareto_2.py生成的。
# pareto_2.py是根据函数的测试信息，生成帕累托优化结果。
# pareto_2.py中的数据是从func1.csv、func2.csv中获取得来的，即直接复制自TGen的结果。
# ！！！func1.csv、func2.csv中的数据不能直接复制自TGen，还需要对其乘以“条件数”！！！

# 与getRealErrCost脚本功能类似的是MPMF_2.sh。
# MPMF_2.sh脚本是直接测试所有不同的混合精度组合下的表达式“误差-性能”信息。

# init for mix-precision
source funcMap.sh
source exprMap.sh
# source init.sh

# init for error detection and performance detection
if [ $# == 3 ]; then
    uniqueLabel=${1}
    x0Start=${x0Starts[${uniqueLabel}]}
    x0End=${x0Ends[${uniqueLabel}]}
    stepX0=${stepX0s[${uniqueLabel}]}

    func1=${2}
    func2=${3}
elif [ $# == 1 ]; then
    uniqueLabel=${1}
    x0Start=${x0Starts[${uniqueLabel}]}
    x0End=${x0Ends[${uniqueLabel}]}
    stepX0=${stepX0s[${uniqueLabel}]}

    func1=sin
    func2=tan
else
    uniqueLabel="NMSEproblem345"
    x0Start=0.01
    x0End=100
    stepX0=0.00019998

    func1=sin
    func2=tan
fi

x0Size=500000
x0startNowIdx=0
x0startOriginInterval=${x0Start}
prefix=expr_${uniqueLabel}
middle=${x0Start}
suffix=origin
errfile=0

# init for TGen
path_TGen=/home/xyy/helloBro/TGen
path_func1=${path_TGen}/${func1}_gen
path_func2=${path_TGen}/${func2}_gen

# init for the expression code file
expr_file=${uniqueLabel}/${prefix}_${suffix}.c
expr_bkp_file=${uniqueLabel}/${prefix}_${suffix}_bkp.c
if [ -f "${expr_bkp_file}" ]; then
    cp ${expr_bkp_file} ${expr_file}
else
    echo "ERROR: no ${expr_bkp_file}"
    exit
fi
sed -i "1a\#include \"${func1}_gen.c\"" ${expr_file}
sed -i "2a\#include \"${func2}_gen.c\"" ${expr_file}
sed -i "s@${func1}(@${func1}_gen(@g" ${expr_file}
sed -i "s@${func2}(@${func2}_gen(@g" ${expr_file}

# 读取CSV文件，保存到数组
csv_file="${uniqueLabel}Output.csv"
data_array=()

# init for current path
path_current=`pwd`
temp_file=${path_current}/temp.log # 临时文件，在循环中被刷新
log_file=${path_current}/${uniqueLabel}_errPerf.csv # 其中依次记录的是: 函数名, bit, degree, 平均误差, 最大误差, 性能节拍

while IFS=$'\t' read -r -a cols
do
    data=()
    for ((i=0; i<${#cols[@]}; i++))
    do
        if [ $i -lt 4 ]; then  # 只保存前4列数据
            data+=("${cols[i]}")
        fi
    done
    data_array+=("${data[@]}")
done < <(tail -n +2 "$csv_file")  # 从第二行开始读取文件，跳过标题行

# loop to generate different version
echo -e "${func1}_regime1\tdegree1\t${func2}_regime2\tdegree2\terror\tperformance" > ${log_file}
step=4
for ((i=0; i<${#data_array[@]}; i+=step))
do
    # echo "Batch $((i/step+1)): ${data_array[@]:i:step}"
    bit1=${data_array[i]}
    degree1=${data_array[i+1]}
    bit2=${data_array[i+2]}
    degree2=${data_array[i+3]}
    
    # echo "当前参数：$bit1 $degree1 $bit2 $degree2"
    cp ${path_func1}/${func1}_gen_${bit1}_1_${degree1}.c ${uniqueLabel}/${func1}_gen.c
    cp ${path_func2}/${func2}_gen_${bit2}_1_${degree2}.c ${uniqueLabel}/${func2}_gen.c
    echo "${func1}: bit = ${bit1} degree = ${degree1}; ${func2}: bit2 = ${bit2} degree = ${degree2}"
    echo -e "${bit1}\t${degree1}\t${bit2}\t${degree2}" > ${temp_file}
    cd ..
    ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${x0startNowIdx} ${x0startOriginInterval} ${stepX0} ${prefix} ${middle} ${suffix} ${errfile} | tee -a ${temp_file}
    cd - >/dev/null
    taskset -c 0 ./testPerformanceOneManual.sh ${uniqueLabel} ${suffix} ${x0Start} ${x0End} | tee -a ${temp_file}
    awk 'NR == 1 { printf "%s\t", $0 }; NR == 3 { printf "%s\t", $1 }; NR == 5 { print $0 }' ${temp_file} >> ${log_file}
    echo
done

cp ${expr_bkp_file} ${expr_file}
echo "The data is logged into ${log_file}"