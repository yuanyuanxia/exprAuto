#include <algorithm>

#include "basic.hpp"

bool isFraction(const std::unique_ptr<ExprAST>& expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "\tERROR: isFraction: the input expr is nullptr!\n");
        return false;
    }
    if(expr->type() == "Binary")
    {
        BinaryExprAST* binOpPtr = dynamic_cast<BinaryExprAST*>(expr.get());
        char op = binOpPtr->getOp();
        if(op == '/')
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<ExprAST> getNumerator(const std::unique_ptr<ExprAST>& expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getNumerator: the input expr is nullptr!\n");
        return nullptr;
    }
    if(expr->type() == "Binary")
    {
        BinaryExprAST* binOpPtr = dynamic_cast<BinaryExprAST*>(expr.get());
        char op = binOpPtr->getOp();
        if(op == '/')
        {
            return std::move(binOpPtr->getLHS());
        }
    }
    fprintf(stderr, "\tWARNING: getNumerator: the input expr is not match for fraction\n");
    return expr->Clone();
}

std::unique_ptr<ExprAST> getDenominator(const std::unique_ptr<ExprAST>& expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getDenominator: the input expr is nullptr!\n");
        return nullptr;
    }
    if(expr->type() == "Binary")
    {
        BinaryExprAST* binOpPtr = dynamic_cast<BinaryExprAST*>(expr.get());
        char op = binOpPtr->getOp();
        if(op == '/')
        {
            return std::move(binOpPtr->getRHS());
        }
    }
    fprintf(stderr, "\tWARNING: getDenominator: the input expr is not match for fraction\n");
    return expr->Clone();
}

std::unique_ptr<ExprAST> createBinaryExpr(const std::unique_ptr<ExprAST>& expr1, const std::unique_ptr<ExprAST>& expr2, const char op)
{
    if(expr1 == nullptr || expr2 == nullptr)
    {
        fprintf(stderr, "\tERROR: createBinaryExpr: the input contain nullptr!");
        return nullptr;
    }

    auto lhs = expr1->Clone();
    auto rhs = expr2->Clone();
    return std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
}

std::unique_ptr<ExprAST> addExpr(const std::unique_ptr<ExprAST>& expr1, const std::unique_ptr<ExprAST>& expr2) { return createBinaryExpr(expr1, expr2, '+'); }

std::unique_ptr<ExprAST> mulExpr(const std::unique_ptr<ExprAST>& expr1, const std::unique_ptr<ExprAST>& expr2) { return createBinaryExpr(expr1, expr2, '*'); }

std::unique_ptr<ExprAST> divExpr(const std::unique_ptr<ExprAST>& expr1, const std::unique_ptr<ExprAST>& expr2) { return createBinaryExpr(expr1, expr2, '/'); }

std::unique_ptr<ExprAST> combineFraction(const std::unique_ptr<ExprAST>& numerator, const std::unique_ptr<ExprAST>& denominator)
{
    // init
    std::unique_ptr<ExprAST> numeratorNew = nullptr;
    std::unique_ptr<ExprAST> denominatorNew = nullptr;
    std::unique_ptr<ExprAST> numeratorFinal = nullptr;
    std::unique_ptr<ExprAST> denominatorFinal = nullptr;
    // numerator
    if(isFraction(numerator))
    {
        const std::unique_ptr<ExprAST> numeratorTmp = getNumerator(numerator);
        const std::unique_ptr<ExprAST> denominatorTmp = getDenominator(numerator);

        numeratorNew = numeratorTmp->Clone();
        denominatorNew = mulExpr(denominator, denominatorTmp);
    }
    else
    {
        fprintf(stderr, "\tNOTE: combineFraction: the input numerator is not a fraction.\n");
        numeratorNew = numerator->Clone();
        denominatorNew = denominator->Clone();
    }
    // denominator
    if(isFraction(denominator))
    {
        const std::unique_ptr<ExprAST> numeratorTmp = getNumerator(denominatorNew);
        const std::unique_ptr<ExprAST> denominatorTmp = getDenominator(denominatorNew);

        numeratorFinal = mulExpr(numeratorNew, denominatorTmp);
        denominatorFinal = numeratorTmp->Clone();
    }
    else
    {
        fprintf(stderr, "\tNOTE: combineFraction: the input denominator is not a fraction.\n");
        numeratorFinal = numeratorNew->Clone();
        denominatorFinal = denominatorNew->Clone();
    }

    return divExpr(numeratorFinal, denominatorFinal);
}

std::vector<std::unique_ptr<ExprAST>> extractItems(const std::unique_ptr<ExprAST>& expr)
{
    fprintf(stderr, "extractItems: start--------\n");
    if(expr == nullptr)
    {
        fprintf(stderr, "\tERROR: extractItems: the input expr is nullptr!\n");
        return {};
    }
    std::string exprType = expr->type();
    std::vector<std::unique_ptr<ExprAST>> items;
    if(exprType != "Binary")
    {  // eg: poly := exp(x)
        std::unique_ptr<ExprAST> exprTmp = expr->Clone();
        items.push_back(std::move(exprTmp));

        // print info for debug
        fprintf(stderr, "\textractItems: expr is monomial\n");
        fprintf(stderr, "\textractItems: No.0: %s\n", PrintExpression(items.at(0)).c_str());
        fprintf(stderr, "extractItems: end----------\n");
        return items;
    }
    BinaryExprAST* binOpPtr = dynamic_cast<BinaryExprAST*>(expr.get());
    char op = binOpPtr->getOp();
    while(op == '+')
    {
        // rhs
        std::unique_ptr<ExprAST>& rhs = binOpPtr->getRHS();
        std::unique_ptr<ExprAST> exprTmp = rhs->Clone();
        items.push_back(std::move(exprTmp));
        // lhs
        std::unique_ptr<ExprAST>& lhs = binOpPtr->getLHS();
        std::string lhsType = lhs->type();
        if(lhsType == "Binary")
        {  // the usual case
            binOpPtr = dynamic_cast<BinaryExprAST*>(lhs.get());
            op = binOpPtr->getOp();
        }
        else
        {
            exprTmp = lhs->Clone();
            items.push_back(std::move(exprTmp));
            break;
        }
    }
    if(op != '+')
    {
        std::unique_ptr<ExprAST> exprTmp = binOpPtr->Clone();
        items.push_back(std::move(exprTmp));
    }
    reverse(items.begin(), items.end());
    // print info for debug
    fprintf(stderr, "\textractItems: expr size = %ld\n", items.size());
    for(long unsigned int i = 0; i < items.size(); i++)
    {
        fprintf(stderr, "\textractItems: No.%lu: %s\n", i, PrintExpression(items[i]).c_str());
    }

    fprintf(stderr, "extractItems: end----------\n");
    return items;
}

// TODO: implement
std::vector<std::unique_ptr<ExprAST>> moveDiv(const std::vector<std::unique_ptr<ExprAST>>& exprs)
{
    fprintf(stderr, "moveDiv: start--------\n");
    std::vector<std::unique_ptr<ExprAST>> items;
    for(long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        // TODO: try to move '/' to the root node equally for each monomial
        items.push_back(std::move(exprTmp));
    }
    // print info for debug
    fprintf(stderr, "\tmoveDiv: expr size = %ld\n", items.size());
    for(long unsigned int i = 0; i < items.size(); i++)
    {
        fprintf(stderr, "\tmoveDiv: No.%lu: %s\n", i, PrintExpression(items[i]).c_str());
    }
    fprintf(stderr, "moveDiv: end----------\n");
    return items;
}

// TODO: implement
std::unique_ptr<ExprAST> mergeFraction(const std::vector<std::unique_ptr<ExprAST>>& exprs)
{
    std::vector<std::unique_ptr<ExprAST>> numerators;
    std::vector<std::unique_ptr<ExprAST>> denominators;
    for(long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        std::unique_ptr<ExprAST> numeratorTmp = nullptr;
        std::unique_ptr<ExprAST> denominatorTmp = nullptr;
        if(isFraction(exprTmp))
        {
            numeratorTmp = getNumerator(exprTmp);
            denominatorTmp = getDenominator(exprTmp);
        }
        else
        {
            numeratorTmp = std::move(exprTmp);
            denominatorTmp = std::make_unique<NumberExprAST>(1);
        }
        numerators.push_back(std::move(numeratorTmp));
        denominators.push_back(std::move(denominatorTmp));
    }
    // demo implementation: return the corresbonding exprAST for the input vector to ensure normal execution
    std::unique_ptr<ExprAST> result = nullptr;
    for(long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = numerators.at(i)->Clone();
        std::unique_ptr<ExprAST> numeratorTmp = nullptr;
        std::unique_ptr<ExprAST> denominatorTmp = nullptr;
        if(i == 0)
        {
            result = std::move(exprTmp);
        }
        else
        {
            result = addExpr(result, exprTmp);
        }
    }
    // a simple example
    // std::unique_ptr<ExprAST> numeratorTmp = numerators.at(0)->Clone();
    // std::unique_ptr<ExprAST> denominatorTmp = denominators.at(0)->Clone();
    // if(isEqual(denominatorTmp, std::make_unique<NumberExprAST>(1)))
    // {
    //     result = std::move(numeratorTmp);
    // }
    // else
    // {
    //     result = divExpr(numeratorTmp, denominatorTmp);
    // }

    return std::move(result);
}

// TODO: check
std::unique_ptr<ExprAST> preprocessInit(const std::unique_ptr<ExprAST>& expr)
{
    std::unique_ptr<ExprAST> exprNew = nullptr;
    if(isFraction(expr))
    {
        exprNew = expr->Clone();
    }
    else
    {
        std::unique_ptr<ExprAST> exprTmp = expandExprWrapper(expr);
        std::vector<std::unique_ptr<ExprAST>> exprs1 = extractItems(exprTmp);
        std::vector<std::unique_ptr<ExprAST>> exprs2 = moveDiv(exprs1);
        exprNew = mergeFraction(exprs2);
    }
    return exprNew;
}

// TODO: check
std::unique_ptr<ExprAST> preprocess(const std::unique_ptr<ExprAST>& expr)
{
    std::unique_ptr<ExprAST> exprNew = preprocessInit(expr);

    if(isFraction(exprNew))
    {
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        std::unique_ptr<ExprAST> numeratorNew = preprocess(numeratorTmp);
        std::unique_ptr<ExprAST> denominatorNew = preprocess(denominatorTmp);
        return combineFraction(numeratorNew, denominatorNew);
    }
    return std::move(exprNew);
}

struct Monomial;
struct funcInfo
{
    std::string callee;
    std::vector<Monomial> args;

    void showInfo() { fprintf(stderr, "\tfuncInfo: callee = %s, args size = %ld;\n", callee.c_str(), args.size()); }
};

struct variableInfo
{
    std::string name;
    int degree;

    void showInfo() { fprintf(stderr, "\tvariableInfo: name = %s; degree = %d\n", name.c_str(), degree); }
};

struct Monomial
{
    double coefficient = 1;
    std::vector<funcInfo> functions;
    std::vector<variableInfo> variables;

    void showInfo()
    {
        fprintf(stderr, "\tcoefficient = %f;\n", coefficient);
        fprintf(stderr, "\tvariables size = %ld;\n", variables.size());
        for(long unsigned int i = 0; i < variables.size(); i++)
        {
            (variables.at(i)).showInfo();
        }
    }
};

std::vector<variableInfo> mergeVariables(std::vector<variableInfo> vec1, std::vector<variableInfo> vec2)
{
    std::vector<variableInfo> vec3;
    for(long unsigned int i = 0; i < vec1.size(); i++)
    {
        variableInfo& varTmp1 = vec1.at(i);
        variableInfo varTmp3 = varTmp1;

        for(long unsigned int j = 0; j < vec2.size(); j++)
        {
            variableInfo& varTmp2 = vec2.at(j);
            if(varTmp1.name == varTmp2.name)
            {
                varTmp3.degree = varTmp1.degree + varTmp2.degree;
                vec2.erase(vec2.begin() + j);
                break;
            }
        }
        vec3.push_back(varTmp3);
    }
    vec3.insert(vec3.end(), vec2.begin(), vec2.end());

    return vec3;
}

Monomial mergeMonomial(const Monomial& mono1, const Monomial& mono2)
{
    Monomial monoFinal;
    double& coefficient = monoFinal.coefficient;
    std::vector<variableInfo>& variables = monoFinal.variables;
    variableInfo variableTmp;

    coefficient = mono1.coefficient * mono2.coefficient;
    variables = mergeVariables(mono1.variables, mono2.variables);

    return monoFinal;
}

Monomial extractInfoKernel(const std::unique_ptr<ExprAST>& expr)
{
    Monomial monoFinal;
    double& coefficient = monoFinal.coefficient;
    std::vector<variableInfo>& variables = monoFinal.variables;
    variableInfo variableTmp;

    std::string exprType = expr->type();
    std::vector<std::unique_ptr<ExprAST>> items;
    if(exprType == "Number")
    {
        // fprintf(stderr, "extractInfoKernel: number\n");
        NumberExprAST* numberPtr = dynamic_cast<NumberExprAST*>(expr.get());
        coefficient = numberPtr->getNumber();
        return monoFinal;
    }
    else if(exprType == "Variable")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        VariableExprAST* variablePtr = dynamic_cast<VariableExprAST*>(expr.get());
        variableTmp.name = variablePtr->getVariable();
        variableTmp.degree = 1;
        variables.push_back(variableTmp);
        return monoFinal;
    }
    if(exprType != "Binary")
    {                                                                                         // eg: poly := exp(x) or x or 2.2
        fprintf(stderr, "extractInfoKernel: expr is neither number, variable, or binary\n");  // print info for debug
        return monoFinal;
    }
    BinaryExprAST* binOpPtr = dynamic_cast<BinaryExprAST*>(expr.get());
    char op = binOpPtr->getOp();
    if(op == '*')
    {
        // fprintf(stderr, "extractInfoKernel: expr op is '*'\n");
        std::unique_ptr<ExprAST>& lhs = binOpPtr->getLHS();
        Monomial monoTmp1 = extractInfoKernel(lhs);
        // monoTmp1.showInfo();
        std::unique_ptr<ExprAST>& rhs = binOpPtr->getRHS();
        Monomial monoTmp2 = extractInfoKernel(rhs);
        // monoTmp2.showInfo();

        monoFinal = mergeMonomial(monoTmp1, monoTmp2);
    }
    else
    {
        fprintf(stderr, "\tERROR: extractInfoKernel: expr contains '+'\n");
    }
    return monoFinal;
}

std::vector<Monomial> extractInfo(const std::vector<std::unique_ptr<ExprAST>>& exprs)
{
    fprintf(stderr, "extractInfo: start--------\n");
    std::vector<Monomial> results;
    for(long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        Monomial monoTmp = extractInfoKernel(exprTmp);
        results.push_back(monoTmp);
    }
    // print information of info
    for(long unsigned int i = 0; i < results.size(); i++)
    {
        fprintf(stderr, "\tThe Monomial No.%lu: \n", i);
        (results.at(i)).showInfo();
    }
    fprintf(stderr, "extractInfo: end----------\n");
    return results;
}

// TODO: implement
std::vector<struct Monomial> mergePolynomial(const std::vector<struct Monomial>& info)
{
    fprintf(stderr, "mergePolynomial: start--------\n");
    fprintf(stderr, "mergePolynomial: end----------\n");
    return {};
}

// TODO: implement
std::unique_ptr<ExprAST> geneExprAST(const std::vector<struct Monomial>& info)
{
    fprintf(stderr, "geneExprAST: start--------\n");
    fprintf(stderr, "geneExprAST: end----------\n");
    return nullptr;
}

// TODO: implement. rewriteExpr should contain poly and math function equal change
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::unique_ptr<ExprAST>& expr)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST>& expr)
{
    std::vector<std::unique_ptr<ExprAST>> items = extractItems(expr);
    std::vector<Monomial> info = extractInfo(items);
    std::vector<Monomial> infoNew = mergePolynomial(info);
    std::unique_ptr<ExprAST> exprNew = geneExprAST(infoNew);
    return rewriteExpr(exprNew);
}

// TODO: implement
std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>>& numerators, std::vector<std::unique_ptr<ExprAST>>& denominators)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> exprAuto(std::unique_ptr<ExprAST>& expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "exprAuto: ERROR: the input expr is nullptr!\n");
        return {};
    }
    fprintf(stderr, "exprAuto: step1: preprocess\n");
    std::unique_ptr<ExprAST> exprNew = preprocess(expr);
    std::vector<std::unique_ptr<ExprAST>> exprsFinal;

    fprintf(stderr, "exprAuto: step2: judge if fraction\n");
    if(isFraction(exprNew))
    {
        fprintf(stderr, "exprAuto: exprNew is a fraction, so perform step3 and step4\n");
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        auto numeratorsFinal = rewriteExprWrapper(numeratorTmp);
        auto denominatorsFinal = rewriteExprWrapper(denominatorTmp);
        exprsFinal = createAll(numeratorsFinal, denominatorsFinal);
    }
    else
    {
        fprintf(stderr, "exprAuto: exprNew is not a fraction, so perform step4\n");
        exprsFinal = rewriteExprWrapper(exprNew);
    }

    fprintf(stderr, "exprAuto: exprsFinal size = %ld\n", exprsFinal.size());
    for(long unsigned int i = 0; i < exprsFinal.size(); i++)
    {
        fprintf(stderr, "exprAuto: No.%lu: %s\n", i, PrintExpression(exprsFinal[i]).c_str());
    }
    return exprsFinal;
}
