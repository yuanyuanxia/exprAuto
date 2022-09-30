#include "basic.hpp"
#include "exprAuto.hpp"
#include "laxerASTLY.hpp"
#include "simplifyExpr.hpp"
#include "color.hpp"
#include "geneCode.hpp"

#include <chrono>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
    installOperatorsForStr();
    initPython();

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
        ast_ptr expr = simplifyExpr(inputStr); // Python SymPy simplify
        printExpr(expr, "main: after SymPy's simplify, expr = ");
        vector<ast_ptr> results = exprAutoNew(expr);
        printExprs(results, BLUE "main: after exprAutoNew: " RESET);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        cout << BLUE << "elapsed time: " << elapsed_seconds.count() << "s" << RESET << endl;
        fprintf(stderr, GREEN "ready> " RESET);
    }
    endPython();
    return 0;
}