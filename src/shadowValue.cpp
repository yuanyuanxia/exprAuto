#include <fmt/ranges.h>
#include <mpfr.h>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <string>
#include <any>
#include "mpreal.h"
#include "shadowValue.hpp"
#include "funclist.hpp"
#include "errorDetect.hpp"


using std::string;
using std::vector;
using std::ofstream;
using std::ios;
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

std::map<char, string> binaryOp = {
    {'+', "add"},
    {'-', "sub"},
    {'*', "mul"},
    {'/', "div"},
};

std::map<string, string> opTypeMap = {
    {"double", "d"},
    {"DD", "dd"},
    {"ld", "ld"},
};

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
    ParamType<T> funcValue;
    ParamType<T> realValue;
};

template <typename T>
valueTwo<T> shadowValueKernel(const ast_ptr &expr, const std::map<string, T> &varsValue, vector<ParamType<T>> &funcValues, vector<ParamType<T>> &funcRealValues, vector<ParamType<T>> &mathRealValues, vector<T> &errorValues, int length = 1)
{
    auto type = expr->type();
    constexpr bool TisDouble = (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer = (std::is_same<double *, T>::value);
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
            auto tmpfuncValues = new mpfr::mpreal[length];
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
            auto tmpfuncValues = new mpfr::mpreal[length];
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
        vector<ParamType<T>> paramResults;
        vector<T> paramResultsDouble;
        vector<ParamType<T>> paramFuncRealResults;
        vector<ParamType<T>> parammathRealResults;

        for(auto& arg : args)
        {
            auto tmp = shadowValueKernel<T>(arg, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
            paramResults.push_back(tmp.funcValue);
            if constexpr (TisDouble)
            {
                paramFuncRealResults.push_back(tmp.funcValue);
                paramResultsDouble.push_back((tmp.funcValue).toDouble());
            }
            parammathRealResults.push_back(tmp.realValue);
        }

        // func
        auto tmpCall = callPtr->getCallback();
        auto callee = callPtr->getCallee();
        auto calleeType = callPtr->getOpType();
        auto callTypeStr = opTypeMap.find(calleeType)->second;
        string mapType;
        for(const auto& arg : args)
        {
            auto opType = arg->getOpType();
            auto opTypeStr = opTypeMap.find(opType)->second;
            mapType += "_" + opTypeStr;
        }

        // func value
        if constexpr (TisDouble)
        {
            if(mapType == "_dd")
            {
                double x1[2]{0};
                x1[0] = paramResults.at(0).toDouble();
                x1[1] = (paramResults.at(0) - x1[0]).toDouble();
                auto tmpSingleCall = singleCall_dd_map.find(callee + "_" + callTypeStr)->second;
                mpfr::mpreal tmp = tmpSingleCall(x1);
                result.funcValue = tmp;
            }
            else
            {
                result.funcValue = tmpCall(paramResultsDouble);
            }
            std::cout << "callee = " << callPtr->getCallee() << " result = " << result.funcValue << std::endl;
            funcValues.push_back(result.funcValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new mpfr::mpreal[length];
            if(mapType == "_dd")
            {
                double x1[2]{0};
                mpfr::mpreal tmp;
                for(int i = 0; i < length; i++)
                {
                    x1[0] = ((paramResults.at(0))[i]).toDouble();
                    x1[1] = ((paramResults.at(0))[i] - x1[0]).toDouble();
                    auto tmpSingleCall = singleCall_dd_map.find(callee + "_" + callTypeStr)->second;
                    tmpfuncValues[i] = tmpSingleCall(x1);
                }
            }
            else
            {
                for(int i = 0; i < length; i++)
                {
                    vector<double> tmpParam;
                    for(auto &param : paramResults)
                    {
                        tmpParam.push_back((param[i]).toDouble());
                    }
                    auto funcValue = tmpCall(tmpParam);
                    tmpfuncValues[i] = funcValue;
                }
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
        
            auto errorValue = computeError(funcRealValue, result.funcValue);
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
                
                tmpErrorValues[i] = computeError(funcRealValue, ((result.funcValue)[i]));
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
        auto op = binPtr->getOp();
        auto opType = binPtr->getOpType();
        auto opTypeStr = opTypeMap.find(opType)->second;
        auto opTypeLHS = lhs->getOpType();
        auto opTypeRHS = rhs->getOpType();
        auto opStr = binaryOp.find(op)->second;
        auto opTypeLhsStr = opTypeMap.find(opTypeLHS)->second;
        auto opTypeRhsStr = opTypeMap.find(opTypeRHS)->second;
        auto mapType = opTypeLhsStr + "_" + opTypeRhsStr;

        // func value
        if constexpr (TisDouble)
        {
            double x1[2]{0};
            double x2[2]{0};
            x1[0] = lhsValue.funcValue.toDouble();
            x2[0] = rhsValue.funcValue.toDouble();
            if(mapType == "d_dd")
            {
                auto tmpDoubleCall = doubleCall_d_dd_map.find(opStr + "_" + opTypeStr)->second;
                result.funcValue = tmpDoubleCall(x1[0], x2);
            }
            else if(mapType == "dd_d")
            {
                auto tmpDoubleCall = doubleCall_dd_d_map.find(opStr + "_" + opTypeStr)->second;
                result.funcValue = tmpDoubleCall(x1, x2[0]);
            }
            else if(mapType == "dd_dd")
            {
                auto tmpDoubleCall = doubleCall_dd_dd_map.find(opStr + "_" + opTypeStr)->second;
                result.funcValue = tmpDoubleCall(x1, x2);
            }
            else
            {
                result.funcValue = tmpCall(x1[0], x2[0]);
            }
            std::cout << "op = " << binPtr->getOp() << " result = " << result.funcValue << std::endl;
            funcValues.push_back(result.funcValue);

        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new mpfr::mpreal[length];
            double x1[2]{0};
            double x2[2]{0};
            if(mapType == "d_dd")
            {
                auto tmpDoubleCall = doubleCall_d_dd_map.find(opStr + "_" + opTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    auto &tmpLhsValue = (lhsValue.funcValue)[i];
                    auto &tmpRhsValue = (rhsValue.funcValue)[i];
                    x2[0] = tmpRhsValue.toDouble();
                    x2[1] = (tmpRhsValue - x2[0]).toDouble();
                    tmpfuncValues[i] = tmpDoubleCall(tmpLhsValue.toDouble(), x2);
                }
            }
            else if(mapType == "dd_d")
            {
                auto tmpDoubleCall = doubleCall_dd_d_map.find(opStr + "_" + opTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    auto &tmpLhsValue = (lhsValue.funcValue)[i];
                    auto &tmpRhsValue = (rhsValue.funcValue)[i];
                    x1[0] = tmpLhsValue.toDouble();
                    x1[1] = (tmpLhsValue - x1[0]).toDouble();
                    tmpfuncValues[i] = tmpDoubleCall(x1, tmpRhsValue.toDouble());
                }
            }
            else if(mapType == "dd_dd")
            {
                auto tmpDoubleCall = doubleCall_dd_dd_map.find(opStr + "_" + opTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    auto &tmpLhsValue = (lhsValue.funcValue)[i];
                    auto &tmpRhsValue = (rhsValue.funcValue)[i];
                    x1[0] = tmpLhsValue.toDouble();
                    x1[1] = (tmpLhsValue - x1[0]).toDouble();
                    x2[0] = tmpRhsValue.toDouble();
                    x2[1] = (tmpRhsValue - x2[0]).toDouble();
                    tmpfuncValues[i] = tmpDoubleCall(x1, x2);
                }
            }
            else // d_d
            {
                for(int i = 0; i < length; i++)
                {
                    auto &tmpLhsValue = (lhsValue.funcValue)[i];
                    auto &tmpRhsValue = (rhsValue.funcValue)[i];
                    tmpfuncValues[i] = tmpCall(tmpLhsValue.toDouble(), tmpRhsValue.toDouble());
                }
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

            auto errorValue = computeError(funcRealValue, result.funcValue);
            errorValues.push_back(errorValue);

            // absolute error
            // double absErrorValue = fabs((funcRealValue - result.funcValue).toDouble());
            // errorValues.push_back(absErrorValue);
            // relative error
            // double relErrorValue = fabs(((funcRealValue - result.funcValue) / funcRealValue).toDouble());
            // errorValues.push_back(relErrorValue);
            // ulp error
            // auto unitUlp = computeUlpUnit(funcRealValue.toDouble());
            // auto ulpErrorValue = fabs((funcRealValue - result.funcValue).toDouble() / unitUlp);
            // errorValues.push_back(ulpErrorValue);
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

                tmpErrorValues[i] = computeError(funcRealValue, (result.funcValue)[i]);

                // absolute error
                // tmpErrorValues[i] = fabs((funcRealValue - (result.funcValue)[i]).toDouble());
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
void shadowValue(const ast_ptr &expr, const std::map<string, T> &varsValue, int length, bool ifUnique, string uniqueLabel, string funcName)
{
    mpfr::mpreal::set_default_prec(128);
    vector<ParamType<T>> funcValues;
    vector<T> errorValues; // this error is f(x+\delta x) between F(x + \delta x)
    if constexpr (std::is_same<double, T>::value)
    {
        std::vector<mpfr::mpreal> funcRealValues;
        std::vector<mpfr::mpreal> mathRealValues;
        shadowValueKernel<T>(expr, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, length);
        // fmt::print("funcValues := {}\n", funcValues);
        for(auto &funcValue : funcValues)
        {
            std::cout << "func value: " << funcValue.toString() << std::endl;
        }
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
        // TODO: free funcValues, funcRealValues, mathRealValues, errorValues
        // print to the screen
        // for(int i = 0; i < length; i++)
        // {
        //     for(size_t j = 0; j < funcValues.size(); j++)
        //     {
        //         fprintf(stderr, "input NO.%d step NO.%ld: funcValue = %s\n", i, j, (funcValues.at(j))[i].toString().c_str());
        //         fprintf(stderr, "input NO.%d step NO.%ld: funcRealValue = %s\n", i, j, (funcRealValues.at(j))[i].toString().c_str());
        //         fprintf(stderr, "input NO.%d step NO.%ld: mathRealValue = %s\n", i, j, (mathRealValues.at(j))[i].toString().c_str());
        //         fprintf(stderr, "input NO.%d step NO.%ld: errorValue = %g\n\n", i, j, (errorValues.at(j))[i]);
        //     }
        //     fprintf(stderr, "================================\n\n");
        // }
        // print to the file
        double **inputsPtr = new double *[varsValue.size()];
        int ptrIdx = 0;
        for(auto &value : varsValue){
            inputsPtr[ptrIdx] = value.second;
            ptrIdx++;
        }
        for(size_t i = 0; i < funcValues.size(); i++)
        {
            std::ofstream fout;

            string filename = "./outputs/"; // i is represent the step of expression
            if(ifUnique)
            {
                filename += uniqueLabel + "/errorFunc_" + funcName + "_" + std::to_string(i) + ".txt"; // i is represent the step of expression
            }
            else
            {
                filename += "errorFunc_" + std::to_string(i) + ".txt"; // i is represent the step of expression
            }
            fout.open(filename, ios::out);
            if (!fout.is_open())
            {
                std::cout << "open " << filename << " failed";
                exit(EXIT_FAILURE);
            }
            for(int j = 0; j < length; j++)
            {
                for(size_t ptrIdx = 0; ptrIdx < varsValue.size(); ptrIdx++) // print all the input variables' value
                {
                    fout << inputsPtr[ptrIdx][j] << " ";
                }
                fout << (errorValues.at(i))[j] << "\n";
            }
        }
        // std::ofstream fout;
        // fout.open("./shadowValueResult.txt", ios::out);
        // if (!fout.is_open())
        // {
        //     std::cout << "open shadowValueResult.txt failed";
        //     exit(EXIT_FAILURE);
        // }
        // fout << "inputIdx stepIdx funcValue funcRealValue mathRealValue errorValue\n";
        // for(int i = 0; i < length; i++)
        // {
        //     for(size_t j = 0; j < funcValues.size(); j++)
        //     {
        //         fout << i << " " << j << " " << (funcValues.at(j))[i] << " " << (funcRealValues.at(j))[i].toString() << " " << (mathRealValues.at(j))[i].toString() << " " << (errorValues.at(j))[i] << "\n";
        //     }
        // }
        // fout << "input NO." << i << " step NO." << j << ": funcValue = " << (funcValues.at(j))[i] << "\n";
        // fout << "input NO." << i << " step NO." << j << ": funcRealValue = " << (funcRealValues.at(j))[i].toString() << "\n";
        // fout << "input NO." << i << " step NO." << j << ": mathRealValue = " << (mathRealValues.at(j))[i].toString() << "\n";
        // fout << "input NO." << i << " step NO." << j << ": errorValue = " << (errorValues.at(j))[i] << "\n";
        // fout << "================================\n\n";
    }
}

template void shadowValue<double*>(const ast_ptr &expr, const std::map<string, double*> &varsValue, int length, bool ifUnique, std::string uniqueLabel, std::string funcName);
template void shadowValue<double>(const ast_ptr &expr, const std::map<string, double> &varsValue, int length, bool ifUnique, std::string uniqueLabel, std::string funcName);

}