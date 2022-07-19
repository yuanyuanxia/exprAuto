# exprAuto

- [exprAuto](#exprauto)
  - [介绍](#介绍)
    - [工作流程](#工作流程)
    - [软件架构](#软件架构)
    - [安装教程](#安装教程)
    - [使用说明](#使用说明)
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
├── main.cpp
├── basic.hpp
├── laxerAST.hpp
├── parserAST.hpp
├── printAST.hpp  ★★
├── changeAST.hpp ★★★
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
- Print
  - PrintExpression(std::unique_ptr\<ExprAST> &expr)  ★★
  - PrintFunction(std::unique_ptr\<FunctionAST> &fun) ★
- changeExpression
  - changeExpression(std::unique_ptr\<ExprAST> &expr) ★★★

### 安装教程

1.  ``clang++ -g -O3 main.cpp `llvm-config --cxxflags` -o exprAuto.exe``

or just use `g++`

2.  `g++ main.cpp -o exprAuto.exe` (the simple and suggested way)

### 使用说明

1.  `./exprAuto.exe`

will get

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

已完成事项：

* 优化代码框架
* 打印数学表达式内容
* 展开（消除括号）数学表达式
* 支持负数和除法

待完成事项：

* 计算表达式中的常数子项
* 合并含单参变量的表达式中的同类项
* 按阶数排序含单参变量的表达式
* 排序含双参乃至多参变量的表达式
* 规范化含除法的表达式
* 支持各类等价变换规则
* 生成 mpfr 代码
* 生成符合 herbie、daisy 等工具格式要求的数学表达式
* ······
