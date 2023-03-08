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

map<string, vector<double>> benchmarkThresholds = {
    {"Bsplines3", {0.8}},
    {"exp1x", {1}},
    {"exp1x_log", {1}},
    {"intro_example", {1}},
    {"logexp", {10}},
    {"NMSEexample31", {2}},
    {"NMSEexample310", {2}},
    {"NMSEexample34", {2.1}},
    {"NMSEexample35", {1}},
    {"NMSEexample36", {2}},
    {"NMSEexample37", {0.5}},
    {"NMSEexample38", {10}},
    {"NMSEexample39", {5}},
    {"NMSEproblem331", {1}},
    {"NMSEproblem333", {1.5}},
    {"NMSEproblem334", {200}},
    {"NMSEproblem336", {1}},
    {"NMSEproblem337", {1}},
    {"NMSEproblem341", {2}},
    {"NMSEproblem343", {1}},
    {"NMSEproblem344", {2}},
    {"NMSEproblem345", {2}},
    {"NMSEsection311", {2}},
    {"predatorPrey", {2}},
    {"sine", {1}},
    {"sineorder3", {3}},
    {"sqroot", {0.6}},
    {"sqrt_add", {1.5}},
    {"test05_nonlin1_r4", {1.2}},
    {"test05_nonlin1_test2", {1.2}},
    {"verhulst", {2}},
};

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
    // int getlineCount = 0;

    fprintf(stderr, GREEN "ready> " RESET);
    string inputStr = "";
    // while (getline(infile, inputStr)) // read line from file's input
    while (getline(cin, inputStr)) // read line from keyboard input
    {
        // only rewrite
        // getlineCount++;
        // if (getlineCount == 35 || getlineCount == 36 || getlineCount < 0)
            // continue;

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
        vector<double> thresholds;
        if (pos != benchMarkData.end())
        {
            name = inputStr;
            inputStr = pos->second.begin()->first;
            cout << "expression := " << inputStr << endl;

            thresholds = benchmarkThresholds.at(name);
            // fmt::print("thresholds := {}\n", thresholds);

            isBenchMark = true;
        }

        const char *split = " ";

        // Get the information about the input expr
        auto originExpr = ParseExpressionFromString(inputStr);
        vector<string> vars;
        getVariablesFromExpr(originExpr, vars);
        int dimension = vars.size();
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
        // ofstream ofs;
        // ofs.open("intervalData.txt", ios::out);
        // TODO-DONE: (but no need do it): judge if intervals.size() == dimension
        // for (int i = 0; i < dimension; i++)
        // {
        //     if (i == dimension - 1)
        //     {
        //         ofs << intervals.at(i);
        //     }
        //     else
        //     {
        //         ofs << intervals.at(i) << " ";
        //     }
        // }
        // ofs.close();

        // Read scale from keyboard
        // fprintf(stderr, GREEN "scale> " RESET);
        // string scaleStr;
        // getline(cin, scaleStr);
        // vector<int> scales = getScales(scaleStr, split);

        // Default scale setting according to the variables' size
        int sampleScale;
        if (dimension == 1)
        {
            sampleScale = 500000;
        }
        else if (dimension == 2)
        {
            sampleScale = 1024;
        }
        else if (dimension == 3)
        {
            sampleScale = 256;
        }
        else
        {
            sampleScale = 10;
        }
        vector<int> scales;
        for (int i = 0; i < dimension; i++)
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

            auto funcNameOrigin = geneExprCodeKernel(inputStr, vars, uniqueLabel, "origin");
            // auto funcNameOrigin = geneExprCode(inputStr, uniqueLabel, "origin");
            // auto funcNameHerbie = geneHerbieCode(inputStr, uniqueLabel, "herbie");
            auto funcNameHerbie = geneHerbieCode(uniqueLabel);
            // auto funcNameDaisy = geneDaisyCode(inputStr, uniqueLabel, "daisy");
            auto funcNameMpfr = geneMpfrCode(inputStr, uniqueLabel, vars);

            // TODO: improve pickTheBest to support more suffix
            // pickTheBest(uniqueLabel, 0, 1, 100);
            vector<string> suffixSet = {"origin", "herbie"};
            auto suffix = pickTheBest(uniqueLabel, suffixSet, intervals, scales);
            if (suffix == "origin")
            {
                inputStr = funcNameOrigin;
            }
            else if (suffix == "herbie")
            {
                inputStr = funcNameHerbie;
            }
            else
            {
                fprintf(stderr, "ERROR: main: we do not support the suffix %s now\n", suffix.c_str());
                exit(EXIT_FAILURE);
            }
            // cout << BLUE << "main: start testError for origin: " << inputStr << RESET << endl;
            // auto timeTmp1 = std::chrono::high_resolution_clock::now();
            // geneSampleData();
            // auto infoTmp = testError(uniqueLabel, "origin", intervals, scales);
            // auto timeTmp2 = std::chrono::high_resolution_clock::now();
            // cout << BLUE << "main: ending testError for origin: " << inputStr << RESET << endl;
            // std::chrono::duration<double> testError_seconds = timeTmp2 - timeTmp1;
            // cout << BLUE << "testError time: " << testError_seconds.count() << "s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;
            
            // auto infoTmp = testError(uniqueLabel, "origin", intervals, scales); // TODO-done: 完善origin误差测试 // WHY do it? Because no pickTheBest before. So, have to use testError to get the sample data.
            auto upEdgeFileName = geneBoundaryData(uniqueLabel, suffix); // sample data == matlab ==> upEdge data // TODO: support multiple dimension
            cout << "upEdgeFileName: " << upEdgeFileName << "\n";
            vector<string> upEdgeFileNames;
            upEdgeFileNames.push_back(upEdgeFileName);
            auto intervalData = getIntervalData(upEdgeFileNames, thresholds);
            fmt::print("after regime, we have {} intervals: {}\n", intervalData.size(), intervalData);
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;

            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite start =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            // auto timeTmp3 = std::chrono::high_resolution_clock::now();
            auto exprInfoVector = rewrite(inputStr, uniqueLabel, intervalData);
            // auto timeTmp4 = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> rewrite_seconds = timeTmp4 - timeTmp3;
            // cout << BLUE << "rewrite time: " << rewrite_seconds.count() << " s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite end   =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

            auto funcNameFinal = geneFinalCodeKernel(inputStr, uniqueLabel, exprInfoVector, vars);

            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= test final code's error and performance start =-=-=-=-=-=-=-=-=-=-=-=-=\n";
            auto infoTmp = testError(uniqueLabel, "final", intervals, scales);
            infoTmp.performance = testPerformance(uniqueLabel, "final", intervals);
            cout << "performance: " << infoTmp.performance << "\n\n";
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= test final code's error and performance end   =-=-=-=-=-=-=-=-=-=-=-=-=\n";

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
            // fout << "-------------------------------------NO." << getlineCount << ": " << inputStr << endl;
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