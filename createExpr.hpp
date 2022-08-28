#ifndef _CREATEEXPR
#define _CREATEEXPR

#include "basic.hpp"
#include "printAST.hpp"

template <typename T>
void reverseMine(T *p, int size);

double getCoefficient(std::unique_ptr<ExprAST> &expr);

void getTermSingle(BinaryExprAST *binOpPtr, int *term, double *coefficient);

std::string getVariableInvStr(std::unique_ptr<ExprAST> &expr);

std::string getVariableStr(const std::unique_ptr<ExprAST> &expr);

// 获取多项式的系数和阶数
void getReady(const std::unique_ptr<ExprAST> &expr, std::string *variablePtr, int *term, double *coefficient, int *lenPtr);

std::unique_ptr<ExprAST> createSingle(const std::string variable, const int term, const double coefficient, std::unique_ptr<ExprAST> expr);

std::unique_ptr<ExprAST> createBA(const std::string variable, const int *term, const double *coefficient, int len);

std::unique_ptr<ExprAST> createContinuedMul(const std::string variable, const int commonDegree);

std::vector<std::unique_ptr<ExprAST>> joinExpr(std::unique_ptr<ExprAST> exprBefore, std::unique_ptr<ExprAST> common,
                                               std::vector<std::unique_ptr<ExprAST>> exprMiddles, std::unique_ptr<ExprAST> exprAfter);

std::vector<std::unique_ptr<ExprAST>> createMiddle(const std::string variable, const int *term, const double *coefficient, const int len);

std::vector<std::unique_ptr<ExprAST>> createExpr(const std::unique_ptr<ExprAST> &exprInit);

#endif
