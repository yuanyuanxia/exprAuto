#!/bin/bash
# set -x

# 多精度版本函数的路径是TGen的路径，特定于北京服务器的固定路径。
# getRealErrCost_1 脚本的测试对象是帕累托优化得到的所有混合精度方案，用来验证帕累托优化效果
# getRealErrCost_1 脚本从 funcOutput.csv 中读取数据。该数据是利用 pareto_singleFunc.py 生成的。
# pareto_singleFunc.py 是根据函数的测试信息，生成帕累托优化结果。
# pareto_singleFunc.py 中的数据是从 funcInput.csv 中获取得来的

# 与 getRealErrCost_1 脚本功能类似的是 MPMF_1.sh。
# MPMF_1.sh 脚本是直接测试所有不同的混合精度组合下的表达式“误差-性能”信息。

# init for mix-precision
source funcMap.sh
source exprMap.sh

# init for error detection and performance detection
if [ $# == 2 ]; then
    uniqueLabel=${1}
    x0Start=${x0Starts[${uniqueLabel}]}
    x0End=${x0Ends[${uniqueLabel}]}
    stepX0=${stepX0s[${uniqueLabel}]}

    func1=${2}
elif [ $# == 1 ]; then
    uniqueLabel=${1}
    x0Start=${x0Starts[${uniqueLabel}]}
    x0End=${x0Ends[${uniqueLabel}]}
    stepX0=${stepX0s[${uniqueLabel}]}

    func1=sin
else
    uniqueLabel="NMSEproblem345"
    x0Start=0.01
    x0End=100
    stepX0=0.00019998

    func1=sin
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

# 读取CSV文件，保存到数组
csv_file="${uniqueLabel}Output.csv"
data_array=()

# init for current path
path_current=`pwd`
temp_file=${path_current}/temp.log # 临时文件，在循环中被刷新
log_file=${path_current}/${uniqueLabel}_errPerf.csv # 其中依次记录的是: 函数名, bit, degree, 平均误差, 最大误差, 性能节拍

# 解析 csv 文件，保存到数组 data_array
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
echo -e "${func1}_regime\tdegree\taverageErr\tperformance" > ${log_file}
step=4
for ((i=0; i<${#data_array[@]}; i+=step))
do
    # echo "Batch $((i/step+1)): ${data_array[@]:i:step}"
    bit1=${data_array[i]}
    degree1=${data_array[i+1]}
    
    # echo "当前参数：$bit1 $degree1 $bit2 $degree2"
    cp ${path_func1}/${func1}_gen_${bit1}_1_${degree1}.c ${uniqueLabel}/${func1}_gen.c
    echo "${func1}: bit = ${bit1} degree = ${degree1}"
    echo -e "${bit1}\t${degree1}" > ${temp_file}
    cd ..
    ./detectErrorOneFPEDParallel.sh ${uniqueLabel} ${x0Start} ${x0End} ${x0Size} ${x0startNowIdx} ${x0startOriginInterval} ${stepX0} ${prefix} ${middle} ${suffix} ${errfile} | tee -a ${temp_file}
    cd - >/dev/null
    taskset -c 0 ./testPerformanceOneManual.sh ${uniqueLabel} ${suffix} ${x0Start} ${x0End} | tee -a ${temp_file}
    awk 'NR == 1 { printf "%s\t", $0 }; NR == 3 { printf "%s\t", $1 }; NR == 5 { print $0 }' ${temp_file} >> ${log_file}
    echo
done

cp ${expr_bkp_file} ${expr_file}
echo "Input file: ${csv_file}"
echo "Output file: ${uniqueLabel}_errPerf.csv"