#include "basic.hpp"

//===----------------------------------------------------------------------===//
// basic operation
//===----------------------------------------------------------------------===//

bool isEqual(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2)
{
    if(expr1 == nullptr || expr2 == nullptr)
    {
        fprintf(stderr, "empty\n");
        return false;
    }

    const std::string expr1Type = expr1->type();
    const std::string expr2Type = expr2->type();

    if(expr1Type == expr2Type)
    {
        if(expr1->type() == "Number")
        {
            NumberExprAST *numberExpr1 = dynamic_cast<NumberExprAST *>(expr1.get());
            double number1 = (numberExpr1->getNumber());
            NumberExprAST *numberExpr2 = dynamic_cast<NumberExprAST *>(expr2.get());
            double number2 = (numberExpr2->getNumber());

            return (number1 == number2);
        }
        else if(expr1->type() == "Variable")
        {
            VariableExprAST *variableExpr1 = dynamic_cast<VariableExprAST *>(expr1.get());
            std::string variable1 = (variableExpr1->getVariable());
            VariableExprAST *variableExpr2 = dynamic_cast<VariableExprAST *>(expr2.get());
            std::string variable2 = (variableExpr2->getVariable());

            return (variable1 == variable2);
        }
        else if(expr1->type() == "Call")
        {
            CallExprAST *callExpr1 = dynamic_cast<CallExprAST *>(expr1.get());
            std::string callee1 = (callExpr1->getCallee());
            CallExprAST *callExpr2 = dynamic_cast<CallExprAST *>(expr2.get());
            std::string callee2 = (callExpr2->getCallee());

            if(callee1 == callee2)
            {
                std::vector<std::unique_ptr<ExprAST>> &args1 = callExpr1->getArgs();
                std::vector<std::unique_ptr<ExprAST>> &args2 = callExpr2->getArgs();

                if(args1.size() == args2.size())
                {
                    for(long unsigned int i = 0; i < args1.size(); ++i)
                    {
                        if(!isEqual(args1.at(i), args2.at(i)))
                        {
                            return false;
                        }
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if(expr1Type == "Binary")
        {
            BinaryExprAST *binOp1 = dynamic_cast<BinaryExprAST *>(expr1.get());
            char op1 = binOp1->getOp();
            std::string opStr1(1, op1);
            BinaryExprAST *binOp2 = dynamic_cast<BinaryExprAST *>(expr2.get());
            char op2 = binOp2->getOp();
            std::string opStr2(1, op2);
            if(opStr1 == opStr2)
            {
                std::unique_ptr<ExprAST> &lhs1 = binOp1->getLHS();
                std::unique_ptr<ExprAST> &rhs1 = binOp1->getRHS();
                std::unique_ptr<ExprAST> &lhs2 = binOp2->getLHS();
                std::unique_ptr<ExprAST> &rhs2 = binOp2->getRHS();

                return (isEqual(lhs1, lhs2) && isEqual(rhs1, rhs2));
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;  // TODO: handle the other cases
        }
    }
    else
    {
        return false;
    }
}

bool isFraction(const std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: isFraction: the input expr is nullptr!\n");
        return false;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return true;
        }
    }
    return false;
}

bool isConstant(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    size_t size = exprs.size();
    if (size != 1)
    {
        return false;
    }
    const std::unique_ptr<ExprAST> &expr = exprs.at(0);
    if(expr->type() == "Number")
    {
        return true;
    }
    return false;
}

std::unique_ptr<ExprAST> getNumerator(const std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getNumerator: the input expr is nullptr!\n");
        return nullptr;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return std::move(binOpPtr->getLHS());
        }
    }
    fprintf(stderr, "\tWARNING: getNumerator: the input expr is not match for fraction\n");
    return expr->Clone();
}

std::unique_ptr<ExprAST> getDenominator(const std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getDenominator: the input expr is nullptr!\n");
        return nullptr;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return std::move(binOpPtr->getRHS());
        }
    }
    fprintf(stderr, "\tWARNING: getDenominator: the input expr is not match for fraction\n");
    return expr->Clone();
}

std::unique_ptr<ExprAST> createBinaryExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2, const char op)
{
    if (expr1 == nullptr && expr2 == nullptr)
    {
        fprintf(stderr, "\tWARNING: createBinaryExpr: all the inputs are nullptr!\n");
        return nullptr;
    }
    if (expr1 == nullptr)
    {
        return expr2->Clone();
    }
    if (expr2 == nullptr)
    {
        return expr1->Clone();
    }
    auto lhs = expr1->Clone();
    auto rhs = expr2->Clone();
    return std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
}

std::unique_ptr<ExprAST> addExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2) { return createBinaryExpr(expr1, expr2, '+'); }

std::unique_ptr<ExprAST> mulExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2) { return createBinaryExpr(expr1, expr2, '*'); }

std::unique_ptr<ExprAST> divExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2) { return createBinaryExpr(expr1, expr2, '/'); }

void mineAppend(std::vector<ast_ptr> &dest, std::vector<ast_ptr> &origin)
{
    dest.insert(dest.end(), std::make_move_iterator(origin.begin()), std::make_move_iterator(origin.end()));
}

//===----------------------------------------------------------------------===//
// print information
//===----------------------------------------------------------------------===//

std::string PrintExpression(const std::unique_ptr<ExprAST> &expr)
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

        exprStr += "(" + lhsStr + " " + opStr + " " + rhsStr + ")";
    }
    else
    {
        exprStr = "unknown expression";
    }
    return exprStr;
}

void PrintFunction(std::unique_ptr<FunctionAST> &fun)
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

void printExpr(const ast_ptr &expr, std::string prefix, int index)
{
    if(index == -1)
        fprintf(stderr, "%s%s\n", prefix.c_str(), PrintExpression(expr).c_str());
    else
        fprintf(stderr, "%sNo.%d: %s\n", prefix.c_str(), index, PrintExpression(expr).c_str());
}

void printExprs(const std::vector<ast_ptr> &exprs, std::string prefix)
{
    for(size_t i = 0; i < exprs.size(); i++)
    {
        auto &expr = exprs.at(i);
        fprintf(stderr, "%sNo.%ld: %s\n", prefix.c_str(), i, PrintExpression(expr).c_str());
    }
}
