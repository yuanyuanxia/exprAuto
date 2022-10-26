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

//测试误差
void testError(std::string uniqueLabel, double start, double end, int scale)
{
    // string scriptName = "/home/yhr/project/expr-auto/sampleOne.sh";
    std::string scriptName = "./sampleOne.sh";
    char command[200] = {0};
    std::string file = scriptName + " ";
    strcpy(command, file.c_str());

    // parameter1
    std::string parameter1 = "NMSEproblem334";
    parameter1 = parameter1 + " ";
    strcat(command, parameter1.c_str());

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
std::vector<exprInfo> rewrite(std::string exprStr, std::string uniqueLabel)
{
    std::string filename = "./intervalData.txt";

    std::vector<std::vector<double>> intervalData;
    intervalData = getIntervalData(filename);

    ast_ptr tempExpr;
    tempExpr = ParseExpressionFromString(exprStr);

    std::vector<exprInfo> exprInfoVector;

    for (int i = 0; i < intervalData.size(); i++)
    {
        std::vector<ast_ptr> newTempExprs;
        newTempExprs = exprAutoNew(tempExpr);
        std::string bestRewriteExpr;
        double start = intervalData.at(i).at(0);
        double end = intervalData.at(i).at(1);
        int scale = 100;

        //在此确定改写的最佳表达式
        for (int j = 0; j < newTempExprs.size(); j++)
        {

            std::cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
            std::string newExpr = PrintExpression(newTempExprs.at(j));
            std::cout << "No." << j + 1 << " " << newExpr << std::endl;

            //生成.c文件并测试误差
            geneOriginCode(newExpr, uniqueLabel, "temp");
            testError(uniqueLabel, start, end, scale);

            std::cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << std::endl;
            bestRewriteExpr = newExpr;
        }

        exprInfo tempInfo;
        tempInfo.start = start;
        tempInfo.end = end;
        tempInfo.exprStr = bestRewriteExpr;
        tempInfo.error = 0.1;
        tempInfo.performance = 0.2;
        exprInfoVector.push_back(tempInfo);
    }
    return exprInfoVector;
}

//生成.c文件
void geneFinalCode(std::string exprStr, std::string uniqueLabel, std::vector<exprInfo> exprInfoVector)
{

    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
    std::cout << "size of exprInfoVector:" << exprInfoVector.size() << std::endl;
    std::cout << "generating code" << std::endl;
    for (int i = 0; i < exprInfoVector.size(); i++)
    {
        std::cout << "NO." << i + 1 << std::endl;
        std::cout << "Intervale:[" << exprInfoVector.at(i).start << "," << exprInfoVector.at(i).end << "]" << std::endl;
        std::cout << "expr:" << exprInfoVector.at(i).exprStr << std::endl;
    }
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
    std::string fileName = "expr_" + uniqueLabel + "_final.c";
    std::ofstream fout;
    fout.open(fileName.c_str());
    fout << "double func(double x){" << std::endl;
    for (int i = 0; i < exprInfoVector.size(); i++)
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
    std::cout << "generate file:" << fileName;
}