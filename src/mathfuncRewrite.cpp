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

// x*y+a, y*x+a, a+x*y, a+y*x => fma(x,y,a), x*y+a*b, y*x+a*b, x*y+b*a, y*x+a*b,  => fma(x,y,a*b) or fma(a,b,x*y)
// NOTE: if no match, the origin input expr will also be returned.
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
        ast_ptr tmpNegOne = makePtr<NumberExprAST>(-1.0);
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

                if(!isEqual(lhsL, tmpNegOne) && !isEqual(rhsL, tmpNegOne))
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

                if(!isEqual(lhsR, tmpNegOne) && !isEqual(rhsR, tmpNegOne))
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

                if(!isEqual(lhsL, tmpNegOne) && !isEqual(rhsL, tmpNegOne))
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

                if(!isEqual(lhsR, tmpNegOne) && !isEqual(rhsR, tmpNegOne))
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
                if(!isEqual(lhsL, tmpNegOne) && !isEqual(rhsL, tmpNegOne))
                {
                    argsNew.push_back(std::move(lhsL->Clone()));
                    argsNew.push_back(std::move(rhsL->Clone()));
                    argsNew.push_back(std::move(rhs->Clone()));
                    string calleeNew = "fma";
                    ast_ptr exprFinal = makePtr<CallExprAST>(calleeNew, std::move(argsNew));
                    results.push_back(std::move(exprFinal));

                    argsNew.clear();
                }

                if(!isEqual(lhsR, tmpNegOne) && !isEqual(rhsR, tmpNegOne))
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
    if (callCount == 1) cout << prompt << "start--------" << endl;

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

    if (callCount == 1) printExprs(exprsFinal, prompt + "exprsFinal: ");
    if (callCount == 1) cout << prompt << "end--------" << endl;
    callCount--;
    callLevel--;
    return exprsFinal;
}