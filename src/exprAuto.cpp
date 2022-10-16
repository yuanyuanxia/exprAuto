#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include "basic.hpp"
#include "monoFracInfo.hpp"
#include "expandAST.hpp"
#include "preprocess.hpp"
#include "geneExpr.hpp"
#include "polyRewrite.hpp"
#include "mathfuncRewrite.hpp"
#include "parserASTLY.hpp"
#include "exprAuto.hpp"
#include "geneCode.hpp"
#include "simplifyExpr.hpp"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

int callLevel{-1};
const char callLevelChar{' '};
const char callCountChar{'>'};
const size_t promtTimes = 2;

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

// Delete the same expr in exprs using the function 'isEqual'
void deleteTheSame(vector<ast_ptr> &exprs)
{
    const size_t size = exprs.size();
    if(size == 0)
    {
        fprintf(stderr, "WARNING: deleteTheSame: the input exprs is empty\n");
        exit(1);
    }
    // Mark duplicate elements as shouldDelete[i] = true
    bool *shouldDelete = new bool[size]{false};
    for(size_t i = 0; i < size; i++) {
        if(shouldDelete[i] == true) {
            continue;
        }
        for(size_t j = i + 1; j < size; j++) {
            if(isEqual(exprs.at(i), exprs.at(j))) {
                shouldDelete[j] = true;
            }
        }
    }
    // Erase the duplicate elements according to array 'shouldDelete'
    for(int i = size - 1; i >= 0; i--) {
        if(shouldDelete[i] == true) {
            exprs.erase(exprs.begin() + i);
        }
    }
}

void deleteTheSameLY(vector<ast_ptr> &exprs)
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
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "dealWithCalls: ";
    cout << prompt << "start--------" << endl;
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

    // printExprs(variants, "\t\tdealWithCalls: before middle, ");
    vector<ast_ptr> middles = combineCallArgs(variants, widths, callee);
    // printExprs(middles, "\t\tdealWithCalls: after middle, ");

    vector<ast_ptr> allResults;
    for(const auto &middle : middles)
    {
        auto result = dealWithCallsKernel(middle, callee);
        allResults.push_back(std::move(result));
    }

    deleteTheSame(allResults);
    printExprs(allResults, prompt + "at the end, ");
    cout << prompt << "end--------" << endl;
    callCount--;
    callLevel--;
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
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "mathfuncRewrite: ";
    cout << prompt << "start--------" << endl;

    // fprintf(stderr, "\tmathfuncRewrite: start--------\n");
    if(expr == nullptr)
    {
        cerr << prompt << "ERROR: empty" << endl;
        exit(EXIT_FAILURE);
    }
    // printExpr(expr, "\tmathfuncRewrite: at the beginning: ");
    ast_ptr newexpr = expr->Clone();
    vector<ast_ptr> exprsFinal;
    exprsFinal.push_back(std::move(newexpr->Clone()));

    if(newexpr->type() == "Call")
    {
        printExpr(expr, prompt + "before dealWithCalls: newexpr = ");
        auto newASTs = dealWithCalls(newexpr);
        exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(newASTs.begin()), std::make_move_iterator(newASTs.end()));

        if(callCount == 1) printExprs(exprsFinal, "\tmathfuncRewrite: ");
        cout << prompt << "end--------" << endl;
        callCount--;
        callLevel--;
        return exprsFinal;
    }

    if(expr->type() != "Binary")  // May be variable or number
    {
        if(callCount == 1) printExprs(exprsFinal, "\tmathfuncRewrite: ");
        cout << prompt << "end--------" << endl;
        callCount--;
        callLevel--;
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
    // if(callCount == 1)
    // {
    //     for(auto &exprFinal : exprsFinal)
    //     {
    //         vector<ast_ptr> items = extractItems(exprFinal);
    //         vector<monoInfo> info = extractInfo(items);
    //         vector<monoInfo> infoNew = mergePolynomial(info);
    //         exprFinal = geneExprAST(infoNew);
    //     }
    // }
    deleteTheSame(exprsFinal);

    // fprintf(stderr, "\tmathfuncRewrite: end--------\n");
    if(callCount == 1) printExprs(exprsFinal, prompt + "exprsFinal: ");
    cout << prompt << "end--------" << endl;
    callCount--;
    callLevel--;
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
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "polyRewrite: ";
    cout << prompt << "start--------" <<endl;

    ast_ptr middleNew = expandExprWrapper(expr);
    vector<ast_ptr> items = extractItems(middleNew);
    auto infoNew = extractFracInfo(items);
    if(checkMonoFrac(infoNew)) // all the denominators are 1
    {
        vector<monoInfo> numerators;
        for(const auto &monoFrac: infoNew)
        {
            numerators.push_back(monoFrac.numerator);
        }
        auto numeratorsFinal = mergePolynomial(numerators);
        auto results = createExpr(numeratorsFinal);
        vector<ast_ptr> resultsNew;
        for(auto &result : results)
        {
            resultsNew.push_back(result->Clone()); // the key to control the rewrite results
            auto tmp = fmaRewrite(result);
            mineAppend(resultsNew, tmp);
        }
        cout << prompt << "end--------" <<endl;
        callCount--;
        callLevel--;
        return resultsNew;
    }
    else if(infoNew.size() == 1)
    {
        auto results = createExpr(infoNew);
        
        cout << prompt << "end--------" <<endl;
        callCount--;
        callLevel--;
        return results;
    }

    cerr << prompt << "we can not handle this situation!" << endl;
    exit(EXIT_FAILURE);
}

vector<ast_ptr> tryRewrite(ast_ptr expr)
{
    static size_t callCount = 0;
    callCount++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "tryRewrite: ";
    cout << prompt << "start--------" <<endl;

    vector<ast_ptr> items = extractItems(expr);
    auto info = extractFracInfo(items);
    ast_ptr exprNew;
    if(checkMonoFrac(info)) // all the denominators are 1
    {
        vector<monoInfo> numerators;
        for(const auto &monoFrac: info)
        {
            numerators.push_back(monoFrac.numerator);
        }
        auto infoNew = mergePolynomial(numerators);
        exprNew = geneExprAST(infoNew);
    }
    else if(info.size() == 1)
    {
        exprNew = geneExprAST(info);
    }
    else
    {
        cerr << prompt << "we can not handle this situation!" << endl;
        exit(EXIT_FAILURE);
    }
    if(callCount == 1) printExpr(exprNew, prompt + "tryRewrites: before mathfuncRewrite: ");
    auto middles = mathfuncRewrite(exprNew);
    vector<ast_ptr> results;
    size_t index = 0;
    for(const auto &middle : middles)
    {
        cout << prompt << "For expr NO." << index << ": " << PrintExpression(middle) << ", do polyRewrite" << endl;
        vector<string> vars;
        getVariablesFromExpr(middle, vars);
        /*if(vars.size() > 1)
        {
            results.push_back(std::move(middle->Clone()));
        }
        else
        {
            auto tmp = polyRewrite(middle);
            mineAppend(results, tmp);
        }*/
	auto tmp = polyRewrite(middle);
        mineAppend(results, tmp);
        index++;
    }
    // if(callCount == 1) printExprs(results, "tryRewrites: before delete: ");
    deleteTheSame(results);

    if(callCount == 1) printExprs(results, prompt + "at the last: ");
    cout << prompt << "end--------" <<endl;
    callCount--;
    return results;
}

vector<ast_ptr> createAll(vector<ast_ptr> &numerators, vector<ast_ptr> &denominators)
{
    ast_ptr resultsTemp;
    vector<ast_ptr> results;

    if(numerators.at(0) != nullptr && denominators.at(0) != nullptr)
    {
        ast_ptr tmpOne = makePtr<NumberExprAST>(1.0);
        for(auto &denominator : denominators)
        {
            if(isEqual(denominator, tmpOne))
            {
                for(auto &numerator : numerators)
                {
                    results.push_back(std::move(numerator->Clone()));
                }
            }
            else
            {
                for(auto &numerator : numerators)
                {
                    resultsTemp = divExpr(numerator, denominator);
                    results.push_back(std::move(resultsTemp));
                }
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
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "exprAutoNew: ";
    cout << prompt << "start-----------" << endl;
    if (expr == nullptr)
    {
        cerr << prompt << "ERROR: the input expr is nullptr!" << endl;
        exit(EXIT_FAILURE);
    }
    cout << prompt << "step1: preprocess" << endl;
    cout << prompt << "expr = " << PrintExpression(expr) << endl;

    ast_ptr exprNew = preprocess(expr);
    exprNew = simplifyExpr(exprNew);
    exprNew = minusRewrite(exprNew);
    cout << prompt << "after preprocess, exprNew = " << PrintExpression(exprNew) << endl;

    vector<ast_ptr> results;
    cout << prompt << "step2: judge if exprNew is a fraction" << endl;
    if (isFraction(exprNew))
    {
        cout << prompt << "exprNew is a fraction, so perform step3 and step4" << endl;
        ast_ptr numerator = getNumerator(exprNew);
        ast_ptr denominator = getDenominator(exprNew);

        cout << prompt << "step3: perform on numerator." << endl;
        auto numerators = tryRewrite(std::move(numerator));
        cout << prompt << "step3: end perform on numerator." << endl;
        
        cout << prompt << "step3: perform on denominator." << endl;
        auto denominators = tryRewrite(std::move(denominator));
        cout << prompt << "step3: end perform on denominator." << endl;
        
        cout << prompt << "step4: combine numerator and denominator." << endl;
        if(isConstant(denominators)) // only one element, and the element is constant
        {
            if(isConstant(numerators))
            {
                auto tmp = divExpr(numerators.at(0), denominators.at(0));
                results.push_back(std::move(tmp));
            }
            else
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
        }
        else
        {
            results = createAll(numerators, denominators);
        }
    }
    else
    {
        cout << prompt << "exprNew is not a fraction, so perform step4" << endl;
        results = tryRewrite(std::move(exprNew));
    }

    cout << prompt << "results size = " << results.size() << endl;
    printExprs(results, prompt);
    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return results;
}
