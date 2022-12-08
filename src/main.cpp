#include "basic.hpp"
#include "exprAuto.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"
#include "simplifyExpr.hpp"
#include "color.hpp"
#include "geneCode.hpp"
#include "tools.hpp"
#include "benchMark.hpp"

#include <fstream>
#include <chrono>
#include <iomanip>
#include <fmt/ranges.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
#define MAIN_PRECISION 6
#ifndef DOUBLE_PRECISION
#define DOUBLE_PRECISION 17
#endif
//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
    installOperatorsForStr();
    initPython();

    // tmp files for input and output.
    ifstream infile;
    infile.open("tmpInput.txt", ios::in);
    if (!infile.is_open())
    {
        cout << "open tmpInput.txt failed";
        exit(EXIT_FAILURE);
    }
    std::ofstream fout;
    fout.open("./tmpResult.txt");
    if (!fout.is_open())
    {
        cout << "open tmpResult.txt failed";
        exit(EXIT_FAILURE);
    }
    int getlineCount = 0;

    fprintf(stderr, GREEN "ready> " RESET);
    string inputStr = "";
    // while (getline(infile, inputStr)) // read line from file's input
    while (getline(cin, inputStr)) // read line from keyboard input
    {
        // only rewrite
        getlineCount++;
        if (getlineCount == 35 || getlineCount == 36 || getlineCount < 0)
            continue;

        if (inputStr == "exit;" || inputStr == "quit;" || inputStr == "exit" || inputStr == "quit")
        {
            fprintf(stderr, "main: the application is end after inputing '%s'\n", inputStr.c_str());
            break;
        }
        if (inputStr.empty())
        {
            fprintf(stderr, "WARNIGN: main: please input a exprssion before pressing Enter\n");
            fprintf(stderr, GREEN "ready> " RESET);
            continue;
        }
        else if (inputStr.back() == ';')
        {
            fprintf(stderr, "you do not need to add a ';' after the expression\n");
            inputStr.pop_back(); // remove the last char ';'
        }

        auto benchMarkData = initalBenchMark();
        auto pos = benchMarkData.find(inputStr);
        bool isBenchMark = false;
        string name;
        if (pos != benchMarkData.end())
        {
            name = inputStr;
            inputStr = pos->second.begin()->first;
            cout << inputStr << endl;
            isBenchMark = true;
        }

        const char *split = " ";

        // Get the information about the input expr
        auto originExpr = ParseExpressionFromString(inputStr);
        vector<string> vars;
        getVariablesFromExpr(originExpr, vars);
        // cout << vars.size() << endl;

        fprintf(stderr, GREEN "interval> " RESET);

        // after input completed, start timing
        auto timeStart = std::chrono::high_resolution_clock::now();
        vector<double> intervals;
        if (isBenchMark)
        {
            intervals = pos->second.begin()->second;
            string intervalsStr = fmt::format("{}\n", intervals);
            fprintf(stderr, "%s", intervalsStr.c_str());
        }
        else
        {
            string intervalStr;
            getline(cin, intervalStr);
            intervals = getIntervals(intervalStr, split);
        }

        // For temporary use only . It will be replaced by geneBoundaryData and geneIntervalData
        ofstream ofs;
        ofs.open("intervalData.txt", ios::out);
        for (size_t i = 0; i < intervals.size(); i++)
        {
            if (i == intervals.size() - 1)
            {
                ofs << intervals.at(i);
            }
            else
            {
                ofs << intervals.at(i) << " ";
            }
        }
        ofs.close();

        // Read scale from keyboard
        // fprintf(stderr, GREEN "scale> " RESET);
        // string scaleStr;
        // getline(cin, scaleStr);
        // vector<int> scales = getScales(scaleStr, split);

        // Default scale setting according to the variables' size
        int sampleScale;
        if (vars.size() == 1)
        {
            sampleScale = 500000;
        }
        else if (vars.size() == 2)
        {
            sampleScale = 1024;
        }
        else if (vars.size() == 3)
        {
            sampleScale = 256;
        }
        else
        {
            sampleScale = 10;
        }
        vector<int> scales;
        for (size_t i = 0; i < vars.size(); i++)
        {
            scales.push_back(sampleScale);
        }

        bool runAllFlag = true;
        if (runAllFlag)
        { // the whole process
            auto uniqueLabel = getUniqueLabel();
            if (isBenchMark)
            {
                uniqueLabel = name;
            }
            cout << BLUE << "uniqueLabel: " << uniqueLabel << RESET << endl;
            string mkdirCommand = "mkdir -p srcTest/" + uniqueLabel + " outputs/" + uniqueLabel;
            system(mkdirCommand.c_str());

            auto funcNameOrigin = geneOriginCodeKernel(inputStr, vars, uniqueLabel, "origin");
            // auto funcNameOrigin = geneOriginCode(inputStr, uniqueLabel, "origin");
            // auto funcNameHerbie = geneHerbieCode(inputStr, uniqueLabel, "herbie");
            // auto funcNameDaisy = geneDaisyCode(inputStr, uniqueLabel, "daisy");
            auto funcNameMpfr = geneMpfrCode(inputStr, uniqueLabel, vars);

            // TODO: pick the best from origin, herbie, daisy
            // pickTheBest(uniqueLabel, 0, 1, 100);
            
            cout << BLUE << "main: start  testError for origin: " << inputStr << RESET << endl;
            // auto timeTmp1 = std::chrono::high_resolution_clock::now();
            auto infoTmp = testError(uniqueLabel, "origin", intervals, scales);
            // auto timeTmp2 = std::chrono::high_resolution_clock::now();
            cout << BLUE << "main: ending testError for origin: " << inputStr << RESET << endl;
            // std::chrono::duration<double> testError_seconds = timeTmp2 - timeTmp1;
            // cout << BLUE << "testError time: " << testError_seconds.count() << "s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;

            geneBoundaryData(uniqueLabel, "origin"); // matlab

            vector<string> intervalsTemp;
            vector<double> threholdsTemp;
            geneIntervalData(uniqueLabel, intervalsTemp, threholdsTemp);

            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite start =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            // auto timeTmp3 = std::chrono::high_resolution_clock::now();
            auto exprInfoVector = rewrite(inputStr, uniqueLabel);
            // auto timeTmp4 = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> rewrite_seconds = timeTmp4 - timeTmp3;
            // cout << BLUE << "rewrite time: " << rewrite_seconds.count() << " s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite end   =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            auto funcNameFinal = geneFinalCodeKernel(inputStr, uniqueLabel, exprInfoVector, vars);


            // write the results to file
            // auto results = exprAutoWrapper(inputStr, intervals, scales);
            // for (size_t i = 0; i < results.size(); i++)
            // {
            //     auto &result = results.at(i);
            //     // string treeStr;
            //     // printAST(result, treeStr, MAIN_PRECISION);
            //     fout << PrintExpression(result, MAIN_PRECISION) << endl;
            //     // fout << "*resultPtr = " << PrintExpression(result, DOUBLE_PRECISION) << ";" << endl;
            //     // fout << treeStr << endl;
            // }
            // fout << endl;
        } // the whole process end
        else
        { // only rewrite
            // auto timeTmp1 = std::chrono::high_resolution_clock::now();
            auto results = exprAutoWrapper(inputStr, intervals, scales);
            // auto timeTmp2 = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> exprAutoWrapper_seconds = timeTmp2 - timeTmp1;
            // cout << BLUE << "exprAutoWrapper time: " << exprAutoWrapper_seconds.count() << " s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;

            // write the results to file
            fout << "-------------------------------------NO." << getlineCount << ": " << inputStr << endl;
            for (size_t i = 0; i < results.size(); i++)
            {
                auto &result = results.at(i);
                // string treeStr;
                // printAST(result, treeStr, MAIN_PRECISION);
                fout << PrintExpression(result, MAIN_PRECISION) << endl;
                // fout << "*resultPtr = " << PrintExpression(result, DOUBLE_PRECISION) << ";" << endl;
                // fout << treeStr << endl;
            }
            fout << endl;
        } // only rewrite end

        auto timeEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = timeEnd - timeStart;
        cout << BLUE << "elapsed time: " << elapsed_seconds.count() << "s" << RESET << endl;
        fprintf(stderr, GREEN "ready> " RESET);
    }

    // only rewrite
    infile.close();
    fout.close();

    endPython();
    return 0;
}