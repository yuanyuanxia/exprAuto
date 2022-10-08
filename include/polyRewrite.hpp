#ifndef _CREATEEXPR
#define _CREATEEXPR

#include "basic.hpp"
#include "monoFracInfo.hpp"

using std::vector;
using std::string;

template <typename T>
void reverseMine(T *p, size_t size);

double getCoefficient(ast_ptr &expr);

void getTermSingle(BinaryExprAST *binOpPtr, int *term, double *coefficient);

string getVariableInvStr(ast_ptr &expr);

string getVariableStr(const ast_ptr &expr);

// 获取多项式的系数和阶数
void getReady(const ast_ptr &expr, string *variablePtr, int *term, double *coefficient, size_t *lenPtr);

ast_ptr createSingle(const string variable, const int term, const double coefficient, ast_ptr expr);

ast_ptr createBA(const string variable, const int *term, const double *coefficient, int len);

ast_ptr createContinuedMul(const string variable, const int commonDegree);

vector<ast_ptr> joinExpr(ast_ptr exprBefore, ast_ptr common,
                                               vector<ast_ptr> exprMiddles, ast_ptr exprAfter);

vector<ast_ptr> createMiddle(const string variable, const int *term, const double *coefficient, const int len);

vector<ast_ptr> createExpr(const ast_ptr &exprInit);

vector<ast_ptr> createExpr(const vector<monoInfo> &monomials);

vector<ast_ptr> createExpr(const vector<monoFracInfo> &monoFracs);

#endif
