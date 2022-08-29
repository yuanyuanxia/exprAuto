#include "basic.hpp"
#include "laxerAST.hpp"
#include "parserAST.hpp"
#include "expandAST.hpp"
#include "polyRewrite.hpp"
#include "mathfuncRewrite.hpp"
#include "exprAuto.hpp"
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

        std::unique_ptr<ExprAST> exprNew = expandExprWrapper(exprOrigin);
        std::string funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\texpandExpr: %s\n", funcBodyStr.c_str());

        exprNew = sqrtTohypot(exprOrigin);
        funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\tsqrtTohypot: %s\n", funcBodyStr.c_str());

        exprNew = expToexpm1(exprOrigin);
        funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\texpToexpm1: %s\n", funcBodyStr.c_str());

        exprNew = logTolog1p(exprOrigin);
        funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\tlogTolog1p: %s\n", funcBodyStr.c_str());

        exprNew = lex_x_Or_elx_x(exprOrigin);
        funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\tlex_x_Or_elx_x: %s\n", funcBodyStr.c_str());

        auto exprs = createExpr((exprOrigin));

        auto exprsFinal = exprAuto(exprOrigin);
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