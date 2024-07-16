# exprAuto

- [exprAuto](#exprauto)
  - [介绍](#介绍)
    - [工作流程](#工作流程)
    - [软件架构](#软件架构)
    - [编译运行](#编译运行)
      - [依赖环境](#依赖环境)
      - [运行结果](#运行结果)
  - [参考链接](#参考链接)
  - [目前进度](#目前进度)

## 介绍
exprAuto 是一个浮点表达式精度优化工具。

exprAuto 支持数学表达式的解析、等价变换、多种类型的代码生成（包括double、double-double、mpfr）、误差检测、误差区间划分等功能，是一个命令行工具。

exprAuto 的表达式解析功能基于 [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html) 实现。

数学表达式的 BNF 定义 (BNF 定义详见参考链接 3)：

```
expr   -> expr + term | expr - term | term
term   -> term * factor | term / factor | factor / term | factor
factor -> number | variable | (expr) | func(expr, ...)
```

数学表达式示例：

* 1 + 2 + 3
* (1 + 2) * 3
* pow(x, y)
* log(x + 1) + exp(x) - 1
* 1 / x + 1 / (x + 1)

### 工作流程

1. 表达式重写
   
   数学表达式 -> 词法分析 -> 词法单元 -> 语法分析 -> 语法分析树(AST) -> **等价变换** -> 代码生成

2. 区间划分 + 表达式重写

   数学表达式 -> 区间划分 -> 等价变换 -> 代码生成

### 软件架构

主要文件目录如下：
``` shell
.
├── include/
├── includeDD/
├── includeNumOpt/
├── includeTEST/
├── src/
├── srcTest/
├── srcNumOpt/
├── script/
├── libs/
├── objs/
├── bin/
├── outputs/
├── README.md
├── Makefile
├── benchMarkInterval.txt
├── benchMark.txt
├── detectError**.sh
```

include: 项目主要代码的头文件目录

includeDD: 实现dd代码自动生成所需的头文件

includeNumOpt: 测试NumOpt所需的头文件

includeTEST: 误差检测所需的头文件

src: 存放项目的主要代码

srcTest: 存放误差和性能等测试所需的代码以及表达式生成代码

srcNumOpt: 存放测试NumOpt所需的代码

srcNumOpt: 存放测试NumOpt的相关脚本和输入输出

libs: 存放测试所需的库文件

objs: 存放编译过程生成的目标代码

bin: 存放编译生成的可执行文件

outputs: 存放运行过程中生成的结果文件

benchMark.txt: 存放exprAuto已测试过的benchmark

benchMarkInterval.txt: 存放exprAuto已测试过的benchmark的测试区间

detectError[Num][FPED][Single][Parallel][NumOpt]**.sh: 误差检测的相关shell脚本. Num可取值One, Two, Three, Four, Five, 表示不同参数数量. 后缀带有Single的，是串行版本. 后缀带有Parallel的，是并行版本. 后缀带有NumOpt的，是用于测试NumOpt的版本. 

sampleError[Num].sh: 误差数据采样脚本。Num可取值Two, Three, Four, Five, 表示不同参数数量. (Num = One的情况，等价于detectError*.sh脚本中Num = One的情况)

### 编译运行
#### 依赖环境

gcc, g++, make, openMPI, rlwrap, Matlab, Python, SymPy library, QD库, NumOpt

QD库：https://www.davidhbailey.com/dhbsoftware/

NumOpt： http://seg.nju.edu.cn/eytang/numopt/numopt.tar.gz

```bash
https://github.com/yuanyuanxia/exprAuto.git
cd path/to/exprAuto
make
./bin/exprAuto.exe
# OR use the external tool rlwrap
# The use of the external tool rlwrap is highly recommended but not required to use the exprAuto interactive tool.
rlwrap ./bin/exprAuto.exe
```

#### 运行结果
执行 ```$./bin/exprAuto.exe```后，运行结果如下：

```
ready> log(1 - x) / log(1 + x)
main: after SymPy's simplify, expr = (log((1.000000 - x)) / log((x + 1.000000)))
exprAutoNew: step1: preprocess
exprAutoNew: exprNew = (log((1.000000 + (-1.000000 * x))) / log((x + 1.000000)))

exprAutoNew: step2: judge if exprNew is a fraction
exprAutoNew: exprNew is a fraction, so perform step3 and step4

exprAutoNew: step3: perform on numerator.
        tryRewrite: start--------
        ......
        ......
        ......
        tryRewrites: No.0: log((1.000000 + (-1.000000 * x)))
        tryRewrites: No.1: log1p((-1.000000 * x))
        tryRewrite: end--------
exprAutoNew: step3: end perform on numerator.

exprAutoNew: step3: perform on denominator.
        tryRewrite: start--------
        ......
        ......
        ......
        tryRewrites: No.0: log((1.000000 + x))
        tryRewrites: No.1: log1p(x)
        tryRewrite: end--------
exprAutoNew: step3: end perform on denominator.

exprAutoNew: step4: combine numerator and denominator.
        ......

main: after exprAutoNew: No.0: (log((1.000000 + (-1.000000 * x))) / log((1.000000 + x)))
main: after exprAutoNew: No.1: (log((1.000000 + (-1.000000 * x))) / log1p(x))
main: after exprAutoNew: No.2: (log1p((-1.000000 * x)) / log((1.000000 + x)))
main: after exprAutoNew: No.3: (log1p((-1.000000 * x)) / log1p(x))

elapsed time: 0.0429435s
```

## 参考链接

1. [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html): 本项目的表达式解析框架
2. [LLVM Tutorial -- 2. Kaleidoscope: Implementing a Parser and AST](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html): 上述教程的英文原版
3. [从编译原理看一个解释器的实现](https://zhuanlan.zhihu.com/p/27450417): 辅助理解数学表达式的解析方法
4. C++ primer (5th-Edition). Stanley B. Lippman, Josée Lajoie, Barbara E. Moo. 2012. ISBN-13: 9780321714114
5. Panchekha, Pavel, Alex Sanchez-Stern, James R. Wilcox, and Zachary Tatlock. “Automatically Improving Accuracy for Floating Point Expressions.” In Proceedings of the 36th ACM SIGPLAN Conference on Programming Language Design and Implementation, 1–11. PLDI ’15. New York, NY, USA: Association for Computing Machinery, 2015. https://doi.org/10.1145/2737924.2737959.
6. Saiki, Brett, Oliver Flatt, Chandrakana Nandi, Pavel Panchekha, and Zachary Tatlock. “Combining Precision Tuning and Rewriting.” In 2021 IEEE 28th Symposium on Computer Arithmetic (ARITH), 1–8. Lyngby, Denmark: IEEE, 2021. https://doi.org/10.1109/ARITH51176.2021.00013.
7. Willsey, Max, Chandrakana Nandi, Yisu Remy Wang, Oliver Flatt, Zachary Tatlock, and Pavel Panchekha. “Egg: Fast and Extensible Equality Saturation.” Proceedings of the ACM on Programming Languages 5, no. POPL (January 4, 2021): 1–29. https://doi.org/10.1145/3434304.
8. Nandi, Chandrakana, Max Willsey, Amy Zhu, Yisu Remy Wang, Brett Saiki, Adam Anderson, Adriana Schulz, Dan Grossman, and Zachary Tatlock. “Rewrite Rule Inference Using Equality Saturation.” Proceedings of the ACM on Programming Languages 5, no. OOPSLA (2021): 119:1-119:28. https://doi.org/10.1145/3485496.


## 目前进度

* [x] 支持解析四则运算、函数、括号、变量、常数 (含科学计数法格式) ★★
* [x] 支持多种输入方式：屏幕输入、文件输入
* [x] 支持打印表达式 ★
* [x] 支持化简表达式 ★★
* [x] 支持多项式等价变换 ★★★
* [x] 支持数学函数等价变换 (80%) ★★★
  * [x] 指数函数和对数函数的相关变换
  * [x] 和差化积等三角函数变换
  * [x] fma相关变换
  * [x] sqrt, pow相关变换
  * [ ] ······ (更多变换)
* [ ] 支持误差检测 (90%) ★★
  * [x] 特定参数数量的误差检测（目前支持1-5个参数）
  * [x] 并行化
  * [ ] 任意参数的误差检测
* [x] 支持性能测试功能 (90%) ★
  * [x] 特定参数数量的误差检测（目前支持1-3个参数）
  * [ ] 任意参数的性能检测
* [ ] 精度优化功能 (80%) ★★★
  * [x] 精度优化区间划分
  * [x] 表达式重写框架
  * [ ] 优化重写框架
* [x] 代码生成 (90%) ★
  * [x] 生成double、d-d、mpfr版本的代码
  * [x] 生成带判断的表达式代码
  * [ ] 支持FPCore格式
* [ ] 支持对 herbie、daisy 等优化工具的调用 ★
* [ ] 实现多精度变换 ★★★
* [ ] ...... (更多功能)
