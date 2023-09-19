#!/bin/bash
set -x
# !!! 缺少对函数input.csv 的预处理，即去除低精度代码

python3 pareto_singleFunc.py exp_gen_0.01-0.5
cp exp_gen_0.01-0.5Output.csv exp1xOutput.csv
./getRealErrCost_1.sh exp1x exp
python3 pareto_singleFunc.py exp1x exp
python3 sortData.py exp1x exp