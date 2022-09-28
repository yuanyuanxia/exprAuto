# exprAuto

- [exprAuto](#exprauto)
  - [介绍](#介绍)
    - [工作流程](#工作流程)
    - [软件架构](#软件架构)
    - [编译运行](#编译运行)
      - [Linux](#linux)
      - [Windows](#windows)
      - [运行结果](#运行结果)
  - [参考链接](#参考链接)
  - [目前进度](#目前进度)

## 介绍
exprAuto 支持数学表达式的解析、等价变换、mpfr 代码生成等功能，是一个命令行工具。

exprAuto 基于 [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html) 实现。

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

数学表达式 -> **词法分析** -> 词法单元 -> **语法分析** -> 语法分析树(AST) -> **等价变换/代码生成**

### 软件架构

主要文件目录如下：
```
.
├── bin
├── include
│   ├── basic.hpp
│   ├── expandAST.hpp
│   ├── exprAuto.hpp
│   ├── funcInfo.hpp
│   ├── geneExpr.hpp
│   ├── laxerAST.hpp
│   ├── laxerASTLY.hpp
│   ├── mathfuncRewrite.hpp
│   ├── monoInfo.hpp
│   ├── parserAST.hpp
│   ├── parserASTLY.hpp
│   ├── polyInfo.hpp
│   ├── polyRewrite.hpp
│   ├── preprocess.hpp
│   ├── simplifyExpr.hpp
│   └── variableInfo.hpp
├── Makefile
├── README.md
└── src
    ├── basic.cpp
    ├── expandAST.cpp
    ├── exprAuto.cpp
    ├── funcInfo.cpp
    ├── geneExpr.cpp
    ├── laxerAST.cpp
    ├── laxerASTLY.cpp
    ├── main.cpp
    ├── mathfuncRewrite.cpp
    ├── monoInfo.cpp
    ├── parserAST.cpp
    ├── parserASTLY.cpp
    ├── polyInfo.cpp
    ├── polyRewrite.cpp
    ├── preprocess.cpp
    ├── pythonAfter.txt
    ├── pythonBefore.txt
    ├── simplifyExpr.cpp
    ├── simplify.py
    └── variableInfo.cpp
```

主要类、方法如下 (待更新)：
- Top-Level parsing
  - HandleDefinition()
  - HandleExtern()
  - HandleTopLevelExpression() ★
  - MainLoop()
- Abstract Syntax Tree (aka Parse Tree)
  - ExprAST
  - NumberExprAST
  - VariableExprAST
  - BinaryExprAST
  - CallExprAST
  - PrototypeAST
  - FunctionAST
- Lexer
  - Token
  - gettok()
  - getNextToken()
- Parser
  - ParseExpression()
  - ParseNumberExpr()
  - ParseIdentifierExpr()
  - ParseParenExpr()
  - ParseBinOpRHS(int, std::unique_ptr\<ExprAST>) ★
  - ParseExpressin()
  - ParsePrototype()
  - ParseDefinition()
  - ParseTopLevelxpr()
  - ParseExtern()
- ExpandExpr
  - std::unique_ptr\<ExprAST> **expandExpr**(std::unique_ptr\<ExprAST> &expr) ★★★
- PolyRewrite
  - vector<std::unique_ptr\<ExprAST>> **createExpr**(const std::unique_ptr\<ExprAST> exprInit)
  - void **getReady**(const std::unique_ptr\<ExprAST> &expr, std::string *variablePtr, int *term, double *coefficient, int *lenPtr)
  - vector<std::unique_ptr\<ExprAST>> **createMiddle**(const std::string variable, const int *term, const double *coefficient, const int len)
- MathfuncRewrite
  - std::unique_ptr<ExprAST> **expToexpm1**(const std::unique_ptr<ExprAST> &expr)
  - **logTolog1**
  - **sqrtTohypot**
  - ······
- ExprAuto
  - std::unique_ptr<ExprAST> **preprocess**(const std::unique_ptr<ExprAST> &expr)
  - vector<std::unique_ptr<ExprAST>> **rewriteExprWrapper**(std::unique_ptr<ExprAST> &expr)
  - vector<std::unique_ptr<ExprAST>> **createAll**(vector<std::unique_ptr<ExprAST>> &numerators, vector<std::unique_ptr<ExprAST>> &denominators)
  - ······

### 编译运行
#### Linux
```bash
git clone https://gitee.com/mathlib/expr-auto.git
cd path/to/exprAuto
make
./bin/exprAuto.exe
# OR use the external tool rlwrap
# The use of the external tool rlwrap is highly recommended but not required to use the Sollya interactive tool.
rlwrap ./bin/exprAuto.exe
```

OR

```bash
git clone https://gitee.com/mathlib/expr-auto.git
cd path/to/exprAuto
g++ -o ./bin/exprAuto.exe ./src/*.cpp -Iinclude -I/usr/include/python3.8 -lpython3.8 -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
./bin/exprAuto.exe
```

#### Windows
```powershell
cd path/to/exprAuto
g++.exe -o .\bin\exprAuto.exe .\src\*.cpp -I .\include\ -I C:\path\to\Python\Python37\include\ -L C:\path\to\Python\Python\Python37\libs\ -L C:\path\to\Python\\Python37\Lib\ -lpython37 -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
./bin/exprAuto.exe
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

1. [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html): 本项目代码框架
2. [LLVM Tutorial -- 2. Kaleidoscope: Implementing a Parser and AST](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html): 上述教程的英文原版
3. [从编译原理看一个解释器的实现](https://zhuanlan.zhihu.com/p/27450417): 辅助理解数学表达式的解析方法
4. C++ primer (5th-Edition). Stanley B. Lippman, Josée Lajoie, Barbara E. Moo. 2012. ISBN-13: 9780321714114


## 目前进度

* [x] 支持解析四则运算、函数、括号、变量、常数 ★★
* [x] 支持打印表达式 ★
* [x] 支持化简表达式 ★★
* [x] 支持多项式等价变换 ★★★
* [x] 支持数学函数等价变换 ★★★
  * [x] $exp(x)-1 \Rightarrow expm1(x)$
  * [x] $log(x+1) \Rightarrow log1p(x)$
  * [x] $sqrt(x \times x + y \times y) \Rightarrow hypot(x, y)$
  * [x] $log(exp(x)) \Rightarrow x$
  * [x] $exp(log(x)) \Rightarrow x$
  * [x] $sqrt(x) \times sqrt(y) \Rightarrow sqrt(x \times y)$
  * [x] $sqrt(x) / sqrt(y) \Rightarrow sqrt(x / y)$
  * [ ] ······
* [x] 整合单参多项式变换和数学函数表换 ★★★
* [ ] 支持含双参乃至多参变量的表达★★★
* [ ] 生成 mpfr 代码 ★★
* [ ] 整合误差检测功能 ★★
  * [ ] 单参表达式误差测试
  * [ ] 多参表达式误差测试
* [ ] 整合精度优化功能 ★★★
  * [ ] matlab获取轮廓线数据
  * [ ] 精度优化区间划分
  * [ ] 整合表达式重写
* [ ] 整合性能测试功能 ★
  * [ ] 单参表达式性能测试
  * [ ] 多参表达式性能测试
* [ ] 生成符合 herbie、daisy 等工具格式要求的数学表达式 ★
* [ ] ...... (更多功能)
