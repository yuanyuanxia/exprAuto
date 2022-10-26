#ifndef _TOOLS
#define _TOOLS
#include <iostream>
#include <vector>
using std::string;
using std::vector;
class exprInfo
{
public:
    double start;
    double end;
    string exprStr;
    double error;
    double performance;
};
string getUniqueLabel();
void testError(string uniqueLabel, double start, double end, int scale);
void geneBoundaryData(string exprStr, string uniqueLabel);
void geneIntervalData(string exprStr, string unqiueLabel);
vector<string> getIntervalData();
vector<exprInfo> rewrite(string exprSr, string uniqueLabel);
void geneFinalCode(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector);
#endif