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
    string suffix;
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

exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales, bool errfile = false);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, int scale);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size);

double testPerformance(string uniqueLabel, string suffix, const vector<double> &intervals);

string geneBoundaryData(string uniqueLabel, string suffix);

vector<string> geneIntervalData(vector<string> upEdgeFileNames, string uniqueLabel, vector<double> &thresholds);

vector<vector<double>> getIntervalData(string filename);

vector<vector<double>> getIntervalData(vector<string> upEdgeFileNames, vector<double> &thresholds);

vector<exprInfo> rewrite(string exprStr, string uniqueLabel, vector<vector<double>> &intervalData);

vector<vector<double>> permuteMultiVec(vector<vector<double>> vec);

#endif