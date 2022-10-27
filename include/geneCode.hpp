#ifndef _GENECODE
#define _GENECODE

#include "basic.hpp"
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

void geneHerbieCode(string exprstr, vector<string> cs, string exprname, float v[], float u[]);

void geneDaisyCode(string exprStr);

string geneMpfrCode(const string exprStr, const string uniqueLabel, const string tail);

#endif