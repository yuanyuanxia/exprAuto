# exprAuto

- [exprAuto](#exprauto)
  - [介绍](#介绍)
    - [工作流程](#工作流程)
    - [软件架构](#软件架构)
    - [编译运行](#编译运行)
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
│   └── main.exe
├── include
│   ├── basic.hpp
│   ├── changeAST.hpp
│   ├── createExpr.hpp
│   ├── exprAuto.hpp
│   ├── funcInfo.hpp
│   ├── laxerAST.hpp
│   ├── mathfunctransAST.hpp
│   ├── monoInfo.hpp
│   ├── parserAST.hpp
│   ├── polyInfo.hpp
│   ├── printAST.hpp
│   └── variableInfo.hpp
├── Makefile
├── objs
├── README.md
└── src
    ├── changeAST.cpp
    ├── createExpr.cpp
    ├── exprAuto.cpp
    ├── funcInfo.cpp
    ├── laxerAST.cpp
    ├── main.cpp
    ├── mathfunctransAST.cpp
    ├── monoInfo.cpp
    ├── parserAST.cpp
    ├── printAST.cpp
    └── variableInfo.cpp
```

主要类、方法如下：
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
- PrintAST
  - std::string **PrintExpression**(std::unique_ptr\<ExprAST> &expr)  ★★
  - void **PrintFunction**(std::unique_ptr\<FunctionAST> &fun) ★
- ChangeExpression
  - std::unique_ptr\<ExprAST> **expandExpr**(std::unique_ptr\<ExprAST> &expr) ★★★
  - bool **isEqual**(std::unique_ptr\<ExprAST> &expr1, std::unique_ptr\<ExprAST> &expr2) ★
- CreateExpr
  - vector<std::unique_ptr\<ExprAST>> **createExpr**(const std::unique_ptr\<ExprAST> exprInit)
  - void **getReady**(const std::unique_ptr\<ExprAST> &expr, std::string *variablePtr, int *term, double *coefficient, int *lenPtr)
  - std::vector<std::unique_ptr\<ExprAST>> **createMiddle**(const std::string variable, const int *term, const double *coefficient, const int len)
- Mathfunctrans
  - std::unique_ptr<ExprAST> **expToexpm1**(const std::unique_ptr<ExprAST> &expr)
  - **logTolog1**
  - **sqrtTohypot**
- ExprAuto
  - std::unique_ptr<ExprAST> **preprocess**(const std::unique_ptr<ExprAST> &expr)
  - std::vector<std::unique_ptr<ExprAST>> **rewriteExprWrapper**(std::unique_ptr<ExprAST> &expr)
  - std::vector<std::unique_ptr<ExprAST>> **createAll**(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators)
  - ······


### 编译运行

```bash
git clone https://gitee.com/mathlib/expr-auto.git
cd path/to/exprAuto
mkdir objs  # if "objs" directory do not exist
mkdir bin   # if "bin" directory do not exist
make
./bin/main.exe
```

OR

```bash
git clone https://gitee.com/mathlib/expr-auto.git
cd path/to/exprAuto
cd src
g++ *.c -o main.exe
./main.exe
```

```
cd path/to/exprAuto
make
./bin/exprAuto.exe
```
will get

**TODO**: update the follwing screen output which is the earliest version.
```
ready> (1+3)*4
```

"(1+3)*4" is the input expression.

then, press the Enter key, will generate the **`PrintExpression`** mehtod's result.

```
ready> Parsed a top-level expr
funcName: __anon_expr
Args list:
        empty args
Func Body:
expr type: Binary;      op: *
expr type: Binary;      op: +
expr type: Number;      number: 1.000000
expr type: Number;      number: 3.000000
expr type: Number;      number: 4.000000
        ((1.000000+3.000000)*4.000000)
```

## 参考链接

1. [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html): 本项目代码框架
2. [LLVM Tutorial -- 2. Kaleidoscope: Implementing a Parser and AST](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html): 上述教程的英文原版
3. [从编译原理看一个解释器的实现](https://zhuanlan.zhihu.com/p/27450417): 辅助理解数学表达式的解析方法


## 目前进度

* [x] 优化代码框架
* [x] 打印数学表达式内容
* [x] 展开（消除括号）数学表达式
* [x] 支持负数和除法
* [x] 判断2个表达式是否相等：isEqual(expr1, expr2) return *true* or *false* ★★
* [x] 计算表达式中的常数子项：$1+3.4/2+x \Rightarrow 2.7 + x$ ★
* [x] 合并含单参变量的表达式中的同类项：$x+2 \times x \Rightarrow 3 \times x$ ★★
* [x] 按阶数排序含单参变量的表达式：$5 \times x + 4 \times x \times x + 1 \Rightarrow 1 + 5 \times x + 4 \times x \times x$ ★★
* [x] 规范化含除法的表达式：$\frac{1}{x+1} - \frac{1}{x} \Rightarrow \frac{x-(x+1)}{(x+1) \times x}$ ★★★
* [x] 支持多项式等价变换 ★★★
* [ ] 支持数学函数等价变换 ★★★
  * [x] $exp(x)-1 \Rightarrow expm1(x)$
  * [x] $log(x+1) \Rightarrow log1p(x)$
  * [x] $sqrt(x \times x + y \times y) \Rightarrow hypot(x, y)$
  * [x] $log(exp(x)) \Rightarrow x$
  * [x] $exp(log(x)) \Rightarrow x$
  * [ ] $sqrt(x) \times sqrt(y) \Rightarrow sqrt(x \times y)$
  * [ ] $sqrt(x) / sqrt(y) \Rightarrow sqrt(x / y)$
* [ ] 整合单参多项式变换和数学函数表换 ★★★
* [ ] 支持含双参乃至多参变量的表达式 ★★★
* [ ] 生成 mpfr 代码 ★★
* [ ] 生成符合 herbie、daisy 等工具格式要求的数学表达式 ★
* ······
