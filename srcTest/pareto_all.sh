# !!! 缺少对函数input.csv 的预处理，即去除低精度代码
python3 pareto_singleFunc.py sin
python3 pareto_singleFunc.py tan
python3 combine_2.py
./getRealErrCost.sh
python3 pareto_twoFunc.py
python3 sortData.py