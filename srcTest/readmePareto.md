# 函数级混合精度优化思路

## 旧思路

第一步：
先用MP脚本生成不同混合精度配置的实际性能-精度二元组信息。
使用上述数据，构建数据表，即func1.csv，func2.csv
然后调用pareto_2.py脚本，构建混合精度方案的误差-开销二元组。并从中生成帕累托最优集合（帕累托前沿）
第一步得到的信息就是纯作为对比。这是理想情况下能找到的最优组合。

第二步（与第一步并行）：
执行TGen，生成函数的性能-精度数据
使用上述数据，构建数据表，即func1.csv，func2.csv
然后调用pareto_2.py脚本，构建混合精度方案的误差-开销二元组。并从中生成帕累托最优集合（帕累托前沿）
这是初步通过估算得到的预测的最优组合。当然此时的组合因为没有实测，其性能-精度数据并不一定对

第三步（承接第二步）：
把第二步最后得到的帕累托最优集合，再重新用MP脚本测一下表达式的性能-精度二元组数据。
然后再对实测的二元组信息执行帕累托优化。


## 新思路

双函数的步骤：

0. 预处理：把误差低于float32的都删除
1. 先用 pareto_singleFunc.py 生成各函数的帕累托优化结果 (NMSEproblem345 sin tan)
2. 再用 combine_2.py 将其两两组合，生成所有可能的混合精度方案
3. 再用 getRealErrCost.sh 对所有的混合精度方案进行测试，生成表达式的实际值构成的误差-开销二元组集合
4. 再用 pareto_twoFunc.py 对该集合执行帕累托优化，缩小数量
5. 最后用 sortData.py 对混合精度方案按照误差升序排序

双函数执行脚本：

shell script name: pareto_all.sh

```shell
python3 pareto_singleFunc.py sin
python3 pareto_singleFunc.py tan
python3 combine_2.py
./getRealErrCost.sh
python3 pareto_twoFunc.py
python3 sortData.py
```

单函数的步骤：

0. 预处理：把误差低于float32的都删除
1. 先用 pareto_singleFunc.py 生成函数的帕累托优化结果 (exp1x exp)
2. 再用 getRealErrCost_1.sh 对所有的混合精度方案进行测试，生成表达式的实际值构成的误差-开销二元组集合
3. 再用 pareto_singleFunc.py 对该集合执行帕累托优化，缩小数量
4. 最后用 sortData.py 对混合精度方案按照误差升序排序

单函数执行脚本：

shell script name: pareto_all_1.sh

```shell
python3 pareto_singleFunc.py exp_gen_0.01-0.5
./getRealErrCost_1.sh exp1x exp
python3 pareto_singleFunc.py exp1x exp
python3 sortData.py exp1x exp
```