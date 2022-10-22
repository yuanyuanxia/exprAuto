#include "polyRewrite.hpp"
#include "exprAuto.hpp"
#include "geneExpr.hpp"
#include "geneCode.hpp"
#include <cmath>
#include <math.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

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

double getCoefficient(ast_ptr &expr)  // useless
{
    if(expr == nullptr)
        return -1;
    string exprType = expr->type();
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

        ast_ptr &lhs = binOpPtr->getLHS();
        string lhsType = lhs->type();
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

string getVariableInvStr(ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "getVariableInvStr: expr is nullptr!\n");
        return "NULLPTR";
    }
    string exprType = expr->type();
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
            ast_ptr &rhs = binOpPtr->getRHS();
            string rhsType = rhs->type();
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
            ast_ptr &rhs = binOpPtr->getRHS();
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

string getVariableStr(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "getVariableStr: expr is nullptr!\n");
        return "NULLPTR";
    }
    string exprType = expr->type();
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
            ast_ptr &rhs = binOpPtr->getRHS();
            string rhsType = rhs->type();
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
                ast_ptr &lhs = binOpPtr->getLHS();
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
            ast_ptr &rhs = binOpPtr->getRHS();
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
void getReady(const ast_ptr &expr, string *variablePtr, int *term, double *coefficient, size_t *lenPtr)
{
    if(expr == nullptr)
        return;
    *variablePtr = getVariableStr(expr);

    string exprType = expr->type();
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
        ast_ptr &rhs = binOpPtr->getRHS();
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
        ast_ptr &lhs = binOpPtr->getLHS();
        string lhsType = lhs->type();
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

ast_ptr createSingle(const string variable, const int term, const double coefficient, ast_ptr expr)
{
    ast_ptr exprNew = makePtr<NumberExprAST>(coefficient);
    for(int i = 0; i < term; i++)
    {
        auto rhs = makePtr<VariableExprAST>(variable);
        exprNew = makePtr<BinaryExprAST>('*', std::move(exprNew), std::move(rhs));
    }

    string exprType = expr->type();
    if(exprType == "Base")
    {
        return exprNew;
    }
    else
    {
        return makePtr<BinaryExprAST>('+', std::move(expr), std::move(exprNew));
    }
}

ast_ptr createBA(const string variable, const int *term, const double *coefficient, int len)
{
    if(len == 0)
        return nullptr;

    auto expr = makePtr<ExprAST>();
    for(int i = 0; i < len; i++)
    {
        expr = createSingle(variable, term[i], coefficient[i], std::move(expr));
    }
    return expr;
}

ast_ptr createSingle(const string variable, const int term, const double coefficient, const monoInfo &monomial, ast_ptr &expr)
{
    double num = coefficient;
    double r;
    do{
        r = fmod(num,2.0);
        num /= 2;
    } while(r==0 &&num > 0);
    ast_ptr exprNew = nullptr;
    ast_ptr specialNum = nullptr;
    if(coefficient == 1 || coefficient == -1 || num == 0)
    {
        specialNum = makePtr<NumberExprAST>(coefficient);
    }else{
        exprNew = makePtr<NumberExprAST>(coefficient);
    }

    if(term > 0){
        ast_ptr var = makePtr<VariableExprAST>(variable);
        for(int i = 1; i < term; i++)
        {
            ast_ptr rhs = makePtr<VariableExprAST>(variable);
            var = mulExpr(var, rhs);
        }
        if(exprNew == nullptr){
            exprNew = std::move(var);
        }else{
            exprNew = mulExpr(exprNew, var);
        }
    }
    
    if((int)monomial.variables.size() > 0){
        ast_ptr otherVar = nullptr;
        for(int i = 0;i < (int)monomial.variables.size() ;i++){
            if(monomial.variables.at(i).name != variable){
                ast_ptr other = makePtr<VariableExprAST>(monomial.variables.at(i).name);
                for(int j = 1; j < monomial.variables.at(i).degree; j++){
                    ast_ptr rhs = makePtr<VariableExprAST>(monomial.variables.at(i).name);
                    other = mulExpr(other, rhs);
                }
                if(otherVar == nullptr){
                    otherVar = std::move(other);
                }else{
                    otherVar = mulExpr(otherVar, other);
                }
            }
        }
        if(otherVar != nullptr){
            if(exprNew == nullptr){
                exprNew = std::move(otherVar);
            }else{
                exprNew = mulExpr(exprNew, otherVar);
            }
        }
    }

    ast_ptr polyAST = nullptr;
    const auto &poly = monomial.poly;
    const auto &monoFracs = poly.monoFracs;
    for (const auto &monoFrac : monoFracs)
    {
        ast_ptr monoAST = geneMonomialAST(monoFrac);
        polyAST = addExpr(polyAST, monoAST);
    }
    if (polyAST != nullptr)
    {
        if(exprNew == nullptr)
        {
            exprNew = std::move(polyAST);
        }
        else
        {
            exprNew = mulExpr(exprNew, polyAST);
        }
    }

    const vector<funcInfo> &functions = monomial.functions;
    if((int)functions.size()>0){
        auto firstfunc = functions.at(0);
        ast_ptr func = geneFunctionAST(firstfunc);
        for (int i = 1;i<(int)functions.size();i++)
        {
            const auto &fun = functions.at(i);
            ast_ptr funcAST = geneFunctionAST(fun);
            func = mulExpr(func, funcAST);
        }
        if(exprNew == nullptr){
            exprNew = std::move(func);
        }else{
            exprNew = mulExpr(exprNew, func);
        }
    }

    if(specialNum != nullptr){
        if(exprNew != nullptr){
            if(coefficient != 1)
                exprNew = mulExpr(specialNum, exprNew);
        }else{
            exprNew = std::move(specialNum);
        }
    }
    return addExpr(expr, exprNew);
}

ast_ptr createBA(const string variable, const int *term, const double *coefficient, const vector<monoInfo> &monomials, int len)
{
    if(len == 0)
        return nullptr;

    ast_ptr expr = nullptr;
    for(int i = 0; i < len; i++)
    {
        expr = createSingle(variable, term[i], coefficient[i], monomials.at(i), expr);
    }
    return expr;
}

ast_ptr createContinuedMul(const string variable, const int commonDegree)
{
    if(commonDegree == 0)
        return nullptr;

    ast_ptr expr = makePtr<VariableExprAST>(variable);
    for(int i = 1; i < commonDegree; i++)
    {
        auto rhs = makePtr<VariableExprAST>(variable);
        expr = makePtr<BinaryExprAST>('*', std::move(expr), std::move(rhs));
    }
    return expr;
}

vector<ast_ptr> joinExpr(ast_ptr exprBefore, ast_ptr common,
                                               vector<ast_ptr> exprMiddles, ast_ptr exprAfter)
{
    vector<ast_ptr> exprsFinal;
    for(long unsigned int i = 0; i < exprMiddles.size(); i++)
    {
        auto exprMiddle = std::move(exprMiddles.at(i));
        if(exprMiddle == nullptr)
        {
            fprintf(stderr, "exprMiddles[%lu] is nullptr, please check!\n", i);
            return exprsFinal;
        }
        ast_ptr expr = std::move(exprMiddle);

        if(common != nullptr)
        {
            ast_ptr commonClone = common->Clone();
            expr = makePtr<BinaryExprAST>('*', std::move(commonClone), std::move(expr));
        }
#ifdef DEBUG
        else
        {
            fprintf(stderr, "joinExpr: common is nullptr.\n");
        }
#endif

        if(exprBefore != nullptr)
        {
            ast_ptr exprBeforeClone = exprBefore->Clone();
            expr = makePtr<BinaryExprAST>('+', std::move(exprBeforeClone), std::move(expr));
        }
#ifdef DEBUG
        else
        {
            fprintf(stderr, "joinExpr: exprBefore is nullptr.\n");
        }
#endif

        if(exprAfter != nullptr)
        {
            ast_ptr exprAfterClone = exprAfter->Clone();
            expr = makePtr<BinaryExprAST>('+', std::move(expr), std::move(exprAfterClone));
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

vector<ast_ptr> createMiddle(const string variable, const int *term, const double *coefficient, const int len)
{
    debugLevel++;

    vector<ast_ptr> exprsFinal;
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
            auto common = makePtr<VariableExprAST>(variable);
            // auto common = createContinuedMul(variable, 1); // overqualified
            vector<ast_ptr> exprMiddles = createMiddle(variable, termNew, coefficient + start, (end - start + 1));
            auto exprAfterK = createBA(variable, term + end + 1, coefficient + end + 1, len - end - 1);
            vector<ast_ptr> exprsTmp;
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

vector<ast_ptr> createMiddle(const string variable, const int *term, const double *coefficient, const vector<monoInfo> &monomials, const int len)
{
    debugLevel++;

    vector<ast_ptr> exprsFinal;
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
            vector<monoInfo> monomialsNew;
            monomialsNew.insert(monomialsNew.end(), monomials.begin(), monomials.begin() + start);
            auto exprBeforeI = createBA(variable, term, coefficient, monomialsNew, start);
            
            auto common = makePtr<VariableExprAST>(variable);
            // auto common = createContinuedMul(variable, 1); // overqualified
            
            monomialsNew.clear();
            monomialsNew.insert(monomialsNew.end(), monomials.begin() + start, monomials.begin() + end + 1);
            vector<ast_ptr> exprMiddles = createMiddle(variable, termNew, coefficient + start, monomialsNew, (end + 1 - start));
            
            monomialsNew.clear();
            monomialsNew.insert(monomialsNew.end(), monomials.begin() + end + 1, monomials.end());
            auto exprAfterK = createBA(variable, term + end + 1, coefficient + end + 1, monomialsNew, len - (end + 1));
            vector<ast_ptr> exprsTmp;
            
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

vector<ast_ptr> createExpr(const ast_ptr &exprInit)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "createExpr: ";

    string variable = "z";
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

    cout << prompt << "exprsFinal: " << exprsFinal.size() << endl;
    printExprs(exprsFinal, prompt);
    callCount--;
    callLevel--;
    return exprsFinal;
}

void getReady(const vector<monoInfo> &monomials, string variable, int *term, double *coefficient, size_t *lenPtr)
{
    *lenPtr = monomials.size();
    for (size_t i = 0; i < *lenPtr; i++)
    {
        const monoInfo &monomial = monomials.at(i);
        if(monomial.variables.size() == 0)
        {
            term[i] = 0;
            coefficient[i] = monomial.coefficient;
        }
        for (int j = 0;j < (int)monomial.variables.size();j++){
            if(variable.compare(monomial.variables.at(j).name) == 0){
                term[i] = monomial.variables.at(j).degree;
                break;
            }
        }
        coefficient[i] = monomial.coefficient;
    }
}

bool findVarFromMonoinfo(string variable ,monoInfo target){
    for(int i = 0;i < (int)target.variables.size();i++){
        if(variable == target.variables.at(i).name){
            return true;
        }
    }
    return false;
}

vector<monoInfo> sortMonomials(string variable ,const vector<monoInfo> &monomials){
    vector<monoInfo> primary;
    vector<monoInfo> secondary;
    int j = 0;
    int good = 0;
    while(!findVarFromMonoinfo(variable ,monomials.at(j++))){
        good++;
    }
    j = 0;
    while(j!=good){
        primary.push_back(monomials.at(j++));
    }

    for (int i = good;i<(int)monomials.size();i++){
        if(findVarFromMonoinfo(variable ,monomials.at(i))){
            primary.push_back(monomials.at(i));
        }else{
            secondary.push_back(monomials.at(i));
        }
    }
    primary.insert(primary.end(),secondary.begin(),secondary.end());
    return primary;
}

vector<ast_ptr> createExpr(const vector<monoInfo> &monomials)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "createExpr: ";

    string variable = "z";
    size_t len = 1;
    vector<ast_ptr> exprsFinal;
    vector<ast_ptr> exprsFinalTMP;
    len = monomials.size();
 
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
        vector<string> vars;
        auto tmp = geneExprAST(monomials);
        getVariablesFromExpr(tmp, vars);
        int flag = 0;    // the key to control the rewrite results. 'flag == 0' is much,'flag == 1' is little.
        for(int i = 0; i < (int)vars.size();i++){
            int term[10] = {0}; // {0, 1, 2, 3, 4, 5};
            double coefficient[10] = {0.0}; // {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
            variable = vars.at(i);

            vector<monoInfo> sortAftermonomials = sortMonomials(variable,monomials);
            getReady(sortAftermonomials, variable, term, coefficient, &len);
            exprsFinalTMP = createMiddle(variable, term, coefficient, sortAftermonomials, len);

            if(i > 0 && flag == 1){
                vector<ast_ptr>::iterator k = exprsFinalTMP.begin();
                exprsFinalTMP.erase(k);
            }
            mineAppend(exprsFinal,exprsFinalTMP);
            exprsFinalTMP.clear();
        }
    }
    cout << prompt << "exprsFinal: " << exprsFinal.size() << endl;
    printExprs(exprsFinal, prompt);
    callCount--;
    callLevel--;
    return exprsFinal;
}

vector<ast_ptr> createExpr(const vector<monoFracInfo> &monoFracs)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "createExpr: ";

    vector<monoInfo> numeratorInfos;
    vector<monoInfo> denominatorInfos;
    for(const auto &monoFrac: monoFracs)
    {
        numeratorInfos.push_back(monoFrac.numerator);
        denominatorInfos.push_back(monoFrac.denominator);
    }
    auto numerators = createExpr(numeratorInfos);
    auto denominators = createExpr(denominatorInfos);
    auto exprsFinal = createAll(numerators, denominators);

    cout << prompt << "exprsFinal: " << exprsFinal.size() << endl;
    printExprs(exprsFinal, prompt);
    callCount--;
    callLevel--;
    return exprsFinal;
}
