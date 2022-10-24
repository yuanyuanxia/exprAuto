#include "basic.hpp"
#include "exprAuto.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"
#include "simplifyExpr.hpp"
#include "color.hpp"
#include "geneCode.hpp"

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

    // tmp
    std::ofstream fout;
    fout.open("./tmpResult.txt");    
    string inputStr = "";
    fprintf(stderr, GREEN "ready> " RESET);
    while (getline(cin, inputStr))
    {
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
        // geneOriginCode(inputStr);
        // testError();
        // pickTheBest();
        // sample();
        // drawBoundary(); // call Matlab
        // devideInterval();
        // rewrite();
        // testPerformance();
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
            results = exprAutoNew(expr);
        }
        else
        {
            results = exprAutoNew(expr);
            vector<ast_ptr> results1 = exprAutoNew(expr1);
            mineAppend(results, results1);
        }
        for(auto& result : results)
        {
            sortExpr(result);
        }
        deleteTheSame(results);
        printExprs(results, BLUE "main: after exprAutoNew: " RESET, false, DOUBLE_PRECISION);
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