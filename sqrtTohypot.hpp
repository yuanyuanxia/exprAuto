#include "basic.hpp"
#include "changeAST.hpp"
#include <vector>

std::unique_ptr<ExprAST> sqrtTohypot(std::unique_ptr<ExprAST> &expr)
{
    if (expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();
        if (callee == "sqrt" && args.size() == 1)
        {
            //取出sqrt里面的参数-->x*x+y*y
            auto expr = expandExpr(args.at(0));
            //对参数表达式进行处理
            if (expr->type() == "Binary")
            {
                BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
                char op = binOp->getOp();

                std::unique_ptr<ExprAST> &lhs = binOp->getLHS(); //左表达式
                std::unique_ptr<ExprAST> &rhs = binOp->getRHS(); //右表达式

                const std::string exprTypeLHS = lhs->type();
                const std::string exprTypeRHS = rhs->type();

                if (op == '+')
                {
                    if ((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary"))
                    {
                        BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                        BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                        char opL = binOpL->getOp(); //左表达式操作符
                        char opR = binOpR->getOp(); //右表达式操作符
                        if ((opL == '*') && (opR == '*'))
                        {
                            std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS(); //左表达式左子表达式
                            std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS(); //左表达式右子表达式
                            std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS(); //右表达式左子表达式
                            std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS(); //右表达式右子表达式
                            std::vector<std::unique_ptr<ExprAST>> args;
                            if (isEqual(lhsL, rhsL) && isEqual(lhsR, rhsR))
                            {

                                std::vector<std::unique_ptr<ExprAST>> argsNew; //建立一个新的args,存放hypot参数

                                argsNew.push_back(std::move(lhsL));
                                argsNew.push_back(std::move(lhsR));

                                std::string calleeNew = "hypot";
                                std::unique_ptr<ExprAST> exprFinal = std::make_unique<CallExprAST>(calleeNew, std::move(argsNew));
                                return exprFinal;
                            }
                        }
                    }
                }
            }
        }
    }

    return expr->Clone();
}
