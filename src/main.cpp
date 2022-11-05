#include "basic.hpp"
#include "exprAuto.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"
#include "simplifyExpr.hpp"
#include "color.hpp"
#include "geneCode.hpp"
#include "tools.hpp"


#include <fstream>
#include <chrono>
#include <iomanip>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
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
        if(getlineCount == 35 || getlineCount == 36 || getlineCount < 0) continue;

        auto start = std::chrono::high_resolution_clock::now();

        if(inputStr == "exit;" || inputStr == "quit;" || inputStr == "exit" || inputStr == "quit") {
            fprintf(stderr, "main: the application is end after inputing '%s'\n", inputStr.c_str());
            break;
        }
        if(inputStr.empty()) {
            fprintf(stderr, "WARNIGN: main: please input a exprssion before pressing Enter\n");
            fprintf(stderr, GREEN "ready> " RESET);
            continue;
        } else if(inputStr.back() == ';') {
            fprintf(stderr, "you do not need to add a ';' after the expression\n");
            inputStr.pop_back(); // remove the last char ';'
        }
        bool runAllFlag = true;
        std::chrono::_V2::system_clock::time_point tmp1, tmp2, tmp3;
        if(runAllFlag)
        { // the whole process
        auto uniqueLabel = getUniqueLabel();
        cout << "uniqueLabel: " << uniqueLabel << endl;

        // get the information about the input expr
        auto originExpr = ParseExpressionFromString(inputStr);
        vector<string> vars;
        getVariablesFromExpr(originExpr, vars);

        auto funcNameOrigin = geneOriginCodeKernel(inputStr, vars, uniqueLabel, "origin");
        // auto funcNameOrigin = geneOriginCode(inputStr, uniqueLabel, "origin");
        // auto funcNameHerbie = geneHerbieCode(inputStr, uniqueLabel, "herbie");
        // auto funcNameDaisy = geneDaisyCode(inputStr, uniqueLabel, "daisy");
        auto funcNameMpfr = geneMpfrCode(inputStr, uniqueLabel, vars);

        // TODO: pick the best from origin, herbie, daisy
        // pickTheBest(uniqueLabel, 0, 1, 100);
        int scale = 256;
        vector<double> intervals{3.8, 7.8, -4.5, -0.3, 0.4, 0.9};
        vector<int> scales{scale, scale, scale};
        tmp1 = std::chrono::high_resolution_clock::now();
        testError(uniqueLabel, "origin", intervals, scales);
        tmp2 = std::chrono::high_resolution_clock::now();
        // geneBoundaryData(inputStr, uniqueLabel); // matlab

        // geneIntervalData(inputStr, uniqueLabel);

        cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite start =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        auto exprInfoVector = rewrite(inputStr, uniqueLabel);
        tmp3 = std::chrono::high_resolution_clock::now();
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-= rewrite end   =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        auto funcNameFinal = geneFinalCodeKernel(inputStr, uniqueLabel, exprInfoVector, vars);
        } // the whole process end
        else
        { // only rewrite
        auto results = exprAutoWrapper(inputStr);

        // write the results to file
        fout << "-------------------------------------NO." << getlineCount <<": " << inputStr << endl;
        for(size_t i = 0; i < results.size(); i++)
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

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> testError_seconds = tmp2 - tmp1;
        std::chrono::duration<double> rewrite_seconds = tmp3 - tmp2;
        std::chrono::duration<double> elapsed_seconds = end - start;
        cout << BLUE << "testError time: " << testError_seconds.count() << "s" << RESET << endl;
        cout << BLUE << "rewrite time: " << rewrite_seconds.count() << "s" << RESET << endl;
        cout << BLUE << "elapsed time: " << elapsed_seconds.count() << "s" << RESET << endl;
        fprintf(stderr, GREEN "ready> " RESET);
    }

    // only rewrite
    infile.close();
    fout.close();

    endPython();
    return 0;
}