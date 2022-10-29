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

void testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size)
{
    string scriptName = "./detectErrorTwo.sh";
    char command[200] = {0};
    string file = scriptName + " ";
    strcpy(command, file.c_str());

    string param1 = uniqueLabel + " ";
    string param2 = suffix + " ";
    string param3 = std::to_string(x0Start) + " ";
    string param4 = std::to_string(x0End) + " ";
    string param5 = std::to_string(x1Start) + " ";
    string param6 = std::to_string(x1End) + " ";
    string param7 = std::to_string(x0Size) + " ";
    string param8 = std::to_string(x1Size) + " ";
    // strcat(command, param8.c_str());

    string params = param1 + param2 + param3 + param4 + param5 + param6 + param7 + param8;
    strcat(command, params.c_str());
    cout << "command: " << command << endl;
    system(command);
}

exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size)
{
    string scriptName = "./detectErrorThree.sh";
    char command[200] = {0};
    string file = scriptName + " ";
    strcpy(command, file.c_str());

    string param1 = uniqueLabel + " ";
    string param2 = suffix + " ";
    string param3 = std::to_string(x0Start) + " ";
    string param4 = std::to_string(x0End) + " ";
    string param5 = std::to_string(x1Start) + " ";
    string param6 = std::to_string(x1End) + " ";
    string param7 = std::to_string(x2Start) + " ";
    string param8 = std::to_string(x2End) + " ";
    string param9 = std::to_string(x0Size) + " ";
    string param10 = std::to_string(x1Size) + " ";
    string param11 = std::to_string(x2Size) + " ";
    // strcat(command, param8.c_str());

    string params = param1 + param2 + param3 + param4 + param5 + param6 + param7 + param8 + param9 + param10 + param11;
    strcat(command, params.c_str());
    cout << "command: " << command << endl;
    system(command);

    string fileName;
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
    std::string testName = "./outputs/expr_" + uniqueLabel + "__" + os1.str() + "_" + os2.str() + "_" + os3.str() + "_" + os4.str() + "_" + os5.str() + "_" + os6.str() + "_" + os7.str() + "_" + os8.str() + "_" + os9.str() + "_" + suffix + "_error.txt";
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
        std::ifstream ifs1(testName, std::ios::in);
        ifs1.open(testName, std::ios::in);
        std::string buf;
        std::getline(ifs1, buf);
        std::getline(ifs1, buf);

        int spacePos;

        for (int i = 0; i < buf.length(); i++)
        {
            if (buf[i] == ' ')
            {
                spacePos = i;
                break;
            }
        }
        aveError = atof(buf.substr(0, spacePos).c_str());
        maxError = atof(buf.substr(spacePos + 1, buf.length() - 1 - spacePos).c_str());

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
    string filename = "./intervalData.txt"; // TODO: get the filename from uniqueLabel

    auto intervalData = getIntervalData(filename);

    auto tempExpr = ParseExpressionFromString(exprStr);

    vector<exprInfo> exprInfoVector;

    // for (int i = 0; i < intervalData.size(); i++)
    size_t count = 0;
    for (auto &intervalTmp : intervalData)
    {
        auto newTempExprs = exprAutoWrapper(tempExpr);
        string bestRewriteExpr;
        double maxError;
        double aveError;
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
                    testError(uniqueLabel, suffixTmp, intervalTmp.at(0), intervalTmp.at(1), intervalTmp.at(2), intervalTmp.at(3), scale, scale);
                    break;

                case 3:
                    tempError = testError(uniqueLabel, suffixTmp, intervalTmp.at(0), intervalTmp.at(1), intervalTmp.at(2), intervalTmp.at(3), intervalTmp.at(4), intervalTmp.at(5), scale, scale, scale);
                    // std::cout << "*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*" << std::endl;
                    // std::cout << suffixTmp << std::endl;
                    // std::cout << "*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*&*" << std::endl;
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
            }
            else
            {
                if (tempError.maxError < maxError)
                {
                    maxError = tempError.maxError;
                    bestRewriteExpr = newExpr;
                    aveError = tempError.aveError;
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
        exprInfoVector.push_back(tempInfo);

        count++;
    }
    return exprInfoVector;
}

// TODO: improve
// generate final code
void geneFinalCode(string exprStr, string uniqueLabel, std::vector<exprInfo> exprInfoVector)
{
    cout << "\n&&&&&&&&&&&&&&&&&&&&&&& geneFinalCode &&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;

    cout << "size of exprInfoVector:" << exprInfoVector.size() << endl;
    cout << "the general information" << endl;
    for (size_t i = 0; i < exprInfoVector.size(); i++)
    {
        cout << "NO." << i << endl;
        cout << "Interval: [" << exprInfoVector.at(i).start << "," << exprInfoVector.at(i).end << "]" << endl;
        cout << "expr: " << exprInfoVector.at(i).exprStr << endl;
    }

    // generate code to file
    string fileName = "expr_" + uniqueLabel + "_final.c";
    std::ofstream fout;
    fout.open(fileName.c_str());
    fout << "double func(double x){" << std::endl;
    for (size_t i = 0; i < exprInfoVector.size(); i++)
    {
        double start = exprInfoVector.at(i).start;
        double end = exprInfoVector.at(i).end;
        string exprStr = exprInfoVector.at(i).exprStr;
        if (i == 0)
        {
            fout << "    if(" << start << "<x<" << end << "){" << std::endl;
            fout << "        result = " << exprStr << ";" << std::endl;
        }
        else
        {
            fout << "    }else if(" << start << "<x<" << end << "){" << std::endl;
            fout << "        result = " << exprStr << ';' << std::endl;
        }
    }
    fout << "    }else{" << std::endl;
    fout << "        result =  " << exprStr << std::endl;
    fout << "    }" << std::endl;
    fout << "    return result" << std::endl;
    fout << "}" << std::endl;

    fout << std::flush;
    fout.close();
    cout << "generate file: " << fileName << endl;

    cout << "&&&&&&&&&&&&&&&&&&&&&&& geneFinalCode &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n"
         << endl;
}