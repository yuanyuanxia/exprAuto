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
    cout << "command: " << command << endl;
    system(command);
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

    cout << "&&&&&&&&&&&&&&&&&&&&&&& geneFinalCode &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}