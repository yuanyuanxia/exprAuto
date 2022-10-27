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

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::to_string;

//获取唯一标识符(时间戳)
std::string getUniqueLabel()
{
    time_t currentTime = time(NULL);
    char chCurrentTime[256];
    strftime(chCurrentTime, sizeof(chCurrentTime), "%Y%m%d%H%M%S", localtime(&currentTime));
    std::string uniqueLabel = chCurrentTime;
    return uniqueLabel;
}

// test error
// TODO: set the return value as a exprInfo object
void testError(string uniqueLabel, string suffix, double start, double end, int scale)
{
    std::string scriptName = "./detectErrorOne.sh";
    char command[200] = {0};
    std::string file = scriptName + " ";
    strcpy(command, file.c_str());

    // parameter1
    std::string parameter1 = uniqueLabel;
    parameter1 = parameter1 + " ";
    strcat(command, parameter1.c_str());

    // parameter1.1
    std::string parameter11 = suffix + " ";
    strcat(command, parameter11.c_str());

    // parameter2
    std::string parameter2 = std::to_string(start);
    parameter2 = parameter2 + " ";
    strcat(command, parameter2.c_str());

    // parameter3
    std::string parameter3 = std::to_string(end);
    parameter3 = parameter3 + " ";
    strcat(command, parameter3.c_str());

    std::string parameter4 = std::to_string(scale);
    parameter4 = parameter4 + " ";
    strcat(command, parameter4.c_str());

    std::cout << "command:" << command << std::endl;
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
    std::cout << "command: " << command << std::endl;
    system(command);
}

void testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size)
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
    std::cout << "command: " << command << std::endl;
    system(command);
}

//生成边界数据
void geneBoundaryData(std::string exprStr, std::string uniqueLabel)
{
    std::string filename = "expr_" + uniqueLabel + "_boudary.txt";
    std::ofstream ofs;
    ofs.open(filename, std::ios::out);
    ofs << "123" << std::endl;
    ofs.close();
    std::cout << "generate boundaryData file:" << filename << std::endl;
}

//生成区间数据
void geneIntervalData(std::string exprStr, std::string uniqueLabel)
{
    std::string filename = "expr_" + uniqueLabel + "_interval.txt";
    std::ofstream ofs;
    ofs.open(filename, std::ios::out);
    ofs << "123" << std::endl;
    ofs.close();
    std::cout << "generate boundaryData file:" << filename << std::endl;
}

//生成区间数据
std::vector<std::vector<double>> getIntervalData(std::string filename)
{
    std::vector<std::vector<double>> intervalData;
    std::ifstream ifs;
    filename = "./intervalData.txt";
    ifs.open(filename, std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "open intervalData.txt fail" << std::endl;
        return intervalData;
    }

    std::string buf;
    //[1,2] 对每行数据进行处理
    while (getline(ifs, buf))
    {
        std::cout << buf << std::endl;
        std::vector<double> interval;
        int pos = buf.find(',');
        double start, end;
        int len = buf.length();
        start = atof(buf.substr(1, pos - 1).c_str());
        end = atof(buf.substr(pos + 1, len - pos - 2).c_str());
        std::cout << "start:" << start << std::endl;
        std::cout << "end:" << end << std::endl;
        interval.push_back(start);
        interval.push_back(end);
        intervalData.push_back(interval);
    }
    ifs.close();
    return intervalData;
}

//调用exprAuto进行表达式重写
vector<exprInfo> rewrite(string exprStr, string uniqueLabel)
{
    string filename = "./intervalData.txt"; // TODO: get the filename from uniqueLabel

    vector<vector<double>> intervalData;
    // auto intervalData = getIntervalData(filename);
    vector<double> intervalTmp{-4.5, -0.3, 0.4, 0.9, 3.8, 7.8};
    intervalData.push_back(intervalTmp);
    // X0: [-3.07246,  -1.65432] [-1.35614,  -0.854668]
    // X1: [0.4,  0.9]
    // X2: [4.28352,  4.33717] [4.69062,  4.83126] [4.92649,  7.79219]
    // vector<double> intervalTmp1{-3.07246, -1.65432, 0.4, 0.9, 4.28352, 4.33717};
    // intervalData.push_back(intervalTmp1);
    // vector<double> intervalTmp2{-3.07246, -1.65432, 0.4, 0.9, 4.69062, 4.83126};
    // intervalData.push_back(intervalTmp2);
    // vector<double> intervalTmp3{-3.07246, -1.65432, 0.4, 0.9, 4.92649, 7.79219};
    // intervalData.push_back(intervalTmp3);
    // vector<double> intervalTmp4{-1.35614, -0.854668, 0.4, 0.9, 4.28352, 4.33717};
    // intervalData.push_back(intervalTmp4);
    // vector<double> intervalTmp5{-1.35614, -0.854668, 0.4, 0.9, 4.69062, 4.83126};
    // intervalData.push_back(intervalTmp5);
    // vector<double> intervalTmp6{-1.35614, -0.854668, 0.4, 0.9, 4.92649, 7.79219};
    // intervalData.push_back(intervalTmp6);

    auto tempExpr = ParseExpressionFromString(exprStr);

    std::vector<exprInfo> exprInfoVector;

    // for (int i = 0; i < intervalData.size(); i++)
    for (auto &intervalTmp : intervalData)
    {
        auto newTempExprs = exprAutoWrapper(tempExpr);
        string bestRewriteExpr;
        int scale = 100;

        //在此确定改写的最佳表达式
        for (size_t j = 0; j < newTempExprs.size(); j++)
        {
            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << endl;
            string newExpr = PrintExpression(newTempExprs.at(j));
            cout << "No." << j << ": " << newExpr << endl;

            // generate function code and test error
            geneOriginCode(newExpr, uniqueLabel, "temp");
            switch(intervalTmp.size())
            {
                case 2:
                    testError(uniqueLabel, "origin", intervalTmp.at(0), intervalTmp.at(1), scale);
                    break;

                case 4:
                    testError(uniqueLabel, "origin", intervalTmp.at(0), intervalTmp.at(1), intervalTmp.at(2), intervalTmp.at(3), scale, scale);
                    break;

                case 6:
                    testError(uniqueLabel, "origin", intervalTmp.at(0), intervalTmp.at(1), intervalTmp.at(2), intervalTmp.at(3), intervalTmp.at(4), intervalTmp.at(5), scale, scale, scale);
                    break;

                default:
                    fprintf(stderr, "WRONG: rewrite: the intervalTmp size is %ld\n", intervalTmp.size());
                    exit(EXIT_FAILURE);
                    break;
            }

            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n" << endl;
            bestRewriteExpr = newExpr;
        }

        exprInfo tempInfo;
        tempInfo.start = 0;
        tempInfo.end = 1;
        tempInfo.intervals = intervalTmp;
        tempInfo.exprStr = bestRewriteExpr;
        tempInfo.error = 0.1;
        tempInfo.aveError = 0.2;
        tempInfo.maxError = 0.6;
        tempInfo.performance = 0.2;
        exprInfoVector.push_back(tempInfo);
    }
    return exprInfoVector;
}

//生成.c文件
void geneFinalCode(std::string exprStr, std::string uniqueLabel, std::vector<exprInfo> exprInfoVector)
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
    std::string fileName = "expr_" + uniqueLabel + "_final.c";
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

    cout << "&&&&&&&&&&&&&&&&&&&&&&& geneFinalCode &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}