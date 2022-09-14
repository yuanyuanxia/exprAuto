#include "basic.hpp"
#include "polyRewrite.hpp"
#include "monoInfo.hpp"
#include "exprAuto.hpp"

#define DEBUG_LEVEL 0
int debugLevel = -1;

template <typename T>
void reverseMine(T *p, size_t size)
{
    for(size_t i = 0; i < size / 2; ++i)
    {
        T temp = p[i];
        p[i] = p[size - 1 - i];
        p[size - 1 - i] = temp;
    }
}

double getCoefficient(std::unique_ptr<ExprAST> &expr)  // useless
{
    if(expr == nullptr)
        return -1;
    std::string exprType = expr->type();
    if(exprType != "Number")
    {
        fprintf(stderr, "getCoefficient: ERROR at the begin about the expr type, shouble be Number!\n");
        return -1;
    }
    NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
    return numberPtr->getNumber();
}

void getTermSingle(BinaryExprAST *binOpPtr, int *term, double *coefficient)
{
    if(binOpPtr == nullptr)
    {
        fprintf(stderr, "getTermSingle: binOpPtr is nullptr!\n");
        return;
    }
    int mulTime = 0;
    char op = binOpPtr->getOp();
    while(op == '*')
    {
        mulTime++;

        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        std::string lhsType = lhs->type();
        if(lhsType == "Binary")
        {
            binOpPtr = dynamic_cast<BinaryExprAST *>(lhs.get());
            op = binOpPtr->getOp();
        }
        else if(lhsType == "Variable")
        {
            *coefficient = 1.0;
            *term = mulTime + 1;
            return;
        }
        else if(lhsType == "Number")
        {  // 到最左边了，应当是一个系数
            NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(lhs.get());
            *coefficient = numberPtr->getNumber();
            *term = mulTime;
            return;
        }
        else
        {
            fprintf(stderr, "getTermSingle: ERROR about the type!\n");
        }
    }
    fprintf(stderr, "getTermSingle: ERROR about the operator, should always be *!\n");
}

std::string getVariableInvStr(std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "getVariableInvStr: expr is nullptr!\n");
        return "NULLPTR";
    }
    std::string exprType = expr->type();
    if(exprType == "Variable")
    {
        VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(expr.get());
        return variablePtr->getVariable();
    }
    else if(exprType == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if(op == '+')
        {
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            std::string rhsType = rhs->type();
            if(rhsType == "Binary")
            {
                binOpPtr = dynamic_cast<BinaryExprAST *>(rhs.get());
                op = binOpPtr->getOp();
            }
            else if(rhsType == "Variable")
            {
                VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(rhs.get());
                return variablePtr->getVariable();
            }
            else
            {
                fprintf(stderr, "getVariableInvStr: ERROR0 about the operator!\n");
                return "ERROR";
            }
        }

        if(op == '*')
        {
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            if(rhs->type() == "Variable")
            {
                VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(rhs.get());
                return variablePtr->getVariable();
            }
            else
            {
                fprintf(stderr, "getVariableInvStr: ERROR0 about the type!\n");
                return "ERROR";
            }
        }
        else
        {
            fprintf(stderr, "getVariableInvStr: ERROR1 about the operator!\n");
            return "ERROR";
        }
    }
    else
    {
        fprintf(stderr, "getVariableInvStr: ERROR1 about the type!\n");
        return "ERROR";
    }
}

std::string getVariableStr(const std::unique_ptr<ExprAST> &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "getVariableStr: expr is nullptr!\n");
        return "NULLPTR";
    }
    std::string exprType = expr->type();
    if(exprType == "Number")
    {
        return "CONSTANT";
    }
    else if(exprType == "Variable")
    {
        VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(expr.get());
        return variablePtr->getVariable();
    }
    else if(exprType == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if(op == '+')
        {
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            std::string rhsType = rhs->type();
            if(rhsType == "Binary")
            {
                binOpPtr = dynamic_cast<BinaryExprAST *>(rhs.get());
                op = binOpPtr->getOp();
            }
            else if(rhsType == "Variable")
            {
                VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(rhs.get());
                return variablePtr->getVariable();
            }
            else if(rhsType == "Number")
            {  // !!! the poly order is inverse, such as "x*x*x+x*x+1"
                std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
                return getVariableInvStr(lhs);
            }
            else
            {
                fprintf(stderr, "getVariableStr: ERROR0 about the operator!\n");
                return "ERROR";
            }
        }

        if(op == '*')
        {
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            if(rhs->type() == "Variable")
            {
                VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(rhs.get());
                return variablePtr->getVariable();
            }
            else
            {
                fprintf(stderr, "getVariableStr: ERROR0 about the type!\n");
                return "ERROR";
            }
        }
        else
        {
            fprintf(stderr, "getVariableStr: ERROR1 about the operator!\n");
            return "ERROR";
        }
    }
    else
    {
        fprintf(stderr, "getVariableStr: ERROR1 about the type!\n");
        return "ERROR";
    }
}

// 获取多项式的系数和阶数
void getReady(const std::unique_ptr<ExprAST> &expr, std::string *variablePtr, int *term, double *coefficient, size_t *lenPtr)
{
    if(expr == nullptr)
        return;
    *variablePtr = getVariableStr(expr);

    std::string exprType = expr->type();
    if(exprType == "Number")
    {  // eg: poly := 3.2
        NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
        *coefficient = numberPtr->getNumber();
        *term = 0;
        *lenPtr = 1;
        return;
    }
    else if(exprType == "Variable")
    {  // eg: poly := x
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        *variablePtr = varPtr->getVariable();
        *coefficient = 1;
        *term = 1;
        *lenPtr = 1;
        return;
    }
    else if(exprType != "Binary")
    {  // eg: poly := exp(x)
        fprintf(stderr, "getReady: NOT polynomial!\n");
        return;
    }

    size_t plusTime = 0;
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    while(op == '+')
    {
        // rhs
        std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
        BinaryExprAST *binOpRPtr = nullptr;
        if(rhs->type() == "Variable")
        {  // eg: poly := 1 + x OR the "1+x" part in "poly := 1 + x + 3 * x * x"
            *(coefficient + plusTime) = 1;
            *(term + plusTime) = 1;
        }
        else if(rhs->type() == "Binary")
        {  // the usual case
            binOpRPtr = dynamic_cast<BinaryExprAST *>(rhs.get());
            getTermSingle(binOpRPtr, term + plusTime, coefficient + plusTime);
        }
        else if(rhs->type() == "Number")
        {
            NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(rhs.get());
            *(coefficient + plusTime) = numberPtr->getNumber();
            *(term + plusTime) = 0;
        }
        else
        {  // exception happens
            fprintf(stderr, "getReady: ERROR about poly term, please check the expr!\n");
            return;
        }

        plusTime++;

        // lhs
        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        std::string lhsType = lhs->type();
        if(lhsType == "Binary")
        {  // the usual case
            binOpPtr = dynamic_cast<BinaryExprAST *>(lhs.get());
            op = binOpPtr->getOp();
        }
        else if(lhsType == "Number")
        {  // eg: poly := 1 + others
            NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(lhs.get());
            *(coefficient + plusTime) = numberPtr->getNumber();
            *(term + plusTime) = 0;
            *lenPtr = plusTime + 1;
            reverseMine(coefficient, *lenPtr);
            reverseMine(term, *lenPtr);
            return;
        }
        else if(lhsType == "Variable")
        {  // eg: poly := 1 + others
            *(coefficient + plusTime) = 1;
            *(term + plusTime) = 1;
            *lenPtr = plusTime + 1;
            reverseMine(coefficient, *lenPtr);
            reverseMine(term, *lenPtr);
            return;
        }
        else
        {  // handle exception
            fprintf(stderr, "getReady: ERROR about poly type, please check the expr!\n");
            return;
        }
    }
    if(op == '*')
    {  // op != '+'，说明没有常数项，最左边是a*x^n
        getTermSingle(binOpPtr, term + plusTime, coefficient + plusTime);
        *lenPtr = plusTime + 1;
        reverseMine(coefficient, *lenPtr);
        reverseMine(term, *lenPtr);
        return;
    }
    else
    {
        fprintf(stderr, "getReady: UNKNOWN CHAR: %c\n", op);
        *variablePtr = "none";
        *lenPtr = 0;
        return;
    }
}

std::unique_ptr<ExprAST> createSingle(const std::string variable, const int term, const double coefficient, std::unique_ptr<ExprAST> expr)
{
    std::unique_ptr<ExprAST> exprNew = std::make_unique<NumberExprAST>(coefficient);
    for(int i = 0; i < term; i++)
    {
        auto rhs = std::make_unique<VariableExprAST>(variable);
        exprNew = std::make_unique<BinaryExprAST>('*', std::move(exprNew), std::move(rhs));
    }

    std::string exprType = expr->type();
    if(exprType == "Base")
    {
        return exprNew;
    }
    else
    {
        return std::make_unique<BinaryExprAST>('+', std::move(expr), std::move(exprNew));
    }
}

std::unique_ptr<ExprAST> createBA(const std::string variable, const int *term, const double *coefficient, int len)
{
    if(len == 0)
        return nullptr;

    auto expr = std::make_unique<ExprAST>();
    for(int i = 0; i < len; i++)
    {
        expr = createSingle(variable, term[i], coefficient[i], std::move(expr));
    }
    return expr;
}

std::unique_ptr<ExprAST> createSingle(const std::string variable, const int term, const double coefficient, const monoInfo &monomial, std::unique_ptr<ExprAST> &expr)
{
    std::unique_ptr<ExprAST> exprNew = nullptr;
    exprNew = std::make_unique<NumberExprAST>(coefficient);

    const std::vector<funcInfo> &functions = monomial.functions;
    const polyInfo &poly = monomial.poly;

    std::unique_ptr<ExprAST> polyAST = nullptr;
    for (const auto &mono : poly.monos)
    {
        std::unique_ptr<ExprAST> monoAST = geneMonomialAST(mono);
        polyAST = addExpr(polyAST, monoAST);
    }
    exprNew = mulExpr(exprNew, polyAST);

    for (const auto &func : functions)
    {
        std::unique_ptr<ExprAST> funcAST = geneFunctionAST(func);
        exprNew = mulExpr(exprNew, funcAST);
    }

    for(int i = 0; i < term; i++)
    {
        std::unique_ptr<ExprAST> rhs = std::make_unique<VariableExprAST>(variable);
        exprNew = mulExpr(exprNew, rhs);
    }

    return addExpr(expr, exprNew);
}

std::unique_ptr<ExprAST> createBA(const std::string variable, const int *term, const double *coefficient, const std::vector<monoInfo> &monomials, int len)
{
    if(len == 0)
        return nullptr;

    std::unique_ptr<ExprAST> expr = nullptr;
    for(int i = 0; i < len; i++)
    {
        expr = createSingle(variable, term[i], coefficient[i], monomials.at(i), expr);
    }
    return expr;
}

std::unique_ptr<ExprAST> createContinuedMul(const std::string variable, const int commonDegree)
{
    if(commonDegree == 0)
        return nullptr;

    std::unique_ptr<ExprAST> expr = std::make_unique<VariableExprAST>(variable);
    for(int i = 1; i < commonDegree; i++)
    {
        auto rhs = std::make_unique<VariableExprAST>(variable);
        expr = std::make_unique<BinaryExprAST>('*', std::move(expr), std::move(rhs));
    }

    return expr;
}

std::vector<std::unique_ptr<ExprAST>> joinExpr(std::unique_ptr<ExprAST> exprBefore, std::unique_ptr<ExprAST> common,
                                               std::vector<std::unique_ptr<ExprAST>> exprMiddles, std::unique_ptr<ExprAST> exprAfter)
{
    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    for(long unsigned int i = 0; i < exprMiddles.size(); i++)
    {
        auto exprMiddle = std::move(exprMiddles.at(i));
        if(exprMiddle == nullptr)
        {
            fprintf(stderr, "exprMiddles[%lu] is nullptr, please check!\n", i);
            return exprsFinal;
        }
        std::unique_ptr<ExprAST> expr = std::move(exprMiddle);

        if(common != nullptr)
        {
            std::unique_ptr<ExprAST> commonClone = common->Clone();
            expr = std::make_unique<BinaryExprAST>('*', std::move(commonClone), std::move(expr));
        }
#ifdef DEBUG
        else
        {
            fprintf(stderr, "joinExpr: common is nullptr.\n");
        }
#endif

        if(exprBefore != nullptr)
        {
            std::unique_ptr<ExprAST> exprBeforeClone = exprBefore->Clone();
            expr = std::make_unique<BinaryExprAST>('+', std::move(exprBeforeClone), std::move(expr));
        }
#ifdef DEBUG
        else
        {
            fprintf(stderr, "joinExpr: exprBefore is nullptr.\n");
        }
#endif

        if(exprAfter != nullptr)
        {
            std::unique_ptr<ExprAST> exprAfterClone = exprAfter->Clone();
            expr = std::make_unique<BinaryExprAST>('+', std::move(expr), std::move(exprAfterClone));
        }
#ifdef DEBUG
        else
        {
            fprintf(stderr, "joinExpr: exprAfter is nullptr.\n");
        }
#endif

        exprsFinal.push_back(std::move(expr));
    }
    return exprsFinal;
}

std::vector<std::unique_ptr<ExprAST>> createMiddle(const std::string variable, const int *term, const double *coefficient, const int len)
{
    debugLevel++;

    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    auto exprOrigin = createBA(variable, term, coefficient, len);
    exprsFinal.push_back(std::move(exprOrigin));

    int *termNew = (int *)malloc(sizeof(int) * len);

    if(debugLevel < DEBUG_LEVEL)
        fprintf(stderr, "Debug level %d: createMiddle: before for, exprsFinal.size() = %lu\n", debugLevel, exprsFinal.size());

    for(int start = 0; start < len - 1; start++)
    {
        for(int end = start + 1; end < len; end++)
        {
            // prepare
            bool flag = false;
            for(int i = start; i <= end; i++)
            {
                int termTmp = term[i] - 1;
                if(termTmp < 0)
                {
                    flag = true;
                    break;
                }
                termNew[i - start] = termTmp;
            }
            if(flag)
                break;

            if(debugLevel < DEBUG_LEVEL)
                fprintf(stderr, "Debug level %d: start = %d, end = %d, len = %d\n", debugLevel, start, end, len);

            // exprsTmp := exprBeforeI + common * exprMiddles + exprAfterK
            auto exprBeforeI = createBA(variable, term, coefficient, start);
            auto common = std::make_unique<VariableExprAST>(variable);
            // auto common = createContinuedMul(variable, 1); // overqualified
            std::vector<std::unique_ptr<ExprAST>> exprMiddles = createMiddle(variable, termNew, coefficient + start, (end - start + 1));
            auto exprAfterK = createBA(variable, term + end + 1, coefficient + end + 1, len - end - 1);
            std::vector<std::unique_ptr<ExprAST>> exprsTmp;
            exprsTmp = joinExpr(std::move(exprBeforeI), std::move(common), std::move(exprMiddles), std::move(exprAfterK));

            // exprsFinal += exprsTmp
            for(long unsigned int i = 0; i < exprsTmp.size(); i++)
            {
                auto exprTmp = std::move(exprsTmp.at(i));
                if(debugLevel < DEBUG_LEVEL)
                    fprintf(stderr, "Debug level %d: createMiddle: No.%lu %s\n", debugLevel, exprsFinal.size(), PrintExpression(exprTmp).c_str());
                exprsFinal.push_back(std::move(exprTmp));
            }

            if(debugLevel < DEBUG_LEVEL)
                fprintf(stderr, "Debug level %d: createMiddle: exprsTmp.size() = %lu, exprsFinal.size() = %lu\n", debugLevel, exprsTmp.size(),
                        exprsFinal.size());
        }
    }
    free(termNew);

    if(debugLevel < DEBUG_LEVEL)
        fprintf(stderr, "Debug level %d: createMiddle: after for, exprsFinal.size() = %lu\n", debugLevel, exprsFinal.size());

    debugLevel--;
    return exprsFinal;
}

std::vector<std::unique_ptr<ExprAST>> createMiddle(const std::string variable, const int *term, const double *coefficient, const std::vector<monoInfo> &monomials, const int len)
{
    debugLevel++;

    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    auto exprOrigin = createBA(variable, term, coefficient, monomials, len);
    exprsFinal.push_back(std::move(exprOrigin));

    int *termNew = (int *)malloc(sizeof(int) * len);

    if(debugLevel < DEBUG_LEVEL)
        fprintf(stderr, "Debug level %d: createMiddle: before for, exprsFinal.size() = %lu\n", debugLevel, exprsFinal.size());

    for(int start = 0; start < len - 1; start++)
    {
        for(int end = start + 1; end < len; end++)
        {
            // prepare
            bool flag = false;
            for(int i = start; i <= end; i++)
            {
                int termTmp = term[i] - 1;
                if(termTmp < 0)
                {
                    flag = true;
                    break;
                }
                termNew[i - start] = termTmp;
            }
            if(flag)
                break;

            if(debugLevel < DEBUG_LEVEL)
                fprintf(stderr, "Debug level %d: start = %d, end = %d, len = %d\n", debugLevel, start, end, len);

            // exprsTmp := exprBeforeI + common * exprMiddles + exprAfterK
            
            std::vector<monoInfo> monomialsNew;
            monomialsNew.insert(monomialsNew.end(), monomials.begin(), monomials.begin() + start);
            auto exprBeforeI = createBA(variable, term, coefficient, monomialsNew, start);
            
            auto common = std::make_unique<VariableExprAST>(variable);
            // auto common = createContinuedMul(variable, 1); // overqualified
            
            monomialsNew.clear();
            monomialsNew.insert(monomialsNew.end(), monomials.begin() + start, monomials.begin() + end + 1);
            std::vector<std::unique_ptr<ExprAST>> exprMiddles = createMiddle(variable, termNew, coefficient + start, monomialsNew, (end + 1 - start));
            
            monomialsNew.clear();
            monomialsNew.insert(monomialsNew.end(), monomials.begin() + end + 1, monomials.end());
            auto exprAfterK = createBA(variable, term + end + 1, coefficient + end + 1, monomialsNew, len - (end + 1));
            std::vector<std::unique_ptr<ExprAST>> exprsTmp;
            
            exprsTmp = joinExpr(std::move(exprBeforeI), std::move(common), std::move(exprMiddles), std::move(exprAfterK));

            // exprsFinal += exprsTmp
            for(size_t i = 0; i < exprsTmp.size(); i++)
            {
                auto exprTmp = std::move(exprsTmp.at(i));
                if(debugLevel < DEBUG_LEVEL)
                    fprintf(stderr, "Debug level %d: createMiddle: No.%lu %s\n", debugLevel, exprsFinal.size(), PrintExpression(exprTmp).c_str());
                exprsFinal.push_back(std::move(exprTmp));
            }

            if(debugLevel < DEBUG_LEVEL)
                fprintf(stderr, "Debug level %d: createMiddle: exprsTmp.size() = %lu, exprsFinal.size() = %lu\n", debugLevel, exprsTmp.size(),
                        exprsFinal.size());
        }
    }
    free(termNew);

    if(debugLevel < DEBUG_LEVEL)
        fprintf(stderr, "Debug level %d: createMiddle: after for, exprsFinal.size() = %lu\n", debugLevel, exprsFinal.size());

    debugLevel--;
    return exprsFinal;
}

std::vector<std::unique_ptr<ExprAST>> createExpr(const std::unique_ptr<ExprAST> &exprInit)
{
    std::string variable = "z";
    int term[] = {0, 1, 2, 3, 4, 5};
    double coefficient[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    size_t len = 1;
    getReady(exprInit, &variable, term, coefficient, &len);

    // for(size_t i = 0; i < len; i++)
    // {
    //     fprintf(stderr, "createExpr: %lu: %d, %f\n", i, term[i], coefficient[i]);
    // }
    // fprintf(stderr, "createExpr: %d %s\n", len, variable.c_str());

    auto exprsFinal = createMiddle(variable, term, coefficient, len);

    fprintf(stderr, "\texprsFinal: %ld\n", exprsFinal.size());
    for(long unsigned int i = 0; i < exprsFinal.size(); i++)
    {
        fprintf(stderr, "\tpolyRewrite: No.%lu: %s\n", i, PrintExpression(exprsFinal[i]).c_str());
    }

    return exprsFinal;
}

void getReady(const std::vector<monoInfo> &monomials, std::string *variablePtr, int *term, double *coefficient, size_t *lenPtr)
{
    bool variableFlags = true;
    *variablePtr = "novariable";
    *lenPtr = monomials.size();
    for (size_t i = 0; i < *lenPtr; i++)
    {
        const monoInfo &monomial = monomials.at(i);
        if(monomial.variables.size() == 0)
        {
            term[i] = 0;
            coefficient[i] = monomial.coefficient;
        }
        else if(monomial.variables.size() == 1)
        {
            term[i] = monomial.variables.at(0).degree;
            coefficient[i] = monomial.coefficient;
            if(variableFlags)
            {
                *variablePtr = monomial.variables.at(0).name;
                variableFlags = false;
            }
        }
        else
        {
            fprintf(stderr, "ERROR: we can not support multi variables!\n");
            exit(1);
        }
    }
        
}

std::vector<std::unique_ptr<ExprAST>> createExpr(const std::vector<monoInfo> &monomials)
{
    std::string variable = "z";
    int term[] = {0, 1, 2, 3, 4, 5};
    double coefficient[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    size_t len = 1;
    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    getReady(monomials, &variable, term, coefficient, &len);
    // getReady(exprInit, &variable, term, coefficient, &len);

    // for(size_t i = 0; i < len; i++)
    // {
    //     fprintf(stderr, "createExpr: %lu: %d, %f\n", i, term[i], coefficient[i]);
    // }
    // fprintf(stderr, "createExpr: length = %lu, variable = %s\n", len, variable.c_str());
    if (len == 1)
    {
        auto tmp = geneMonomialAST(monomials.at(0));
        exprsFinal.push_back(std::move(tmp));
    }
    else
    {
        exprsFinal = createMiddle(variable, term, coefficient, monomials, len);
    }

    fprintf(stderr, "\tcreateExpr: exprsFinal: %ld\n", exprsFinal.size());
    for(long unsigned int i = 0; i < exprsFinal.size(); i++)
    {
        fprintf(stderr, "\tcreateExpr: No.%lu: %s\n", i, PrintExpression(exprsFinal[i]).c_str());
    }

    return exprsFinal;
}