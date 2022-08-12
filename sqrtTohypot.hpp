#include <basic.hpp>
#include <vector>

std::unique_ptr<ExprAST> sqrtTohypot(std::unique_ptr<ExprAST> &expr)
{
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();

        std::unique_ptr<ExprAST> &lhs = binOp->getLHS();
        std::unique_ptr<ExprAST> &rhs = binOp->getRHS();

        const std::string exprTypeLHS = lhs->type();
        const std::string exprTypeRHS = rhs->type();

        if (op == '+')
        {
            if ((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary"))
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();
                char opR = binOpR->getOp();
                if ((opL == '*') && (opR == '*'))
                {
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();
                    vector<ExprAST> args;
                    if (isEqual(lhsL, rhsL) && isEqual(lhsR, rhsR))
                    {
                        args.push_back(lhsL);
                        args.push_back(lhsR);
                        return CallExprAST("hypot", args)
                    }
                }
            }
        }
    }
    return expr->Clone();
}
