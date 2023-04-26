#include <fmt/ranges.h>
#include <mpfr.h>
#include <typeinfo>
#include "mpreal.h"
#include "shadowValue.hpp"
#include "funclist.hpp"


using std::string;
using std::vector;
using std::ofstream;
namespace Shadow {

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

template <typename T>
struct ParamTypeMapper
{
    using ParamType = T;
};

template <>
struct ParamTypeMapper<double>
{
    using ParamType = mpfr::mpreal;
};

template <>
struct ParamTypeMapper<double *>
{
    using ParamType = mpfr::mpreal *;
};

template <typename T>
using ParamType = typename ParamTypeMapper<T>::ParamType;

template <typename T>
struct valueTwo {
    T funcValue;
    ParamType<T> realValue;
};

template <typename T>
valueTwo<T> shadowValueKernel(const ast_ptr &expr, const std::map<string, T> &varsValue, vector<T> &funcValues, vector<ParamType<T>> &funcRealValues, vector<ParamType<T>> &mathRealValues, vector<T> &errorValues, int length = 1)
{
    auto type = expr->type();
    constexpr bool TisDouble =  (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer =  (std::is_same<double *, T>::value);
    valueTwo<T> result;
    if(type == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        auto funcValue = (numberExpr->getNumber());
        if constexpr (TisDouble)
        {
            result.funcValue = funcValue;
            result.realValue = result.funcValue;
            funcValues.push_back(result.funcValue);
            funcRealValues.push_back(result.realValue);
            mathRealValues.push_back(result.realValue);
            errorValues.push_back(0);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new double[length];
            auto tmpRealValues = new mpfr::mpreal[length];
            auto tmperrorValues = new double[length]();
            for(int i = 0; i < length; i++)
            {
                tmpfuncValues[i] = funcValue;
                tmpRealValues[i] = funcValue;
            }
            result.funcValue = tmpfuncValues;
            result.realValue = tmpRealValues;
            funcValues.push_back(tmpfuncValues);
            funcRealValues.push_back(tmpRealValues);
            mathRealValues.push_back(tmpRealValues);
            errorValues.push_back(tmperrorValues);
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }
        return result;
    }
    else if(type == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());
        auto varValue = varsValue.find(variable)->second;
        // if(std::is_same<std::decay<const double &>::type, T>::value)
        if constexpr (TisDouble)
        {
            result.funcValue = varValue;
            result.realValue = result.funcValue;
            funcValues.push_back(result.funcValue);
            funcRealValues.push_back(result.realValue);
            mathRealValues.push_back(result.realValue);
            errorValues.push_back(0);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new double[length];
            auto tmpRealValues = new mpfr::mpreal[length];
            auto tmperrorValues = new double[length]();
            for(int i = 0; i < length; i++)
            {
                auto &tmpfuncValue = varValue[i];
                tmpfuncValues[i] = tmpfuncValue;
                tmpRealValues[i] = tmpfuncValue;
            }
            result.funcValue = tmpfuncValues;
            result.realValue = tmpRealValues;
            funcValues.push_back(tmpfuncValues);
            funcRealValues.push_back(tmpRealValues);
            mathRealValues.push_back(tmpRealValues);
            errorValues.push_back(tmperrorValues);
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        return result;
    }
    else if(type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        vector<T> paramResults;
        vector<ParamType<T>> paramFuncRealResults;
        vector<ParamType<T>> parammathRealResults;
        
        for(auto& arg : args)
        {
            auto tmp = shadowValueKernel<T>(arg, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
            paramResults.push_back(tmp.funcValue);
            if constexpr (TisDouble)
            {
                paramFuncRealResults.push_back(tmp.funcValue);
            }
            parammathRealResults.push_back(tmp.realValue);
        }

        // func
        auto tmpCall = callPtr->getCallback();
        
        // func value
        if constexpr (TisDouble)
        {
            result.funcValue = tmpCall(paramResults);
            std::cout << "callee = " << callPtr->getCallee() << " result = " << result.funcValue << std::endl;
            funcValues.push_back(result.funcValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new double[length];
            for(int i = 0; i < length; i++)
            {
                vector<double> tmpParam;
                for(auto &param : paramResults)
                {
                    tmpParam.push_back(param[i]);
                }
                auto funcValue = tmpCall(tmpParam);
                tmpfuncValues[i] = funcValue;
            }
            result.funcValue = tmpfuncValues;
            funcValues.push_back(tmpfuncValues);
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // real func
        auto tmpRealCall = callPtr->getRealCallback();

        // func real value
        if constexpr (TisDouble)
        {
            mpfr::mpreal funcRealValue = tmpRealCall(paramFuncRealResults);
            std::cout << "callee = " << callPtr->getCallee() << " funcRealValue = " << funcRealValue.toString() << std::endl;
            funcRealValues.push_back(funcRealValue);
        
            // absolute error
            double errorValue = fabs((funcRealValue - result.funcValue).toDouble());
            errorValues.push_back(errorValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncRealValues = new mpfr::mpreal[length];
            auto tmpErrorValues = new double[length];
            for(int i = 0; i < length; i++)
            {
                vector<mpfr::mpreal> tmpParam;
                for(auto &param : paramResults)
                {
                    tmpParam.push_back(param[i]);
                }
                auto funcRealValue = tmpRealCall(tmpParam);
                tmpfuncRealValues[i] = funcRealValue;
                
                tmpErrorValues[i] = fabs((funcRealValue - (result.funcValue)[i]).toDouble());
            }
            funcRealValues.push_back(tmpfuncRealValues);
            errorValues.push_back(tmpErrorValues);
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // math real value
        if constexpr (TisDouble)
        {
            result.realValue = tmpRealCall(parammathRealResults);
            std::cout << "callee = " << callPtr->getCallee() << " mathRealResult = " << result.realValue.toString() << std::endl;
            mathRealValues.push_back(result.realValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpmathRealValues = new mpfr::mpreal[length];
            for(int i = 0; i < length; i++)
            {
                vector<mpfr::mpreal> tmpParam;
                for(auto &param : parammathRealResults)
                {
                    tmpParam.push_back(param[i]);
                }
                auto mathRealValue = tmpRealCall(tmpParam);
                tmpmathRealValues[i] = mathRealValue;
            }
            mathRealValues.push_back(tmpmathRealValues);
            result.realValue = tmpmathRealValues;
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        return result;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        auto lhsValue = shadowValueKernel<T>(lhs, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
        auto rhsValue = shadowValueKernel<T>(rhs, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
        
        // func
        auto tmpCall = binPtr->getCallback();
        
        // func value
        if constexpr (TisDouble)
        {
            result.funcValue = tmpCall(lhsValue.funcValue, rhsValue.funcValue);
            std::cout << "op = " << binPtr->getOp() << " result = " << result.funcValue << std::endl;
            funcValues.push_back(result.funcValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new double[length];
            for(int i = 0; i < length; i++)
            {
                double &tmpLhsValue = (lhsValue.funcValue)[i];
                double &tmpRhsValue = (rhsValue.funcValue)[i];
                
                auto funcValue = tmpCall(tmpLhsValue, tmpRhsValue);
                tmpfuncValues[i] = funcValue;
            }
            result.funcValue = tmpfuncValues;
            funcValues.push_back(tmpfuncValues);
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // real func
        auto tmpRealCall = binPtr->getRealCallback();

        // func real value
        if constexpr (TisDouble)
        {
            mpfr::mpreal funcRealValue = tmpRealCall(lhsValue.funcValue, rhsValue.funcValue);
            std::cout << "op = " << binPtr->getOp() << " funcRealValue = " << funcRealValue.toString() << std::endl;
            funcRealValues.push_back(funcRealValue);

            // absolute error
            double errorValue = fabs((funcRealValue - result.funcValue).toDouble());
            errorValues.push_back(errorValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncRealValues = new mpfr::mpreal[length];
            auto tmpErrorValues = new double[length];
            for(int i = 0; i < length; i++)
            {
                mpfr::mpreal tmpLhsValue = (lhsValue.funcValue)[i];
                mpfr::mpreal tmpRhsValue = (rhsValue.funcValue)[i];
                auto funcRealValue = tmpRealCall(tmpLhsValue, tmpRhsValue);
                tmpfuncRealValues[i] = funcRealValue;
                
                tmpErrorValues[i] = fabs((funcRealValue - (result.funcValue)[i]).toDouble());
            }
            funcRealValues.push_back(tmpfuncRealValues);
            errorValues.push_back(tmpErrorValues);
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // math real value
        if constexpr (TisDouble)
        {
            result.realValue = tmpRealCall(lhsValue.realValue, rhsValue.realValue);
            std::cout << "op = " << binPtr->getOp() << " mathRealValue = " << result.realValue.toString() << std::endl;
            mathRealValues.push_back(result.realValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpmathRealValues = new mpfr::mpreal[length];
            for(int i = 0; i < length; i++)
            {
                mpfr::mpreal tmpLhsValue = (lhsValue.realValue)[i];
                mpfr::mpreal tmpRhsValue = (rhsValue.realValue)[i];
                auto mathRealValue = tmpRealCall(tmpLhsValue, tmpRhsValue);
                tmpmathRealValues[i] = mathRealValue;
            }
            mathRealValues.push_back(tmpmathRealValues);
            result.realValue = tmpmathRealValues;
        }
        else
        {
            fprintf(stderr, "shadowValueKernel: the type of T %s is not supported\n", typeid(T).name());
            exit(EXIT_FAILURE);
        }

        return result;
    }
    else
    {
        fprintf(stderr, "ERROR: unknowntype %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
void shadowValue(const ast_ptr &expr, const std::map<string, T> &varsValue, int length)
{
    mpfr::mpreal::set_default_prec(128);
    vector<T> funcValues;
    vector<T> errorValues; // this error is f(x+\delta x) between F(x + \delta x)
    if constexpr (std::is_same<double, T>::value)
    {
        std::vector<mpfr::mpreal> funcRealValues;
        std::vector<mpfr::mpreal> mathRealValues;
        shadowValueKernel<T>(expr, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
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
    else if constexpr (std::is_same<double *, T>::value)
    {
        std::vector<mpfr::mpreal *> funcRealValues;
        std::vector<mpfr::mpreal *> mathRealValues;
        shadowValueKernel<T>(expr, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
        for(int i = 0; i < length; i++)
        {
            for(size_t j = 0; j < funcValues.size(); j++)
            {
                fprintf(stderr, "i = %d j = %ld: funcValue = %f\n", i, j, (funcValues.at(j))[i]);
            }
        }
    }
}

template void shadowValue<double*>(const ast_ptr &expr, const std::map<string, double*> &varsValue, int length);
template void shadowValue<double>(const ast_ptr &expr, const std::map<string, double> &varsValue, int length);

}