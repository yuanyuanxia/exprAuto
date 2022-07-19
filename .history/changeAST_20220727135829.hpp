#include "basic.hpp"

//===----------------------------------------------------------------------===//
// Change exprAST equally
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> changeExpression(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif

    if (expr-> == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());
#ifdef DEBUG
        fprintf(stderr, "number: %f\n", number);
#endif

        return std::make_unique<NumberExprAST>(number);
    }
    else if (expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        std::string variable = (variableExpr->getVariable());
#ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
#endif
        return std::make_unique<VariableExprAST>(variable);
    }
    else if (expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
#ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
#endif
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();

        std::vector<std::unique_ptr<ExprAST>> argsNew;
        for (long unsigned int i = 0; i < args.size(); ++i)
        {
            auto arg = changeExpression(args.at(i));
            argsNew.push_back(std::move(arg));
        }

        return std::make_unique<CallExprAST>(callee, std::move(argsNew));
    }
    else if (expr->type() == "Binary")
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

        if (op == '*')
        {
            if ((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary")) // LHS = RHS = "Binary" && opL = opR = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();
                std::string opLStr(1, opL);
                char opR = binOpR->getOp();
                std::string opRStr(1, opR);
                if ((opL == '+') && (opR == '+'))
                {
                    // ready for left
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();
                    std::unique_ptr<ExprAST> lhsLNew = changeExpression(lhsL);
                    auto lhsLNewCopy = lhsLNew->Clone();
                    std::unique_ptr<ExprAST> rhsLNew = changeExpression(rhsL);
                    auto rhsLNewCopy = rhsLNew->Clone();

                    // ready for right
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();
                    std::unique_ptr<ExprAST> lhsRNew = changeExpression(lhsR);
                    auto lhsRNewCopy = lhsRNew->Clone();
                    std::unique_ptr<ExprAST> rhsRNew = changeExpression(rhsR);
                    auto rhsRNewCopy = rhsRNew->Clone();

                    // combine left
                    std::unique_ptr<ExprAST> lhsLTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNew), std::move(lhsRNew));
                    std::unique_ptr<ExprAST> lhsLFinal = changeExpression(lhsLTmp);
                    std::unique_ptr<ExprAST> rhsLTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNewCopy), std::move(rhsRNew));
                    std::unique_ptr<ExprAST> rhsLFinal = changeExpression(rhsLTmp);
                    std::unique_ptr<ExprAST> exprLFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsLFinal), std::move(rhsLFinal));
                    // combine right
                    std::unique_ptr<ExprAST> lhsRTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNew), std::move(lhsRNewCopy));
                    std::unique_ptr<ExprAST> lhsRFinal = changeExpression(lhsRTmp);
                    std::unique_ptr<ExprAST> rhsRTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNewCopy), std::move(rhsRNewCopy));
                    std::unique_ptr<ExprAST> rhsRFinal = changeExpression(rhsRTmp);
                    std::unique_ptr<ExprAST> exprRFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsRFinal), std::move(rhsRFinal));
                    // combine together
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(exprLFinal), std::move(exprRFinal));

                    return exprFinal;
                }
            }
            else if (exprTypeLHS == "Binary") // LHS = "Binary" && RHS != "Binary" && opL = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());

                char opL = binOpL->getOp();
                std::string opLStr(1, opL);

                if (opLStr == "+")
                {
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();

                    std::unique_ptr<ExprAST> lhsLNew = changeExpression(lhsL);
                    std::unique_ptr<ExprAST> rhsLNew = changeExpression(rhsL);
                    std::unique_ptr<ExprAST> rhsNew = changeExpression(rhs);
                    auto rhsNewCopy = rhsNew->Clone();

                    std::unique_ptr<ExprAST> lhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNew), std::move(rhsNew));
                    std::unique_ptr<ExprAST> lhsFinal = changeExpression(lhsTmp);
                    std::unique_ptr<ExprAST> rhsTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNew), std::move(rhsNewCopy));
                    std::unique_ptr<ExprAST> rhsFinal = changeExpression(rhsTmp);
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));

                    return exprFinal;
                }
            }
            else if (exprTypeRHS == "Binary") // LHS != "Binary" && RHS = "Binary" && opR = "+"
            {
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opR = binOpR->getOp();
                std::string opRStr(1, opR);

                if (opRStr == "+")
                {
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();

                    std::unique_ptr<ExprAST> lhsNew = changeExpression(lhs);
                    auto lhsNewCopy = lhsNew->Clone();
                    std::unique_ptr<ExprAST> lhsRNew = changeExpression(lhsR);
                    std::unique_ptr<ExprAST> rhsRNew = changeExpression(rhsR);

                    std::unique_ptr<ExprAST> lhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsNew), std::move(lhsRNew));
                    std::unique_ptr<ExprAST> lhsFinal = changeExpression(lhsTmp);
                    std::unique_ptr<ExprAST> rhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsNewCopy), std::move(rhsRNew));
                    std::unique_ptr<ExprAST> rhsFinal = changeExpression(rhsTmp);
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));
                    return exprFinal;
                }
            }
        }
        // other situations
        std::unique_ptr<ExprAST> lhsNew = changeExpression(lhs);
        std::unique_ptr<ExprAST> rhsNew = changeExpression(rhs);
        std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>(op, std::move(lhsNew), std::move(rhsNew));
        return exprFinal;
    }
    else // TODO: Error handling is to be supported
    {
        std::string exprStr = "unknown expression";
        return std::make_unique<VariableExprAST>(exprStr);
    }
}
