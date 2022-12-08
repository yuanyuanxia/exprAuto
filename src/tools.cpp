#include "tools.hpp"
#include "exprAuto.hpp"
#include "parserASTLY.hpp"
#include "basic.hpp"
#include "geneCode.hpp"
#include "color.hpp"
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
#include <iomanip>
#include <chrono>
#include <fmt/core.h>

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::to_string;

//get unique label(timestamp)
string getUniqueLabel()
{
    time_t currentTime = time(NULL);
    char chCurrentTime[256];
    strftime(chCurrentTime, sizeof(chCurrentTime), "%Y%m%d%H%M%S", localtime(&currentTime));
    string uniqueLabel = chCurrentTime;
    return uniqueLabel;
}

//get init interval info
vector<double> getIntervals(string interval, const char *split)
{
    char *p;
    char *a = new char[interval.length() + 1]();
    size_t i;
    for (i = 0; i < interval.length(); i++)
    {
        a[i] = interval[i];
    }
    a[i] = '\0';
    vector<double> intervalVector;
    p = strtok(a, split);
    while (p != NULL)
    {
        intervalVector.push_back(atof(p));
        p = strtok(NULL, split);
    }

    delete [] a;
    return intervalVector;
}

//get init scale info
vector<int> getScales(string scale, const char *split)
{
    char *q;
    char *b = new char[scale.length() + 1]();
    size_t i;
    for (i = 0; i < scale.length(); i++)
    {
        b[i] = scale[i];
    }
    b[i] = '\0';
    vector<int> scaleVector;
    q = strtok(b, split);
    while (q != NULL)
    {
        scaleVector.push_back(stoi(q));
        q = strtok(NULL, split);
    }
    delete [] b;
    return scaleVector;
}

// test error
// old method, useless
exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, int x0Size)
{
    string param1 = fmt::format("{}", x0Start);
    string param2 = fmt::format("{}", x0End);
    string param3 = fmt::format("{}", x0Size);

    string prefix = "expr_" + uniqueLabel;
    string middle = param1 + "_" + param2 + "_" + param3;
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorOneFPEDParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + param1 + " " + param2 + " " + param3 + " " + prefix + " " + middle + " " + suffix;
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

        tempError.intervals = intervals;
        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}
// old method, useless
exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size)
{
    string param1 = fmt::format("{}", x0Start);
    string param2 = fmt::format("{}", x0End);
    string param3 = fmt::format("{}", x1Start);
    string param4 = fmt::format("{}", x1End);
    string param5 = fmt::format("{}", x0Size);
    string param6 = fmt::format("{}", x1Size);


    string prefix = "expr_" + uniqueLabel;
    string middle = param1 + "_" + param2 + "_" + param3 + "_" + param4 + "_" + param5 + "_" + param6;
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorTwoFPEDParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + param1 + " " + param2 + " " + param3 + " " + param4 + " " + param5 + " " + param6 + " " + prefix + " " + middle + " " + suffix;
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
// old method, useless
exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size)
{
    string param1 = fmt::format("{}", x0Start);
    string param2 = fmt::format("{}", x0End);
    string param3 = fmt::format("{}", x1Start);
    string param4 = fmt::format("{}", x1End);
    string param5 = fmt::format("{}", x2Start);
    string param6 = fmt::format("{}", x2End);
    string param7 = fmt::format("{}", x0Size);
    string param8 = fmt::format("{}", x1Size);
    string param9 = fmt::format("{}", x2Size);

    string prefix = "expr_" + uniqueLabel;
    string middle = param1 + "_" + param2 + "_" + param3 + "_" + param4 + "_" + param5 + "_" + param6 + "_" + param7 + "_" + param8 + "_" + param9;
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorThreeFPEDParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + param1 + " " + param2 + " " + param3 + " " + param4 + " " + param5 + " " + param6 + " " + param7 + " " + param8 + " " + param9 + " " + prefix + " " + middle + " " + suffix;
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
// for multi parameters, will be useless
exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals)
{
    vector<string> parameters;
    string prefix = "expr_" + uniqueLabel;
    string middle;
    string commandStr = " " + uniqueLabel;
    for(size_t i = 0; i < intervals.size(); ++i)
    {
        auto temp = fmt::format("{}", intervals.at(i));
        parameters.push_back(temp);
        if(i == 0)
        {
            middle = temp;
        }
        else
        {
            middle = middle + "_" + temp;
        }
        commandStr = commandStr + " " + temp;
    }
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + uniqueLabel + "/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorMulti.sh";
    commandStr = scriptName + commandStr + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;
    tempError.intervals = intervals;

    char ch;
    ifs >> ch;
    if(ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();

        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr);  // discard first line
        std::getline(ifs, lineStr);  // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t";  // Sets the delimiter for the string to be decomposed
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}

exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales)
{
    exprInfo tempError;
    size_t size = scales.size();

    if (size < 4)
    {
        string prefix = "expr_" + uniqueLabel;
        vector<string> params;
        for(const auto &interval : intervals)
        {
            auto paraTmp = fmt::format("{}", interval);
            params.push_back(paraTmp);
        }
        for(const auto &scale : scales)
        {
            auto scaleTmp = fmt::format("{}", scale);
            params.push_back(scaleTmp);
        }
        string middle;
        for(size_t i = 0; i < params.size(); ++i)
        {
            if(i == 0)
            {
                middle = params.at(i);
            }
            else
            {
                middle = middle + "_" + params.at(i);
            }
        }
        string fileNameKernel = prefix + "__" + middle + "_" + suffix;
        string testName = "./outputs/" + uniqueLabel + "/" + fileNameKernel + "_error.txt";
        string number[3] = {"One", "Two", "Three"};
        string scriptName = "./detectError" + number[size - 1] + "FPEDParallel.sh";
        string commandStr = scriptName + " " + uniqueLabel;
        for(const auto & param : params)
        {
            commandStr = commandStr + " " + param;
        }
        commandStr = commandStr + " " + prefix + " " + middle + " " + suffix;

        cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
        char command[200] = {0};
        strcat(command, commandStr.c_str());
        system(command);

        std::ifstream ifs(testName, std::ios::in);
        if(ifs.eof())
        {
            std::cout << "is null" << std::endl;
            ifs.close();

            tempError.intervals = intervals;
            tempError.aveError = 0;
            tempError.maxError = 0;
        }
        else
        {
            std::string lineStr;
            std::getline(ifs, lineStr);  // discard first line
            std::getline(ifs, lineStr);  // get the second line
            char *line = (char *)calloc(lineStr.length(), sizeof(char));
            strcpy(line, lineStr.c_str());
            const char *delim = " ,\t";  // Sets the delimiter for the string to be decomposed
            string aveErrorTemp = strtok(line, delim);
            string maxErrorTemp = strtok(NULL, delim);
            // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

            tempError.intervals = intervals;
            tempError.aveError = atof(aveErrorTemp.c_str());
            tempError.maxError = atof(maxErrorTemp.c_str());
        }

        return tempError;
    }
    else if (size == 4)
    {
        tempError = testError(uniqueLabel, suffix, intervals);
    }
    else
    {
        fprintf(stderr, "WRONG: rewrite: the intervalTmp's dimension is %ld, which we don't support now.\n", size);
        exit(EXIT_FAILURE);
    }

    return tempError;
}

// TODO: implement
// call matlab to generate the boundaryData to file
void geneBoundaryData(string uniqueLabel, string suffix)
{
    suffix = "origin";
    string filename = "./outputs/" + uniqueLabel + "/expr_" + uniqueLabel + "_" + suffix + "_boudary.txt";
    std::ofstream ofs;
    ofs.open(filename, std::ios::out);
    ofs << "123" << std::endl;
    ofs.close();
    std::cout << "generate boundaryData file:" << filename << std::endl;
}

// TODO: improve
// generate the interval info to file
void geneIntervalData(string uniqueLabel, vector<string> &intervals, vector<double> &threholds)
{
    // filename = "expr_" + uniqueLabel + "_interval.txt";
    for (size_t j = 1; j < threholds.size(); j++)
    {
        intervals.push_back(to_string(j * 2));
        intervals.push_back(to_string(j * 2 + 1));
        threholds.push_back(j + 0.0002);
    }
    for (size_t i = 0; i < threholds.size(); i++)
    {
        string fileName;
        string prefix = "./outputs/" + uniqueLabel + "/expr_" + uniqueLabel;
        string middle = "_" + intervals.at(i * 2) + "_" + intervals.at(i * 2 + 1) + "_" + to_string(threholds.at(i));
        string suffix = "_interval.txt";
        fileName = prefix + middle + suffix;
        std::ofstream ofs;
        ofs.open(fileName, std::ios::out);
        ofs << "123" << std::endl;
        ofs.close();
        std::cout << "generate boudaryDate file:" << fileName << std::endl;
    }
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
        vector<int> scales(dimension, scale);
        auto newTempExprs = exprAutoWrapper(tempExpr, intervalTmp, scales);
        string suffix = "temp_" + std::to_string(count) + "_";

        // pick the best rewrite expression
        string bestRewriteExpr;
        double maxError = -1;
        double aveError = 0;
        size_t maxIdx = -1;
        size_t jEnd = min(newTempExprs.size(), size_t(100000));
        // std::ofstream fTimeout;
        // fTimeout.open("./tmpTimeResult.txt");
        // if (!fTimeout.is_open())
        // {
        //     cout << "open tmpResult.txt failed";
        //     exit(EXIT_FAILURE);
        // }

        for (size_t j = 0; j < jEnd; j++)
        {
            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << endl;
            string newExpr = PrintExpression(newTempExprs.at(j));
            cout << "No." << j << ": " << newExpr << endl;

            // generate function code and test error
            string suffixTmp = suffix + std::to_string(j);
            geneOriginCode(newExpr, uniqueLabel, suffixTmp);
            // auto timeTmp1 = std::chrono::high_resolution_clock::now();
            auto tempError = testError(uniqueLabel, suffixTmp, intervalTmp, scales);
            // auto timeTmp2 = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> testError_seconds = timeTmp2 - timeTmp1;
            // cout << BLUE << "rewrite: For NO." << j << ": testError time: " << testError_seconds.count() << " s" << RESET << endl;
            // fTimeout << testError_seconds.count() << endl;
            if (j == 0)
            {
                maxError = tempError.maxError;
                bestRewriteExpr = newExpr;
                aveError = tempError.aveError;
                maxIdx = j;
            }
            else
            {
                if ((tempError.maxError < maxError) || ((tempError.maxError == maxError) && (tempError.aveError < aveError)))
                {
                    maxError = tempError.maxError;
                    bestRewriteExpr = newExpr;
                    aveError = tempError.aveError;
                    maxIdx = j;
                }
            }

            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n"
                 << endl;
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

vector<vector<double>> zuhe(vector<vector<double>> vec)
{
    vector<vector<double>> suzu;
    size_t row = vec.size();
    if (row == 3)
    {
        int rowSize = vec.size();      // 确定有几行
        int columnSize = 2 * rowSize;  // 新生成二维数组的列数
        int *b = new int[rowSize]();   // b[]存储每一行的元素个数
        int *f = new int[rowSize]();   // f[]中的每个元素是b[]中的0.5倍。即每一行有几对。
        int *d = new int[rowSize]();   // 建立数组，求e值
        for(size_t i = 0; i < vec.size(); i++)
        {
            b[i] = vec[i].size();
            f[i] = 0.5 * b[i];
            d[i] = 0.5 * b[i];
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            d[i + 1] = d[i] * d[i + 1];
        }
        int e = d[vec.size() - 1]; //新生成二维数组的行数为e
        double **sz;
        sz = new double *[e];
        for(int i = 0; i < e; i++)
        {
            sz[i] = new double[columnSize]();
        }

        for (int i = 0; i < e; i++) //确定前两列
        {
            int j = i % f[0];
            sz[i][0] = vec[0][2 * j];
            sz[i][1] = vec[0][2 * j + 1];
        }
        for (int i = 0; i < f[2]; i++) //确定中间两列
        {
            for (int j = 0; j < f[1]; j++)
            {
                for (int k = 0; k < f[0]; k++)
                {
                    sz[k + j * f[0] + i * f[0] * f[1]][2] = vec[1][2 * j];
                    sz[k + j * f[0] + i * f[0] * f[1]][3] = vec[1][2 * j + 1];
                }
            }
        }
        for (int i = 0; i < f[2]; i++) //确定最后两列
        {
            for (int j = 0; j < f[0] * f[1]; j++)
            {
                sz[j + i * f[0] * f[1]][4] = vec[2][2 * i];
                sz[j + i * f[0] * f[1]][5] = vec[2][2 * i + 1];
            }
        }
        for (int i = 0; i < e; i++)
        {
            vector<double> tmps;
            for (int j = 0; j < columnSize; j++)
            {
                tmps.push_back(sz[i][j]);
            }
            suzu.push_back(tmps);
        }

        delete [] b;
        delete [] f;
        delete [] d;
        for(int i = 0; i < e; i++)
        {
            delete[] sz[i];
        }
        delete[] sz;
    }
    else if (row == 2)
    {
        int rowSize = vec.size();     //确定有几行
        int columnSize = 2 * rowSize; //新生成二维数组的列数
        int *b = new int[rowSize]();   // b[]存储每一行的元素个数
        int *f = new int[rowSize]();   // f[]中的每个元素是b[]中的0.5倍。即每一行有几对。
        int *d = new int[rowSize]();   // 建立数组，求e值
        for (size_t i = 0; i < vec.size(); i++)
        {
            b[i] = vec[i].size();
            f[i] = 0.5 * b[i];
            d[i] = 0.5 * b[i];
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            d[i + 1] = d[i] * d[i + 1];
        }
        int e = d[vec.size() - 1]; //新生成二维数组的行数为e
        double **sz;
        sz = new double *[e];
        for(int i = 0; i < e; i++)
        {
            sz[i] = new double[2 * rowSize]();
        }
        for (int i = 0; i < e; i++) //确定前两列
        {
            int j = i % f[0];
            sz[i][0] = vec[0][2 * j];
            sz[i][1] = vec[0][2 * j + 1];
        }
        for (int i = 0; i < f[1]; i++) //确定最后两列
        {
            for (int j = 0; j < f[0]; j++)
            {
                sz[j + i * f[0]][2] = vec[1][2 * i];
                sz[j + i * f[0]][3] = vec[1][2 * i + 1];
            }
        }
        for (int i = 0; i < e; i++)
        {
            vector<double> tmpss;
            for (int j = 0; j < columnSize; j++)
            {
                tmpss.push_back(sz[i][j]);
            }
            suzu.push_back(tmpss);
        }

        delete [] b;
        delete [] f;
        delete [] d;
        for(int i = 0; i < e; i++)
        {
            delete[] sz[i];
        }
        delete[] sz;
    }
    else if (row == 1)
    {
        size_t rowSize = vec[0].size();
        int b = 0.5 * rowSize;
        double **sz;
        sz = new double *[b];
        for(int i = 0; i < b; i++)
        {
            sz[i] = new double[2]();
        }
        for (int i = 0; i < b; i++)
        {
            sz[i][0] = vec[0][2 * i];
            sz[i][1] = vec[0][2 * i + 1];
        }
        for (int i = 0; i < b; i++)
        {
            vector<double> tmps;
            for (int j = 0; j < 2; j++)
            {
                tmps.push_back(sz[i][j]);
            }
            suzu.push_back(tmps);
        }
        for(int i = 0; i < b; i++)
        {
            delete[] sz[i];
        }
        delete[] sz;
    }
    else
    {
        cout << "The prime group you entered does not conform to the specification!" << endl;
    }
    return suzu;
}