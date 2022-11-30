#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <fmt/ranges.h>

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
#include "color.hpp"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

int callLevel{-1};
const char callLevelChar{' '};
const char callCountChar{'>'};
const size_t promtTimes = 2;

// conbine the constants in expr
void combineConstant(ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: combineConstant: the input expression is nullptr!\n");
        exit(EXIT_FAILURE);
    }

    if(expr->type() == "Call")
    {
        CallExprAST *exprTmp = dynamic_cast<CallExprAST *>(expr.get());
		
        auto &args = exprTmp->getArgs();
        for(size_t i = 0; i < args.size(); ++i)
        {
            combineConstant(args.at(i));
        }
        return;
    }

    if(expr->type() != "Binary")
    {
        return;
    }

    BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOp->getOp();

    ast_ptr &lhs = binOp->getLHS();
    ast_ptr &rhs = binOp->getRHS();

    combineConstant(lhs);
    combineConstant(rhs);

    if((lhs->type() == "Number") && (rhs->type() != "Number"))
    {
        NumberExprAST *numberPtrL = dynamic_cast<NumberExprAST *>(lhs.get());
        auto numberL = numberPtrL->getNumber();
        if(numberL == 0)
        {
            if(op == '+')
            {
                expr = std::move(rhs);
            }
            else if (op == '-')
            {
                lhs = makePtr<NumberExprAST>(-1);
                binOp->setOp('*'); 
            }
            else if (op == '*')
            {
                expr = makePtr<NumberExprAST>(0);
            }
            else if (op == '/')
            {
                expr = makePtr<NumberExprAST>(0);
            }
            else
            {
                fprintf(stderr, "\tERROR: combineConstant: expr contains '%c'\n", op);
                exit(EXIT_FAILURE);
            }
        }
        else if (numberL == 1)
        {
            if(op == '+')
            {
                ;
            }
            else if (op == '-')
            {
                ; 
            }
            else if (op == '*')
            {
                expr = std::move(rhs);
            }
            else if (op == '/')
            {
                ;
            }
            else
            {
                fprintf(stderr, "\tERROR: combineConstant: expr contains '%c'\n", op);
                exit(EXIT_FAILURE);
            }
        }
    }
    else if((lhs->type() != "Number") && (rhs->type() == "Number"))
    {
        NumberExprAST *numberPtrR = dynamic_cast<NumberExprAST *>(rhs.get());
        auto numberR = numberPtrR->getNumber();
        if(numberR == 0)
        {
            if(op == '+')
            {
                expr = std::move(lhs);
            }
            else if (op == '-')
            {
                expr = std::move(lhs);
            }
            else if (op == '*')
            {
                expr = makePtr<NumberExprAST>(0);
            }
            else if (op == '/')
            {
                fprintf(stderr, "\tERROR: combineConstant: denominator is zero!\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr, "\tERROR: combineConstant: expr contains '%c'\n", op);
                exit(EXIT_FAILURE);
            }
        }
        else if (numberR == 1)
        {
            if(op == '+')
            {
                ;
            }
            else if (op == '-')
            {
                ; 
            }
            else if (op == '*')
            {
                expr = std::move(lhs);
            }
            else if (op == '/')
            {
                expr = std::move(lhs);
            }
            else
            {
                fprintf(stderr, "\tERROR: combineConstant: expr contains '%c'\n", op);
                exit(EXIT_FAILURE);
            }
        }
    }
    else if((lhs->type() == "Number") && (rhs->type() == "Number"))
    {
        NumberExprAST *numberPtrL = dynamic_cast<NumberExprAST *>(lhs.get());
        NumberExprAST *numberPtrR = dynamic_cast<NumberExprAST *>(rhs.get());
        auto numberL = numberPtrL->getNumber();
        auto numberR = numberPtrR->getNumber();
        if(op == '+')
        {
            auto result = numberL + numberR;
            expr = makePtr<NumberExprAST>(result);
        }
        else if (op == '-')
        {
            auto result = numberL - numberR;
            expr = makePtr<NumberExprAST>(result);
        }
        else if (op == '*')
        {
            auto result = numberL * numberR;
            expr = makePtr<NumberExprAST>(result);
        }
        else if (op == '/')
        {
            auto result = numberL / numberR;
            expr = makePtr<NumberExprAST>(result);
        }
        else
        {
            fprintf(stderr, "\tERROR: combineConstant: expr contains '%c'\n", op);
            exit(EXIT_FAILURE);
        }
    }
    
}

// combine 1 + x + y + 3 to 4 + x + y
ast_ptr combineConstant1(ast_ptr &expr)
{
    auto items = extractItems(expr);
    ast_ptr result = nullptr;
    double constant = 0;
    for (const auto &item : items)
    {
        // printExpr(item, "const:: ");
        if (item->type() == "Number")
        {
            NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(item.get());
            auto number = (numberExpr->getNumber());
            constant += number;
        }
        else
        {
            result = addExpr(result, item);
        }
    }
    if(constant != 0)
    {
        result = addExpr(result, makePtr<NumberExprAST>(constant));
    }

    return result;
}

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
    // if(callCount == 1) cout << prompt << "start--------" << endl;

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
        // printExpr(expr, prompt + "before dealWithCalls: newexpr = ");
        auto newASTs = dealWithCalls(newexpr);
        exprsFinal.insert(exprsFinal.end(), std::make_move_iterator(newASTs.begin()), std::make_move_iterator(newASTs.end()));

        // if(callCount == 1) printExprs(exprsFinal, "\tmathfuncRewrite: ");
        // if(callCount == 1) cout << prompt << "end--------" << endl;
        callCount--;
        callLevel--;
        return exprsFinal;
    }

    if(expr->type() != "Binary")  // May be variable or number
    {
        // if(callCount == 1) printExprs(exprsFinal, "\tmathfuncRewrite: ");
        // if(callCount == 1) cout << prompt << "end--------" << endl;
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
    // if(callCount == 1) printExprs(exprsFinal, prompt + "exprsFinal: ");
    // if(callCount == 1) cout << prompt << "end--------" << endl;
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

void sortExprAST(string &min ,string &max ,string &v,ast_ptr &expr){
    //参数v用来表示子树向父节点传递的信息
	if (expr->type() == "Number") {
        min = "minNumber@";
        max = "minNumber@";
        NumberExprAST* tmp = dynamic_cast<NumberExprAST *>(expr.get());
        ostringstream out;
        out << tmp->getNumber();
        v = out.str();    //传递数字本身
        return;
    }
    if (expr->type() == "Variable") {
        VariableExprAST* tmp = dynamic_cast<VariableExprAST *>(expr.get());
        min = tmp->getVariable();
        max = tmp->getVariable();
        v = tmp->getVariable();    //传递变量
        return;
    }
    if (expr->type() == "Call") {
        CallExprAST* tmp = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = tmp->getArgs();
        for(auto &arg : args){
            sortExprAST(min ,max ,v ,arg);    //对其参数排序
        }
        min = "maxCall@";
        max = "maxCall@";
        v = PrintExpression(expr);    //传递函数表达式字符串
        return;
    }
    BinaryExprAST* tmp = dynamic_cast<BinaryExprAST *>(expr.get());
    auto &l = (tmp->getLHS());
    auto &r = (tmp->getRHS());
    string Lmin,Lmax;
    string Rmin,Rmax;
    string Lv,Rv;    //接收子树传递过来的信息
    sortExprAST(min ,max ,v, l);
    Lmin = min;
    Lmax = max;
    Lv = v;
    sortExprAST(min ,max ,v, r);
    Rmin = min;
    Rmax = max;
    Rv = v;
    double Lnum,Rnum;

    ast_ptr ll = l->Clone();
    ast_ptr rr = r->Clone();

    //L、R代表左右子树类型
    int L,R;
    if(Lmin.compare("max-or/@") == 0)    L = 1;    //binary_"-or/"
    else if(Lmin.compare("maxCall@") == 0)    L = 3;     //callee
    else if(Lmin.compare("minNumber@") == 0)    L = 5;    //number
    else    L = 7;    //variable

    if(Rmin.compare("max-or/@") == 0)    R = -2;
    else if(Rmin.compare("maxCall@") == 0)    R = -4;
    else if(Rmin.compare("minNumber@") == 0)    R = -7;
    else    R = -9;

    int lr = L*R;

    if(lr == -35){
        //当左右子树最小值都是数字时
        stringstream sl(Lv);
        stringstream sr(Rv);
        sl>>Lnum;
        sr>>Rnum;
    }

    if(tmp->getOp() == '+' || tmp->getOp() == '*') {
        switch (lr){
            case -2:    //1*-2
                if(Lv.compare(Rv) > 0 ){
                    tmp->setLHS(rr);    //交换左右子树
                    tmp->setRHS(ll);
                    min = "max-or/@";
                    max = "max-or/@";
                    v = Rv;
                    break;
                }else{
                    min = "maxcall@";
                    max = "maxcall@";
                    v = Lv;
                    break;
                }
                break;
            case -4:    //1*-4
                min = "max-or/@";
                max = "maxcall";
                v = Lv;
                break;
            case -7:    //1*-7
                tmp->setLHS(rr);    //交换左右子树
                tmp->setRHS(ll);
                min = "minNumber@";
                max = "max-or/@";
                v = Rv;
                break;
            case -9:    //1*-9
                tmp->setLHS(rr);    //交换左右子树
                tmp->setRHS(ll);
                min = Rmin;
                max = "max-or/@";
                v = Rv;
                break;
            case -6:    //3*-2
                tmp->setLHS(rr);    //交换左右子树
                tmp->setRHS(ll);
                min = "max-or/@";
                max = "maxCall@";
                v = Rv;
                break;
            case -12:    //3*-4
                if(Lv.compare(Rv) > 0 ){
                    tmp->setLHS(rr);    //交换左右子树
                    tmp->setRHS(ll);
                    min = "maxcall@";
                    max = "maxcall@";
                    v = Rv;
                    break;
                }else{
                    min = "maxcall@";
                    max = "maxcall@";
                    v = Lv;
                    break;
                }
            case -21:    //3*-7
                tmp->setLHS(rr);    //交换左右子树
                tmp->setRHS(ll);
                min = "minNumber@";
                max = "maxcall@";
                v = Rv;
                break;
            case -27:    //3*-9
                tmp->setLHS(rr);    //交换左右子树
                tmp->setRHS(ll);
                min = Rmin;
                max = "maxcall@";
                v = Rv;
                break;
            case -10:    //5*-2
                min = "minNumber@";
                max = "max-or/@";
                v = Lv;
                break;
            case -20:    //5*-4
                min = "minNumber@";
                max = "maxcall@";
                v = Lv;
                break;
            case -35:    //5*-7
                if(Lnum>Rnum){
                    tmp->setLHS(rr);    //交换左右子树
                    tmp->setRHS(ll);
                    min = "minNumber@";
                    max = "minNumber@";
                    v = Rv;
                    break;
                }else{
                    min = "minNumber@";
                    max = "minNumber@";
                    v = Lv;
                    break;
                }
            case -45:    //5*-9
                min = "minNumber@";
                max = Rmin;
                v = Lv;
                break;
            case -14:    //7*-2
                min = Lmin;
                max = "max-or/@";
                v = Lv;
                break;
            case -28:    //7*-4
                min = Lmin;
                max = "maxcall@";
                v = Lv;
                break;
            case -49:    //7*-7
                tmp->setLHS(rr);    //交换左右子树
                tmp->setRHS(ll);
                min = "minNumber@";
                max = Lmin;
                v = Rv;
                break;
            case -63:    //7*-9
                if (Lmin > Rmin){
                    tmp->setLHS(rr);
                    tmp->setRHS(ll);
                    min = Rmin;
                    max = (Lmax > Rmax) ? Lmax : Rmax;
                    v = Rv;
                    return;
                }else{
                    min = Lmin;
                    max = (Lmax > Rmax) ? Lmax : Rmax;
                    v = Lv;
                    return;
                }
                break;
        }
    }else{
        min = "max-or/@";
        max = "max-or/@";
        v = PrintExpression(expr);
        return;
    }
}
void sortExpr(ast_ptr &expr){
    string min;
    string max;
    string v;
    sortExprAST(min ,max ,v ,expr);
}

vector<ast_ptr> polyRewrite(const ast_ptr &expr)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "polyRewrite: ";
    // cout << prompt << "start--------" <<endl;

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
        for(int i = 0 ;i < (int)results.size();i++){
            ast_ptr &a = results.at(i);
            sortExpr(a);
        }
        vector<ast_ptr> resultsNew;
        for(auto &result : results)
        {
            resultsNew.push_back(result->Clone()); // the key to control the rewrite results
            auto tmps = fmaRewrite(result);
            // mineAppend(resultsNew, tmps);

            // the following code: checkFma, toPow
            // auto tmps1 = checkFma(tmps);
            vector<ast_ptr> tmps1;
            for(auto &tmp : tmps)
            {
                auto tmp1 = toPow(tmp);
                tmps1.push_back(std::move(tmp->Clone()));
                tmps1.push_back(std::move(tmp1));
            }
            if(tmps1.empty())
            {
                resultsNew.push_back(std::move(result));
            }
            else
            {
                mineAppend(resultsNew, tmps1);
            }
        }
        // cout << prompt << "end--------" <<endl;
        callCount--;
        callLevel--;
        return resultsNew;
    }
    else if(infoNew.size() == 1)
    {
        auto results = createExpr(infoNew);
        
        // cout << prompt << "end--------" <<endl;
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
    // cout << prompt << "start--------" <<endl;
    // if(callCount == 1) printExpr(expr, prompt + "tryRewrites: at the begin: ");
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
        combineConstant(exprNew);
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
    // if(callCount == 1) printExpr(exprNew, prompt + "tryRewrites: before mathfuncRewrite: ");
    auto middles = mathfuncRewrite(exprNew);
    vector<ast_ptr> results;
    size_t index = 0;
    for(const auto &middle : middles)
    {
        // cout << prompt << "For expr NO." << index << ": " << PrintExpression(middle) << ", do polyRewrite" << endl;
        vector<string> vars;
        getVariablesFromExpr(middle, vars);
        // if(vars.size() > 1)
        // {
        //     results.push_back(std::move(middle->Clone()));
        // }
        // else
        // {
        //     auto tmp = polyRewrite(middle);
        //     mineAppend(results, tmp);
        // }
        auto tmp = polyRewrite(middle);
        mineAppend(results, tmp);
        index++;
    }
    // if(callCount == 1) printExprs(results, "tryRewrites: before delete: ");
    for(int i = 0 ;i < (int)results.size();i++){
        ast_ptr &a = results.at(i);
        // cout << "JJJ" << PrintExpression(a) << endl;
        sortExpr(a);
        // cout << "WWW" << PrintExpression(a) << endl;
    }
    deleteTheSame(results);

    if(callCount == 1) printExprs(results, prompt + "at the last: ");
    // cout << prompt << "end--------" <<endl;
    callCount--;
    return results;
}

ast_ptr geneRandomAST(const vector<ast_ptr> &items, size_t first, size_t second)
{
    ast_ptr result;
    for (size_t i = 0; i < items.size(); ++i)
    {
        if(i != first && i != second)
        {
            result = addExpr(result, items.at(i));
        }
    }
    const ast_ptr &lhs = items.at(first);
    const ast_ptr &rhs = items.at(second);
    ast_ptr tmp = addExpr(lhs, rhs);
    result = addExpr(result, tmp);
    return result;
}

void combineTheTwo(vector<ast_ptr> &items, size_t first, size_t second)
{
    static size_t callCount = 0;
    callCount++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "combineTheTwo: ";

    auto former = min(first, second);
    auto later = max(first, second);

    const ast_ptr &lhs = items.at(former);
    const ast_ptr &rhs = items.at(later);
    ast_ptr tmp = addExpr(lhs, rhs);
    
    items.erase(items.begin() + later);
    items.erase(items.begin() + former);
    items.push_back(std::move(tmp));

    if(callCount == 1) printExpr(items.back(), prompt + "at the last: ");
    // cout << prompt << "end--------" <<endl;
    callCount--;
}

size_t computeRandomNum(size_t sum)
{
    double radio = 0.2;
    size_t randomNum = combination(2, sum) * radio; // combination(2, sum) = sum * (sum - 1) / 2
    return randomNum;
}

size_t pickTheBest(vector<ast_ptr> &items, ast_ptr &originExpr)
{
    auto uniqueLabel = getUniqueLabel();
    vector<string> vars;
    getVariablesFromExpr(originExpr, vars);
    auto funcNameMpfr = geneMpfrCode(originExpr, uniqueLabel, vars);

    string filename = "./intervalData.txt"; // TODO: get the filename from uniqueLabel
    auto intervalData = getIntervalData(filename);
    size_t scale;
    auto intervalTmp = intervalData.at(0);
    auto dimension = intervalTmp.size() / 2;
    if (dimension == 1)
        scale = 500000;
    else if (dimension == 2)
        scale = 1024;
    else if (dimension == 3)
        scale = 256;
    else
        scale = 10;
    vector<int> scales(dimension, scale);
    size_t count = 0;
    string suffix = "temp_" + std::to_string(count) + "_";
    string bestRewriteExpr;
    double maxError = -1;
    double aveError = 0;
    size_t maxIdx = -1;
    size_t iEnd = min(items.size(), size_t(300));
    for (size_t i = 0; i < iEnd; i++)
    {
        string item = PrintExpression(items.at(i));
        cout << "*-*-*-pickTheBest" << "No." << i << ": " << item << endl;

        // generate function code and test error
        string suffixTmp = suffix + std::to_string(i);
        geneOriginCodeKernel(item, vars, uniqueLabel, suffixTmp);
        // auto timeTmp1 = std::chrono::high_resolution_clock::now();
        auto tempError = testError(uniqueLabel, suffixTmp, intervalTmp, scales);
        // auto timeTmp2 = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> testError_seconds = timeTmp2 - timeTmp1;
        // cout << BLUE << "rewrite: For NO." << i << ": testError time: " << testError_seconds.count() << " s" << RESET << endl;
        // fTimeout << testError_seconds.count() << endl;
        cout << "pickTheBest: maxError: " << tempError.maxError << endl;
        cout << "pickTheBest: aveError: " << tempError.aveError << endl << endl;
        if (i == 0)
        {
            maxError = tempError.maxError;
            bestRewriteExpr = item;
            aveError = tempError.aveError;
            maxIdx = i;
        }
        else
        {
            if (tempError.maxError < maxError)
            {
                maxError = tempError.maxError;
                bestRewriteExpr = item;
                aveError = tempError.aveError;
                maxIdx = i;
            }
        }
    }

    exprInfo tempInfo;
    tempInfo.intervals = intervalTmp;
    tempInfo.exprStr = bestRewriteExpr;
    tempInfo.aveError = aveError;
    tempInfo.maxError = maxError;
    tempInfo.rewriteID = maxIdx;

    cout << "*-*-*-pickTheBest: maxIdx: " << maxIdx << endl;
    return maxIdx;
}

vector<ast_ptr> tryRewriteRandom(ast_ptr expr)
{
    static size_t callCount = 0;
    callCount++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "tryRewriteRandom: ";
    // cout << prompt << "start--------" <<endl;
    if(callCount == 1) printExpr(expr, prompt + "at the begin: ");
    
    vector<ast_ptr> items = extractItems(expr);
    auto itemSize = items.size();
    for(size_t i = itemSize; i > 4; i--) // 从items中每次随机挑2个进行合并，直到只剩下一个 // 或者更成熟的思路：直到剩下4个就不合并了，剩下的可以直接穷举，也就15个
    {
        vector<size_t> randomTwos;
        vector<ast_ptr> randomSet;
        size_t randomNum = computeRandomNum(i); // 5 should be replaced by the number of terms;
        for(size_t j = 0; j < randomNum; j++) // 随机randomNum次，调用pickTheBest，从中选出最佳
        {
            auto randomTwo = geneRandom(0, items.size() - 1);
            auto &first = randomTwo.at(0);
            auto &second = randomTwo.at(1);
            randomTwos.push_back(first);
            randomTwos.push_back(second);
            // fprintf(stderr, "combine NO.%ld: random NO.%ld: pick %ld and %ld\n", i, j, first, second);
            auto randomAST = geneRandomAST(items, first, second);
            randomSet.push_back(std::move(randomAST));
        }
        auto bestNum = pickTheBest(randomSet, expr);
        auto &first = randomTwos.at(bestNum * 2);
        auto &second = randomTwos.at(bestNum * 2 + 1);
        combineTheTwo(items, first, second); // items.size-- should happen after combining;
    }
    vector<ast_ptr> results;
    if (items.size() != 4) // exception handling
    {
        fprintf(stderr, "ERROR: size should be 1, not %ld\n", items.size());
        exit(EXIT_FAILURE);
    }
    else
    {
        // 当items中只有4个的时候，对其进行穷举，应当输出15个表达式
        vector<ast_ptr> &items1 = items;
        vector<ast_ptr> candidates;
        const vector<int> indexs = {0, 1, 2, 3};
        // 3 items and 1 item
        {
            auto lhsIndexs = combination(3, indexs);
            for (auto &lhsIndex : lhsIndexs) 
            {
                auto rhsIndex = lhsIndex.back(); // because of special combination, the last element in lhsIndex is rhsIndex.
                lhsIndex.pop_back();
                auto lhsOflhsIndexs = combination(2, lhsIndex);
                for (auto &lhsOflhsIndex : lhsOflhsIndexs)
                {
                    auto rhsOflhsIndex = lhsOflhsIndex.back();
                    lhsOflhsIndex.pop_back();
                    ast_ptr lhsOflhsAST = addExpr(items1.at(lhsOflhsIndex.at(0)), items1.at(lhsOflhsIndex.at(1)));
                    ast_ptr &rhsOflhsAST = items1.at(rhsOflhsIndex);
                    ast_ptr lhsAST = addExpr(lhsOflhsAST, rhsOflhsAST);
                    ast_ptr &rhsAST = items1.at(rhsIndex);
                    ast_ptr tmpAST = addExpr(lhsAST, rhsAST);
                    candidates.push_back(std::move(tmpAST));
                }
            }
        }
        // 2 items and 2 items
        {
            auto lhsIndexs = combination(2, indexs);
            lhsIndexs.erase(lhsIndexs.begin() + lhsIndexs.size() / 2, lhsIndexs.end()); // just erase the duplicate half part
            for (auto &lhsIndex : lhsIndexs) 
            {
                vector<int> rhsIndex;
                rhsIndex.assign(lhsIndex.begin() + 2, lhsIndex.end()); // because of special combination, the last element in lhsIndex is rhsIndex.
                lhsIndex.erase(lhsIndex.begin() + 2, lhsIndex.end());
                fmt::print("hhh lhsIndex = {}, ", lhsIndex);
                fmt::print("rhsIndex = {}\n", rhsIndex);
                ast_ptr lhsAST = addExpr(items1.at(lhsIndex.at(0)), items1.at(lhsIndex.at(1)));
                ast_ptr rhsAST = addExpr(items1.at(rhsIndex.at(0)), items1.at(rhsIndex.at(1)));
                ast_ptr tmpAST = addExpr(lhsAST, rhsAST);
                candidates.push_back(std::move(tmpAST));
            }
        }
        printExprs(candidates, "the 4 items: ");
        pickTheBest(candidates, expr);
        mineAppend(results, candidates);
    }

    if(callCount == 1) printExprs(results, prompt + "at the last: ");
    // cout << prompt << "end--------" <<endl;
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
    cout << prompt << "at the beginning: expr = " << PrintExpression(expr) << endl;
    cout << prompt << "step1: preprocess" << endl;

    ast_ptr exprNew = preprocess(expr);
    // exprNew = simplifyExpr(exprNew);
    exprNew = minusRewrite(exprNew);
    combineConstant(exprNew);
    cout << prompt << "after preprocess: exprNew = " << PrintExpression(exprNew) << endl;

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
        // results = tryRewriteRandom(std::move(exprNew));
    }

    cout << prompt << "at the last: results size = " << results.size() << endl;
    printExprs(results, prompt);
    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return results;
}

vector<ast_ptr> exprAutoWrapper(ast_ptr &expr, const std::vector<double> &intervals, const std::vector<int> &scales)
{
    cout << "\n>exprAutoWrapper: start-----------" << endl;

    expr = minusRewrite(expr);
    combineConstant(expr);
    expr = combineConstant1(expr);
    sortExpr(expr);
    printExpr(expr, "exprAutoWrapper: after parse, expr = ", DOUBLE_PRECISION);
    ast_ptr expr1 = simplifyExpr(expr); // Python SymPy simplify
    combineConstant(expr1);
    sortExpr(expr1);
    printExpr(expr1, "\nexprAutoWrapper: after SymPy's simplify, expr = ", DOUBLE_PRECISION);
    cout << endl;
    vector<ast_ptr> results;
    if(isEqual(expr, expr1))
    {
        cout << YELLOW << "-------------------------------------origin rewrite-------------------------------------" << RESET << endl;
        results = exprAutoNew(expr);
    }
    else
    {
        bool pickOne = true;
        if(pickOne)
        {
        // pick the better one from expr and expr1
        string uniqueLabel = "pickBetter";
        vector<string> vars;
        getVariablesFromExpr(expr, vars);
        
        auto exprStr = PrintExpression(expr);
        auto funcNameOrigin = geneOriginCodeKernel(exprStr, vars, uniqueLabel, "origin");
        auto expr1Str = PrintExpression(expr1);
        auto funcNameSympy = geneOriginCodeKernel(expr1Str, vars, uniqueLabel, "sympy");
        auto funcNameMpfr = geneMpfrCode(exprStr, uniqueLabel, vars);
        
        // int scale = 256;
        // turbine1
        // auto info = testError(uniqueLabel, "origin", 3.8, 7.8, -4.5, -0.3, 0.4, 0.9, scale, scale, scale);
        // auto info1 = testError(uniqueLabel, "sympy", 3.8, 7.8, -4.5, -0.3, 0.4, 0.9, scale, scale, scale);
        // doppler1
        // auto info = testError(uniqueLabel, "origin", -30, 50, -100, 100, 20, 20000, scale, scale, scale);
        // auto info1 = testError(uniqueLabel, "sympy", -30, 50, -100, 100, 20, 20000, scale, scale, scale);
        // int scale = 500000;
        // sine
        // auto info = testError(uniqueLabel, "origin", -1.57079632679, 1.57079632679, scale);
        // auto info1 = testError(uniqueLabel, "sympy", -1.57079632679, 1.57079632679, scale);
        // sqroot
        // auto info = testError(uniqueLabel, "origin", 0, 1, scale);
        // auto info1 = testError(uniqueLabel, "sympy", 0, 1, scale);

        auto info = testError(uniqueLabel, "origin", intervals, scales);
        auto info1 = testError(uniqueLabel, "sympy", intervals, scales);
        auto maxError = info.maxError;
        auto maxError1 = info1.maxError;

        if(maxError < maxError1)
        {
            cout << YELLOW << "-------------------------------------origin rewrite is better-------------------------------------" << RESET << endl;
            results = exprAutoNew(expr);
        }
        else
        {
            cout << YELLOW << "-------------------------------------sympy rewrite is better-------------------------------------" << RESET << endl;
            results = exprAutoNew(expr1);
        }
        }
        else
        {
        cout << YELLOW << "-------------------------------------origin rewrite-------------------------------------" << RESET << endl;
        results = exprAutoNew(expr);
        cout << YELLOW << "-------------------------------------sympy rewrite-------------------------------------" << RESET << endl;
        vector<ast_ptr> results1 = exprAutoNew(expr1);
        mineAppend(results, results1);
        }
    }
    for(auto& result : results)
    {
        sortExpr(result);
    }
    deleteTheSame(results);

    cout << YELLOW << "-------------------------------------final results-------------------------------------" << RESET << endl;
    printExprs(results, BLUE "exprAutoWrapper: after exprAutoNew: " RESET, false, DOUBLE_PRECISION);

    cout << ">exprAutoWrapper: end-----------\n" << endl;
    return results;
}

vector<ast_ptr> exprAutoWrapper(const string &inputStr, const std::vector<double> &intervals, const std::vector<int> &scales)
{
    auto expr = ParseExpressionFromString(inputStr);
    return exprAutoWrapper(expr, intervals, scales);
}