#include "basic.hpp"
#include "parserAST.hpp"
#include "printAST.hpp"
#include "changeAST.hpp"
// #define DEBUG

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition()
{
    if (ParseDefinition())
    {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern()
{
    if (ParseExtern())
    {
        fprintf(stderr, "Parsed an extern\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression()
{
    std::unique_ptr<FunctionAST> fun = ParseTopLevelExpr();
    // Evaluate a top-level expression into an anonymous function.
    if (fun)
    {
        fprintf(stderr, "Parsed a top-level expr\n");
        PrintFunction(fun);

        fprintf(stderr, "After change\n");

        std::unique_ptr<ExprAST> &exprOrigin = fun->getFuncBody();
        std::unique_ptr<ExprAST> exprNew1 = changeExpression(exprOrigin);
        std::unique_ptr<ExprAST> exprNew2 = changeExpression(exprNew1);
        std::unique_ptr<ExprAST> exprNew = changeExpression(exprNew2);
        std::string funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\t%s\n", funcBodyStr.c_str());
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop()
{
    while (true)
    {
        fprintf(stderr, "ready> ");
        switch (CurTok)
        {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            getNextToken();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
    // Install standard binary operators.
    installOperators();

    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();

    // Run the main "interpreter loop" now.

    MainLoop();

    return 0;
}