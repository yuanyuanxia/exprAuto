#include <fmt/ranges.h>
#include <mpfr.h>
#include "mpreal.h"
#include "shadowValue.hpp"
#include "funclist.hpp"


using std::string;
using std::vector;
using std::ofstream;

struct valueTwo {
    double funcValue;
    mpfr::mpreal realValue;
};

// demo
double shadowValueKernel(ast_ptr &expr, double &input, vector<double> &middleValues)
{
    auto type = expr->type();
    double result;
    if(type == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        result = (numberExpr->getNumber());
        middleValues.push_back(result);
        return result;
    }
    else if(type == "Variable")
    {
        middleValues.push_back(input);
        return input;
    }
    else if(type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        vector<double> paramResults;
        for(auto& arg : args)
        {
            auto tmp = shadowValueKernel(arg, input, middleValues);
            paramResults.push_back(tmp);
        }
        auto tmpCall = callPtr->getCallback();
        result = tmpCall(paramResults);
        std::cout << "callee = " << callPtr->getCallee() << " result = " << result << std::endl;
        middleValues.push_back(result);

        return result;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        auto lhsValue = shadowValueKernel(lhs, input, middleValues);
        auto rhsValue = shadowValueKernel(rhs, input, middleValues);
        auto tmpCall = binPtr->getCallback();
        result = tmpCall(lhsValue, rhsValue);
        std::cout << "op = " << binPtr->getOp() << " result = " << result << std::endl;
        middleValues.push_back(result);

        return result;
    }
    else
    {
        fprintf(stderr, "ERROR: unknowntype %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

// demo
void shadowValue(ast_ptr &expr, double input)
{
    vector<double> middleValues;
    shadowValueKernel(expr, input, middleValues);
    fmt::print("middleValues := {}\n", middleValues);
}

valueTwo shadowValueKernel(const ast_ptr &expr, const std::map<string, double> &varsValue, vector<double> &funcValues, vector<mpfr::mpreal> &funcRealValues, vector<mpfr::mpreal> &mathRealValues, vector<double> &errorValues)
{
    auto type = expr->type();
    valueTwo result;
    if(type == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        result.funcValue = (numberExpr->getNumber());
        result.realValue = result.funcValue;
        funcValues.push_back(result.funcValue);
        funcRealValues.push_back(result.realValue);
        mathRealValues.push_back(result.realValue);
        errorValues.push_back(0);
        return result;
    }
    else if(type == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());
        result.funcValue = varsValue.find(variable)->second;
        result.realValue = result.funcValue;
        funcValues.push_back(result.funcValue);
        funcRealValues.push_back(result.realValue);
        mathRealValues.push_back(result.realValue);
        errorValues.push_back(0);

        return result;
    }
    else if(type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        vector<double> paramResults;
        vector<mpfr::mpreal> paramRealResults;
        vector<mpfr::mpreal> parammathRealResults;
        
        for(auto& arg : args)
        {
            auto tmp = shadowValueKernel(arg, varsValue, funcValues, funcRealValues, mathRealValues, errorValues);
            paramResults.push_back(tmp.funcValue);
            mpfr::mpreal tmpRealValue = tmp.funcValue;
            paramRealResults.push_back(tmpRealValue);
            parammathRealResults.push_back(tmp.realValue);
        }

        // func
        auto tmpCall = callPtr->getCallback();
        
        // func value
        result.funcValue = tmpCall(paramResults);
        std::cout << "callee = " << callPtr->getCallee() << " result = " << result.funcValue << std::endl;
        funcValues.push_back(result.funcValue);

        // real func
        auto tmpRealCall = callPtr->getRealCallback();

        // func real value
        mpfr::mpreal funcRealValue = tmpRealCall(paramRealResults);
        std::cout << "callee = " << callPtr->getCallee() << " funcRealValue = " << funcRealValue.toString() << std::endl;
        funcRealValues.push_back(funcRealValue);

        // math real value
        result.realValue = tmpRealCall(parammathRealResults);
        std::cout << "callee = " << callPtr->getCallee() << " mathRealResult = " << result.realValue.toString() << std::endl;
        mathRealValues.push_back(result.realValue);

        // absolute error
        double errorValue = fabs((funcRealValue - result.funcValue).toDouble());
        errorValues.push_back(errorValue);

        return result;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        auto lhsValue = shadowValueKernel(lhs, varsValue, funcValues, funcRealValues, mathRealValues, errorValues);
        auto rhsValue = shadowValueKernel(rhs, varsValue, funcValues, funcRealValues, mathRealValues, errorValues);
        
        // func
        auto tmpCall = binPtr->getCallback();
        
        // func value
        result.funcValue = tmpCall(lhsValue.funcValue, rhsValue.funcValue);
        std::cout << "op = " << binPtr->getOp() << " result = " << result.funcValue << std::endl;
        funcValues.push_back(result.funcValue);

        // real func
        auto tmpRealCall = binPtr->getRealCallback();

        // func real value
        mpfr::mpreal funcRealValue = tmpRealCall(lhsValue.funcValue, rhsValue.funcValue);
        std::cout << "op = " << binPtr->getOp() << " funcRealValue = " << funcRealValue.toString() << std::endl;
        funcRealValues.push_back(funcRealValue);

        // math real value
        result.realValue = tmpRealCall(lhsValue.realValue, rhsValue.realValue);
        std::cout << "op = " << binPtr->getOp() << " funcRealValue = " << result.realValue.toString() << std::endl;
        mathRealValues.push_back(result.realValue);

        // absolute error
        double errorValue = fabs((funcRealValue - result.funcValue).toDouble());
        errorValues.push_back(errorValue);

        return result;
    }
    else
    {
        fprintf(stderr, "ERROR: unknowntype %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

void shadowValue(const ast_ptr &expr, const std::map<string, double> &varsValue)
{
    mpfr::mpreal::set_default_prec(128);
    std::vector<mpfr::mpreal> funcRealValues;
    std::vector<mpfr::mpreal> mathRealValues;

    vector<double> funcValues;
    vector<double> errorValues; // this error is f(x+\delta x) between F(x + \delta x)
    shadowValueKernel(expr, varsValue, funcValues, funcRealValues, mathRealValues, errorValues);
    fmt::print("funcValues := {}\n", funcValues);
    for(auto &funRealValue : funcRealValues)
    {
        std::cout << "func real value: " << funRealValue.toString() << std::endl;
    }
    for(auto &mathRealValue : mathRealValues)
    {
        std::cout << "math real value: " << mathRealValue.toString() << std::endl;
    }
    fmt::print("errorValues := {}\n", errorValues);
}