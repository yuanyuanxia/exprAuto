#include "basic.hpp"
#include "expandAST.hpp"

using std::string;
using std::vector;

//===----------------------------------------------------------------------===//
// Change exprAST equally
//===----------------------------------------------------------------------===//

ast_ptr expandExpr(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const string exprType = expr->type();
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

        return makePtr<NumberExprAST>(number);
    }
    else if(expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());
#ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
#endif
        return makePtr<VariableExprAST>(variable);
    }
    else if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        string callee = (callExpr->getCallee());
#ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
#endif
        vector<ast_ptr> &args = callExpr->getArgs();

        vector<ast_ptr> argsNew;
        for(long unsigned int i = 0; i < args.size(); ++i)
        {
            auto arg = expandExpr(args.at(i));
            argsNew.push_back(std::move(arg));
        }

        return makePtr<CallExprAST>(callee, std::move(argsNew));
    }
    else if(expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();
        string opStr(1, op);
#ifdef DEBUG
        fprintf(stderr, "op: %s\n", opStr.c_str());
#endif

        ast_ptr &lhs = binOp->getLHS();
        ast_ptr &rhs = binOp->getRHS();

        const string exprTypeLHS = lhs->type();
        const string exprTypeRHS = rhs->type();

        if(op == '*')
        {
            if((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary"))  // LHS = RHS = "Binary" && opL = opR = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();
                string opLStr(1, opL);
                char opR = binOpR->getOp();
                string opRStr(1, opR);
                if((opL == '+') && (opR == '+'))
                {
                    // ready for left
                    ast_ptr &lhsL = binOpL->getLHS();
                    ast_ptr &rhsL = binOpL->getRHS();
                    ast_ptr lhsLNew = expandExpr(lhsL);
                    auto lhsLNewCopy = lhsLNew->Clone();
                    ast_ptr rhsLNew = expandExpr(rhsL);
                    auto rhsLNewCopy = rhsLNew->Clone();

                    // ready for right
                    ast_ptr &lhsR = binOpR->getLHS();
                    ast_ptr &rhsR = binOpR->getRHS();
                    ast_ptr lhsRNew = expandExpr(lhsR);
                    auto lhsRNewCopy = lhsRNew->Clone();
                    ast_ptr rhsRNew = expandExpr(rhsR);
                    auto rhsRNewCopy = rhsRNew->Clone();

                    // combine left
                    ast_ptr lhsLTmp = makePtr<BinaryExprAST>('*', std::move(lhsLNew), std::move(lhsRNew));
                    ast_ptr lhsLFinal = expandExpr(lhsLTmp);
                    ast_ptr rhsLTmp = makePtr<BinaryExprAST>('*', std::move(lhsLNewCopy), std::move(rhsRNew));
                    ast_ptr rhsLFinal = expandExpr(rhsLTmp);
                    ast_ptr exprLFinal = makePtr<BinaryExprAST>('+', std::move(lhsLFinal), std::move(rhsLFinal));
                    // combine right
                    ast_ptr lhsRTmp = makePtr<BinaryExprAST>('*', std::move(rhsLNew), std::move(lhsRNewCopy));
                    ast_ptr lhsRFinal = expandExpr(lhsRTmp);
                    ast_ptr rhsRTmp = makePtr<BinaryExprAST>('*', std::move(rhsLNewCopy), std::move(rhsRNewCopy));
                    ast_ptr rhsRFinal = expandExpr(rhsRTmp);
                    ast_ptr exprRFinal = makePtr<BinaryExprAST>('+', std::move(lhsRFinal), std::move(rhsRFinal));
                    // combine together
                    ast_ptr exprFinal = makePtr<BinaryExprAST>('+', std::move(exprLFinal), std::move(exprRFinal));

                    return exprFinal;
                }
                else if((opL == '+') && (opR != '+')) // opL is '+' and opR is not '+'
                {
                    ast_ptr &lhsL = binOpL->getLHS();
                    ast_ptr &rhsL = binOpL->getRHS();

                    ast_ptr lhsLNew = expandExpr(lhsL);
                    ast_ptr rhsLNew = expandExpr(rhsL);
                    ast_ptr rhsNew = expandExpr(rhs);
                    auto rhsNewCopy = rhsNew->Clone();

                    ast_ptr lhsTmp = makePtr<BinaryExprAST>('*', std::move(lhsLNew), std::move(rhsNew));
                    ast_ptr lhsFinal = expandExpr(lhsTmp);
                    ast_ptr rhsTmp = makePtr<BinaryExprAST>('*', std::move(rhsLNew), std::move(rhsNewCopy));
                    ast_ptr rhsFinal = expandExpr(rhsTmp);
                    ast_ptr exprFinal = makePtr<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));

                    return exprFinal;
                }
                else if((opL != '+') && (opR == '+'))// TODO: opL or opR is /
                {
                    ast_ptr &lhsR = binOpR->getLHS();
                    ast_ptr &rhsR = binOpR->getRHS();

                    ast_ptr lhsNew = expandExpr(lhs);
                    auto lhsNewCopy = lhsNew->Clone();
                    ast_ptr lhsRNew = expandExpr(lhsR);
                    ast_ptr rhsRNew = expandExpr(rhsR);

                    ast_ptr lhsTmp = makePtr<BinaryExprAST>('*', std::move(lhsNew), std::move(lhsRNew));
                    ast_ptr lhsFinal = expandExpr(lhsTmp);
                    ast_ptr rhsTmp = makePtr<BinaryExprAST>('*', std::move(lhsNewCopy), std::move(rhsRNew));
                    ast_ptr rhsFinal = expandExpr(rhsTmp);
                    ast_ptr exprFinal = makePtr<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));
                    return exprFinal;
                }
                // else if((opL == '/') && (opR != '/'))// TODO: opL or opR is /
                // {

                // }
                // else if((opL != '/') && (opR == '/'))// TODO: opL or opR is /
                // {

                // }
            }
            else if(exprTypeLHS == "Binary")  // LHS = "Binary" && RHS != "Binary" && opL = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());

                char opL = binOpL->getOp();
                string opLStr(1, opL);

                if(opLStr == "+")
                {
                    ast_ptr &lhsL = binOpL->getLHS();
                    ast_ptr &rhsL = binOpL->getRHS();

                    ast_ptr lhsLNew = expandExpr(lhsL);
                    ast_ptr rhsLNew = expandExpr(rhsL);
                    ast_ptr rhsNew = expandExpr(rhs);
                    auto rhsNewCopy = rhsNew->Clone();

                    ast_ptr lhsTmp = makePtr<BinaryExprAST>('*', std::move(lhsLNew), std::move(rhsNew));
                    ast_ptr lhsFinal = expandExpr(lhsTmp);
                    ast_ptr rhsTmp = makePtr<BinaryExprAST>('*', std::move(rhsLNew), std::move(rhsNewCopy));
                    ast_ptr rhsFinal = expandExpr(rhsTmp);
                    ast_ptr exprFinal = makePtr<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));

                    return exprFinal;
                }
            }
            else if(exprTypeRHS == "Binary")  // LHS != "Binary" && RHS = "Binary" && opR = "+"
            {
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opR = binOpR->getOp();
                string opRStr(1, opR);

                if(opRStr == "+")
                {
                    ast_ptr &lhsR = binOpR->getLHS();
                    ast_ptr &rhsR = binOpR->getRHS();

                    ast_ptr lhsNew = expandExpr(lhs);
                    auto lhsNewCopy = lhsNew->Clone();
                    ast_ptr lhsRNew = expandExpr(lhsR);
                    ast_ptr rhsRNew = expandExpr(rhsR);

                    ast_ptr lhsTmp = makePtr<BinaryExprAST>('*', std::move(lhsNew), std::move(lhsRNew));
                    ast_ptr lhsFinal = expandExpr(lhsTmp);
                    ast_ptr rhsTmp = makePtr<BinaryExprAST>('*', std::move(lhsNewCopy), std::move(rhsRNew));
                    ast_ptr rhsFinal = expandExpr(rhsTmp);
                    ast_ptr exprFinal = makePtr<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));
                    return exprFinal;
                }
            }
        }
        // other situations
        ast_ptr lhsNew = expandExpr(lhs);
        ast_ptr rhsNew = expandExpr(rhs);
        ast_ptr exprFinal = makePtr<BinaryExprAST>(op, std::move(lhsNew), std::move(rhsNew));
        return exprFinal;
    }
    else  // TODO: Error handling is to be supported
    {
        string exprStr = "unknown expression";
        return makePtr<VariableExprAST>(exprStr);
    }
}

ast_ptr expandExprWrapper(const ast_ptr &exprOrigin)
{
    ast_ptr exprTmp = exprOrigin->Clone();
    ast_ptr exprNew = nullptr;
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
