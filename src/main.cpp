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
    string inputStr = "";
    int getlineCount = 0;
    fprintf(stderr, GREEN "ready> " RESET);
    // while (getline(infile, inputStr)) // read line from file's input
    while (getline(cin, inputStr)) // read line from keyboard input
    {
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

        auto uniqueLabel = getUniqueLabel();
        cout << "uniqueLabel:" << uniqueLabel << endl;

        geneOriginCode(inputStr, uniqueLabel,"origin");

        testError(uniqueLabel, 0, 1, 100);
        geneBoundaryData(inputStr, uniqueLabel);
        geneIntervalData(inputStr, uniqueLabel);
        vector<exprInfo> exprInfoVector;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        exprInfoVector = rewrite(inputStr, uniqueLabel);
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        geneFinalCode(inputStr, uniqueLabel, exprInfoVector);



        ast_ptr expr = ParseExpressionFromString(inputStr);
        combineConstant(expr);
        sortExpr(expr);
        printExpr(expr, "main: after parse, expr = ", DOUBLE_PRECISION);
        ast_ptr expr1 = simplifyExpr(expr); // Python SymPy simplify
        combineConstant(expr1);
        sortExpr(expr1);
        printExpr(expr1, "main: after SymPy's simplify, expr = ", DOUBLE_PRECISION);
        vector<ast_ptr> results;
        if(isEqual(expr, expr1))
        {
            cout << YELLOW << "-------------------------------------origin rewrite-------------------------------------" << RESET << endl;
            results = exprAutoNew(expr);
        }
        else
        {
            cout << YELLOW << "-------------------------------------origin rewrite-------------------------------------" << RESET << endl;
            results = exprAutoNew(expr);
            cout << YELLOW << "-------------------------------------sympy rewrite-------------------------------------" << RESET << endl;
            vector<ast_ptr> results1 = exprAutoNew(expr1);
            mineAppend(results, results1);
        }
        for(auto& result : results)
        {
            sortExpr(result);
        }
        deleteTheSame(results);
        cout << YELLOW << "-------------------------------------final results-------------------------------------" << RESET << endl;
        printExprs(results, BLUE "main: after exprAutoNew: " RESET, false, DOUBLE_PRECISION);
        fout << "-------------------------------------NO." << getlineCount <<": " << inputStr << endl;
        for(size_t i = 0; i < results.size(); i++)
        {
            auto &result = results.at(i);
            // string treeStr;
            // printAST(result, treeStr, MAIN_PRECISION);
            fout << "No." << i << ": " << PrintExpression(result, MAIN_PRECISION) << endl;
            // fout << "*resultPtr = " << PrintExpression(result, DOUBLE_PRECISION) << ";" << endl;
            // fout << treeStr << endl;
        }
        fout << endl;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        cout << BLUE << "elapsed time: " << elapsed_seconds.count() << "s" << RESET << endl;
        fprintf(stderr, GREEN "ready> " RESET);
    }

    // tmp end
    fout.close();

    endPython();
    return 0;
}