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
#include <cstdio>

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
    {"logexp", {1}},
    {"NMSEexample31", {2}},
    {"NMSEexample310", {2}},
    {"NMSEexample34", {0.5}},
    {"NMSEexample35", {1}},
    {"NMSEexample36", {2}},
    {"NMSEexample37", {0.5}},
    {"NMSEexample38", {2}},
    {"NMSEexample39", {5}},
    {"NMSEproblem331", {1}},
    {"NMSEproblem333", {1.5}},
    {"NMSEproblem334", {2}},
    {"NMSEproblem336", {1}},
    {"NMSEproblem337", {1}},
    {"NMSEproblem341", {2}},
    {"NMSEproblem343", {1}},
    {"NMSEproblem344", {1}},
    {"NMSEproblem345", {2}},
    {"NMSEsection311", {1}},
    {"predatorPrey", {2}},
    {"sine", {1}},
    {"sineorder3", {3}},
    {"sqroot", {0.6}},
    {"sqrt_add", {1.5}},
    {"test05_nonlin1_r4", {1.2}},
    {"test05_nonlin1_test2", {1.2}},
    {"verhulst", {1}},
    {"ComplexSinCos", {2.861064e+02, 6.019811}},
    {"ComplexSquareRoot", {1, 1}},
    {"doppler1", {2.588287, 2, 2.586046}},
    {"doppler2", {2, 2, 3.107020}},
    {"doppler3", {2.509605, 2, 2.508455}},
    {"hypot32", {2, 2}},
    {"i4", {2, 2}},
    {"i6", {7.869156e+06, 8.400076e+06}},
    {"NMSEexample33", {1.678153e+06, 1.540327e+06}},
    {"NMSEproblem332", {4.957687e+02, 1.740876e+03}},
    {"NMSEproblem335", {1, 1}},
    {"NMSEproblem346", {4.068065e+00, 4.041267e+00}},
    {"NMSEsection35", {0.75, 0.75}},
    {"polarToCarthesianX", {8.225613e+12, 2}},
    {"polarToCarthesianY", {2.161373e+12, 1.341743e+12}},
    {"sec4example", {1.25, 1.25}},
    {"test03_nonlin2", {1.375, 1.25}},
    {"theta", {2, 2}},
    {"turbine1", {3.002278, 2.831004, 2.847586}},
    {"squareRoot3_1", {2}},
    {"squareRoot3_2", {2}},
    {"squareRoot3Invalid_1", {2}},
    {"squareRoot3Invalid_2", {2}},
    {"cav10_1", {2}},
    {"cav10_2", {2}},
    {"gustafsonExample_1", {2}},
    {"gustafsonExample_2", {2}},
    {"smartRoot_1", {2}},
    {"smartRoot_2", {2}},
    {"triangleSorted_1", {2, 2, 2}},
    {"triangleSorted_2", {2, 2, 2}},
};

// write to file
void write_to_file_wrapper(string uniqueLabel, string exprOriginBest, int dimension, int numIntervalsBefore, double numOfIntervals, int numOfExprs, vector<double> thresholds, const exprInfo &originExprInfo, const exprInfo &herbieExprInfo, const exprInfo &finalInfo, double originPerformance, double elapsed_seconds, double init_seconds, double matlab_seconds, double regime_seconds, double rewrite_seconds, double final_seconds, double matlabKernelTime)
{
    vector<double> summaryData;
    summaryData.push_back(dimension);
    summaryData.push_back(numIntervalsBefore);
    summaryData.push_back(numOfIntervals);
    summaryData.push_back(double(numOfExprs));
    if (thresholds.size() == 1)
    {
        summaryData.push_back(thresholds.at(0));
        summaryData.push_back(-1);
        summaryData.push_back(-1);
    }
    else if (thresholds.size() == 2)
    {
        summaryData.push_back(thresholds.at(0));
        summaryData.push_back(thresholds.at(1));
        summaryData.push_back(-1);
    }
    else if (thresholds.size() == 3)
    {
        summaryData.push_back(thresholds.at(0));
        summaryData.push_back(thresholds.at(1));
        summaryData.push_back(thresholds.at(2));
    }
    else
    {
        fprintf(stderr, "ERROR: we can not support %ld demision now.\n", thresholds.size());
        exit(EXIT_FAILURE);
    }
    summaryData.push_back(originExprInfo.aveError);
    summaryData.push_back(originExprInfo.maxError);
    summaryData.push_back(herbieExprInfo.aveError);
    summaryData.push_back(herbieExprInfo.maxError);
    summaryData.push_back(finalInfo.aveError);
    summaryData.push_back(finalInfo.maxError);
    summaryData.push_back(originPerformance);
    summaryData.push_back(finalInfo.performance);
    summaryData.push_back(elapsed_seconds);
    summaryData.push_back(init_seconds);
    summaryData.push_back(matlab_seconds);
    summaryData.push_back(regime_seconds);
    summaryData.push_back(rewrite_seconds);
    summaryData.push_back(final_seconds);
    summaryData.push_back(matlabKernelTime);
    write_to_file(uniqueLabel, exprOriginBest, summaryData, "runlog.csv");
}

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
        string uniqueLabel = "";
        vector<double> thresholds;
        if (pos != benchMarkData.end())
        {
            uniqueLabel = inputStr;
            inputStr = pos->second.begin()->first;
            cout << "expression := " << inputStr << endl;

            thresholds = benchmarkThresholds.at(uniqueLabel);
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
        
        // ready for writing to file
        string exprOriginBest = "";
        double originPerformance = -1;
        double herbiePerformance = -1;
        int numIntervalsBefore = -1;
        double numOfIntervals = -1;
        int numOfExprs = -1;
        exprInfo finalInfo; // ave err, max err, performance
        std::chrono::duration<double> init_seconds{0}, matlab_seconds{0}, regime_seconds{0}, rewrite_seconds{0}, final_seconds{0}, elapsed_seconds{0};
        double matlabKernelTime = -1;
        vector<exprInfo> initExprInfos;
        exprInfo tempExprInfo;
        tempExprInfo.suffix = "origin";
        initExprInfos.push_back(tempExprInfo);
        tempExprInfo.suffix = "herbie";
        initExprInfos.push_back(tempExprInfo);
        tempExprInfo.suffix = "daisy";
        initExprInfos.push_back(tempExprInfo);
        auto &originExprInfo = initExprInfos.at(0);
        auto &herbieExprInfo = initExprInfos.at(1);
        auto &daisyExprInfo = initExprInfos.at(2);
        if (runAllFlag)
        { // the whole process
            if (!isBenchMark)
            {
                uniqueLabel = getUniqueLabel();
            }

            cout << BLUE << "uniqueLabel: " << uniqueLabel << RESET << endl;
            string mkdirCommand = "mkdir -p srcTest/" + uniqueLabel + " outputs/" + uniqueLabel;
            system(mkdirCommand.c_str());

            auto exprOrigin = geneExprCodeKernel(inputStr, vars, uniqueLabel, "origin");
            // auto funcNameOrigin = geneExprCode(inputStr, uniqueLabel, "origin");
            // auto funcNameHerbie = geneHerbieCode(inputStr, uniqueLabel, "herbie");
            auto exprHerbie = geneHerbieCode(uniqueLabel);
            auto exprDaisy = geneDaisyCode(uniqueLabel);
            auto funcNameMpfr = geneMpfrCode(inputStr, uniqueLabel, vars);

            if(exprOrigin != "")
            {
                originPerformance = testPerformance(uniqueLabel, "origin", intervals);
                cout << "origin performance: " << originPerformance << "\n\n";
            }
            else
            {
                fprintf(stderr, "exprOrigin is null!\n");
                exit(EXIT_FAILURE);
            }
            if(exprHerbie != "")
            {
                herbiePerformance = testPerformance(uniqueLabel, "herbie", intervals);
                cout << "origin performance: " << originPerformance << "\n\n";
            }
            else
            {
                fprintf(stderr, "exprHerbie is null!\n");
            }

            vector<string> suffixSet = {"origin"};
            if (exprHerbie != "")
            {
                suffixSet.push_back("herbie");
            }
            else
            {
                fprintf(stderr, "For exprOrigin, Herbie's rewrite results can not be used.\n");
            }
            if (exprDaisy != "")
            {
                suffixSet.push_back("daisy");
            }
            else
            {
                fprintf(stderr, "For exprOrigin, Daisy's rewrite results can not be used.\n");
            }
            
            auto &initExprInfo = pickTheBest(uniqueLabel, suffixSet, initExprInfos, intervals, scales);
            exprOriginBest = initExprInfo.suffix;

            if (exprOriginBest == "origin")
            {
                inputStr = exprOrigin;
            }
            else if (exprOriginBest == "herbie")
            {
                inputStr = exprHerbie;
            }
            else if (exprOriginBest == "daisy")
            {
                inputStr = exprDaisy;
            }
            else
            {
                fprintf(stderr, "ERROR: main: we do not support the suffix %s now\n", exprOriginBest.c_str());
                exit(EXIT_FAILURE);
            }
            cout << "the pick expr is " << inputStr << "\n";
            auto timeTmp1 = std::chrono::high_resolution_clock::now(); // init over
            init_seconds = timeTmp1 - timeStart;
            cout << BLUE << "init time: " << init_seconds.count() << " s" << RESET << endl;

            fprintf(stderr, GREEN "ready> " RESET);
            continue;

            auto &initExprMaxError = initExprInfo.maxError;
            if (initExprMaxError <= 0.5)
            {
                fprintf(stderr, "the error of %s is no bigger than 0.5, so do not need precision improvement.\n", inputStr.c_str());
                // TODO: write
                auto timeEnd = std::chrono::high_resolution_clock::now();
                elapsed_seconds = timeEnd - timeStart;
                cout << BLUE << "the whole time: " << elapsed_seconds.count() << " s" << RESET << endl;

                write_to_file_wrapper(uniqueLabel, exprOriginBest, dimension, numIntervalsBefore, numOfIntervals, numOfExprs, thresholds, originExprInfo, herbieExprInfo, finalInfo, originPerformance, elapsed_seconds.count(), init_seconds.count(), matlab_seconds.count(), regime_seconds.count(), rewrite_seconds.count(), final_seconds.count(), matlabKernelTime);
                fprintf(stderr, GREEN "ready> " RESET);
                continue;
            }

            string upEdgeFileName;
            vector<string> upEdgeFileNames;
            if(dimension == 1)
            {
                // No sampling is required because for a single parameter, the error test data is equivalent to the sampled data 
                // Just generate boundary data by matlab
                upEdgeFileName = geneBoundaryData(uniqueLabel, exprOriginBest, matlabKernelTime); // sample data == matlab ==> upEdge data
                upEdgeFileNames.push_back(upEdgeFileName);
            }
            else
            {
                // 1. sample for different dimension
                // 2. generate boundary data by matlab
                string suffixTmp;
                vector<string> suffixTmps;
                if (dimension == 2)
                {
                    suffixTmp = exprOriginBest + "_X";
                    suffixTmps.push_back(suffixTmp);
                    suffixTmp = exprOriginBest + "_Y";
                    suffixTmps.push_back(suffixTmp);
                    vector<int> scales{8192, 2048};
                    sampleError(uniqueLabel, exprOriginBest, intervals, scales);
                }
                else if (dimension == 3)
                {
                    suffixTmp = exprOriginBest + "_X";
                    suffixTmps.push_back(suffixTmp);
                    suffixTmp = exprOriginBest + "_Y";
                    suffixTmps.push_back(suffixTmp);
                    suffixTmp = exprOriginBest + "_Z";
                    suffixTmps.push_back(suffixTmp);
                    vector<int> scales{512, 128}; // actually are drawNum and findMaxNum, so only need 2 numbers
                    sampleError(uniqueLabel, exprOriginBest, intervals, scales);
                }
                else
                {
                    fprintf(stderr, "ERROR: main: we can not handle %d parameters (bigger than 3) now\n", dimension);
                    exit(EXIT_FAILURE);
                }
                upEdgeFileNames = geneBoundaryData(uniqueLabel, exprOriginBest, suffixTmps, matlabKernelTime); // sample data == matlab ==> upEdge data
            }
            fmt::print("upEdgeFileNames: {}\n", upEdgeFileNames);
            auto timeTmp2 = std::chrono::high_resolution_clock::now(); // matlab over
            matlab_seconds = timeTmp2 - timeTmp1;
            cout << BLUE << "regime time (matlab part): " << matlab_seconds.count() << " s" << RESET << endl;

            auto intervalData = getIntervalData(upEdgeFileNames, thresholds, intervals, numIntervalsBefore);
            // fmt::print("[INFO] main: thresholds {}\n", thresholds);
            numOfIntervals = intervalData.size();
            fmt::print("after regime, we have {} intervals: {}\n", numOfIntervals, intervalData);
            auto timeTmp3 = std::chrono::high_resolution_clock::now();
            regime_seconds = timeTmp3 - timeTmp2;
            cout << BLUE << "regime time (other part): " << regime_seconds.count() << " s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;

            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite start =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            auto exprInfoVector = rewrite(inputStr, uniqueLabel, intervalData, numOfExprs, intervals);
            auto timeTmp4 = std::chrono::high_resolution_clock::now(); // rewrite over
            rewrite_seconds = timeTmp4 - timeTmp3;
            cout << BLUE << "rewrite time: " << rewrite_seconds.count() << " s" << RESET << endl;
            // fprintf(stderr, GREEN "ready> " RESET);
            // continue;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite end   =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

            auto funcNameFinal = geneFinalCodeKernel(inputStr, uniqueLabel, exprInfoVector, vars);

            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= test final code's error and performance start =-=-=-=-=-=-=-=-=-=-=-=-=\n";
            vector<int> startNowIdxs(dimension, 0);
            vector<double> startOriginIntervals;
            vector<double> steps;
            for (int i = 0; i < dimension; i++)
            {
                auto &startOriginInterval = intervals.at(i * 2);
                auto &endOriginInterval = intervals.at(i * 2 + 1);
                startOriginIntervals.push_back(startOriginInterval);
                double width = endOriginInterval - startOriginInterval;
                double step = width / (double)scales.at(i);
                steps.push_back(step);
            }
            finalInfo = testError(uniqueLabel, "final", intervals, scales, startNowIdxs, startOriginIntervals, steps);
            finalInfo.performance = testPerformance(uniqueLabel, "final", intervals);
            cout << "performance: " << finalInfo.performance << "\n\n";
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-= test final code's error and performance end   =-=-=-=-=-=-=-=-=-=-=-=-=\n";
            auto timeTmp5 = std::chrono::high_resolution_clock::now();
            final_seconds = timeTmp5 - timeTmp4;
            cout << BLUE << "final time: " << final_seconds.count() << " s" << RESET << endl;
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
        elapsed_seconds = timeEnd - timeStart;
        cout << BLUE << "the whole time: " << elapsed_seconds.count() << " s" << RESET << endl;

        write_to_file_wrapper(uniqueLabel, exprOriginBest, dimension, numIntervalsBefore, numOfIntervals, numOfExprs, thresholds, originExprInfo, herbieExprInfo, finalInfo, originPerformance, elapsed_seconds.count(), init_seconds.count(), matlab_seconds.count(), regime_seconds.count(), rewrite_seconds.count(), final_seconds.count(), matlabKernelTime);
        fprintf(stderr, GREEN "ready> " RESET);
    }

    // only rewrite
    infile.close();
    fout.close();

    endPython();
    return 0;
}