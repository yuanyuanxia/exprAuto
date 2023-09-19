#!/bin/bash
# set -x
# !!! 缺少对函数input.csv 的预处理，即去除低精度代码
source exprMap.sh

# init for error detection and performance detection
if [ $# == 3 ]; then
    uniqueLabel=${1}
    x0Start=${2}
    x0End=${3}
    func1=${func1s[${uniqueLabel}]}
elif [ $# == 1 ]; then
    uniqueLabel=${1}
    x0Start=${x0Starts[${uniqueLabel}]}
    x0End=${x0Ends[${uniqueLabel}]}
    func1=${func1s[${uniqueLabel}]}
else
    uniqueLabel="exp1x"
    x0Start=0.01
    x0End=0.5
    func1=exp
fi
f1=${func1}_gen_${x0Start}-${x0End}

python3 pareto_singleFunc.py ${f1}
cp ${f1}Output.csv ${uniqueLabel}Output.csv
./getRealErrCost_1.sh ${uniqueLabel} ${func1}
python3 pareto_singleFunc.py ${uniqueLabel} ${func1}
python3 sortData.py ${uniqueLabel} ${func1}