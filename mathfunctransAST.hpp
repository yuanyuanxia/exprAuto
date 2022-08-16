#include "basic.hpp"
#include "changeAST.hpp"

//===----------------------------------------------------------------------===//
// Equivalent transformation of mathematical function
//===----------------------------------------------------------------------===//

// exp(expr) - 1 ==> expm1(expr)
std::unique_ptr<ExprAST> expToexpm1(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif

    if(expr->type() == "Call")  // TODO: may change the implement logic
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
        if(op == '-')
        {
            if((exprTypeLHS == "Call") && (exprTypeRHS == "Number"))  // LHS = "Call" && RHS ='Number'
            {
                CallExprAST *callExpr01 = dynamic_cast<CallExprAST *>(lhs.get());       //得到这个表达式
                std::string callee01 = (callExpr01->getCallee());                       //获得表达式中的函数
                std::vector<std::unique_ptr<ExprAST>> &args01 = callExpr01->getArgs();  //表达式中函数的参数AST
                std::vector<std::unique_ptr<ExprAST>> argsNew01;                        //建立一个新的args
#ifdef DEBUG
                fprintf(stderr, "call: %s\n", calleeL.c_str());  //输出表达式的函数
#endif
                NumberExprAST *numberExpr01 = dynamic_cast<NumberExprAST *>(rhs.get());
                double number = (numberExpr01->getNumber());
                //得到这个表达式中的变量
                if((callee01 == "exp"))
                {
                    for(long unsigned int i = 0; i < args01.size(); ++i)
                    {
                        auto arg = expandExpr(args01.at(i));
                        argsNew01.push_back(std::move(arg));
                    }
                    std::string calleeNew = "expm1";  //设置Callee
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<CallExprAST>(calleeNew, std::move(argsNew01));
                    return exprFinal;  //返回等价的AST
                }
                else  // TODO: Error handling is to be supported
                {
                    std::string exprStr = "unknown expression1";
                    return std::make_unique<VariableExprAST>(exprStr);
                }
            }
            else  // TODO: Error handling is to be supported
            {
                std::string exprStr = "unknown expression2";
                return std::make_unique<VariableExprAST>(exprStr);
            }
        }
        else  // TODO: Error handling is to be supported
        {
            std::string exprStr = "unknown expression3";
            return std::make_unique<VariableExprAST>(exprStr);
        }
    }
    else  // TODO: Error handling is to be supported
    {
        std::string exprStr = "unknown expression4";
        return std::make_unique<VariableExprAST>(exprStr);
    }
}

// log(expr+1) ==> log1p(expr)
std::unique_ptr<ExprAST> logTolog1p(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif

    if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
#ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
#endif
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();
        std::vector<std::unique_ptr<ExprAST>> argsNew;

        if(args.size() == 1)
        {
            if((callee == "log") && (args[0]->type() == "Binary"))  //表达式为log并且参数类型为Binary
            {
                BinaryExprAST *argsOp = dynamic_cast<BinaryExprAST *>(args.at(0).get());
                char op = argsOp->getOp();
                std::string opStr(1, op);

                std::unique_ptr<ExprAST> &lhs = argsOp->getLHS();
                std::unique_ptr<ExprAST> &rhs = argsOp->getRHS();
                const std::string exprTypeRHS = rhs->type();

                if((op == '+') && (exprTypeRHS == "Number"))  //二元操作符为‘+’且右操作符类型为Number
                {
                    NumberExprAST *numberExpr1 = dynamic_cast<NumberExprAST *>(rhs.get());
                    double number1 = numberExpr1->getNumber();
                    if(number1 == 1)
                    {
                        auto arg = expandExpr(lhs);
                        argsNew.push_back(std::move(arg));
                        std::unique_ptr<ExprAST> exprFinal = std::make_unique<CallExprAST>("log1p", std::move(argsNew));
                        return exprFinal;
                    }
                }
            }
        }
    }
    return expr->Clone();
}

// sqrt(expr1*expr1+expr2*expr2) ==> hypot(expr1, expr2)
std::unique_ptr<ExprAST> sqrtTohypot(const std::unique_ptr<ExprAST> &expr)
{
    if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();
        if(callee == "sqrt" && args.size() == 1)
        {
            //取出sqrt里面的参数-->x*x+y*y
            auto expr = std::move(args.at(0));
            //对参数表达式进行处理
            if(expr->type() == "Binary")
            {
                BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
                char op = binOp->getOp();

                std::unique_ptr<ExprAST> &lhs = binOp->getLHS();  //左表达式
                std::unique_ptr<ExprAST> &rhs = binOp->getRHS();  //右表达式

                const std::string exprTypeLHS = lhs->type();
                const std::string exprTypeRHS = rhs->type();

                if(op == '+')
                {
                    if((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary"))
                    {
                        BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                        BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                        char opL = binOpL->getOp();  //左表达式操作符
                        char opR = binOpR->getOp();  //右表达式操作符
                        if((opL == '*') && (opR == '*'))
                        {
                            std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();  //左表达式左子表达式
                            std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();  //左表达式右子表达式
                            std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();  //右表达式左子表达式
                            std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();  //右表达式右子表达式
                            std::vector<std::unique_ptr<ExprAST>> args;
                            if(isEqual(lhsL, rhsL) && isEqual(lhsR, rhsR))
                            {
                                std::vector<std::unique_ptr<ExprAST>> argsNew;  //建立一个新的args,存放hypot参数

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
