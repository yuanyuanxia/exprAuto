#!/bin/bash
# !!! 缺少对函数input.csv 的预处理，即去除低精度代码
# set -x
source exprMap.sh

# init for error detection and performance detection
if [ $# == 5 ]; then
    uniqueLabel=${1}
    x0Start=${2}
    x0End=${3}
    x0Start_2=${4}
    x0End_2=${5}
    func1=${func1s[${uniqueLabel}]}
    func2=${func2s[${uniqueLabel}]}
    f1=${func1}_gen_${x0Start}-${x0End}
    f2=${func2}_gen_${x0Start_2}-${x0End_2}
elif [ $# == 3 ]; then
    uniqueLabel=${1}
    x0Start=${2}
    x0End=${3}
    func1=${func1s[${uniqueLabel}]}
    func2=${func2s[${uniqueLabel}]}
    f1=${func1}_gen_${x0Start}-${x0End}
    f2=${func2}_gen_${x0Start}-${x0End}
elif [ $# == 1 ]; then
    uniqueLabel=${1}
    x0Start=${x0Starts[${uniqueLabel}]}
    x0End=${x0Ends[${uniqueLabel}]}
    func1=${func1s[${uniqueLabel}]}
    func2=${func2s[${uniqueLabel}]}
    f1=${func1}_gen_${x0Start}-${x0End}
    f2=${func2}_gen_${x0Start}-${x0End}
else
    uniqueLabel=NMSEexample34
    x0Start=0.01
    x0End=3
    func1=sin
    func2=cos
    f1=${func1}_gen_${x0Start}-${x0End}
    f2=${func2}_gen_${x0Start}-${x0End}
fi

# NMSEexample34
python3 pareto_singleFunc.py ${f1}
python3 pareto_singleFunc.py ${f2}
python3 combine_2.py ${uniqueLabel} ${f1} ${f2}
./getRealErrCost.sh ${uniqueLabel} ${func1} ${func2}
python3 pareto_twoFunc.py ${uniqueLabel} ${func1} ${func2}
python3 sortData.py ${uniqueLabel} ${func1} ${func2}