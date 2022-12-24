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

string geneOriginCodeKernel(string exprStr, vector<string> vars, string uniqueLabel,string tail);

string geneOriginCode(string exprStr, string uniqueLabel, string tail);

void geneHerbieCode(string exprstr, vector<string> cs, string exprname,double v[], double u[]);

void geneDaisyCode(string exprStr);

string geneMpfrCode(const ast_ptr &exprAst, const string uniqueLabel, vector<string> vars);

string geneMpfrCode(const string exprStr, const string uniqueLabel, vector<string> vars);

string geneFinalCodeKernel(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector, vector<string> vars);

string geneFinalCode(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector);

void getDepth(ast_ptr &expr, size_t &depth);

void codegen(ast_ptr &expr, vector<string> &vars);

#endif