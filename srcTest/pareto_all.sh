# !!! 缺少对函数input.csv 的预处理，即去除低精度代码

uniqueLabel=NMSEexample34
x0Start=0.01
x0End=3
func1=sin
func2=cos

# NMSEexample34
python3 pareto_singleFunc.py ${func1}_gen_${x0Start}-${x0End}
python3 pareto_singleFunc.py ${func2}_gen_${x0Start}-${x0End}
python3 combine_2.py ${uniqueLabel} ${func1}_gen_${x0Start}-${x0End} ${func2}_gen_${x0Start}-${x0End}
./getRealErrCost.sh ${uniqueLabel} ${func1} ${func2}
python3 pareto_twoFunc.py ${uniqueLabel} ${func1} ${func2}
python3 sortData.py ${uniqueLabel} ${func1} ${func2}