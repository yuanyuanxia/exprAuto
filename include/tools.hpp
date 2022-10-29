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
    vector<double> intervals;
    string exprStr;
    double error;
    double aveError;
    double maxError;
    double performance;
};

string getUniqueLabel();

void testError(string uniqueLabel, string suffix, double start, double end, int scale);

void testError(string uniqueLabel, string suffix, double xStart, double xEnd, double yStart, double yEnd, int xScale, int yScale);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size);

void geneBoundaryData(string exprStr, string uniqueLabel);

void geneIntervalData(string exprStr, string unqiueLabel);

vector<string> getIntervalData();

vector<exprInfo> rewrite(string exprSr, string uniqueLabel);

void geneFinalCode(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector);

#endif