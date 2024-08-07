#ifndef _TOOLS
#define _TOOLS

#include <iostream>
#include <vector>
#include "basic.hpp"

using std::string;
using std::vector;

string getUniqueLabel();

vector<double> getIntervals(string interval, const char *split);

vector<int> getScales(string scale, const char *split);

void sampleError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales);

exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales, const vector<int> &startNowIdxs, const vector<double> &startOriginIntervals, const vector<double> &steps, bool errfile = false);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, int scale);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size);

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size);

void geneErrorDetectScript(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales, const vector<int> &startNowIdxs, const vector<double> &startOriginIntervals, const vector<double> &steps);

double testPerformance(string uniqueLabel, string suffix, const vector<double> &intervals);

string geneBoundaryData(string uniqueLabel, string suffix, double &costTime);

vector<string> geneBoundaryData(string uniqueLabel, string suffix, vector<string> suffixes, double &costTime);

vector<string> geneIntervalData(vector<string> upEdgeFileNames, string uniqueLabel, vector<double> &thresholds, int &numIntervalsBefore);

vector<vector<double>> getIntervalData(string filename);

vector<vector<double>> getIntervalData(vector<string> upEdgeFileNames, vector<double> &thresholds, vector<double> &intervals, int &numIntervalsBefore, vector<int> &numIntervalsSoloBefore, vector<int> &numIntervalsSoloAfter);

vector<exprInfo> rewrite(string exprStr, string uniqueLabel, vector<vector<double>> &intervalData, int &numOfExprs, const vector<double> &intervals);

vector<vector<double>> permuteMultiVec(vector<vector<double>> vec);

#endif