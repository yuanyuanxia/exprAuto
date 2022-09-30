#ifndef _GENECODE
#define _GENECODE

#include "basic.hpp"

vector<string> getVariablesFromExpr(const ast_ptr &expr);

bool getVariablesFromExpr(const ast_ptr &expr, vector<string> &vars);

void geneCode(string exprStr, vector<string> vars);

void geneOriginCode(string exprStr);

void geneHerbieCode(string exprStr);

void geneDaisyCode(string exprStr);

void geneMpfrCode(string exprStr);

#endif