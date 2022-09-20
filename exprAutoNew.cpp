#include <iostream>
#include <string>
#include <vector>
#include "basic.hpp"
#include "preprocess.hpp"
// #include "exprAuto.hpp"
#include "exprAutoNew.hpp"
#include "mathfuncRewrite.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"

// similiar to getExprFromVariants
ast_ptr getCallFromVariants(const std::vector<ast_ptr> &variants, const std::vector<size_t> orders, const std::string callee)
{
    std::vector<ast_ptr> args;
    for(const auto &order : orders)
    {
        args.push_back(variants.at(order)->Clone());
    }
    return makePtr<CallExprAST>(callee, std::move(args));
}

// similiar to combineMonomial
std::vector<ast_ptr> combineCallArgs(const std::vector<ast_ptr> &variants, const std::vector<size_t> widths, const std::string callee)
{
    std::vector<ast_ptr> results;
    size_t numOfCombinations = std::accumulate(std::begin(widths), std::end(widths), 1, std::multiplies<>());

    for(size_t i = 0; i < numOfCombinations; i++)
    {
        std::vector<size_t> orders = getCombineOrders(widths, i);
        ast_ptr tmp = getCallFromVariants(variants, orders, callee);
        results.push_back(std::move(tmp));
    }
    return results;
}

ast_ptr dealWithCallsKernel(const ast_ptr &expr, const std::string callee)
{
    ast_ptr result;
    if(callee == "exp")
    {
        // std::cout << "dealWithCallsKernel: That is " << callee << std::endl;
        result = lex_x_Or_elx_x(expr);
    }
    else if(callee == "log")
    {   
        // std::cout << "dealWithCallsKernel: That is " << callee << std::endl;
        result = lex_x_Or_elx_x(expr);
        result = logTolog1p(result);
    }
    else if(callee == "sqrt")
    {
        // std::cout << "dealWithCallsKernel: That is " << callee << std::endl;
        result = sqrtTohypot(expr);
    }
    else
    {
        // std::cout << "dealWithCallsKernel: default: That is " << callee << std::endl;
        result = expr->Clone();
    }
    return result;
}

ast_ptr dealWithCallsKernel(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: dealWithCallsKernel: the input expression is nullptr!\n");
        return nullptr;
    }
    if(expr->type() != "Call") // May be variable or number or Binary
    {
        fprintf(stderr, "WARNING: dealWithCallsKernel: the input expression is not Call!\n");
        return expr->Clone();
    }
    CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
    const std::string callee = callPtr->getCallee();
    return dealWithCallsKernel(expr, callee);
}

// similiar to createAll, but high level than createAll
std::vector<std::unique_ptr<ExprAST>> createAllBinary(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators, const char op)
{
    std::unique_ptr<ExprAST> resultsTemp;
    std::vector<std::unique_ptr<ExprAST>> results;

    if(numerators.at(0) != nullptr && denominators.at(0) != nullptr)
    {
        // for(long unsigned int i = 0; i < numerators.size(); i++)
        for(const auto &numerator : numerators)
        {
            // for(long unsigned int j = 0; j < denominators.size(); j++)
            for(const auto &denominator : denominators)
            {
                resultsTemp = createBinaryExpr(numerator, denominator, op);
                results.push_back(std::move(resultsTemp));
            }
        }
        return results;
    }
    else
    {
        fprintf(stderr, "\tERROR: creatAll: the input include nullptr!\n");
        return {};
    }
}

// TODO: Do all the mathFunction rewrite to BinaryAST such as expToexpm1
std::unique_ptr<ExprAST> dealWithBinOpKernel(const std::unique_ptr<ExprAST> &expr, const char &op)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: dealWithBinOpKernel: the input expression is nullptr!\n");
        return nullptr;
    }
    std::unique_ptr<ExprAST> result;
    if(op == '+')
    {
        // std::cout << "dealWithBinOpKernel: That is " << op << std::endl;
        result = expToexpm1(expr);
    }
    else
    {
        // std::cout << "dealWithBinOpKernel: default: That is " << op << std::endl;
        result = expr->Clone();
    }
    return result;
}

std::unique_ptr<ExprAST> dealWithBinOpKernel(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: dealWithBinOpKernel: the input expression is nullptr!\n");
        return nullptr;
    }
    if(expr->type() != "Binary") // May be variable or number or Call
    {
        fprintf(stderr, "WARNING: dealWithBinOpKernel: the input expression is not Binary!\n");
        return expr->Clone();
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    const char op = binOpPtr->getOp();
    return dealWithBinOpKernel(expr, op);
}

std::vector<std::unique_ptr<ExprAST>> dealWithBinOp(std::vector<std::unique_ptr<ExprAST>> &exprs, const char &op)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    if(exprs.size() == 0)
    {
        fprintf(stderr, "ERROR: dealWithBinOp: the input exprs is empty\n");
        return {};
    }
    if(op != '+' && op != '-' && op != '*' && op != '/')
    {
        fprintf(stderr, "ERROR: dealWithBinOp: the input op is illegal\n");
        return {};
    }
    for (const auto &expr : exprs)
    {
        auto result = dealWithBinOpKernel(expr, op);
        results.push_back(std::move(result));
    }
    return results;
}

// TODO: delete the same expr in exprs using the function 'isEqual'
void deleteTheSame(std::vector<ast_ptr> &exprs)
{
    if(exprs.size() == 0)
    {
        fprintf(stderr, "WARNING: deleteTheSame: the input exprs is empty\n");
        exit(1);
    }
}

std::vector<ast_ptr> dealWithCalls(const ast_ptr &expr)
{
    CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
    std::string callee = callExpr->getCallee();
    std::vector<ast_ptr> &args = callExpr->getArgs();

    std::vector<ast_ptr> variants;
    std::vector<size_t> widths;
    for(const auto &arg : args)
    {
        auto exprs = exprAutoNew(arg);
        mineAppend(variants, exprs);
        widths.push_back(exprs.size());
    }

    printExprs(variants, "dealWithCalls: before middle, ");
    std::vector<ast_ptr> middles = combineCallArgs(variants, widths, callee);
    printExprs(middles, "dealWithCalls: after middle, ");

    std::vector<ast_ptr> allResults;
    for(const auto &middle : middles)
    {
        auto result = dealWithCallsKernel(middle, callee);
        allResults.push_back(std::move(result));
    }

    deleteTheSame(allResults);

    return allResults;
}

std::vector<ast_ptr> mathfuncRewrite(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return {};
    }
    ast_ptr newexpr = expr->Clone();
    std::vector<ast_ptr> exprsFinal;
    exprsFinal.push_back(std::move(newexpr->Clone()));

    if(newexpr->type() == "Call")
    {
        auto newASTs = dealWithCalls(newexpr);
        exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(newASTs.begin()), std::make_move_iterator(newASTs.end()));
        return exprsFinal;
    }

    if(expr->type() != "Binary")  // May be variable or number
    {
        return exprsFinal;
    }

    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    auto &lhs = binOpPtr->getLHS();
    auto &rhs = binOpPtr->getRHS();
    auto lhsNewASTs = mathfuncRewrite(lhs);
    auto rhsNewASTs = mathfuncRewrite(rhs);
    auto newASTs = createAllBinary(lhsNewASTs, rhsNewASTs, op);

    newASTs = dealWithBinOp(newASTs, op);
    exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(newASTs.begin()), std::make_move_iterator(newASTs.end()));
    deleteTheSame(exprsFinal);

    return exprsFinal;
}

// a kernel function within rewriteExpr
std::vector<size_t> getCombineOrders(const std::vector<size_t> widths, const size_t identifier)
{
    std::vector<size_t> orders;
    size_t order, start = 0;
    for(const auto &width : widths)
    {
        order = start + identifier % width;
        orders.push_back(order);
        order = identifier / width;
        start += width;
    }
    return orders;
}

std::vector<ast_ptr> polyRewrite(const ast_ptr &expr)
{
    ast_ptr middleNew = expandExprWrapper(expr);
    std::vector<ast_ptr> items = extractItems(middleNew);
    std::vector<monoInfo> info = extractInfo(items);
    std::vector<monoInfo> infoNew = mergePolynomial(info);
    return createExpr(infoNew);
}

std::vector<ast_ptr> tryRewrite(ast_ptr expr)
{
    fprintf(stderr, "\ttryRewrite: start--------\n");

    auto middles = mathfuncRewrite(expr);

    std::vector<ast_ptr> results;
    for(const auto &middle : middles)
    {
        auto tmp = polyRewrite(middle);
        mineAppend(results, tmp);
    }
    fprintf(stderr, "\ttryRewrite: end--------\n");

    return results;
}

std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators)
{
    std::unique_ptr<ExprAST> resultsTemp;
    std::vector<std::unique_ptr<ExprAST>> results;

    if(numerators.at(0) != nullptr && denominators.at(0) != nullptr)
    {
        for(long unsigned int i = 0; i < numerators.size(); i++)
        {
            for(long unsigned int j = 0; j < denominators.size(); j++)
            {
                resultsTemp = divExpr(numerators.at(i), denominators.at(j));
                results.push_back(std::move(resultsTemp));
            }
        }
        return results;
    }
    else
    {
        fprintf(stderr, "\tERROR: creatAll: the input include nullptr!\n");
        return {};
    }
}

std::vector<ast_ptr> exprAutoNew(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "exprAutoNew: ERROR: the input expr is nullptr!\n");
        return {};
    }
    fprintf(stderr, "exprAutoNew: step1: preprocess\n");
    fprintf(stderr, "exprAutoNew: expr = %s\n", PrintExpression(expr).c_str());
    ast_ptr exprNew = preprocess(expr);
    fprintf(stderr, "exprAutoNew: exprNew = %s\n", PrintExpression(exprNew).c_str());

    std::vector<ast_ptr> results;
    fprintf(stderr, "exprAutoNew: step2: judge if exprNew is a fraction\n");
    if (isFraction(exprNew))
    {
        fprintf(stderr, "exprAutoNew: exprNew is a fraction, so perform step3 and step4\n");
        ast_ptr numerator = getNumerator(exprNew);
        ast_ptr denominator = getDenominator(exprNew);

        fprintf(stderr, "exprAutoNew: step3: perform on numerator.\n");
        auto numerators = tryRewrite(std::move(numerator));
        fprintf(stderr, "exprAutoNew: step3: end perform on numerator.\n");
        
        fprintf(stderr, "exprAutoNew: step3: perform on denominator.\n");
        auto denominators = tryRewrite(std::move(denominator));
        fprintf(stderr, "exprAutoNew: step3: end perform on denominator.\n");
        
        fprintf(stderr, "exprAutoNew: step4: combine numerator and denominator.\n");
        if(isConstant(denominators))
        {
            ast_ptr one = makePtr<NumberExprAST>(1.0);
            NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(denominators.at(0).get());
            ast_ptr denominatorTmp = makePtr<NumberExprAST>(numberPtr->getNumber());
            auto coefficient = divExpr(one, denominatorTmp);

            for(const auto& numerator : numerators)
            {
                auto tmp1 = mulExpr(coefficient, numerator);
                auto tmp2 = divExpr(numerator, denominatorTmp);
                results.push_back(std::move(tmp1));
                results.push_back(std::move(tmp2));
            }
        }
        else
        {
            results = createAll(numerators, denominators);
        }
    }
    else
    {
        fprintf(stderr, "exprAuto: exprNew is not a fraction, so perform step4\n");
        results = tryRewrite(std::move(exprNew));
    }

    fprintf(stderr, "exprAuto: results size = %ld\n", results.size());
    printExprs(results, "exprAuto: ");
    return results;
}

int main()
{
    installOperatorsForStr();
    initPython();

    std::string inputStr = "";
    fprintf(stderr, "ready> ");
    while (getline(std::cin, inputStr))
    {
        if(inputStr == "exit;" || inputStr == "quit;" || inputStr == "exit" || inputStr == "quit") {
            fprintf(stderr, "main: the application is end after inputing '%s'\n", inputStr.c_str());
            break;
        }
        if(inputStr.empty()) {
            fprintf(stderr, "WARNIGN: main: please input a exprssion before pressing Enter\n");
            fprintf(stderr, "ready> ");
            continue;
        } else if(inputStr.back() == ';') {
            fprintf(stderr, "you do not need to add a ';' after the expression\n");
            inputStr.pop_back(); // remove the last char ';'
        }
        ast_ptr expr = simplifyExpr(inputStr); // Python SymPy simplify
        printExpr(expr, "main: after SymPy's simplify, expr = ");
        std::vector<ast_ptr> results = exprAutoNew(expr);
        printExprs(results, "main: after exprAutoNew: ");
        fprintf(stderr, "ready> ");
    }
    endPython();
    return 0;
}