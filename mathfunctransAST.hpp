#include "basic.hpp"
#include "changeAST.hpp"

//===----------------------------------------------------------------------===//
// Equivalent transformation of mathematical function
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> isEqualMathFunction(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif

    if (expr->type() == "Number")
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
            auto arg = expandExpr(args.at(i));
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
        if(op =='-')
        {
            if  ((exprTypeLHS == "Call")&&(exprTypeRHS == "Number")) // LHS = "Call" && RHS ='Number'
            {
                CallExprAST *callExpr01 = dynamic_cast<CallExprAST *>(lhs.get()); //得到这个表达式
                std::string callee01 = (callExpr01->getCallee());  //获得表达式中的函数
                std::vector<std::unique_ptr<ExprAST>> &args01= callExpr01->getArgs(); //表达式中函数的参数AST
                std::vector<std::unique_ptr<ExprAST>> argsNew01;  //建立一个新的args
#ifdef DEBUG
                fprintf(stderr, "call: %s\n", calleeL.c_str()); //输出表达式的函数
#endif
                NumberExprAST *numberExpr01 = dynamic_cast<NumberExprAST *>(rhs.get());
                double number = (numberExpr01->getNumber());
                //得到这个表达式中的变量
                if ((callee01=="exp"))
                {
                    for (long unsigned int i = 0; i < args01.size(); ++i)
                    {
                        auto arg = expandExpr(args01.at(i));
                        argsNew01.push_back(std::move(arg));
                    }
                    std::string calleeNew = "expm1";//设置Callee
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<CallExprAST>(calleeNew, std::move(argsNew01));
                    return exprFinal;//返回等价的AST
                }
                else // TODO: Error handling is to be supported
                {
                    std::string exprStr = "unknown expression1";
                    return std::make_unique<VariableExprAST>(exprStr);
                }
            }
            else // TODO: Error handling is to be supported
            {
                std::string exprStr = "unknown expression2";
                return std::make_unique<VariableExprAST>(exprStr);
            }
        }
        else // TODO: Error handling is to be supported
        {
            std::string exprStr = "unknown expression3";
            return std::make_unique<VariableExprAST>(exprStr);
        }
        
    }
    else // TODO: Error handling is to be supported
    {
        std::string exprStr = "unknown expression4";
        return std::make_unique<VariableExprAST>(exprStr);
    }
}