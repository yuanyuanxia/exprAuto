#include "basic.hpp"
#include "expandAST.hpp"
#include "mathfuncRewrite.hpp"

//===----------------------------------------------------------------------===//
// Equivalent transformation of mathematical function
//===----------------------------------------------------------------------===//

// exp(expr) - 1 ==> expm1(expr)
std::unique_ptr<ExprAST> expToexpm1(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "expToexpm1's input is empty!\n");
        return nullptr;
    }

    if(expr->type() != "Binary")
    {
        return expr->Clone();
    }

    BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOp->getOp();
    std::string opStr(1, op);

    std::unique_ptr<ExprAST> &lhs = binOp->getLHS();
    std::unique_ptr<ExprAST> &rhs = binOp->getRHS();
    if((op == '+') && (lhs->type() == "Call") && (rhs->type() == "Number"))  // LHS = "Call" && RHS ='Number'
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(lhs.get());
        std::string callee = (callExpr->getCallee());
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();
        std::vector<std::unique_ptr<ExprAST>> argsNew;

        NumberExprAST *numberExpr01 = dynamic_cast<NumberExprAST *>(rhs.get());
        double number = (numberExpr01->getNumber());
        //得到这个表达式中的变量
        if((callee == "exp") && (number == -1))
        {
            for(long unsigned int i = 0; i < args.size(); ++i)
            {
                auto arg = expandExpr(args.at(i));
                argsNew.push_back(std::move(arg));
            }
            std::string calleeNew = "expm1";
            std::unique_ptr<ExprAST> exprFinal = std::make_unique<CallExprAST>(calleeNew, std::move(argsNew));
            return exprFinal;
        }
    }

    return expr->Clone();
}

// log(expr+1) or log(1+expr) ==> log1p(expr)
std::unique_ptr<ExprAST> logTolog1p(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "logTolog1p's input is empty!\n");
        return nullptr;
    }

    if(expr->type() != "Call")
    {
        return expr->Clone();
    }
    
    CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
    std::string callee = (callExpr->getCallee());
    std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();

    if(callee == "log" && args.size() == 1 && args.at(0)->type() == "Binary")
    {
        BinaryExprAST *argsOp = dynamic_cast<BinaryExprAST *>(args.at(0).get());
        char op = argsOp->getOp();

        if(op == '+')
        {
            std::vector<std::unique_ptr<ExprAST>> argsNew;
            std::unique_ptr<ExprAST> &lhs = argsOp->getLHS();
            std::unique_ptr<ExprAST> &rhs = argsOp->getRHS();
            if(lhs->type() == "Number")
            {
                NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(lhs.get());
                double number = numberExpr->getNumber();
                if(number == 1)
                {
                    argsNew.push_back(std::move(rhs->Clone()));
                    return std::make_unique<CallExprAST>("log1p", std::move(argsNew));
                }
            }
            else if(rhs->type() == "Number")
            {
                NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(rhs.get());
                double number = numberExpr->getNumber();
                if(number == 1)
                {
                    argsNew.push_back(std::move(lhs->Clone()));
                    return std::make_unique<CallExprAST>("log1p", std::move(argsNew));
                }
            }
        }
    }
    return expr->Clone();
}

// sqrt(expr1*expr1+expr2*expr2) ==> hypot(expr1, expr2)
std::unique_ptr<ExprAST> sqrtTohypot(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "sqrtTohypot's input is empty!\n");
        return nullptr;
    }

    if(expr->type() != "Call")
    {
        return expr->Clone();
    }
    
    CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
    std::string callee = (callExpr->getCallee());
    std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();
    if(callee == "sqrt" && args.size() == 1)
    {
        auto &arg = args.at(0); // get the parameter of the sqrt function
        if(arg->type() == "Binary")
        {
            BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(arg.get());
            char op = binOp->getOp();
            std::unique_ptr<ExprAST> &lhs = binOp->getLHS();  // left expression
            std::unique_ptr<ExprAST> &rhs = binOp->getRHS();  // right expression

            // judge if the parameter is like x*x+y*y style
            if((op == '+') && (lhs->type() == "Binary") && (rhs->type() == "Binary"))
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();  // left expression's operator
                char opR = binOpR->getOp();  // right expression's operator
                if((opL == '*') && (opR == '*'))
                {
                    std::unique_ptr<ExprAST> &lhsL = binOpL->getLHS();  // the left  expression of lhs
                    std::unique_ptr<ExprAST> &rhsL = binOpL->getRHS();  // the right expression of lhs
                    std::unique_ptr<ExprAST> &lhsR = binOpR->getLHS();  // the left  expression of rhs
                    std::unique_ptr<ExprAST> &rhsR = binOpR->getRHS();  // the right expression of rhs
                    if(isEqual(lhsL, rhsL) && isEqual(lhsR, rhsR))
                    {
                        std::vector<std::unique_ptr<ExprAST>> argsNew;  // store the parameters for hypot
                        argsNew.push_back(std::move(lhsL->Clone()));
                        argsNew.push_back(std::move(lhsR->Clone()));

                        return std::make_unique<CallExprAST>("hypot", std::move(argsNew));
                    }
                }
            }
        }
    }

    return expr->Clone();
}

// log(exp(x))⇒x OR exp(log(x))⇒x
std::unique_ptr<ExprAST> lex_x_Or_elx_x(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
	if(exprType == "Call")
	{
        CallExprAST *expr01 = dynamic_cast<CallExprAST *>(expr.get());
		
        std::string callee01 = (expr01->getCallee());
        std::vector<std::unique_ptr<ExprAST>> &args01 = expr01->getArgs();
		// log(exp(x))⇒x
        if(callee01 == "log" && args01.size() == 1)    //log(***)
        {    
			std::unique_ptr<ExprAST> &expr02 = args01.at(0);
            if(expr02->type() == "Call")
            {
				std::string callee02 = dynamic_cast<CallExprAST *>(expr02.get())->getCallee();
			    std::vector<std::unique_ptr<ExprAST>> &args02 = dynamic_cast<CallExprAST *>(expr02.get())->getArgs();
				if(callee02 == "exp" && args02.size() == 1)    //log(exp(***))
				{
					return (args02.at(0))->Clone();
                }
            }
        }
		//exp(log(x))⇒x
		if(callee01 == "exp" && args01.size() == 1)    //exp(***)
        {    
			std::unique_ptr<ExprAST> &expr02 = args01.at(0);
            if(expr02->type() == "Call")
            {
				std::string callee02 = dynamic_cast<CallExprAST *>(expr02.get())->getCallee();
			    std::vector<std::unique_ptr<ExprAST>> &args02 = dynamic_cast<CallExprAST *>(expr02.get())->getArgs();
				if(callee02 == "log" && args02.size() == 1)    //exp(log(***))
				{
					return (args02.at(0))->Clone();
                }
            }
        }
    }
	return expr->Clone();
}