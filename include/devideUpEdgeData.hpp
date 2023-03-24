#ifndef _DEVIDEUPEDGEDATA
#define _DEVIDEUPEDGEDATA
// void devideUpEdgeData(string upEdgeFileName, string intervalFileName, double threshold, double thresholdCombine = 101);
vector<double> devideUpEdgeData(string upEdgeFileName, double &threshold, int &numIntervalsBefore, double thresholdCombine = 0);
#endif