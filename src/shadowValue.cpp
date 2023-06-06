#include <fmt/ranges.h>
#include <mpfr.h>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <string>
#include <any>
#include <numeric>
#include "mpreal.h"
#include "shadowValue.hpp"
#include "funclist.hpp"
#include "errorDetect.hpp"


using std::string;
using std::vector;
using std::ofstream;
using std::ios;

namespace Shadow {

std::map<string, std::function<double(double)>> fDrvMapOne = {
    {"sqrt", [](double x) { return 0.5 / sqrt(x); }},
    {"cbrt", [](double x) { return 1 / (3.0 * cbrt(x*x)); }},
    {"exp", [](double x) { return exp(x); }},
    {"exp2", [](double x) { return exp2(x) * log(2); }},
    {"exp10", [](double x) { return exp10(x) * log(10); }},
    {"log", [](double x) { return (1 / x); }},
    {"log2", [](double x) { return (1 / (x * log(2))); }},
    {"log10", [](double x) { return (1 / (x * log(10))); }},
    {"sin", [](double x) { return cos(x); }},
    {"cos", [](double x) { return -sin(x); }},
    {"tan", [](double x) { return 1 / (cos(x) * cos(x)); }},
    {"asin", [](double x) { return 1 / sqrt(1 - x * x); }},
    {"acos", [](double x) { return -1 / sqrt(1 - x * x); }},
    {"atan", [](double x) { return 1 / (1 + x * x); }},
};

std::map<string, std::function<double(double, double)>> fDrvMapTwo = {
    {"addL", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return 1; }},
    {"addR", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return 1; }},
    {"subL", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return 1; }},
    {"subR", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return -1; }},
    {"mulL", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return y; }},
    {"mulR", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return x; }},
    {"divL", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return 1 / y; }},
    {"divR", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return -x / (y * y); }},
    {"powL", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return y * pow(x, y - 1); }},
    {"powR", []([[maybe_unused]] double x, [[maybe_unused]] double y) { return log(x) * pow(x, y); }},
};

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
struct valueThree {
    ParamType<T> funcValue;
    ParamType<T> realValue;
    T ulpValue;
};

template <typename T>
void computeConditionNumberKernel(vector<T> &conditionNumbers, vector<T> &conditionNumbersOp, const int IDfather, int &IDnow, const int length = 1)
{
    constexpr bool TisDouble = (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer = (std::is_same<double *, T>::value);
    if constexpr (TisDouble)
    {
        conditionNumbersOp.at(IDnow) = conditionNumbersOp.at(IDfather) * conditionNumbers.at(IDnow);
    }
    else if constexpr (TisDoublePointer)
    {
        for(int i = 0; i < length; i++)
        {
            conditionNumbersOp.at(IDnow)[i] = conditionNumbersOp.at(IDfather)[i] * conditionNumbers.at(IDnow)[i];
        }
    }
    else
    {
        fprintf(stderr, "ERROR: %s: the type of T %s is not supported\n", __func__, typeid(T).name());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
void computeConditionNumber(const ast_ptr &expr, vector<T> &conditionNumbers, vector<T> &conditionNumbersOp, const int IDfather, int &IDnow, const int length = 1)
{
    auto type = expr->type();
    if(type == "Number")
    {
        return ; // do nothing
    }
    else if (type == "Variable")
    {
        return ; // do nothing
    }
    else if (type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        auto IDparam = new int[args.size()];

        for (int i = args.size() - 1; i >= 0; i--)
        {
            computeConditionNumberKernel(conditionNumbers, conditionNumbersOp, IDfather, IDnow, length);
            IDparam[i] = IDnow;
            IDnow--;
        }
        for (int i = args.size() - 1; i >= 0; i--)
        {
            const auto &arg = args.at(i);
            computeConditionNumber(arg, conditionNumbers, conditionNumbersOp, IDparam[i], IDnow, length);
        }
        delete []IDparam;
    }
    else if (type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();

        computeConditionNumberKernel(conditionNumbers, conditionNumbersOp, IDfather, IDnow, length);
        auto IDrhs = IDnow;
        IDnow--;
        computeConditionNumberKernel(conditionNumbers, conditionNumbersOp, IDfather, IDnow, length);
        auto IDlhs = IDnow;
        IDnow--;

        computeConditionNumber(rhs, conditionNumbers, conditionNumbersOp, IDrhs, IDnow, length);
        computeConditionNumber(lhs, conditionNumbers, conditionNumbersOp, IDlhs, IDnow, length);
    }
    else
    {
        fprintf(stderr, "ERROR: %s : line %d: unknowntype %s\n", __func__, __LINE__, type.c_str());
        exit(EXIT_FAILURE);
    }
}

// The function takes in vecotor<double> v as input, and returns vecotor<double> res, where res[i] represents the rank of the v[i] by descending order."
vector<int> getRanks(const vector<double> &v, bool descending = true)
{
    vector<double> sorted = v;
    if(descending)
    {
        std::sort(sorted.begin(), sorted.end(), std::greater<double>());
    }
    else
    {
        std::sort(sorted.begin(), sorted.end(), std::less<double>());
    }
    auto last = std::unique(sorted.begin(), sorted.end()); // Remove duplicates
    sorted.erase(last, sorted.end());                      // Delete excess elements
    vector<int> res(v.size());
    for (size_t i = 0; i < v.size(); i++)
    {
        auto it = std::find(sorted.begin(), sorted.end(), v[i]); // Find the first occurrence of v[i] in sorted
        res[i] = std::distance(sorted.begin(), it) + 1;          // Record the position, which is the ranking of v[i] in the sorted order
    }

    return res;
}

template <typename T>
vector<string> computeEpsilonE(vector<T> &benefit, vector<T> &epsilonE, const vector<T> &errorValues, const vector<T> &conditionNumbersOp, const vector<int> &condNumOrder, const int length = 1)
{
    constexpr bool TisDouble = (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer = (std::is_same<double *, T>::value);
    if constexpr (TisDouble)
    {
        vector<string> epsilonEStr;
        for(size_t i = 0; i < errorValues.size(); i++)
        {
            epsilonE.at(i) = errorValues.at(i) * conditionNumbersOp.at(condNumOrder.at(i));
            epsilonEStr.push_back(std::to_string(epsilonE.at(i)));
        }
        auto sum = std::accumulate(epsilonE.begin(), epsilonE.end(), 0.0);
        fmt::print("the average of epsilonE are: {}\n", epsilonE);
        std::cout << "The sum of the elements in the vector is: " << sum << std::endl;
        
        // compute benefit
        double minBenefit = std::numeric_limits<double>::infinity();
        int minBenefitIdx = std::numeric_limits<int>::infinity();
        for(size_t i = 0; i < errorValues.size(); i++)
        {
            benefit.at(i) = fabs(sum - epsilonE.at(i));
            if(benefit.at(i) < minBenefit)
            {
                minBenefit = benefit.at(i);
                minBenefitIdx = i;
            }
        }
        fmt::print("the benefit of steps are: {}\n", benefit);
        std::cout << "minBenefitIdx: " << minBenefitIdx << ", minBenefit: " << minBenefit << std::endl;

        return epsilonEStr;
    }
    else if constexpr (TisDoublePointer)
    {
        vector<double> epsilonEAverage(errorValues.size(), 0.0);
        vector<double> errorValuesAverage(errorValues.size(), 0.0);
        vector<double> conditionNumbersOpAverage(errorValues.size(), 0.0);
        vector<double> sumOfAll(length, 0.0);

        // compute epsilonE and its sum. These 3 average values are not useful
        for(size_t i = 0; i < errorValues.size(); i++)
        {
            for(int j = 0; j < length; j++)
            {
                epsilonE.at(i)[j] = errorValues.at(i)[j] * conditionNumbersOp.at(condNumOrder.at(i))[j];
                epsilonEAverage.at(i) += epsilonE.at(i)[j];
                // errorValuesAverage.at(i) += errorValues.at(i)[j];
                // conditionNumbersOpAverage.at(i) += conditionNumbersOp.at(i)[j];
                sumOfAll.at(j) += epsilonE.at(i)[j];
            }
            epsilonEAverage.at(i) /= length;
            // errorValuesAverage.at(i) /= length;
            // conditionNumbersOpAverage.at(i) /= length;
        }
        // auto sum = std::accumulate(epsilonEAverage.begin(), epsilonEAverage.end(), 0.0);
        auto sum = std::accumulate(sumOfAll.begin(), sumOfAll.end(), 0.0);
        auto sumAverage = fabs(sum / length);

        // compute benefit
        vector<double> minBenefit(length, std::numeric_limits<double>::infinity());
        vector<int> minBenefitIdx(length, std::numeric_limits<int>::infinity());
        vector<double> benefitAverage(errorValues.size(), 0.0);
        for(size_t i = 0; i < errorValues.size(); i++)
        {
            for(int j = 0; j < length; j++)
            {
                benefit.at(i)[j] = fabs(sumOfAll.at(j) - epsilonE.at(i)[j]);
                if(benefit.at(i)[j] < minBenefit.at(j))
                {
                    minBenefit.at(j) = benefit.at(i)[j];
                    minBenefitIdx.at(j) = i;
                }
                benefitAverage.at(i) += benefit.at(i)[j];
            }
            benefitAverage.at(i) /= length;
        }
        // for(size_t i = 0; i < errorValues.size(); i++)
        // {
        //     benefitAverage.at(i) = fabs((sum / length) - epsilonEAverage.at(i));
        // }

        // old print
        // auto ranks1 = getRanks(benefitAverage, false);
        // std::cout << "epsilonE rank\n";
        // vector<string> epsilonEStr;
        // for(size_t i = 0; i < epsilonEAverage.size(); i++)
        // {
        //     // fmt::print("{} {}\n", epsilonEAverage.at(i), ranks.at(i));
        //     // fmt::print("rank {}: errorValuesAverage = {} conditionNumbersOpAverage = {} epsilonEAverage = {}\n", ranks.at(i), errorValuesAverage.at(i), conditionNumbersOpAverage.at(i), epsilonEAverage.at(i));
        //     // fmt::print("{}: {:<15e} {:<15e} {:<15e}\n", ranks.at(i), errorValuesAverage.at(i), conditionNumbersOpAverage.at(i), epsilonEAverage.at(i));
        //     // auto epsilonEStrNow = fmt::format("{}: {:<15e} {:<15e} {:<15e}", ranks.at(i), errorValuesAverage.at(i), conditionNumbersOpAverage.at(i), epsilonEAverage.at(i));
        //     auto epsilonEStrNow = fmt::format("{}: {:<15e}", ranks1.at(i), epsilonEAverage.at(i));
        //     epsilonEStr.push_back(epsilonEStrNow);
        // }
        // auto sumEpsilon = std::accumulate(epsilonEAverage.begin(), epsilonEAverage.end(), 0.0);
        // string sumEpsilonStr = fmt::format("The sum of the epsilon is {:g}\n", sumEpsilon);
        // epsilonEStr.push_back(sumEpsilonStr);
        // return epsilonEStr;

        // print
        // fmt::print("the average of benefit are: {}", benefitAverage);
        auto ranks = getRanks(benefitAverage, false);
        fmt::print("{:^4} {:<15}\n", "rank", "benefit");
        // fmt::print("{:<3} {:^4} {:<15}\n", "NO.", "rank", "benefit");
        // fmt::print("{:<3} {:<4} {:<15} {:<15} {:<15} {:<15}\n", "No.", "rank", "errValAver", "condNumOpAver", "epsilonEAver", "benefit");
        vector<string> benefitStr;
        for(size_t i = 0; i < benefitAverage.size(); i++)
        {
            // fmt::print("{} {} {}\n", benefitAverage.at(i), ranks.at(i));
            // fmt::print("rank {}: errorValuesAverage = {} conditionNumbersOpAverage = {} benefitAverage = {}\n", ranks.at(i), errorValuesAverage.at(i), conditionNumbersOpAverage.at(i), benefitAverage.at(i));
            // fmt::print("{}: {:<15e} {:<15e} {:<15e}\n", ranks.at(i), errorValuesAverage.at(i), conditionNumbersOpAverage.at(i), benefitAverage.at(i));
            // auto benefitStrNow = fmt::format("{:<3} {:<4} {:<15e} {:<15e} {:<15e} {:<15e}", i, ranks.at(i), errorValuesAverage.at(i), conditionNumbersOpAverage.at(i), epsilonEAverage.at(i), benefitAverage.at(i));
            auto benefitStrNow = fmt::format("{:^4} {:<15e}", ranks.at(i), benefitAverage.at(i));
            // auto benefitStrNow = fmt::format("{:<3} {:^4} {:<15e}", i, ranks.at(i), benefitAverage.at(i));
            benefitStr.push_back(benefitStrNow);
        }
        string sumStr = fmt::format("The avearge sum of the epsilon is {:g}\n", sumAverage);
        benefitStr.push_back(sumStr);

        return benefitStr;
    }
    else
    {
        fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
valueThree<T> shadowValueKernel(const ast_ptr &expr, const std::map<string, T> &varsValue, vector<ParamType<T>> &funcValues, vector<ParamType<T>> &funcRealValues, vector<ParamType<T>> &mathRealValues, vector<T> &errorValues, vector<T> &conditionNumbers, vector<int> &errorValueOrder, int length = 1)
{
    auto type = expr->type();
    constexpr bool TisDouble = (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer = (std::is_same<double *, T>::value);
    valueThree<T> result;
    if(type == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        auto funcValue = (numberExpr->getNumber());
        if constexpr (TisDouble)
        {
            result.funcValue = funcValue;
            result.realValue = result.funcValue;
            result.ulpValue = computeUlpUnit(funcValue); // may set to ZERO!
            funcValues.push_back(result.funcValue);
            funcRealValues.push_back(result.realValue);
            mathRealValues.push_back(result.realValue);
            errorValues.push_back(0);
            // conditionNumber : do nothing
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new mpfr::mpreal[length];
            auto tmpRealValues = new mpfr::mpreal[length];
            auto tmperrorValues = new double[length]();
            auto tmpUlpValues = new double[length]();
            for(int i = 0; i < length; i++)
            {
                tmpfuncValues[i] = funcValue;
                tmpRealValues[i] = funcValue;
                tmpUlpValues[i] = computeUlpUnit(funcValue); // may set to ZERO!
            }
            result.funcValue = tmpfuncValues;
            result.realValue = tmpRealValues;
            result.ulpValue = tmpUlpValues;
            funcValues.push_back(tmpfuncValues);
            funcRealValues.push_back(tmpRealValues);
            mathRealValues.push_back(tmpRealValues);
            errorValues.push_back(tmperrorValues);
            // conditionNumber : do nothing
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
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
            result.ulpValue = computeUlpUnit(varValue);
            funcValues.push_back(result.funcValue);
            funcRealValues.push_back(result.realValue);
            mathRealValues.push_back(result.realValue);
            errorValues.push_back(0);
            // conditionNumber : do nothing
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new mpfr::mpreal[length];
            auto tmpRealValues = new mpfr::mpreal[length];
            auto tmperrorValues = new double[length]();
            auto tmpUlpValues = new double[length]();
            for(int i = 0; i < length; i++)
            {
                auto &tmpfuncValue = varValue[i];
                tmpfuncValues[i] = tmpfuncValue;
                tmpRealValues[i] = tmpfuncValue;
                tmpUlpValues[i] = computeUlpUnit(tmpfuncValue);
            }
            result.funcValue = tmpfuncValues;
            result.realValue = tmpRealValues;
            result.ulpValue = tmpUlpValues;
            funcValues.push_back(tmpfuncValues);
            funcRealValues.push_back(tmpRealValues);
            mathRealValues.push_back(tmpRealValues);
            errorValues.push_back(tmperrorValues);
            // conditionNumber : do nothing
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
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
        vector<T> paramUlpResults;

        for(auto& arg : args)
        {
            auto tmp = shadowValueKernel<T>(arg, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, conditionNumbers, errorValueOrder, length);
            paramResults.push_back(tmp.funcValue);
            if constexpr (TisDouble)
            {
                paramFuncRealResults.push_back(tmp.funcValue);
                paramResultsDouble.push_back((tmp.funcValue).toDouble());
            }
            parammathRealResults.push_back(tmp.realValue);
            paramUlpResults.push_back(tmp.ulpValue);
            // condition number vector's op order
            errorValueOrder.push_back(arg->getOrder());
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
            if(mapType == "_d_dd")
            {
                double x0[2]{0};
                double x1[2]{0};
                auto tmpDoubleCall = doubleCall_d_dd_map.find(callee + "_" + callTypeStr)->second;
                x0[0] = (paramResults.at(0)).toDouble();
                x1[0] = (paramResults.at(1)).toDouble();
                x1[1] = (paramResults.at(1) - x1[0]).toDouble();
                mpfr::mpreal tmp = tmpDoubleCall(x0[0], x1);
                result.funcValue = tmp;
            }
            else if(mapType == "_dd_d")
            {
                double x0[2]{0};
                double x1[2]{0};
                auto tmpDoubleCall = doubleCall_dd_d_map.find(callee + "_" + callTypeStr)->second;
                x0[0] = (paramResults.at(0)).toDouble();
                x0[1] = (paramResults.at(0) - x0[0]).toDouble();
                x1[0] = (paramResults.at(1)).toDouble();
                mpfr::mpreal tmp = tmpDoubleCall(x0, x1[0]);
                result.funcValue = tmp;
            }
            else if(mapType == "_dd_dd")
            {
                double x0[2]{0};
                double x1[2]{0};
                auto tmpDoubleCall = doubleCall_dd_dd_map.find(callee + "_" + callTypeStr)->second;
                x0[0] = (paramResults.at(0)).toDouble();
                x0[1] = (paramResults.at(0) - x0[0]).toDouble();
                x1[0] = (paramResults.at(1)).toDouble();
                x1[1] = (paramResults.at(1) - x1[0]).toDouble();
                mpfr::mpreal tmp = tmpDoubleCall(x0, x1);
                result.funcValue = tmp;
            }
            else if(mapType == "_d_d")
            {
                double x0[2]{0};
                double x1[2]{0};
                if(callTypeStr == "dd")
                {
                    auto tmpDoubleCall = doubleCall_d_d_map.find(callee + "_" + callTypeStr)->second;
                    x0[0] = (paramResults.at(0)).toDouble();
                    x1[0] = (paramResults.at(1)).toDouble();
                    mpfr::mpreal tmp = tmpDoubleCall(x0[0], x1[0]);
                    result.funcValue = tmp;
                }
                else
                {
                    double tmp = tmpCall(paramResultsDouble);
                    result.funcValue = tmp;
                }
            }
            else if(mapType == "_dd")
            {
                double x1[2]{0};
                x1[0] = paramResults.at(0).toDouble();
                x1[1] = (paramResults.at(0) - x1[0]).toDouble();
                auto tmpSingleCall = singleCall_dd_map.find(callee + "_" + callTypeStr)->second;
                mpfr::mpreal tmp = tmpSingleCall(x1);
                result.funcValue = tmp;
            }
            else if(mapType == "_d")
            {
                if(callTypeStr == "dd")
                {
                    double x1[2]{0};
                    x1[0] = paramResults.at(0).toDouble();
                    auto tmpSingleCall = singleCall_dd_map.find(callee + "_" + callTypeStr)->second;
                    mpfr::mpreal tmp = tmpSingleCall(x1);
                    result.funcValue = tmp;
                }
                else
                {
                    double tmp = tmpCall(paramResultsDouble);
                    result.funcValue = tmp;
                }
            }
            else
            {
                fprintf(stderr, "ERROR: %s : line %d: Invalid map type: %s\n", __func__, __LINE__, mapType.c_str());
                exit(EXIT_FAILURE);
            }
            // std::cout << "callee = " << callPtr->getCallee() << " result = " << result.funcValue << std::endl;
            funcValues.push_back(result.funcValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncValues = new mpfr::mpreal[length];
            if(mapType == "_d_dd")
            {
                double x0[2]{0};
                double x1[2]{0};
                auto tmpDoubleCall = doubleCall_d_dd_map.find(callee + "_" + callTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    x0[0] = ((paramResults.at(0))[i]).toDouble();
                    x1[0] = ((paramResults.at(1))[i]).toDouble();
                    x1[1] = ((paramResults.at(1))[i] - x1[0]).toDouble();
                    tmpfuncValues[i] = tmpDoubleCall(x0[0], x1);
                }
            }
            else if(mapType == "_dd_d")
            {
                double x0[2]{0};
                double x1[2]{0};
                auto tmpDoubleCall = doubleCall_dd_d_map.find(callee + "_" + callTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    x0[0] = ((paramResults.at(0))[i]).toDouble();
                    x0[1] = ((paramResults.at(0))[i] - x0[0]).toDouble();
                    x1[0] = ((paramResults.at(1))[i]).toDouble();
                    tmpfuncValues[i] = tmpDoubleCall(x0, x1[0]);
                }
            }
            else if(mapType == "_dd_dd")
            {
                double x0[2]{0};
                double x1[2]{0};
                auto tmpDoubleCall = doubleCall_dd_dd_map.find(callee + "_" + callTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    x0[0] = ((paramResults.at(0))[i]).toDouble();
                    x0[1] = ((paramResults.at(0))[i] - x0[0]).toDouble();
                    x1[0] = ((paramResults.at(1))[i]).toDouble();
                    x1[1] = ((paramResults.at(1))[i] - x1[0]).toDouble();
                    tmpfuncValues[i] = tmpDoubleCall(x0, x1);
                }
            }
            else if(mapType == "_d_d")
            {
                double x0[2]{0};
                double x1[2]{0};
                if(callTypeStr == "dd")
                {
                    auto tmpDoubleCall = doubleCall_d_d_map.find(callee + "_" + callTypeStr)->second;
                    for(int i = 0; i < length; i++)
                    {
                        x0[0] = ((paramResults.at(0))[i]).toDouble();
                        x1[0] = ((paramResults.at(1))[i]).toDouble();
                        tmpfuncValues[i] = tmpDoubleCall(x0[0], x1[0]);
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
                        tmpfuncValues[i] = tmpCall(tmpParam);
                    }
                }
            }
            else if(mapType == "_dd")
            {
                double x1[2]{0};
                mpfr::mpreal tmp;
                auto tmpSingleCall = singleCall_dd_map.find(callee + "_" + callTypeStr)->second;
                for(int i = 0; i < length; i++)
                {
                    x1[0] = ((paramResults.at(0))[i]).toDouble();
                    x1[1] = ((paramResults.at(0))[i] - x1[0]).toDouble();
                    tmpfuncValues[i] = tmpSingleCall(x1);
                }
            }
            else if(mapType == "_d")
            {
                if(callTypeStr == "dd")
                {
                    auto tmpSingleCall = singleCall_dd_map.find(callee + "_" + callTypeStr)->second;
                    double x1[2]{0};
                    for(int i = 0; i < length; i++)
                    {
                        x1[0] = ((paramResults.at(0))[i]).toDouble();
                        mpfr::mpreal funcValue = tmpSingleCall(x1);
                        tmpfuncValues[i] = funcValue;
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
                        double funcValue = tmpCall(tmpParam);
                        tmpfuncValues[i] = funcValue;
                    }
                }
            }
            else
            {
                fprintf(stderr, "ERROR: %s : line %d: Invalid map type: %s\n", __func__, __LINE__, mapType.c_str());
                exit(EXIT_FAILURE);
            }
            
            result.funcValue = tmpfuncValues;
            funcValues.push_back(tmpfuncValues);
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // real func
        auto tmpRealCall = callPtr->getRealCallback();

        // func real value & math real value & ulp unit value & error value
        if constexpr (TisDouble)
        {
            // func real value
            mpfr::mpreal funcRealValue = tmpRealCall(paramFuncRealResults);
            // std::cout << "callee = " << callPtr->getCallee() << " funcRealValue = " << funcRealValue.toString() << std::endl;
            funcRealValues.push_back(funcRealValue);

            // math real value
            result.realValue = tmpRealCall(parammathRealResults);
            // std::cout << "callee = " << callPtr->getCallee() << " mathRealResult = " << result.realValue.toString() << std::endl;
            mathRealValues.push_back(result.realValue);
            
            // math real value's ulp unit Value
            result.ulpValue = computeUlpUnit(result.realValue);
            
            // error value according funcReal & func & mathReal values
            auto errorValue = computeError(funcRealValue, result.funcValue, result.ulpValue);
            errorValues.push_back(errorValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncRealValues = new mpfr::mpreal[length];
            auto tmpmathRealValues = new mpfr::mpreal[length];
            auto tmpUlpValues = new double[length];
            auto tmpErrorValues = new double[length];
            for(int i = 0; i < length; i++)
            {
                // func real value
                vector<mpfr::mpreal> tmpParam;
                for(auto &param : paramResults)
                {
                    tmpParam.push_back(param[i]);
                }
                auto funcRealValue = tmpRealCall(tmpParam);
                tmpfuncRealValues[i] = funcRealValue;

                // math real value
                vector<mpfr::mpreal> tmpParam1;
                for(auto &param : parammathRealResults)
                {
                    tmpParam1.push_back(param[i]);
                }
                auto mathRealValue = tmpRealCall(tmpParam1);
                tmpmathRealValues[i] = mathRealValue;

                // math real value's ulp unit Value
                tmpUlpValues[i] = computeUlpUnit(mathRealValue);

                // error value according funcReal & func & mathReal values
                tmpErrorValues[i] = computeError(funcRealValue, ((result.funcValue)[i]), tmpUlpValues[i]);
            }
            funcRealValues.push_back(tmpfuncRealValues);
            mathRealValues.push_back(tmpmathRealValues);
            errorValues.push_back(tmpErrorValues);
            result.realValue = tmpmathRealValues;
            result.ulpValue = tmpUlpValues;
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // condition number value
        if constexpr (TisDouble)
        {
            auto &ulpY = result.ulpValue;
            // std::function<double(double)> funcDrv = [](double x){ return exp(x); };
            // std::function<double(double)> funcDrv = &exp; // wrong!
            // std::function<double(double)> funcDrv = exp; // wrong!
            const size_t &lenParam = args.size();
            if (lenParam == 1)
            {
                std::function<double(double)> funcDrv = fDrvMapOne.find(callee)->second;
                auto &ulpX = paramUlpResults.at(0);
                auto input = parammathRealResults.at(0).toDouble();
                auto derivativeFuncValue = funcDrv(input);
                auto conditionNumber = ulpX * derivativeFuncValue / ulpY;
                conditionNumbers.push_back(conditionNumber);
            }
            else if (lenParam == 2)
            {
                std::function<double(double, double)> funcDrv;
                auto inputX1 = parammathRealResults.at(0).toDouble();
                auto inputX2 = parammathRealResults.at(1).toDouble();
                
                funcDrv = fDrvMapTwo.find(callee + "L")->second;
                auto derivativeFuncValue = funcDrv(inputX1, inputX2);
                auto &ulpX1 = paramUlpResults.at(0);
                auto conditionNumber = ulpX1 * derivativeFuncValue / ulpY;
                conditionNumbers.push_back(conditionNumber);

                funcDrv = fDrvMapTwo.find(callee + "R")->second;
                derivativeFuncValue = funcDrv(inputX1, inputX2);
                auto &ulpX2 = paramUlpResults.at(1);
                conditionNumber = ulpX2 * derivativeFuncValue / ulpY;
                conditionNumbers.push_back(conditionNumber);
            }
            else
            {
                fprintf(stderr, "ERROR: %s : line %d: we can not support %ld parameters' function call\n", __func__, __LINE__, lenParam);
                exit(EXIT_FAILURE);
            }
            // for(size_t i = 0; i < parammathRealResults.size(); i++)
            // {
            //     auto &ulpX = paramUlpResults.at(i);
            //     auto input = parammathRealResults.at(i).toDouble();
            //     auto derivativeFuncValue = funcDrv(input);
            //     auto conditionNumber = (ulpX * derivativeFuncValue / ulpY).toDouble();
            //     conditionNumbers.push_back(conditionNumber);
            // }
        }
        else if constexpr (TisDoublePointer)
        {
            auto &ulpYs = result.ulpValue;
            // std::function<double(double)> funcDrv = [](double x){ return exp(x); };
            // std::function<double(double)> funcDrv = &exp; // wrong!
            // std::function<double(double)> funcDrv = exp; // wrong!
            const size_t &lenParam = args.size();
            if (lenParam == 1)
            {
                std::function<double(double)> funcDrv = fDrvMapOne.find(callee)->second;
                auto tmpConditionNumbers = new double[length];
                for(int i = 0; i < length; i++)
                {
                    auto ulpX = paramUlpResults.at(0)[i];
                    auto input = parammathRealResults.at(0)[i].toDouble();
                    auto derivativeFuncValue = funcDrv(input);
                    tmpConditionNumbers[i] = ulpX * derivativeFuncValue / ulpYs[i];
                }
                conditionNumbers.push_back(tmpConditionNumbers);
            }
            else if (lenParam == 2)
            {
                std::function<double(double, double)> funcDrvL, funcDrvR;
                auto tmpConditionNumbers1 = new double[length];
                auto tmpConditionNumbers2 = new double[length];
                funcDrvL = fDrvMapTwo.find(callee + "L")->second;
                funcDrvR = fDrvMapTwo.find(callee + "R")->second;
                for(int i = 0; i < length; i++)
                {
                    auto inputX1 = parammathRealResults.at(0)[i].toDouble();
                    auto inputX2 = parammathRealResults.at(1)[i].toDouble();

                    auto derivativeFuncValue = funcDrvL(inputX1, inputX2);
                    auto &ulpX1 = paramUlpResults.at(0)[i];
                    tmpConditionNumbers1[i] = ulpX1 * derivativeFuncValue / ulpYs[i];

                    derivativeFuncValue = funcDrvR(inputX1, inputX2);
                    auto &ulpX2 = paramUlpResults.at(1)[i];
                    tmpConditionNumbers2[i] = ulpX2 * derivativeFuncValue / ulpYs[i];
                }
                conditionNumbers.push_back(tmpConditionNumbers1);
                conditionNumbers.push_back(tmpConditionNumbers2);
            }
            else
            {
                fprintf(stderr, "ERROR: %s : line %d: we can not support %ld parameters' function call\n", __func__, __LINE__, lenParam);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
            exit(EXIT_FAILURE);
        }

        return result;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        auto lhsValue = shadowValueKernel<T>(lhs, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, conditionNumbers, errorValueOrder, length);
        auto rhsValue = shadowValueKernel<T>(rhs, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, conditionNumbers, errorValueOrder, length);

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

        // condition number vector's op order
        errorValueOrder.push_back(lhs->getOrder());
        errorValueOrder.push_back(rhs->getOrder());

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
                x2[1] = (rhsValue.funcValue - x2[0]).toDouble();
                result.funcValue = tmpDoubleCall(x1[0], x2);
            }
            else if(mapType == "dd_d")
            {
                auto tmpDoubleCall = doubleCall_dd_d_map.find(opStr + "_" + opTypeStr)->second;
                x1[1] = (lhsValue.funcValue - x1[0]).toDouble();
                result.funcValue = tmpDoubleCall(x1, x2[0]);
            }
            else if(mapType == "dd_dd")
            {
                auto tmpDoubleCall = doubleCall_dd_dd_map.find(opStr + "_" + opTypeStr)->second;
                x1[1] = (lhsValue.funcValue - x1[0]).toDouble();
                x2[1] = (rhsValue.funcValue - x2[0]).toDouble();
                result.funcValue = tmpDoubleCall(x1, x2);
            }
            else if(mapType == "d_d")
            {
                if(opTypeStr == "dd")
                {
                    auto tmpDoubleCall = doubleCall_d_d_map.find(opStr + "_" + opTypeStr)->second;
                    result.funcValue = tmpDoubleCall(x1[0], x2[0]);
                }
                else
                {
                    result.funcValue = tmpCall(x1[0], x2[0]);
                }
            }
            else
            {
                fprintf(stderr, "ERROR: %s : line %d: Invalid map type: %s\n", __func__, __LINE__, mapType.c_str());
                exit(EXIT_FAILURE);
            }
            // std::cout << "op = " << binPtr->getOp() << " result = " << result.funcValue << std::endl;
            funcValues.push_back(result.funcValue);

        }
        // TODO: (DONE) 按照double的方法，需要考虑输入为d_d的情况。目前这样可以解决模型不唯一的问题。
        // TODO: 下一步就是怎么把代码实现对应到模型上。可能需要重新考虑把当前步骤更改为dd带来的影响。
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
            else if(mapType == "d_d")
            {
                if(opTypeStr == "dd")
                {
                    auto tmpDoubleCall = doubleCall_d_d_map.find(opStr + "_" + opTypeStr)->second;
                    for(int i = 0; i < length; i++)
                    {
                        auto &tmpLhsValue = (lhsValue.funcValue)[i];
                        auto &tmpRhsValue = (rhsValue.funcValue)[i];
                        tmpfuncValues[i] = tmpDoubleCall(tmpLhsValue.toDouble(), tmpRhsValue.toDouble());
                    }
                }
                else
                {
                    for(int i = 0; i < length; i++)
                    {
                        auto &tmpLhsValue = (lhsValue.funcValue)[i];
                        auto &tmpRhsValue = (rhsValue.funcValue)[i];
                        tmpfuncValues[i] = tmpCall(tmpLhsValue.toDouble(), tmpRhsValue.toDouble());
                    }
                }
            }
            else
            {
                fprintf(stderr, "ERROR: %s : line %d: Invalid map type: %s\n", __func__, __LINE__, mapType.c_str());
                exit(EXIT_FAILURE);
            }
            result.funcValue = tmpfuncValues;
            funcValues.push_back(tmpfuncValues);
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // real func
        auto tmpRealCall = binPtr->getRealCallback();

        // func real value & math real value & ulp unit value & error value
        if constexpr (TisDouble)
        {
            // func Real Value
            mpfr::mpreal funcRealValue = tmpRealCall(lhsValue.funcValue, rhsValue.funcValue);
            // std::cout << "op = " << binPtr->getOp() << " funcRealValue = " << funcRealValue.toString() << std::endl;
            funcRealValues.push_back(funcRealValue);

            // math real value
            result.realValue = tmpRealCall(lhsValue.realValue, rhsValue.realValue);
            // std::cout << "op = " << binPtr->getOp() << " mathRealValue = " << result.realValue.toString() << std::endl;
            mathRealValues.push_back(result.realValue);

            // math real value's ulp unit Value
            result.ulpValue = computeUlpUnit(result.realValue);

            // error value according funcReal & func & mathReal values
            // absolute error
            // double absErrorValue = fabs((funcRealValue - result.funcValue).toDouble());
            // errorValues.push_back(absErrorValue);
            // relative error
            // double relErrorValue = fabs(((funcRealValue - result.funcValue) / funcRealValue).toDouble());
            // errorValues.push_back(relErrorValue);
            // ulp error
            auto errorValue = computeError(funcRealValue, result.funcValue, result.ulpValue);
            errorValues.push_back(errorValue);
        }
        else if constexpr (TisDoublePointer)
        {
            auto tmpfuncRealValues = new mpfr::mpreal[length];
            auto tmpmathRealValues = new mpfr::mpreal[length];
            auto tmpUlpValues = new double[length];
            auto tmpErrorValues = new double[length];
            for(int i = 0; i < length; i++)
            {
                // func real value
                mpfr::mpreal tmpLhsValue = (lhsValue.funcValue)[i];
                mpfr::mpreal tmpRhsValue = (rhsValue.funcValue)[i];
                auto funcRealValue = tmpRealCall(tmpLhsValue, tmpRhsValue);
                tmpfuncRealValues[i] = funcRealValue;

                // math real value
                tmpLhsValue = (lhsValue.realValue)[i];
                tmpRhsValue = (rhsValue.realValue)[i];
                auto mathRealValue = tmpRealCall(tmpLhsValue, tmpRhsValue);
                tmpmathRealValues[i] = mathRealValue;

                // math real value's ulp unit value
                tmpUlpValues[i] = computeUlpUnit(mathRealValue);

                // error value
                tmpErrorValues[i] = computeError(funcRealValue, (result.funcValue)[i], tmpUlpValues[i]);
            }
            funcRealValues.push_back(tmpfuncRealValues);
            mathRealValues.push_back(tmpmathRealValues);
            errorValues.push_back(tmpErrorValues);
            result.realValue = tmpmathRealValues;
            result.ulpValue = tmpUlpValues;
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
            exit(EXIT_FAILURE);
        }

        // condition number value
        if constexpr (TisDouble)
        {
            auto &ulpY = result.ulpValue;
            std::function<double(double, double)> funcDrv;

            auto inputLhs = lhsValue.realValue.toDouble();
            auto inputRhs = rhsValue.realValue.toDouble();

            funcDrv = fDrvMapTwo.find(opStr + "L")->second;
            auto derivativeFuncValue = funcDrv(inputLhs, inputRhs);
            auto &ulpXLhs = lhsValue.ulpValue;
            auto conditionNumber = ulpXLhs * derivativeFuncValue / ulpY;
            conditionNumbers.push_back(conditionNumber);

            funcDrv = fDrvMapTwo.find(opStr + "R")->second;
            derivativeFuncValue = funcDrv(inputLhs, inputRhs);
            auto &ulpXRhs = rhsValue.ulpValue;
            conditionNumber = ulpXRhs * derivativeFuncValue / ulpY;
            conditionNumbers.push_back(conditionNumber);
        }
        else if constexpr (TisDoublePointer)
        {
            auto &ulpYs = result.ulpValue;

            std::function<double(double, double)> funcDrvL, funcDrvR;
            auto tmpConditionNumbers1 = new double[length];
            auto tmpConditionNumbers2 = new double[length];
            funcDrvL = fDrvMapTwo.find(opStr + "L")->second;
            funcDrvR = fDrvMapTwo.find(opStr + "R")->second;
            for(int i = 0; i < length; i++)
            {
                auto inputLhs = lhsValue.realValue[i].toDouble();
                auto inputRhs = rhsValue.realValue[i].toDouble();

                auto derivativeFuncValue = funcDrvL(inputLhs, inputRhs);
                auto &ulpX1 = lhsValue.ulpValue[i];
                tmpConditionNumbers1[i] = ulpX1 * derivativeFuncValue / ulpYs[i];

                derivativeFuncValue = funcDrvR(inputLhs, inputRhs);
                auto &ulpX2 = rhsValue.ulpValue[i];
                tmpConditionNumbers2[i] = ulpX2 * derivativeFuncValue / ulpYs[i];
            }
            conditionNumbers.push_back(tmpConditionNumbers1);
            conditionNumbers.push_back(tmpConditionNumbers2);
        }
        else
        {
            fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
            exit(EXIT_FAILURE);
        }

        return result;
    }
    else
    {
        fprintf(stderr, "ERROR: %s : line %d: unknown type %s\n", __func__, __LINE__, type.c_str());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
void shadowValueInit(vector<T> &conditionNumbersOp, vector<T> &benefit, vector<T> &epsilonE, const size_t condNumOpSize, const int length)
{
    constexpr bool TisDouble = (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer = (std::is_same<double *, T>::value);
    if constexpr (TisDouble)
    {
        for(size_t i = 0; i < condNumOpSize; i++)
        {
            conditionNumbersOp.push_back(1);
            benefit.push_back(0);
            epsilonE.push_back(0);
        }
    }
    else if constexpr (TisDoublePointer)
    {
        for(size_t i = 0; i < condNumOpSize; i++)
        {
            auto tmp = new double[length];
            conditionNumbersOp.push_back(tmp);
        }
        for(int i = 0; i < length; i++)
        {
            conditionNumbersOp.back()[i] = 1;
        }
        for(size_t i = 0; i < condNumOpSize; i++)
        {
            auto tmp = new double[length];
            benefit.push_back(tmp);
        }
        for(size_t i = 0; i < condNumOpSize; i++)
        {
            auto tmp = new double[length];
            epsilonE.push_back(tmp);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
void shadowValuePrint(const vector<ParamType<T>> &funcValues, const vector<ParamType<T>> &funcRealValues, const vector<ParamType<T>> &mathRealValues, const vector<T> &errorValues, const vector<T> &conditionNumbers, const vector<T> &conditionNumbersOp, [[maybe_unused]] const vector<T> &epsilonE, const std::map<string, T> &varsValue, const int length, const bool ifUnique, const string uniqueLabel, const string funcName)
{
    constexpr bool TisDouble = (std::is_same<double, T>::value);
    constexpr bool TisDoublePointer = (std::is_same<double *, T>::value);
    if constexpr (TisDouble)
    {
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
        fmt::print("conditionNumbers := {}\n", conditionNumbers);
        fmt::print("conditionNumbersOp := {}\n", conditionNumbersOp);
        fmt::print("epsilonE := {}\n", epsilonE);
    }
    else if constexpr (TisDoublePointer)
    {
        // TODO: free funcValues, funcRealValues, mathRealValues, errorValues
        // print to the screen
        for(int i = 0; i < length; i++)
        {
            for(size_t j = 0; j < funcValues.size(); j++)
            {
                fprintf(stderr, "input NO.%d step NO.%ld: funcValue = %s\n", i, j, (funcValues.at(j))[i].toString().c_str());
                fprintf(stderr, "input NO.%d step NO.%ld: funcRealValue = %s\n", i, j, (funcRealValues.at(j))[i].toString().c_str());
                fprintf(stderr, "input NO.%d step NO.%ld: mathRealValue = %s\n", i, j, (mathRealValues.at(j))[i].toString().c_str());
                fprintf(stderr, "input NO.%d step NO.%ld: errorValue = %g\n", i, j, (errorValues.at(j))[i]);
                if (j != funcValues.size() - 1)
                    fprintf(stderr, "input NO.%d step NO.%ld: conditionNumbers = %g\n", i, j, (conditionNumbers.at(j))[i]);
                fprintf(stderr, "input NO.%d step NO.%ld: conditionNumbersOp = %g\n\n", i, j, (conditionNumbersOp.at(j))[i]);
            }
            fprintf(stderr, "================================\n\n");
        }
        // print to the file
        double **inputsPtr = new double *[varsValue.size()];
        int ptrIdx = 0;
        for(auto &value : varsValue){
            inputsPtr[ptrIdx] = value.second;
            ptrIdx++;
        }
        for(size_t i = 0; i < funcValues.size(); i++) // i is represent the step of expression
        {
            std::ofstream fout;

            string filename = "./outputs/";
            if(ifUnique)
            {
                filename += uniqueLabel + "/errorFunc_" + funcName + "_" + std::to_string(i) + ".txt";
            }
            else
            {
                filename += "errorFunc_" + std::to_string(i) + ".txt";
            }
            fout.open(filename, ios::out);
            if (!fout.is_open())
            {
                fprintf(stderr, "ERROR: %s : line %d: open %s failed\n", __func__, __LINE__, filename.c_str());
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
    else
    {
        fprintf(stderr, "ERROR: %s : line %d: the type of T %s is not supported\n", __func__, __LINE__, typeid(T).name());
        exit(EXIT_FAILURE);
    }
}

template <typename T>
vector<string> shadowValue(const ast_ptr &expr, const std::map<string, T> &varsValue, int length, bool ifUnique, string uniqueLabel, string funcName)
{
    mpfr::mpreal::set_default_prec(128);
    vector<ParamType<T>> funcValues;
    vector<T> errorValues; // this error is f(x+\delta x) between F(x + \delta x)
    vector<T> conditionNumbers;
    std::vector<ParamType<T>> funcRealValues;
    std::vector<ParamType<T>> mathRealValues;
    vector<int> errorValueOrder; // for conditionNumbers vector, store the corresponding errorValue order
    shadowValueKernel<T>(expr, varsValue, funcValues, funcRealValues, mathRealValues, errorValues, conditionNumbers, errorValueOrder, length);
    errorValueOrder.push_back(conditionNumbers.size()); // 令condNumber最后一个元素指向conditionNumber的最后一个元素，同时保证其长度同errorValues、conditionNumberOp一致
    // fmt::print("errorValueOrder: {}\n", errorValueOrder);
    vector<int> condNumOrder; // for errorValue vector, store the corresponding conditionNumbers order
    for(int i = 0; i < (int)errorValueOrder.size(); i++)
    {
        vector<int>::iterator iter = std::find(errorValueOrder.begin(), errorValueOrder.end(), i);
        if (iter == errorValueOrder.end())
        {
            fprintf(stderr, "ERROR: %s : line %d: %d can not be found in errorValueOrder\n", __func__, __LINE__, i);
            exit(EXIT_FAILURE);
        }
        else
        {
            auto tmp = std::distance(errorValueOrder.begin(), iter);
            condNumOrder.push_back(tmp);
        }
    }
    // fmt::print("condNumOrder: {}\n", condNumOrder);
    vector<T> conditionNumbersOp;
    vector<T> benefit, epsilonE;
    shadowValueInit(conditionNumbersOp, benefit, epsilonE, conditionNumbers.size() + 1, length);
    int IDfather = conditionNumbersOp.size() - 1;
    int IDnow = conditionNumbers.size() - 1;
    computeConditionNumber(expr, conditionNumbers, conditionNumbersOp, IDfather, IDnow, length);
    auto epsilonEStr = computeEpsilonE(benefit, epsilonE, errorValues, conditionNumbersOp, condNumOrder, length);

    // shadowValuePrint(funcValues, funcRealValues, mathRealValues, errorValues, conditionNumbers, conditionNumbersOp, epsilonE, varsValue, length, ifUnique, uniqueLabel, funcName);
    return epsilonEStr;
}

template vector<string> shadowValue<double*>(const ast_ptr &expr, const std::map<string, double*> &varsValue, int length, bool ifUnique, std::string uniqueLabel, std::string funcName);
template vector<string> shadowValue<double>(const ast_ptr &expr, const std::map<string, double> &varsValue, int length, bool ifUnique, std::string uniqueLabel, std::string funcName);

}