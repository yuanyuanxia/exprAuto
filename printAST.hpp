#include "basic.hpp"

//===----------------------------------------------------------------------===//
// print information
//===----------------------------------------------------------------------===//

std::string PrintExpression(std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
// const std::string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif
    std::string exprStr = "";
    if(expr->type() == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        // std::unique_ptr<NumberExprAST> numberExpr;
        // if (tmp != nullptr)
        // {
        //     expr.release();
        //     numberExpr.reset(tmp);
        // }
        double number = (numberExpr->getNumber());
#ifdef DEBUG
        fprintf(stderr, "number: %f\n", number);
#endif

        return std::to_string(number);
    }
    else if(expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        std::string variable = (variableExpr->getVariable());
#ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
#endif

        return variable;
    }
    else if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
#ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
#endif
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();

        std::vector<std::string> argsStr;
        for(long unsigned int i = 0; i < args.size(); ++i)
        {
            std::string strTmp = PrintExpression(args.at(i));  // std::unique_ptr<ExprAST>& exprTmp = args.at(i);
            argsStr.push_back(strTmp);
        }
        callee += "(";
        for(long unsigned int i = 0; i < argsStr.size() - 1; ++i)
        {
            callee += argsStr.at(i) + ", ";
        }
        callee += argsStr.back() + ")";
        return callee;
    }
    else if(expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        // std::unique_ptr<BinaryExprAST> binOp = std::make_unique<BinaryExprAST>(expr); // a old wrong method
        char op = binOp->getOp();
        std::string opStr(1, op);
#ifdef DEBUG
        fprintf(stderr, "op: %s\n", opStr.c_str());
#endif

        std::unique_ptr<ExprAST> &lhs = binOp->getLHS();
        std::string lhsStr = PrintExpression(lhs);
        std::unique_ptr<ExprAST> &rhs = binOp->getRHS();
        std::string rhsStr = PrintExpression(rhs);

        exprStr += "(" + lhsStr + opStr + rhsStr + ")";
    }
    else
    {
        exprStr = "unknown expression";
    }
    return exprStr;
}

static void PrintFunction(std::unique_ptr<FunctionAST> &fun)
{
    if(fun == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
    else
    {
        std::string funcNameStr = fun->getFuncName();
        std::vector<std::string> funcArgsStr = fun->getFuncArgs();
        std::unique_ptr<ExprAST> &expr = fun->getFuncBody();

        fprintf(stderr, "funcName: %s\n", funcNameStr.c_str());
        fprintf(stderr, "Args list:\n");
        if(funcArgsStr.size() == 0)
        {
            fprintf(stderr, "\tempty args");
        }
        else
        {
            for(auto it = funcArgsStr.begin(); it != funcArgsStr.end(); ++it)
            {
                fprintf(stderr, "%s ", (*it).c_str());
            }
        }
        fprintf(stderr, "\nFunc Body:\n");
        // expr->printExpr();
        std::string funcBodyStr = PrintExpression(expr);
        fprintf(stderr, "\t%s\n", funcBodyStr.c_str());
    }
}
