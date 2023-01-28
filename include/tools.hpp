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
    size_t rewriteID;
};

string getUniqueLabel();

vector<double> getIntervals(string interval, const char *split);

vector<int> getScales(string scale, const char *split);

exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, int scale);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size);

double testPerformance(string uniqueLabel, string suffix, const vector<double> &intervals);

void geneBoundaryData(string uniqueLabel, string suffix);

void geneIntervalData(string uniqeuLabel, vector<string> &intervals, vector<double> &threholds);

vector<string> getIntervalData();

vector<vector<double>> getIntervalData(string filename);

vector<exprInfo> rewrite(string exprSr, string uniqueLabel);

vector<vector<double>> zuhe(vector<vector<double>> vec);

#endif