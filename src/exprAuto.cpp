#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include "basic.hpp"
#include "monoInfo.hpp"
#include "expandAST.hpp"
#include "preprocess.hpp"
#include "polyRewrite.hpp"
#include "mathfuncRewrite.hpp"
#include "parserASTLY.hpp"
#include "exprAuto.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

// similiar to getExprFromVariants
ast_ptr getCallFromVariants(const vector<ast_ptr> &variants, const vector<size_t> orders, const string callee)
{
    vector<ast_ptr> args;
    for(const auto &order : orders)
    {
        args.push_back(variants.at(order)->Clone());
    }
    return makePtr<CallExprAST>(callee, std::move(args));
}

// similiar to combineMonomial
vector<ast_ptr> combineCallArgs(const vector<ast_ptr> &variants, const vector<size_t> widths, const string callee)
{
    vector<ast_ptr> results;
    size_t numOfCombinations = std::accumulate(std::begin(widths), std::end(widths), 1, std::multiplies<>());

    for(size_t i = 0; i < numOfCombinations; i++)
    {
        vector<size_t> orders = getCombineOrders(widths, i);
        ast_ptr tmp = getCallFromVariants(variants, orders, callee);
        results.push_back(std::move(tmp));
    }
    return results;
}

ast_ptr dealWithCallsKernel(const ast_ptr &expr, const string callee)
{
    ast_ptr result;
    if(callee == "exp")
    {
        // cout << "dealWithCallsKernel: That is " << callee << endl;
        result = lex_x_Or_elx_x(expr);
    }
    else if(callee == "log")
    {   
        // cout << "dealWithCallsKernel: That is " << callee << endl;
        result = lex_x_Or_elx_x(expr);
        result = logTolog1p(result);
    }
    else if(callee == "sqrt")
    {
        // cout << "dealWithCallsKernel: That is " << callee << endl;
        result = sqrtTohypot(expr);
    }
    else
    {
        // cout << "dealWithCallsKernel: default: That is " << callee << endl;
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
    const string callee = callPtr->getCallee();
    return dealWithCallsKernel(expr, callee);
}

// TODO: delete the same expr in exprs using the function 'isEqual'
void deleteTheSame(vector<ast_ptr> &exprs)
{
    if(exprs.size() == 0)
    {
        fprintf(stderr, "WARNING: deleteTheSame: the input exprs is empty\n");
        exit(1);
    }
    int size = exprs.size();
    vector<string> vec;
    int flag = 0;
    string s;
    ast_ptr es1;
    while(flag < size)
    {
        es1 = exprs.at(flag++)->Clone();
        s = PrintExpression(es1);
        vec.push_back(s);
    }
    std::sort(vec.begin(), vec.end());
    auto end_unique = std::unique(vec.begin(), vec.end());
    vec.erase(end_unique, vec.end());
    flag = 0;
    exprs.clear();
    for(size_t i = 0; i < vec.size(); i++)
    {
        ast_ptr es = ParseExpressionFromString(vec.at(i));
        exprs.push_back(std::move(es));
    }
}

vector<ast_ptr> dealWithCalls(const ast_ptr &expr)
{
    CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
    string callee = callExpr->getCallee();
    vector<ast_ptr> &args = callExpr->getArgs();

    vector<ast_ptr> variants;
    vector<size_t> widths;
    for(const auto &arg : args)
    {
        auto exprs = exprAutoNew(arg);
        mineAppend(variants, exprs);
        widths.push_back(exprs.size());
    }

    printExprs(variants, "dealWithCalls: before middle, ");
    vector<ast_ptr> middles = combineCallArgs(variants, widths, callee);
    printExprs(middles, "dealWithCalls: after middle, ");

    vector<ast_ptr> allResults;
    for(const auto &middle : middles)
    {
        auto result = dealWithCallsKernel(middle, callee);
        allResults.push_back(std::move(result));
    }

    deleteTheSame(allResults);

    return allResults;
}

// similiar to createAll, but high level than createAll
vector<ast_ptr> createAllBinary(vector<ast_ptr> &numerators, vector<ast_ptr> &denominators, const char op)
{
    ast_ptr resultsTemp;
    vector<ast_ptr> results;

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
ast_ptr dealWithBinOpKernel(const ast_ptr &expr, const char &op)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: dealWithBinOpKernel: the input expression is nullptr!\n");
        return nullptr;
    }
    ast_ptr result;
    if(op == '+')
    {
        // cout << "dealWithBinOpKernel: That is " << op << endl;
        result = expToexpm1(expr);
    }
    else
    {
        // cout << "dealWithBinOpKernel: default: That is " << op << endl;
        result = expr->Clone();
    }
    return result;
}

ast_ptr dealWithBinOpKernel(const ast_ptr &expr)
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

vector<ast_ptr> dealWithBinOp(vector<ast_ptr> &exprs, const char &op)
{
    vector<ast_ptr> results;
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

vector<ast_ptr> mathfuncRewrite(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return {};
    }
    ast_ptr newexpr = expr->Clone();
    vector<ast_ptr> exprsFinal;
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
vector<size_t> getCombineOrders(const vector<size_t> widths, const size_t identifier)
{
    vector<size_t> orders;
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

vector<ast_ptr> polyRewrite(const ast_ptr &expr)
{
    ast_ptr middleNew = expandExprWrapper(expr);
    vector<ast_ptr> items = extractItems(middleNew);
    vector<monoInfo> info = extractInfo(items);
    vector<monoInfo> infoNew = mergePolynomial(info);
    return createExpr(infoNew);
}

vector<ast_ptr> tryRewrite(ast_ptr expr)
{
    fprintf(stderr, "\ttryRewrite: start--------\n");

    auto middles = mathfuncRewrite(expr);

    vector<ast_ptr> results;
    for(const auto &middle : middles)
    {
        auto tmp = polyRewrite(middle);
        mineAppend(results, tmp);
    }
    fprintf(stderr, "\ttryRewrite: end--------\n");

    return results;
}

vector<ast_ptr> createAll(vector<ast_ptr> &numerators, vector<ast_ptr> &denominators)
{
    ast_ptr resultsTemp;
    vector<ast_ptr> results;

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

vector<ast_ptr> exprAutoNew(const ast_ptr &expr)
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

    vector<ast_ptr> results;
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