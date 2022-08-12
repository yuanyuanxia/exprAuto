#include <basic.hpp>
#include <vector>

std::unique_ptr<ExprAST> sqrtTohypot(std::unique_ptr<ExprAST> &expr)
{
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp(); //主表达式的二元运算符

        std::unique_ptr<ExprAST> &lhs = binOp->getLHS(); //左子表达式
        std::unique_ptr<ExprAST> &rhs = binOp->getRHS(); //右子表达式

        const std::string exprTypeLHS = lhs->type();
        const std::string exprTypeRHS = rhs->type();

        if (op == '+')
        {
            if ((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary")) // 左子表达式和右子表达式都是二元运算符表达式,且主表达式的运算符为+
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get()); //左子表达式
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get()); //右子表达式

                char opL = binOpL->getOp(); //左子表达式的运算符
                char opR = binOpR->getOp(); //右子表达式的运算符
                if ((opL == '*') && (opR == '*'))
                {
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS(); //左子表达式的左表达式
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS(); //左子表达式的左表达式
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS(); //右子表达式的左表达式
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS(); //右子表达式的右表达式
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
