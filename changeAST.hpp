#ifndef CHANGEEXPR
#define CHANGEEXPR
#include "basic.hpp"

//===----------------------------------------------------------------------===//
// Change exprAST equally
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> expandExpr(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif

    if(expr->type() == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());
#ifdef DEBUG
        fprintf(stderr, "number: %f\n", number);
#endif

        return std::make_unique<NumberExprAST>(number);
    }
    else if(expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        std::string variable = (variableExpr->getVariable());
#ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
#endif
        return std::make_unique<VariableExprAST>(variable);
    }
    else if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
#ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
#endif
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();

        std::vector<std::unique_ptr<ExprAST>> argsNew;
        for(long unsigned int i = 0; i < args.size(); ++i)
        {
            auto arg = expandExpr(args.at(i));
            argsNew.push_back(std::move(arg));
        }

        return std::make_unique<CallExprAST>(callee, std::move(argsNew));
    }
    else if(expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();
        std::string opStr(1, op);
#ifdef DEBUG
        fprintf(stderr, "op: %s\n", opStr.c_str());
#endif

        std::unique_ptr<ExprAST> &lhs = binOp->getLHS();
        std::unique_ptr<ExprAST> &rhs = binOp->getRHS();

        const std::string exprTypeLHS = lhs->type();
        const std::string exprTypeRHS = rhs->type();

        if(op == '*')
        {
            if((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary"))  // LHS = RHS = "Binary" && opL = opR = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();
                std::string opLStr(1, opL);
                char opR = binOpR->getOp();
                std::string opRStr(1, opR);
                if((opL == '+') && (opR == '+'))
                {
                    // ready for left
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();
                    std::unique_ptr<ExprAST> lhsLNew = expandExpr(lhsL);
                    auto lhsLNewCopy = lhsLNew->Clone();
                    std::unique_ptr<ExprAST> rhsLNew = expandExpr(rhsL);
                    auto rhsLNewCopy = rhsLNew->Clone();

                    // ready for right
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();
                    std::unique_ptr<ExprAST> lhsRNew = expandExpr(lhsR);
                    auto lhsRNewCopy = lhsRNew->Clone();
                    std::unique_ptr<ExprAST> rhsRNew = expandExpr(rhsR);
                    auto rhsRNewCopy = rhsRNew->Clone();

                    // combine left
                    std::unique_ptr<ExprAST> lhsLTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNew), std::move(lhsRNew));
                    std::unique_ptr<ExprAST> lhsLFinal = expandExpr(lhsLTmp);
                    std::unique_ptr<ExprAST> rhsLTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNewCopy), std::move(rhsRNew));
                    std::unique_ptr<ExprAST> rhsLFinal = expandExpr(rhsLTmp);
                    std::unique_ptr<ExprAST> exprLFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsLFinal), std::move(rhsLFinal));
                    // combine right
                    std::unique_ptr<ExprAST> lhsRTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNew), std::move(lhsRNewCopy));
                    std::unique_ptr<ExprAST> lhsRFinal = expandExpr(lhsRTmp);
                    std::unique_ptr<ExprAST> rhsRTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNewCopy), std::move(rhsRNewCopy));
                    std::unique_ptr<ExprAST> rhsRFinal = expandExpr(rhsRTmp);
                    std::unique_ptr<ExprAST> exprRFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsRFinal), std::move(rhsRFinal));
                    // combine together
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(exprLFinal), std::move(exprRFinal));

                    return exprFinal;
                }
            }
            else if(exprTypeLHS == "Binary")  // LHS = "Binary" && RHS != "Binary" && opL = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());

                char opL = binOpL->getOp();
                std::string opLStr(1, opL);

                if(opLStr == "+")
                {
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();

                    std::unique_ptr<ExprAST> lhsLNew = expandExpr(lhsL);
                    std::unique_ptr<ExprAST> rhsLNew = expandExpr(rhsL);
                    std::unique_ptr<ExprAST> rhsNew = expandExpr(rhs);
                    auto rhsNewCopy = rhsNew->Clone();

                    std::unique_ptr<ExprAST> lhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNew), std::move(rhsNew));
                    std::unique_ptr<ExprAST> lhsFinal = expandExpr(lhsTmp);
                    std::unique_ptr<ExprAST> rhsTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNew), std::move(rhsNewCopy));
                    std::unique_ptr<ExprAST> rhsFinal = expandExpr(rhsTmp);
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));

                    return exprFinal;
                }
            }
            else if(exprTypeRHS == "Binary")  // LHS != "Binary" && RHS = "Binary" && opR = "+"
            {
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opR = binOpR->getOp();
                std::string opRStr(1, opR);

                if(opRStr == "+")
                {
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();

                    std::unique_ptr<ExprAST> lhsNew = expandExpr(lhs);
                    auto lhsNewCopy = lhsNew->Clone();
                    std::unique_ptr<ExprAST> lhsRNew = expandExpr(lhsR);
                    std::unique_ptr<ExprAST> rhsRNew = expandExpr(rhsR);

                    std::unique_ptr<ExprAST> lhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsNew), std::move(lhsRNew));
                    std::unique_ptr<ExprAST> lhsFinal = expandExpr(lhsTmp);
                    std::unique_ptr<ExprAST> rhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsNewCopy), std::move(rhsRNew));
                    std::unique_ptr<ExprAST> rhsFinal = expandExpr(rhsTmp);
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));
                    return exprFinal;
                }
            }
        }
        // other situations
        std::unique_ptr<ExprAST> lhsNew = expandExpr(lhs);
        std::unique_ptr<ExprAST> rhsNew = expandExpr(rhs);
        std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>(op, std::move(lhsNew), std::move(rhsNew));
        return exprFinal;
    }
    else  // TODO: Error handling is to be supported
    {
        std::string exprStr = "unknown expression";
        return std::make_unique<VariableExprAST>(exprStr);
    }
}

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

std::unique_ptr<ExprAST> expandExprWrapper(const std::unique_ptr<ExprAST> &exprOrigin)
{
    std::unique_ptr<ExprAST> exprTmp = exprOrigin->Clone();
    std::unique_ptr<ExprAST> exprNew = nullptr;
    while(true)
    {
        exprNew = expandExpr(exprTmp);
        if(isEqual(exprNew, exprTmp))
        {
            break;
        }
        else
        {
            exprTmp = std::move(exprNew);
        }
    }
    return exprNew;
}

#endif