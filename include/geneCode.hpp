#ifndef _GENECODE
#define _GENECODE

#include "basic.hpp"
#include "tools.hpp"

#include<stdio.h>
#include <fstream>
#include<iostream>
#include<vector>
using namespace std;
using std::cout;
using std::cin;
using std::vector;
using std::endl;
using std::string;

vector<string> getVariablesFromExpr(const ast_ptr &expr);

bool getVariablesFromExpr(const ast_ptr &expr, vector<string> &vars);

string geneExprCodeKernel(string exprStr, vector<string> vars, string uniqueLabel,string tail);

string geneExprCode(string exprStr, string uniqueLabel, string tail);

string geneTGenCode(string exprStr, vector<string> vars, string uniqueLabel, string tail);

void geneHerbieCode(string exprstr, vector<string> cs, string exprname, double v[], double u[]);

string geneHerbieCode(string uniqueLabel);

string geneDaisyCode(string uniqueLabel);

string geneMpfrCode(const ast_ptr &exprAst, const string uniqueLabel, vector<string> vars);

string geneMpfrCode(const string exprStr, const string uniqueLabel, vector<string> vars);

string geneFinalCodeKernel(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector, vector<string> vars);

string geneFinalCode(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector);

void getDepth(ast_ptr &expr, int &depth);

vector<int> setOrder(ast_ptr &expr);

int showOrder(ast_ptr &expr);

int codegenWrapper(ast_ptr &expr, vector<string> &vars, const string uniqueLabel, string tail, std::map<string, double *> values, size_t inputNum);

#endif