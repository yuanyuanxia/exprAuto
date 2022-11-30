#include "basic.hpp"
#include "expandAST.hpp"
#include "mathfuncRewrite.hpp"
#include "exprAuto.hpp"

#include <algorithm>
#include <queue>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

#define THRESHOLD 2

//===----------------------------------------------------------------------===//
// Equivalent transformation of mathematical function
//===----------------------------------------------------------------------===//

// exp(expr) - 1 ==> expm1(expr)
ast_ptr expToexpm1(const ast_ptr &expr)
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
    string opStr(1, op);

    ast_ptr &lhs = binOp->getLHS();
    ast_ptr &rhs = binOp->getRHS();
    if((op == '+') && (lhs->type() == "Call") && (rhs->type() == "Number"))  // LHS = "Call" && RHS ='Number'
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(lhs.get());
        string callee = (callExpr->getCallee());
        vector<ast_ptr> &args = callExpr->getArgs();
        vector<ast_ptr> argsNew;

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
            string calleeNew = "expm1";
            ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
            return exprFinal;
        }
    }

    return expr->Clone();
}

// log(expr+1) or log(1+expr) ==> log1p(expr)
ast_ptr logTolog1p(const ast_ptr &expr)
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
    string callee = (callExpr->getCallee());
    vector<ast_ptr> &args = callExpr->getArgs();

    if(callee == "log" && args.size() == 1 && args.at(0)->type() == "Binary")
    {
        BinaryExprAST *argsOp = dynamic_cast<BinaryExprAST *>(args.at(0).get());
        char op = argsOp->getOp();

        if(op == '+')
        {
            vector<ast_ptr> argsNew;
            ast_ptr &lhs = argsOp->getLHS();
            ast_ptr &rhs = argsOp->getRHS();
            if(lhs->type() == "Number")
            {
                NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(lhs.get());
                double number = numberExpr->getNumber();
                if(number == 1)
                {
                    argsNew.push_back(std::move(rhs->Clone()));
                    return makePtr<CallExprAST>("log1p", std::move(argsNew));
                }
            }
            else if(rhs->type() == "Number")
            {
                NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(rhs.get());
                double number = numberExpr->getNumber();
                if(number == 1)
                {
                    argsNew.push_back(std::move(lhs->Clone()));
                    return makePtr<CallExprAST>("log1p", std::move(argsNew));
                }
            }
        }
    }
    return expr->Clone();
}

// sqrt(expr1*expr1+expr2*expr2) ==> hypot(expr1, expr2)
ast_ptr sqrtTohypot(const ast_ptr &expr)
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
    string callee = (callExpr->getCallee());
    vector<ast_ptr> &args = callExpr->getArgs();
    if(callee == "sqrt" && args.size() == 1)
    {
        auto &arg = args.at(0); // get the parameter of the sqrt function
        if(arg->type() == "Binary")
        {
            BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(arg.get());
            char op = binOp->getOp();
            ast_ptr &lhs = binOp->getLHS();  // left expression
            ast_ptr &rhs = binOp->getRHS();  // right expression

            // judge if the parameter is like x*x+y*y style
            if((op == '+') && (lhs->type() == "Binary") && (rhs->type() == "Binary"))
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();  // left expression's operator
                char opR = binOpR->getOp();  // right expression's operator
                if((opL == '*') && (opR == '*'))
                {
                    ast_ptr &lhsL = binOpL->getLHS();  // the left  expression of lhs
                    ast_ptr &rhsL = binOpL->getRHS();  // the right expression of lhs
                    ast_ptr &lhsR = binOpR->getLHS();  // the left  expression of rhs
                    ast_ptr &rhsR = binOpR->getRHS();  // the right expression of rhs
                    if(isEqual(lhsL, rhsL) && isEqual(lhsR, rhsR))
                    {
                        vector<ast_ptr> argsNew;  // store the parameters for hypot
                        argsNew.push_back(std::move(lhsL->Clone()));
                        argsNew.push_back(std::move(lhsR->Clone()));

                        return makePtr<CallExprAST>("hypot", std::move(argsNew));
                    }
                }
            }
        }
    }

    return expr->Clone();
}

// log(exp(x))⇒x OR exp(log(x))⇒x
ast_ptr lex_x_Or_elx_x(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
    }
    const string exprType = expr->type();
	if(exprType == "Call")
	{
        CallExprAST *expr01 = dynamic_cast<CallExprAST *>(expr.get());
		
        string callee01 = (expr01->getCallee());
        vector<ast_ptr> &args01 = expr01->getArgs();
		// log(exp(x))⇒x
        if(callee01 == "log" && args01.size() == 1)    //log(***)
        {    
			ast_ptr &expr02 = args01.at(0);
            if(expr02->type() == "Call")
            {
				string callee02 = dynamic_cast<CallExprAST *>(expr02.get())->getCallee();
			    vector<ast_ptr> &args02 = dynamic_cast<CallExprAST *>(expr02.get())->getArgs();
				if(callee02 == "exp" && args02.size() == 1)    //log(exp(***))
				{
					return (args02.at(0))->Clone();
                }
            }
        }
		//exp(log(x))⇒x
		if(callee01 == "exp" && args01.size() == 1)    //exp(***)
        {    
			ast_ptr &expr02 = args01.at(0);
            if(expr02->type() == "Call")
            {
				string callee02 = dynamic_cast<CallExprAST *>(expr02.get())->getCallee();
			    vector<ast_ptr> &args02 = dynamic_cast<CallExprAST *>(expr02.get())->getArgs();
				if(callee02 == "log" && args02.size() == 1)    //exp(log(***))
				{
					return (args02.at(0))->Clone();
                }
            }
        }
    }
	return expr->Clone();
}

// sqrt(x)*sqrt(y) ==> sqrt(x*y)
ast_ptr sqrtMult(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return nullptr;
    }
    if(expr->type() == "Binary")
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
            if((exprTypeLHS == "Call") && (exprTypeRHS == "Call"))
            {
                CallExprAST *callExprL = dynamic_cast<CallExprAST *>(lhs.get());
                CallExprAST *callExprR = dynamic_cast<CallExprAST *>(rhs.get());

                string calleeL = (callExprL->getCallee());
                string calleeR = (callExprR->getCallee());

                vector<ast_ptr> &argsL = callExprL->getArgs();  //左表达式中函数的参数
                vector<ast_ptr> &argsR = callExprR->getArgs();  //右表达式中函数的参数

                vector<ast_ptr> argsNew;
#ifdef DEBUG
                fprintf(stderr, "call: %s\n", calleeL.c_str());
#endif
                if((calleeL == "sqrt") && (calleeR == "sqrt"))
                {
                    if((argsL.size() == 1) && (argsR.size() == 1))
                    {//取出左右表达式中的参数
                        auto argL = std::move(argsL.at(0));
                        auto argR = std::move(argsR.at(0));
                        //存放转换后的表达式x*y
                        auto argsFinal =mulExpr(argL, argR);
                        argsNew.push_back(std::move(argsFinal));
                    }
                    string calleeNew = "sqrt";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    return exprFinal;
                }
                else
                return expr->Clone();
            }
            else
            return expr->Clone();
        }
        else
        return expr->Clone();
    }
    return expr->Clone();
}

// sqrt(x)/sqrt(y) ==> sqrt(x/y)
ast_ptr sqrtDiv(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return nullptr;
    }
    if(expr->type() == "Binary")
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

        if(op == '/')
        {
            if((exprTypeLHS == "Call") && (exprTypeRHS == "Call"))
            {
                CallExprAST *callExprL = dynamic_cast<CallExprAST *>(lhs.get());
                CallExprAST *callExprR = dynamic_cast<CallExprAST *>(rhs.get());

                string calleeL = (callExprL->getCallee());
                string calleeR = (callExprR->getCallee());

                vector<ast_ptr> &argsL = callExprL->getArgs();  //左表达式中函数的参数
                vector<ast_ptr> &argsR = callExprR->getArgs();  //右表达式中函数的参数

                vector<ast_ptr> argsNew;
#ifdef DEBUG
                fprintf(stderr, "call: %s\n", calleeL.c_str());
#endif
                if((calleeL == "sqrt") && (calleeR == "sqrt"))
                {
                    if((argsL.size() == 1) && (argsR.size() == 1))
                    {//取出左右表达式中的参数
                        auto argL = std::move(argsL.at(0));
                        auto argR = std::move(argsR.at(0));
                        //存放转换后的表达式x/y
                        auto argsFinal =divExpr(argL, argR);
                        argsNew.push_back(std::move(argsFinal));
                    }
                    string calleeNew = "sqrt";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    return exprFinal;
                }
                else
                return expr->Clone();
            }
            else
            return expr->Clone();
        }
        else
        return expr->Clone();
    }
    return expr->Clone();
}

bool isSpecialNumber(const ast_ptr &expr)
{
    auto type = expr->type();
    if(type != "Number")
        return false;
    NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
    double number = numberPtr->getNumber();

    // deal with exception. TODO: complete the exception handling
    if(number == 0)
    {
        fprintf(stderr, "ERROR: isSpecialNumber: the input expr is 0!\n");
        exit(EXIT_FAILURE);
    }
    // 1/2, 1/4, 1/8, 1, 2, 4, 8, ... are special numbers.
    unsigned long int numberInt;
    numberInt = *(unsigned long int *)(&number);
    const unsigned long int mantissaMask = 0x000fffffffffffffUL;
    unsigned long int tmp = numberInt & mantissaMask;
    if(tmp == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

    // old way
    // int numberInt = number;
    // // judge if integer
    // if(number != (double)numberInt) return false;
    // // get the absolute value of numberInt
    // numberInt = abs(numberInt);
    // // judge if 1, 2, 4, 8, ...
    // if((numberInt & (numberInt - 1)) == 0) return true; // NOTE: if number is 0, then the result is also true!!!
    // else return false;
}

// x*y+a, y*x+a, a+x*y, a+y*x => fma(x,y,a), x*y+a*b, y*x+a*b, x*y+b*a, y*x+a*b,  => fma(x,y,a*b) or fma(a,b,x*y)
// not consider 2, 4, 8 and so on...
// NOTE: if no match, the origin input expr will also be returned.
size_t countMul(const ast_ptr &expr)
{
    if (expr->type() != "Binary")
    {
        return 0;
    }
    BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOp->getOp();
    if(op == '*')
    {
        ast_ptr &lhs = binOp->getLHS();
        ast_ptr &rhs = binOp->getRHS();
        size_t numOfMul = 1 + countMul(lhs) + countMul(rhs); // plus 1 is because that the op is '*'
        return numOfMul;
    }
    else
    {
        return 0;
    }
}

bool hasTooManyMul(const ast_ptr &expr, size_t threshold)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "hasTooManyMul's input is nullptr!\n");
        exit(EXIT_FAILURE);
    }
    // return false;
    if (expr->type() != "Binary")
    {
        return false;
    }

    BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOp->getOp();
    ast_ptr &lhs = binOp->getLHS();
    ast_ptr &rhs = binOp->getRHS();
    
    if (op != '*')
    {
        // fprintf(stderr, "hasTooManyMul's op is not *, but is %c!\n", op);
        // printExpr(expr, "HHJJWW: ");
        // exit(EXIT_FAILURE);
        return false;
    }
    size_t numOfMul = 1 + countMul(lhs) + countMul(rhs); // plus 1 is because that the op is '*'
    if (numOfMul < threshold)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool worthToDoFmaRewrite(const ast_ptr &e0, const ast_ptr &e1, const ast_ptr &e2)
{
    size_t threshold = 4;
    if(hasTooManyMul(e0, threshold) || hasTooManyMul(e1, threshold))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool worthToDoFmaRewriteWrapper(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "worthToDoFmaRewriteWrapper's input is nullptr!\n");
        exit(EXIT_FAILURE);
    }
    auto type = expr->type();
    if(type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto callee = callPtr->getCallee();
        if(callee == "fma")
        {
            const auto &e0 = callPtr->getArgs().at(0);
            const auto &e1 = callPtr->getArgs().at(1);
            const auto &e2 = callPtr->getArgs().at(2);
            if(!worthToDoFmaRewrite(e0, e1, e2))
            {
                return false;
            }
            if(worthToDoFmaRewriteWrapper(e0) && worthToDoFmaRewriteWrapper(e1) && worthToDoFmaRewriteWrapper(e2))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        for(const auto &arg : callPtr->getArgs())
        {
            if(!worthToDoFmaRewriteWrapper(arg))
            {
                return false;
            }
        }
        return true;
    }

    if (expr->type() != "Binary")
    {
        return true;
    }

    BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
    ast_ptr &lhs = binOp->getLHS();
    ast_ptr &rhs = binOp->getRHS();

    if(worthToDoFmaRewriteWrapper(lhs) && worthToDoFmaRewriteWrapper(rhs))
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<ast_ptr> checkFma(vector<ast_ptr> &exprs)
{
    vector<ast_ptr> results;
    for(auto &expr : exprs)
    {
        if(worthToDoFmaRewriteWrapper(expr))
        {
            results.push_back(std::move(expr));
        }
    }
    return results;
}

vector<ast_ptr> toFma(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "toFma's input is nullptr!\n");
        exit(EXIT_FAILURE);
    }

    if (expr->type() != "Binary")
    {
        fprintf(stderr, "toFma's input type is not Binary!\n");
        exit(EXIT_FAILURE);
    }

    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "toFma: ";
    // cout << prompt << "start-------- " << PrintExpression(expr) << " " << endl;

    vector<ast_ptr> results;
    // results.push_back(std::move(expr->Clone()));  // the key to control the rewrite results
    ast_ptr newExpr = expr->Clone();

    BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(newExpr.get());
    char op = binOp->getOp();
    ast_ptr &lhs = binOp->getLHS();
    ast_ptr &rhs = binOp->getRHS();

    if (op == '+')
    {
        // ast_ptr tmpNegOne = makePtr<NumberExprAST>(-1.0);
        auto lhsType = lhs->type();
        auto rhsType = rhs->type();
        if (lhsType == "Binary" && rhsType != "Binary")
        {
            BinaryExprAST *binOpLhs = dynamic_cast<BinaryExprAST *>(lhs.get());
            char opL = binOpLhs->getOp();
            if (opL == '*')
            {
                ast_ptr &lhsL = binOpLhs->getLHS();
                ast_ptr &rhsL = binOpLhs->getRHS();
                if(!isSpecialNumber(lhsL) && !isSpecialNumber(rhsL))
                // if(!isEqual(lhsL, tmpNegOne) && !isEqual(rhsL, tmpNegOne))
                {
                    vector<ast_ptr> argsNew;
                    argsNew.push_back(std::move(lhsL));
                    argsNew.push_back(std::move(rhsL));
                    argsNew.push_back(std::move(rhs));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));
                } 
            }
        }
        else if (lhsType != "Binary" && rhsType == "Binary")
        {
            BinaryExprAST *binOpRhs = dynamic_cast<BinaryExprAST *>(rhs.get());
            char opR = binOpRhs->getOp();
            if (opR == '*')
            {
                ast_ptr &lhsR = binOpRhs->getLHS();
                ast_ptr &rhsR = binOpRhs->getRHS();

                if(!isSpecialNumber(lhsR) && !isSpecialNumber(rhsR))
                // if(!isEqual(lhsR, tmpNegOne) && !isEqual(rhsR, tmpNegOne))
                {
                    vector<ast_ptr> argsNew;
                    argsNew.push_back(std::move(lhsR));
                    argsNew.push_back(std::move(rhsR));
                    argsNew.push_back(std::move(lhs));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));
                }
            }
        }
        else if (lhsType == "Binary" && rhsType == "Binary")
        {
            BinaryExprAST *binOpLhs = dynamic_cast<BinaryExprAST *>(lhs.get());
            char opL = binOpLhs->getOp();
            BinaryExprAST *binOpRhs = dynamic_cast<BinaryExprAST *>(rhs.get());
            char opR = binOpRhs->getOp();
            if (opL == '*' && opR != '*')
            {
                ast_ptr &lhsL = binOpLhs->getLHS();
                ast_ptr &rhsL = binOpLhs->getRHS();

                if(!isSpecialNumber(lhsL) && !isSpecialNumber(rhsL))
                // if(!isEqual(lhsL, tmpNegOne) && !isEqual(rhsL, tmpNegOne))
                {
                    vector<ast_ptr> argsNew;
                    argsNew.push_back(std::move(lhsL));
                    argsNew.push_back(std::move(rhsL));
                    argsNew.push_back(std::move(rhs));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));
                }
            }
            else if (opL != '*' && opR == '*')
            {
                ast_ptr &lhsR = binOpRhs->getLHS();
                ast_ptr &rhsR = binOpRhs->getRHS();

                if(!isSpecialNumber(lhsR) && !isSpecialNumber(rhsR))
                // if(!isEqual(lhsR, tmpNegOne) && !isEqual(rhsR, tmpNegOne))
                {
                    vector<ast_ptr> argsNew;
                    argsNew.push_back(std::move(lhsR));
                    argsNew.push_back(std::move(rhsR));
                    argsNew.push_back(std::move(lhs));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));
                }
            }
            else if (opL == '*' && opR == '*')
            {
                ast_ptr &lhsL = binOpLhs->getLHS();
                ast_ptr &rhsL = binOpLhs->getRHS();
                ast_ptr &lhsR = binOpRhs->getLHS();
                ast_ptr &rhsR = binOpRhs->getRHS();

                vector<ast_ptr> argsNew;
                if(!isSpecialNumber(lhsL) && !isSpecialNumber(rhsL))
                // if(!isEqual(lhsL, tmpNegOne) && !isEqual(rhsL, tmpNegOne))
                {
                    argsNew.push_back(std::move(lhsL->Clone()));
                    argsNew.push_back(std::move(rhsL->Clone()));
                    argsNew.push_back(std::move(rhs->Clone()));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));

                    argsNew.clear();
                }

                if(!isSpecialNumber(lhsR) && !isSpecialNumber(rhsR))
                // if(!isEqual(lhsR, tmpNegOne) && !isEqual(rhsR, tmpNegOne))
                {
                    argsNew.push_back(std::move(lhsR));
                    argsNew.push_back(std::move(rhsR));
                    argsNew.push_back(std::move(lhs));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));
                }
            }
        }
    }
    
    // the key to control the rewrite results
    if(results.size() == 0) {
        results.push_back(std::move(expr->Clone()));
    }
    // printExprs(results, prompt + "results: ");
    // cout << prompt << "end--------" << endl;
    callCount--;
    callLevel--;
    return results;
}

vector<ast_ptr> fmaRewriteKernel(vector<ast_ptr> &exprs, const char &op)
{
    if (exprs.size() == 0)
    {
        fprintf(stderr, "ERROR: fmaRewriteKernel: the input exprs is empty\n");
        exit(EXIT_FAILURE);
    }
    if (op != '+' && op != '-' && op != '*' && op != '/')
    {
        fprintf(stderr, "ERROR: fmaRewriteKernel: the input op %c is illegal\n", op);
        exit(EXIT_FAILURE);
    }
    vector<ast_ptr> results;
    for (const auto &expr : exprs)
    {
        auto tmp = toFma(expr);
        mineAppend(results, tmp);
    }
    return results;
}

vector<ast_ptr> fmaRewrite(const ast_ptr &expr)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "fmaRewrite: ";
    // if (callCount == 1) cout << prompt << "start--------" << endl;

    if (expr == nullptr)
    {
        cerr << prompt << "ERROR: expr is nullptr" << endl;
        exit(EXIT_FAILURE);
    }
    // printExpr(expr, "\tfmaRewrite: at the beginning: ");

    if (expr->type() != "Binary")  // May be call, variable or number
    {
        vector<ast_ptr> exprsFinal;
        exprsFinal.push_back(std::move(expr->Clone()));
        if (callCount == 1) printExprs(exprsFinal, prompt + "exprsFinal: ");
        if (callCount == 1) cout << prompt << "end--------" << endl;
        callCount--;
        callLevel--;
        return exprsFinal;
    }

    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    auto &lhs = binOpPtr->getLHS();
    auto &rhs = binOpPtr->getRHS();
    auto lhsNewASTs = fmaRewrite(lhs);
    auto rhsNewASTs = fmaRewrite(rhs);
    auto newASTs = createAllBinary(lhsNewASTs, rhsNewASTs, op);

    auto exprsFinal = fmaRewriteKernel(newASTs, op);
    
    deleteTheSame(exprsFinal);

    // if (callCount == 1) printExprs(exprsFinal, prompt + "exprsFinal: ");
    // if (callCount == 1) cout << prompt << "end--------" << endl;
    callCount--;
    callLevel--;
    return exprsFinal;
}

bool countVariablesFromExpr(const ast_ptr &expr, vector<string> &vars)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "ERROR: countVariablesFromExpr: the input is nullptr\n");
        exit(EXIT_FAILURE);
    }
    auto type = expr->type();
    if (type == "Number")
    {
        return true;
    }
    else if (type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        vars.push_back(var);
        return true;
    }
    else if (type == "Call")
    {
        return true;
        // CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        // auto callee = callPtr->getCallee();
        // for(const auto &arg : callPtr->getArgs())
        // {
        //     if(!worthToDoFmaRewriteWrapper(arg))
        //     {
        //         return false;
        //     }
        // }
    }
    if (type != "Binary")
    {
        fprintf(stderr, "ERROR: countVariablesFromExpr: type %s is wrong\n", type.c_str());
        exit(EXIT_FAILURE);
    }
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto op = binaryPtr->getOp();
    if(op != '*')
    {
        return false;
    }
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    countVariablesFromExpr(lhs, vars);
    countVariablesFromExpr(rhs, vars);
    std::sort(vars.begin(), vars.end()); // sort to unique the parameters queue
    return true;
}


vector<size_t> getVariablesCount(const ast_ptr &expr, vector<string> &vars)
{
    vector<string> varsInit;
    // printExpr(expr, "getVariablesCount: before countVariablesFromExpr: ");
    countVariablesFromExpr(expr, varsInit);
    // cout << "getVariablesCount: varsInit.size() = " << varsInit.size() << endl;
    vector<size_t> countVars;
    if(varsInit.size() == 0)
    {
        return countVars;
    }
    string key = varsInit.at(0);
    vars.push_back(key);
    size_t count = 0;
    for(size_t i = 0; i < varsInit.size(); i++)
    {
        if(key == varsInit.at(i))
        {
            count++;
        }
        else
        {
            countVars.push_back(count);
            key = varsInit.at(i);
            vars.push_back(key);
            count = 1;
        }
    }
    countVars.push_back(count);

    // for(size_t i = 0; i < vars.size(); i++)
    // {
    //     cout << "getVariablesCount: " << vars.at(i) << " " << countVars.at(i) << endl;
    // }

    return countVars;
}

ast_ptr dropTargetVar(const ast_ptr &expr, const string &target)
{
    if (expr == nullptr)
    {
        return nullptr; // the multiple result of both target variables
    }

    auto type = expr->type();
    if (type == "Number")
    {
        return expr->Clone();
    }
    else if (type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        if(var == target)
        {
            return nullptr; // drop the target variable
        }
        else
        {
            return expr->Clone();
        }
    }
    else if (type == "Call")
    {
        return expr->Clone();
        // CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        // auto &args = callPtr->getArgs();
        // vector<ast_ptr> argsNew;
        // for (auto &arg : args)
        // {
        //     auto argNew = ToPow(arg);
        //     argsNew.push_back(std::move(argNew));
        // }
        // return makePtr<CallExprAST>(callPtr->getCallee(), std::move(argsNew));
    }
    if (expr->type() != "Binary")
    {
        fprintf(stderr, "dropTargetVar: input type is not Binary!\n");
        exit(EXIT_FAILURE);
    }
    
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto op = binaryPtr->getOp();
    if(op != '*')
    {
        return expr->Clone();
    }
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    auto tmp1 = dropTargetVar(lhs, target);
    auto tmp2 = dropTargetVar(rhs, target);
    if (tmp1 == nullptr)
    {
        return tmp2;
    }
    else if (tmp2 == nullptr)
    {
        return tmp1;
    }
    else
    {
        return mulExpr(tmp1, tmp2); // if both tmp1 and tmp2 were nullptr, then a warning would be show and nullptr would be returned.
    }
}


ast_ptr ToPowKernel(const ast_ptr &expr)
{
    printExpr(expr, "ToPowKernel: 1106 expr = ");
    vector<string> vars;
    auto countVars = getVariablesCount(expr, vars);
    
    size_t threshold = THRESHOLD; // if the number of '*' >= 4, then convert '*' to pow()
    auto exprNew = expr->Clone();
    for(size_t i = 0; i < vars.size(); i++)
    {
        if(countVars.at(i) > threshold)
        {
            vector<ast_ptr> args;
            args.push_back(makePtr<VariableExprAST>(vars.at(i)));
            args.push_back(makePtr<NumberExprAST>(countVars.at(i)));
            ast_ptr tmp1 = makePtr<CallExprAST>("pow", std::move(args));
            auto tmp = dropTargetVar(exprNew, vars.at(i));
            exprNew = mulExpr(tmp, tmp1);
        }
    }
    return exprNew;
}

ast_ptr updateForPowRewrite(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "ERROR: updateForPowRewrite: the input is nullptr\n");
        exit(EXIT_FAILURE);
    }

    auto type = expr->type();
    if (type == "Number")
    {
        return expr->Clone();
    }
    else if (type == "Variable")
    {
        return expr->Clone();
    }
    else if (type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto callee = callPtr->getCallee();
        if(callee == "pow")
        {
            return expr->Clone();
        }
        printExpr(expr, "updateForPowRewrite: happen to Call: ");
        auto &args = callPtr->getArgs();
        vector<ast_ptr> argsNew;
        for (auto &arg : args)
        {
            auto argNew = toPow(arg);
            argsNew.push_back(std::move(argNew));
        }
        ast_ptr result = makePtr<CallExprAST>(callPtr->getCallee(), std::move(argsNew));
        printExpr(result, "updateForPowRewrite: result = ");
        return result;
    }
    if (expr->type() != "Binary")
    {
        fprintf(stderr, "updateForPowRewrite: input type is not Binary!\n");
        exit(EXIT_FAILURE);
    }
    
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto op = binaryPtr->getOp();
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    if (op == '+' || op == '-' || op == '/')
    {
        auto lhsNew = toPow(lhs);
        auto rhsNew = toPow(rhs);
        return createBinaryExpr(lhsNew, rhsNew, op);
    }
    else if (op == '*')
    {
        auto lhsNew = updateForPowRewrite(lhs);
        auto rhsNew = updateForPowRewrite(rhs);
        return createBinaryExpr(lhsNew, rhsNew, op);
    }
    fprintf(stderr, "updateForPowRewrite: op is not [* / + -], but is %c!\n", op);
    printExpr(expr, "updateForPowRewrite: the wrong expr: ");
    exit(EXIT_FAILURE);
}

ast_ptr toPow(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "ERROR: toPow: the input is nullptr\n");
        exit(EXIT_FAILURE);
    }

    auto type = expr->type();
    if (type == "Number")
    {
        return expr->Clone();
    }
    else if (type == "Variable")
    {
        return expr->Clone();
    }
    else if (type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        auto callee = callPtr->getCallee();
        if(callee != "fma")
        {
            return expr->Clone();
        }
        else
        {
            vector<ast_ptr> argsNew;
            for (auto &arg : args)
            {
                auto argNew = toPow(arg);
                argsNew.push_back(std::move(argNew));
            }
            return makePtr<CallExprAST>(callee, std::move(argsNew));
        }
    }
    if (expr->type() != "Binary")
    {
        fprintf(stderr, "toPow: input type is not Binary!\n");
        exit(EXIT_FAILURE);
    }
    
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto op = binaryPtr->getOp();
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    if (op == '+' || op == '-' || op == '/')
    {
        auto lhsNew = toPow(lhs);
        auto rhsNew = toPow(rhs);
        return createBinaryExpr(lhsNew, rhsNew, op);
    }
    else if (op == '*')
    {
        vector<string> vars;
        auto countVars = getVariablesCount(expr, vars);
        if(countVars.size() == 0)
        {
            return expr->Clone();
        }
        // cout << "toPow: vars.size() = " << vars.size() << endl;
        // for(size_t i = 0; i < vars.size(); i++)
        // {
        //     cout << "toPow: " << vars.at(i) << " " << countVars.at(i) << endl;
        // }
        size_t threshold = THRESHOLD; // if the number of '*' >= threshold, then convert '*' to pow()
        auto exprNew = expr->Clone();
        for(size_t i = 0; i < vars.size(); i++)
        {
            if(countVars.at(i) > threshold)
            {
                vector<ast_ptr> args;
                args.push_back(makePtr<VariableExprAST>(vars.at(i)));
                args.push_back(makePtr<NumberExprAST>(countVars.at(i)));
                ast_ptr tmp1 = makePtr<CallExprAST>("pow", std::move(args));
                auto tmp = dropTargetVar(exprNew, vars.at(i));
                exprNew = mulExpr(tmp, tmp1);
            }
        }
        exprNew = updateForPowRewrite(exprNew);
        return exprNew;
    }
    
    fprintf(stderr, "toPow: op is not [* / + -], but is %c!\n", op);
    printExpr(expr, "toPow: the wrong expr: ");
    exit(EXIT_FAILURE);
    // vector<ast_ptr> items = extractItems(expr);
    // ast_ptr result;
    // for (auto &item : items)
    // {
    //     printExpr(item, "HHJJWW: 1106 ");
    //     auto tmp = ToPowKernel(item);
    //     result = addExpr(result, tmp);
    // }
    // return result;
}