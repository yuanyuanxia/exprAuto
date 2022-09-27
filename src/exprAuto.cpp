#include <algorithm>
#include <queue>
#include <numeric>
#include <functional>

#include "basic.hpp"
#include "monoInfo.hpp"
#include "expandAST.hpp"
#include "exprAuto.hpp"
#include "polyRewrite.hpp"
#include "mathfuncRewrite.hpp"
#include "simplifyExpr.hpp"
#include "parserASTLY.hpp"

#include "preprocess.hpp"
#include "geneExpr.hpp"

// similiar to getExprFromVariants
std::unique_ptr<ExprAST> getCallFromVariants(const std::vector<std::unique_ptr<ExprAST>> &variants, const std::vector<size_t> orders, const std::string callee)
{
    std::vector<std::unique_ptr<ExprAST>> args;
    for(const auto &order : orders)
    {
        args.push_back(variants.at(order)->Clone());
    }
    return std::make_unique<CallExprAST>(callee, std::move(args));
}

// similiar to combineMonomial
std::vector<std::unique_ptr<ExprAST>> combineCallArgs(const std::vector<std::unique_ptr<ExprAST>> &variants, const std::vector<size_t> widths, const std::string callee)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    size_t numOfCombinations = std::accumulate(std::begin(widths), std::end(widths), 1, std::multiplies<>());

    for(size_t i = 0; i < numOfCombinations; i++)
    {
        std::vector<size_t> orders = getCombineOrders(widths, i);
        std::unique_ptr<ExprAST> tmp = getCallFromVariants(variants, orders, callee);
        results.push_back(std::move(tmp));
    }
    return results;
}

std::unique_ptr<ExprAST> dealWithCallsKernel(const std::unique_ptr<ExprAST> &expr, const std::string callee)
{
    std::unique_ptr<ExprAST> result;
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

std::unique_ptr<ExprAST> dealWithCallsKernel(const std::unique_ptr<ExprAST> &expr)
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

// TODO: delete the same expr in exprs using the function 'isEqual'
void deleteTheSame(std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    if(exprs.size() == 0)
    {
        fprintf(stderr, "WARNING: deleteTheSame: the input exprs is empty\n");
        exit(1);
    }
    int size = exprs.size();
    std::vector<std::string> vec;
    int flag = 0;
    std::string s;
    std::unique_ptr<ExprAST> es1;
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
        std::unique_ptr<ExprAST> es = ParseExpressionFromString(vec.at(i));
        exprs.push_back(std::move(es));
    }
}

std::vector<std::unique_ptr<ExprAST>> dealWithCalls(const std::unique_ptr<ExprAST> &expr)
{
    CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
    std::string callee = callExpr->getCallee();
    std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();
    
    std::vector<std::unique_ptr<ExprAST>> variants;
    std::vector<size_t> widths;
    for(const auto &arg : args)
    {
        auto exprs = mathfuncRewriteNew(arg);
        variants.insert(variants.end(), std::make_move_iterator(exprs.begin()), std::make_move_iterator(exprs.end()));
        widths.push_back(exprs.size());
    }

    std::for_each(variants.begin(), variants.end(),
                [index = 0](const auto &variant) mutable { fprintf(stderr, "dealWithCalls: before middle, No.%d: %s\n", index++, PrintExpression(variant).c_str()); });
    std::vector<std::unique_ptr<ExprAST>> middles = combineCallArgs(variants, widths, callee);
    std::for_each(middles.begin(), middles.end(),
                [index = 0](const auto &variant) mutable { fprintf(stderr, "dealWithCalls: after middle, No.%d: %s\n", index++, PrintExpression(variant).c_str()); });

    std::vector<std::unique_ptr<ExprAST>> allResults;
    for(const auto &middle : middles)
    {
        auto result = dealWithCallsKernel(middle, callee);
        allResults.push_back(std::move(result));
    }
    
    deleteTheSame(allResults);
    
    return allResults;
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

std::vector<std::unique_ptr<ExprAST>> mathfuncRewriteNew(const std::unique_ptr<ExprAST> &expr)
{
    fprintf(stderr, "\tmathfuncRewrite: start--------\n");
    
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return {};
    }
    std::unique_ptr<ExprAST> newexpr = expr->Clone();
    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    exprsFinal.push_back(std::move(newexpr->Clone()));
    if(newexpr->type() == "Call")
    {
        auto newASTs = dealWithCalls(newexpr);
        exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(newASTs.begin()), std::make_move_iterator(newASTs.end()));
        return exprsFinal;
    }

    if(expr->type() != "Binary") // May be variable or number
    {
        return exprsFinal;
    }

    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    auto &lhs = binOpPtr->getLHS();
    auto &rhs = binOpPtr->getRHS();
    auto lhsNewASTs = mathfuncRewriteNew(lhs);
    auto rhsNewASTs = mathfuncRewriteNew(rhs);
    auto newASTs = createAllBinary(lhsNewASTs, rhsNewASTs, op);
    newASTs = dealWithBinOp(newASTs, op);
    exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(newASTs.begin()), std::make_move_iterator(newASTs.end()));
    deleteTheSame(exprsFinal);
    fprintf(stderr, "\tmathfuncRewrite: end--------\n");
    return exprsFinal;  
}

// old: A kernel function within mathfuncRewrite
std::unique_ptr<ExprAST> dealWithCall(const std::unique_ptr<ExprAST> &expr)
{
    CallExprAST *callExpr1 = dynamic_cast<CallExprAST *>(expr.get());
    std::string callee = callExpr1->getCallee();
    std::unique_ptr<ExprAST> result;
    if(callee == "exp")
    {
        std::cout << "That is " << callee << std::endl;
        result = lex_x_Or_elx_x(expr);
    }
    else if(callee == "log")
    {   
        std::cout << "That is " << callee << std::endl;
        result = lex_x_Or_elx_x(expr);
        result = logTolog1p(result);
    }
    else if(callee == "sqrt")
    {
        std::cout << "That is " << callee << std::endl;
        result = sqrtTohypot(expr); // if success then will be core dumped
    }
    else
    {
        std::cout << "default: That is " << callee << std::endl;
        result = expr->Clone();
    }
    return result;
}

// old
std::vector<std::unique_ptr<ExprAST>> mathfuncRewrite(const std::unique_ptr<ExprAST> &expr)
{
    // fprintf(stderr, "\tmathfuncRewrite: start--------\n");
    
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return {};
    }
    std::unique_ptr<ExprAST> newexpr = expr->Clone();
    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    exprsFinal.push_back(std::move(expr->Clone()));
    if(newexpr->type() == "Call")
    {
        newexpr = dealWithCall(newexpr);
    }
    else if(newexpr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(newexpr.get());
        char op = binOpPtr->getOp();
        if (op == '+')
        {
            newexpr = expToexpm1(newexpr);
        }
        while(op == '*')
        {
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            // rhs
            BinaryExprAST *binOpRPtr = nullptr;
            if (rhs->type() == "Call")
            {
                std::unique_ptr<ExprAST> tmp = dealWithCall(rhs);
                binOpPtr->setRHS(tmp);
            }
            else if (rhs->type() == "Binary")
            {  // the usual case
                binOpRPtr = dynamic_cast<BinaryExprAST *>(rhs.get());
                char rhsOp = binOpRPtr->getOp();
                if (rhsOp == '+')
                {
                    std::unique_ptr<ExprAST> tmp = expToexpm1(rhs);
                    binOpPtr->setRHS(tmp);
                }
            }

            // lhs
            std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
            std::string lhsType = lhs->type();
            if(lhsType == "Binary")
            {
                binOpPtr = dynamic_cast<BinaryExprAST *>(lhs.get());
                op = binOpPtr->getOp();
                if(op == '+')
                {
                    std::unique_ptr<ExprAST> tmp = expToexpm1(lhs);
                    binOpPtr->setLHS(tmp);
                    break;
                }
            }
            else if(lhsType == "Call")
            {
                std::unique_ptr<ExprAST> tmp = dealWithCall(lhs);
                binOpPtr->setLHS(tmp);
                break;
            }
            else
            {
                break;
            }
        }
    }
    
    if(!isEqual(expr, newexpr))
    {
        exprsFinal.push_back(std::move(newexpr));
    }
    // fprintf(stderr, "\tmathfuncRewrite: end--------\n");
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

// a kernel function within rewriteExpr
std::unique_ptr<ExprAST> getExprFromVariants(const std::vector<std::unique_ptr<ExprAST>> &variants, const std::vector<size_t> orders)
{
    std::unique_ptr<ExprAST> result = nullptr;
    for(const auto &order : orders)
    {
        result = addExpr(result, variants.at(order));
    }
    return result;
}

// a kernel function within rewriteExpr
std::vector<std::unique_ptr<ExprAST>> combineMonomial(const std::vector<std::unique_ptr<ExprAST>> &variants, const std::vector<size_t> widths)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    size_t numOfCombinations = std::accumulate(std::begin(widths), std::end(widths), 1, std::multiplies<>());

    for(size_t i = 0; i < numOfCombinations; i++)
    {
        std::vector<size_t> orders = getCombineOrders(widths, i);
        std::unique_ptr<ExprAST> tmp = getExprFromVariants(variants, orders);
        results.push_back(std::move(tmp));
    }
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::vector<monoInfo> &monomials)
{
    fprintf(stderr, "rewriteExpr: start--------\n");
    std::vector<std::unique_ptr<ExprAST>> variants;
    std::vector<size_t> widths;

    // size_t i = 0;
    // for (auto &monomial : monomials)
    // {
    //     fprintf(stderr, "rewriteExpr: The Monomial No.%lu: \n", i);
    //     (monomial).showInfo(); // not support const. error: passing ‘const monoInfo’ as ‘this’ argument discards qualifiers [-fpermissive]
    //     fprintf(stderr, "\n");
    //     i++;
    // }
    
    // generate the variants of each monomial.
    // Then store them into vector 'variants' and record their size into vector 'widths'
    for(const auto &monomial : monomials) 
    {
        std::unique_ptr<ExprAST> expr = geneMonomialAST(monomial);
        fprintf(stderr, "rewriteExpr: before mathRewrite: %s\n", PrintExpression(expr).c_str());

        std::vector<std::unique_ptr<ExprAST>> exprs;
        if(monomial.functions.size() != 0 || monomial.poly.monos.size() != 0)
        {
            // exprs = mathfuncRewrite(expr);
            exprs = mathfuncRewriteNew(expr);
            std::for_each(exprs.begin(), exprs.end(),
                    [index = 0](const auto &expr1) mutable { fprintf(stderr, "rewriteExpr: after mathRewrite No.%d: %s\n", index++, PrintExpression(expr1).c_str()); });
        }
        else
        {
            exprs.push_back(std::move(expr));
        }
        variants.insert(variants.end(), std::make_move_iterator(exprs.begin()), std::make_move_iterator(exprs.end()));
        widths.push_back(exprs.size());
    }

    std::for_each(variants.begin(), variants.end(),
                [index = 0](const auto &variant) mutable { fprintf(stderr, "rewriteExpr: before middle, No.%d: %s\n", index++, PrintExpression(variant).c_str()); });
    // generate all possible poly based on the monomials's variants, and store them into vector 'middles'
    std::vector<std::unique_ptr<ExprAST>> middles = combineMonomial(variants, widths);
    std::for_each(middles.begin(), middles.end(),
                [index = 0](const auto &variant) mutable { fprintf(stderr, "rewriteExpr: after middle, No.%d: %s\n", index++, PrintExpression(variant).c_str()); });

    std::vector<std::unique_ptr<ExprAST>> results;
    // do poly rewrite for each poly generated in the last step, and store the results into vector 'results'
    for(const auto &middle : middles)
    {
        std::unique_ptr<ExprAST> middleNew = expandExprWrapper(middle);
        std::vector<std::unique_ptr<ExprAST>> items = extractItems(middleNew);
        std::vector<monoInfo> info = extractInfo(items);
        std::vector<monoInfo> infoNew = mergePolynomial(info);
        std::vector<std::unique_ptr<ExprAST>> tmps = createExpr(infoNew);
        // std::vector<std::unique_ptr<ExprAST>> tmps = createExpr(middle); // old method
        results.insert(results.end(), std::make_move_iterator(tmps.begin()), std::make_move_iterator(tmps.end()));
    }

    // fprintf(stderr, "rewriteExpr: results size = %ld\n", results.size());
    // for (size_t i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "rewriteExpr: No.%lu: %s\n", i, PrintExpression(results[i]).c_str());
    // }
    fprintf(stderr, "rewriteExpr: end--------\n");
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST> &expr)
{
    std::vector<std::unique_ptr<ExprAST>> exprsInit, exprsFinal;
    exprsInit.push_back(std::move(expr->Clone()));
    fprintf(stderr, "rewriteExprWrapper: start: expr = %s\n", PrintExpression(expr).c_str());
    std::unique_ptr<ExprAST> exprNew = simplifyExpr(expr);
    fprintf(stderr, "rewriteExprWrapper: after simplifyExpr: exprNew = %s\n", PrintExpression(exprNew).c_str());
    exprNew = minusRewrite(exprNew);
    fprintf(stderr, "rewriteExprWrapper: after minusRewrite: exprNew = %s\n", PrintExpression(exprNew).c_str());
    if(!isEqual(expr, exprNew))
    {
        exprsInit.push_back(std::move(exprNew));
    }
    for(const auto &exprInit : exprsInit)
    {
        std::vector<std::unique_ptr<ExprAST>> items = extractItems(exprInit);
        std::vector<monoInfo> info = extractInfo(items);
        std::vector<monoInfo> infoNew = mergePolynomial(info);
        auto tmp = geneExprAST(infoNew); // just show information
        fprintf(stderr, "rewriteExprWrapper: after geneExprAST: tmp = %s\n", PrintExpression(tmp).c_str());
        auto tmps = rewriteExpr(infoNew); // rewrite from monoInfo
        exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(tmps.begin()), std::make_move_iterator(tmps.end()));
    }
    deleteTheSame(exprsFinal);
    return exprsFinal; // rewrite from monoInfo
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

// TODO: check
std::vector<std::unique_ptr<ExprAST>> exprAuto(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "exprAuto: ERROR: the input expr is nullptr!\n");
        return {};
    }
    fprintf(stderr, "exprAuto: step1: preprocess\n");
    fprintf(stderr, "exprAuto: expr = %s\n", PrintExpression(expr).c_str());
    std::unique_ptr<ExprAST> exprNew = preprocess(expr);
    fprintf(stderr, "exprAuto: exprNew = %s\n", PrintExpression(exprNew).c_str());

    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    fprintf(stderr, "exprAuto: step2: judge if exprNew is a fraction\n");
    if (isFraction(exprNew))
    {
        fprintf(stderr, "exprAuto: exprNew is a fraction, so perform step3 and step4\n");
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        fprintf(stderr, "exprAuto: step3: perform on numerator.\n");
        auto numeratorsFinal = rewriteExprWrapper(numeratorTmp);
        fprintf(stderr, "exprAuto: step3: end perform on numerator.\n");
        fprintf(stderr, "exprAuto: step3: perform on denominator.\n");
        auto denominatorsFinal = rewriteExprWrapper(denominatorTmp);
        fprintf(stderr, "exprAuto: step3: end perform on denominator.\n");
        fprintf(stderr, "exprAuto: step4: combine numerator and denominator.\n");
        if(isConstant(denominatorsFinal))
        {
            std::unique_ptr<ExprAST> one = std::make_unique<NumberExprAST>(1.0);
            NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(denominatorsFinal.at(0).get());
            std::unique_ptr<ExprAST> denominator = std::make_unique<NumberExprAST>(numberPtr->getNumber());
            auto coefficient = divExpr(one, denominator);
            for(const auto& numerator : numeratorsFinal)
            {
                auto tmp1 = mulExpr(coefficient, numerator);
                auto tmp2 = divExpr(numerator, denominator);
                exprsFinal.push_back(std::move(tmp1));
                exprsFinal.push_back(std::move(tmp2));
            }
        }
        else
        {
            exprsFinal = createAll(numeratorsFinal, denominatorsFinal);
        }
    }
    else
    {
        fprintf(stderr, "exprAuto: exprNew is not a fraction, so perform step4\n");
        exprsFinal = rewriteExprWrapper(exprNew);
    }

    fprintf(stderr, "exprAuto: exprsFinal size = %ld\n", exprsFinal.size());
    for (long unsigned int i = 0; i < exprsFinal.size(); i++)
    {
        fprintf(stderr, "exprAuto: No.%lu: %s\n", i, PrintExpression(exprsFinal[i]).c_str());
    }
    return exprsFinal;
}
