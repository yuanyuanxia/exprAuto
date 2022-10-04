#include "basic.hpp"
#include "geneCode.hpp"
#include "parserASTLY.hpp"

#include <algorithm>

using std::cout;
using std::endl;

// 0.000287413s
vector<string> getVariablesFromExpr(const ast_ptr &expr)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: the input is nullptr");
        exit(EXIT_FAILURE);
    }
    auto type = expr->type();
    if(type == "Number")
    {
        return {};
    }
    else if(type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        vector<string> vars{var};
        return vars;
    }
    else if(type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        vector<string> vars;
        for(auto &arg : args) {
            auto varsTmp = getVariablesFromExpr(arg);
            vars.insert(vars.end(), varsTmp.begin(), varsTmp.end());
        }
        std::sort(vars.begin(), vars.end());
        vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
        return vars;
    }
    if(type != "Binary")
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: type %s is wrong\n", type.c_str());
        exit(EXIT_FAILURE);
    }
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    auto varsL = getVariablesFromExpr(lhs);
    auto varsR = getVariablesFromExpr(rhs);
    vector<string> vars;
    vars.insert(vars.end(),varsL.begin(),varsL.end());
    vars.insert(vars.end(),varsR.begin(),varsR.end());
    std::sort(vars.begin(), vars.end());
    vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
    return vars;
}

// this version is more faster. 0.000232973s
bool getVariablesFromExpr(const ast_ptr &expr, vector<string> &vars)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: the input is nullptr\n");
        exit(EXIT_FAILURE);
    }
    auto type = expr->type();
    if(type == "Number")
    {
        return true;
    }
    else if(type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        vars.push_back(var);
        return true;
    }
    else if(type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        for(auto &arg : args) {
            if(!getVariablesFromExpr(arg, vars))
            {
                fprintf(stderr, "ERROR: getVariablesFromExpr: run failed\n");
                exit(EXIT_FAILURE);
            }
        }
        return true;
    }
    if(type != "Binary")
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: type %s is wrong\n", type.c_str());
        exit(EXIT_FAILURE);
    }
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    getVariablesFromExpr(lhs, vars);
    getVariablesFromExpr(rhs, vars);
    std::sort(vars.begin(), vars.end());
    vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
    return true;
}

void geneCode(string exprStr, vector<string> vars)
{
    // just print input info
    cout << "expression    : " << exprStr << endl;
    cout << "variables list:";
    for(auto &var : vars)
    {
        cout << " " << var;
    }
    cout << endl;
    
    std::ofstream fout;
    fout.open("./hanshuti.c"); // 清空并写入, ios::trunc
    fout<<"double function (";
    // for(auto &var : vars)
    for(int i=0;i<(vars.size()-1);i++)

    {
        fout<<"double"<<" "<<vars[i]<<",";
    }
    fout<<"double"<<" "<<vars[vars.size()-1];
    fout<<") {"<<endl;
    fout<<"\t"<<"double result = "<<exprStr<<";"<<endl;
    fout<<"\t"<<"return result;"<<endl;
    fout<<"}"<<endl;
    fout.close();
}

void geneOriginCode(string exprStr)
{
    auto originExpr = ParseExpressionFromString(exprStr);
    
    // another way: auto vars = getVariablesFromExpr(originExpr);
    vector<string> vars;
    getVariablesFromExpr(originExpr, vars);
    geneCode(exprStr, vars);
}

void geneHerbieCode(string exprStr)
{
    cout << exprStr << endl;
}

void geneDaisyCode(string exprStr)
{
    cout << exprStr << endl;
}

void geneMpfrCode(string exprStr)
{
    cout << exprStr << endl;
}