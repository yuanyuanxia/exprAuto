#include "tools.hpp"
#include "exprAuto.hpp"
#include "parserASTLY.hpp"
#include "basic.hpp"
#include "geneCode.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::to_string;

//获取唯一标识符(时间戳)
string getUniqueLabel()
{
    time_t currentTime = time(NULL);
    char chCurrentTime[256];
    strftime(chCurrentTime, sizeof(chCurrentTime), "%Y%m%d%H%M%S", localtime(&currentTime));
    string uniqueLabel = chCurrentTime;
    return uniqueLabel;
}

// TODO: set the return value as a exprInfo object
// test error
void testError(string uniqueLabel, string suffix, double start, double end, int scale)
{
    string scriptName = "./detectErrorOne.sh";
    char command[200] = {0};
    string file = scriptName + " ";
    strcpy(command, file.c_str());

    // parameter1
    string parameter1 = uniqueLabel;
    parameter1 = parameter1 + " ";
    strcat(command, parameter1.c_str());

    // parameter1.1
    string parameter11 = suffix + " ";
    strcat(command, parameter11.c_str());

    // parameter2
    string parameter2 = std::to_string(start);
    parameter2 = parameter2 + " ";
    strcat(command, parameter2.c_str());

    // parameter3
    string parameter3 = std::to_string(end);
    parameter3 = parameter3 + " ";
    strcat(command, parameter3.c_str());

    string parameter4 = std::to_string(scale);
    parameter4 = parameter4 + " ";
    strcat(command, parameter4.c_str());

    cout << "command: " << command << endl;
    system(command);
}

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size)
{
    std::ostringstream os1;
    os1 << x0Start;
    std::ostringstream os2;
    os2 << x0End;
    std::ostringstream os3;
    os3 << x1Start;
    std::ostringstream os4;
    os4 << x1End;
    
    std::ostringstream os7;
    os7 << x0Size;
    std::ostringstream os8;
    os8 << x1Size;


    string prefix = "expr_" + uniqueLabel;
    string middle = os1.str() + "_" + os2.str() + "_" + os3.str() + "_" + os4.str() + "_" + os7.str() + "_" + os8.str();
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorTwo.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + os1.str() + " " + os2.str() + " " + os3.str() + " " + os4.str() + " " + os7.str() + " " + os8.str() + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;
    
    char ch;
    ifs >> ch;
    if (ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();
        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);

        tempError.intervals = intervals;
        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr); // discard first line 
        std::getline(ifs, lineStr); // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));        
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t"; // Sets the delimiter for the string to be decomposed 
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);

        tempError.intervals = intervals;
        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size)
{
    std::ostringstream os1;
    os1 << x0Start;
    std::ostringstream os2;
    os2 << x0End;
    std::ostringstream os3;
    os3 << x1Start;
    std::ostringstream os4;
    os4 << x1End;
    std::ostringstream os5;
    os5 << x2Start;
    std::ostringstream os6;
    os6 << x2End;
    std::ostringstream os7;
    os7 << x0Size;
    std::ostringstream os8;
    os8 << x1Size;
    std::ostringstream os9;
    os9 << x2Size;

    string prefix = "expr_" + uniqueLabel;
    string middle = os1.str() + "_" + os2.str() + "_" + os3.str() + "_" + os4.str() + "_" + os5.str() + "_" + os6.str() + "_" + os7.str() + "_" + os8.str() + "_" + os9.str();
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorThreeParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + os1.str() + " " + os2.str() + " " + os3.str() + " " + os4.str() + " " + os5.str() + " " + os6.str() + " " + os7.str() + " " + os8.str() + " " + os9.str() + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;
    
    char ch;
    ifs >> ch;
    if (ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();
        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);
        intervals.push_back(x2Start);
        intervals.push_back(x2End);

        tempError.intervals = intervals;
        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr); // discard first line 
        std::getline(ifs, lineStr); // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));        
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t"; // Sets the delimiter for the string to be decomposed 
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);
        intervals.push_back(x2Start);
        intervals.push_back(x2End);

        tempError.intervals = intervals;
        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}

// TODO: implement
// call matlab to generate the boundaryData to file
void geneBoundaryData(string exprStr, string uniqueLabel)
{
    string filename = "expr_" + uniqueLabel + "_boudary.txt";
    std::ofstream ofs;
    ofs.open(filename, std::ios::out);
    ofs << "123" << std::endl;
    ofs.close();
    std::cout << "generate boundaryData file:" << filename << std::endl;
}

// TODO: improve
// generate the interval info to file
void geneIntervalData(string exprStr, string uniqueLabel)
{
    string filename = "expr_" + uniqueLabel + "_interval.txt";
    std::ofstream ofs;
    ofs.open(filename, std::ios::out);
    ofs << "123" << std::endl;
    ofs.close();
    std::cout << "generate boundaryData file:" << filename << std::endl;
}

//生成区间数据
vector<vector<double>> getIntervalData(string filename)
{
    vector<vector<double>> intervalData;
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "ERROR: getIntervalData: fail to open " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    string buf;
    while (getline(ifs, buf))
    {
        vector<double> lineData;
        // cout << buf << endl;
        int flag = 1;
        vector<int> index;
        for (size_t i = 0; i < buf.length(); i++)
        {
            if (buf[i] == ' ')
            {
                flag += 1;
                index.push_back(i);
            }
        }
        for (size_t i = 0; i < index.size(); i++)
        {
            if (i == 0)
            {
                string data = buf.substr(0, index.at(i));
                lineData.push_back(atof(data.c_str()));
            }
            else
            {
                int n = index.at(i) - index.at(i - 1) - 1;
                string data = buf.substr(index.at(i - 1) + 1, n);
                lineData.push_back(atof(data.c_str()));
            }
        }
        int pos = index.back() + 1;
        int n = buf.length() - 1 - index.at(index.size() - 1);
        string last = buf.substr(pos, n);
        lineData.push_back(atof(last.c_str()));
        intervalData.push_back(lineData);
    }
    ifs.close();
    return intervalData;
}

// call exprAuto to rewrite the input expression
vector<exprInfo> rewrite(string exprStr, string uniqueLabel)
{
    // string filename = "expr_" + uniqueLabel + "_interval.txt";
    string filename = "./intervalData.txt"; // TODO: get the filename from uniqueLabel

    auto intervalData = getIntervalData(filename);

    auto tempExpr = ParseExpressionFromString(exprStr);

    vector<exprInfo> exprInfoVector;

    // for (int i = 0; i < intervalData.size(); i++)
    size_t count = 0;
    for (auto &intervalTmp : intervalData)
    {
        auto newTempExprs = exprAutoWrapper(tempExpr);
        size_t scale;
        auto dimension = intervalTmp.size() / 2;
        if (dimension == 1)
            scale = 500000;
        else if (dimension == 2)
            scale = 1024;
        else if (dimension == 3)
            scale = 256;
        else
            scale = 10;

        string suffix = "temp_" + std::to_string(count) + "_";

        // pick the best rewrite expression
        string bestRewriteExpr;
        double maxError;
        double aveError;
        size_t maxIdx = -1;
        for (size_t j = 0; j < newTempExprs.size(); j++)
        {
            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << endl;
            string newExpr = PrintExpression(newTempExprs.at(j));
            cout << "No." << j << ": " << newExpr << endl;

            // generate function code and test error
            string suffixTmp = suffix + std::to_string(j);
            geneOriginCode(newExpr, uniqueLabel, suffixTmp);
            exprInfo tempError;
            switch (dimension)
            {
                case 1:
                    testError(uniqueLabel, suffixTmp, intervalTmp.at(0), intervalTmp.at(1), scale);
                    break;

                case 2:
                    tempError = testError(uniqueLabel, suffixTmp, intervalTmp.at(0), intervalTmp.at(1), intervalTmp.at(2), intervalTmp.at(3), scale, scale);
                    break;

                case 3:
                    tempError = testError(uniqueLabel, suffixTmp, intervalTmp.at(0), intervalTmp.at(1), intervalTmp.at(2), intervalTmp.at(3), intervalTmp.at(4), intervalTmp.at(5), scale, scale, scale);
                    break;

                default:
                    fprintf(stderr, "WRONG: rewrite: the intervalTmp's dimension is %ld, which we don't support now.\n", dimension);
                    exit(EXIT_FAILURE);
                    break;
            }
            if (j == 0)
            {
                maxError = tempError.maxError;
                bestRewriteExpr = newExpr;
                aveError = tempError.aveError;
                maxIdx = j;
            }
            else
            {
                if (tempError.maxError < maxError)
                {
                    maxError = tempError.maxError;
                    bestRewriteExpr = newExpr;
                    aveError = tempError.aveError;
                    maxIdx = j;
                }
            }

            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n" << endl;
        }

        exprInfo tempInfo;
        tempInfo.start = 0;
        tempInfo.end = 1;
        tempInfo.intervals = intervalTmp;
        tempInfo.exprStr = bestRewriteExpr;
        tempInfo.error = 0.1;
        tempInfo.aveError = aveError;
        tempInfo.maxError = maxError;
        tempInfo.performance = 0.2;
        tempInfo.rewriteID = maxIdx;
        
        exprInfoVector.push_back(tempInfo);

        count++;
    }
    return exprInfoVector;
}
