# exprAuto

Updating.....

#### 介绍
exprAuto支持数学表达式的解析、等价变换、mpfr 代码生成等功能，是一个命令行工具。

exprAuto基于 [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html) 实现。

数学表达式的BNF定义 (BNF定义详见参考链接 3)：

```
expr   -> expr + term | expr - term | term
term   -> term * factor | term / factor | factor / term | factor
factor -> number | variable | (expr) | func(expr, ...)
```

数学表达式示例：

* 1 + 2 + 3
* (1 + 2) * 3
* pow(x,y)
* log(x+1) + exp(x) - 1
* 1 / x + 1 / (x + 1)

#### 工作流程

数学表达式 -> **词法分析** -> 词法单元 -> **语法分析** -> 语法分析树(AST) -> **等价变换/代码生成**

#### 软件架构

- Lexer

  - Token
  - gettok()
  - getNextToken()

- Abstract Syntax Tree (aka Parse Tree)

  - ExprAST
  - NumberExprAST
  - VariableExprAST
  - BinaryExprAST
  - CallExprAST
  - PrototypeAST
  - FunctionAST

* Parser

  - ParseExpression()
  - ParseNumberExpr()
  - ParseIdentifierExpr()
  - ParseParenExpr()
  - ParseBinOpRHS(int, std::unique_ptr<ExprAST>) ★
  - ParseExpressin()
  - ParsePrototype()
  - ParseDefinition()
  - ParseTopLevelxpr()
  - ParseExtern()


- Print

  - PrintExpression(std::unique_ptr<ExprAST> &expr) ★★
  - PrintFunction(std::unique_ptr<FunctionAST> &fun) ★

- changeExpression
  - changeExpression(std::unique_ptr<ExprAST> &expr) ★★★

- Top-Level parsing
  - HandleDefinition()
  - HandleExtern()
  - HandleTopLevelExpression() ★
  - MainLoop()

#### 安装教程

1.  ``clang++ -g -O3 main.cpp `llvm-config --cxxflags` -o exprAuto.exe``

or just use `g++`

2.  `g++ main.cpp -o exprAuto.exe` (the simple and suggested way)

#### 使用说明

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

#### 参考链接

1. [LLVM教程--第二章 实现语法分析器和AST](https://llvm-tutorial-cn.readthedocs.io/en/latest/chapter-2.html): 本项目代码框架
2. [LLVM Tutorial -- 2. Kaleidoscope: Implementing a Parser and AST](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html): 上述教程的英文原版
3. [从编译原理看一个解释器的实现](https://zhuanlan.zhihu.com/p/27450417): 辅助理解数学表达式的解析方法

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
