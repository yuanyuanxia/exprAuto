#include "basic.hpp"

#include <sstream>
#include <iomanip>
#include <string>
#include <random>
#include <algorithm>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "mpreal.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;
using std::ofstream;
using std::ios;

std::map<string, singleCall> singleCall_map = {
    {"sin", minesin},
    {"cos", minecos},
    {"tan", minetan},
    {"exp", mineexp},
    {"log", minelog},
    {"asin", mineasin},
    {"acos", mineacos},
    {"atan", mineatan},
};

std::map<string, doubleCall> doubleCall_map = {
    {"+", mineadd},
    {"-", minesub},
    {"*", minemul},
    {"/", minediv},
};

std::map<string, commonCall> commonCall_map = {
    {"sin", common_sin},
    {"cos", common_cos},
    {"tan", common_tan},
    {"pow", common_pow},
    {"exp", common_exp},
    {"log", common_log},
    {"asin", common_asin},
    {"acos", common_acos},
    {"atan", common_atan},
    {"expm1", common_expm1},
    {"fma", common_fma},
    {"log1p", common_log1p},
    {"sqrt", common_sqrt},
    {"hypot", common_hypot},
    {"cbrt", common_cbrt},
    {"atan2", common_atan2},
};

std::map<string, doubleRealCall> doubleRealCall_map = {
    {"+", realadd},
    {"-", realsub},
    {"*", realmul},
    {"/", realdiv},
};

std::map<string, commonRealCall> commonRealCall_map = {
    {"sin", real_sin},
    {"cos", real_cos},
    {"tan", real_tan},
    {"pow", real_pow},
    {"exp", real_exp},
    {"log", real_log},
    {"asin", real_asin},
    {"acos", real_acos},
    {"atan", real_atan},
    {"expm1", real_expm1},
    {"fma", real_fma},
    {"log1p", real_log1p},
    {"sqrt", real_sqrt},
    {"hypot", real_hypot},
    {"cbrt", real_cbrt},
    {"atan2", real_atan2},
};

std::map<string, singleCall_dd> singleCall_dd_map = {
    {"sin_d", minesin_dd<double>},
    {"sin_dd", minesin_dd<mpfr::mpreal>},
    {"tan_d", minetan_dd<double>},
    {"tan_dd", minetan_dd<mpfr::mpreal>},
    {"exp_d", mineexp_dd<double>},
    {"exp_dd", mineexp_dd<mpfr::mpreal>},
    {"log_d", minelog_dd<double>},
    {"log_dd", minelog_dd<mpfr::mpreal>},
};

std::map<string, doubleCall_d_d> doubleCall_d_d_map = {
    {"add_d", mineadd_d_d<double>},
    {"add_dd", mineadd_d_d<mpfr::mpreal>},
    {"sub_d", minesub_d_d<double>},
    {"sub_dd", minesub_d_d<mpfr::mpreal>},
    {"mul_d", minemul_d_d<double>},
    {"mul_dd", minemul_d_d<mpfr::mpreal>},
    {"div_d", minediv_d_d<double>},
    {"div_dd", minediv_d_d<mpfr::mpreal>},
};

std::map<string, doubleCall_d_dd> doubleCall_d_dd_map = {
    {"add_d", mineadd_d_dd<double>},
    {"add_dd", mineadd_d_dd<mpfr::mpreal>},
    {"sub_d", minesub_d_dd<double>},
    {"sub_dd", minesub_d_dd<mpfr::mpreal>},
    {"mul_d", minemul_d_dd<double>},
    {"mul_dd", minemul_d_dd<mpfr::mpreal>},
    {"div_d", minediv_d_dd<double>},
    {"div_dd", minediv_d_dd<mpfr::mpreal>},
};

std::map<string, doubleCall_dd_d> doubleCall_dd_d_map = {
    {"add_d", mineadd_dd_d<double>},
    {"add_dd", mineadd_dd_d<mpfr::mpreal>},
    {"sub_d", minesub_dd_d<double>},
    {"sub_dd", minesub_dd_d<mpfr::mpreal>},
    {"mul_d", minemul_dd_d<double>},
    {"mul_dd", minemul_dd_d<mpfr::mpreal>},
    {"div_d", minediv_dd_d<double>},
    {"div_dd", minediv_dd_d<mpfr::mpreal>},
};

std::map<string, doubleCall_dd_dd> doubleCall_dd_dd_map = {
    {"add_d", mineadd_dd_dd<double>},
    {"add_dd", mineadd_dd_dd<mpfr::mpreal>},
    {"sub_d", minesub_dd_dd<double>},
    {"sub_dd", minesub_dd_dd<mpfr::mpreal>},
    {"mul_d", minemul_dd_dd<double>},
    {"mul_dd", minemul_dd_dd<mpfr::mpreal>},
    {"div_d", minediv_dd_dd<double>},
    {"div_dd", minediv_dd_dd<mpfr::mpreal>},
};

//===----------------------------------------------------------------------===//
// basic operation
//===----------------------------------------------------------------------===//

bool isEqual(const ast_ptr &expr1, const ast_ptr &expr2)
{
    if(expr1 == nullptr || expr2 == nullptr)
    {
        fprintf(stderr, "empty\n");
        return false;
    }

    const string expr1Type = expr1->type();
    const string expr2Type = expr2->type();

    if(expr1Type == expr2Type)
    {
        if(expr1->type() == "Number")
        {
            NumberExprAST *numberExpr1 = dynamic_cast<NumberExprAST *>(expr1.get());
            double number1 = (numberExpr1->getNumber());
            NumberExprAST *numberExpr2 = dynamic_cast<NumberExprAST *>(expr2.get());
            double number2 = (numberExpr2->getNumber());

            return (number1 == number2);
        }
        else if(expr1->type() == "Variable")
        {
            VariableExprAST *variableExpr1 = dynamic_cast<VariableExprAST *>(expr1.get());
            string variable1 = (variableExpr1->getVariable());
            VariableExprAST *variableExpr2 = dynamic_cast<VariableExprAST *>(expr2.get());
            string variable2 = (variableExpr2->getVariable());

            return (variable1 == variable2);
        }
        else if(expr1->type() == "Call")
        {
            CallExprAST *callExpr1 = dynamic_cast<CallExprAST *>(expr1.get());
            string callee1 = (callExpr1->getCallee());
            CallExprAST *callExpr2 = dynamic_cast<CallExprAST *>(expr2.get());
            string callee2 = (callExpr2->getCallee());

            if(callee1 == callee2)
            {
                vector<ast_ptr> &args1 = callExpr1->getArgs();
                vector<ast_ptr> &args2 = callExpr2->getArgs();

                if(args1.size() == args2.size())
                {
                    for(long unsigned int i = 0; i < args1.size(); ++i)
                    {
                        if(!isEqual(args1.at(i), args2.at(i)))
                        {
                            return false;
                        }
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if(expr1Type == "Binary")
        {
            BinaryExprAST *binOp1 = dynamic_cast<BinaryExprAST *>(expr1.get());
            char op1 = binOp1->getOp();
            string opStr1(1, op1);
            BinaryExprAST *binOp2 = dynamic_cast<BinaryExprAST *>(expr2.get());
            char op2 = binOp2->getOp();
            string opStr2(1, op2);
            if(opStr1 == opStr2)
            {
                ast_ptr &lhs1 = binOp1->getLHS();
                ast_ptr &rhs1 = binOp1->getRHS();
                ast_ptr &lhs2 = binOp2->getLHS();
                ast_ptr &rhs2 = binOp2->getRHS();

                return (isEqual(lhs1, lhs2) && isEqual(rhs1, rhs2));
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;  // TODO: handle the other cases
        }
    }
    else
    {
        return false;
    }
}

bool isFraction(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: isFraction: the input expr is nullptr!\n");
        return false;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return true;
        }
    }
    return false;
}

bool isConstant(const vector<ast_ptr> &exprs)
{
    size_t size = exprs.size();
    if (size != 1)
    {
        return false;
    }
    const ast_ptr &expr = exprs.at(0);
    if(expr->type() == "Number")
    {
        return true;
    }
    return false;
}

ast_ptr getNumerator(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getNumerator: the input expr is nullptr!\n");
        return nullptr;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return std::move(binOpPtr->getLHS());
        }
    }
    fprintf(stderr, "\tWARNING: getNumerator: the input expr is not match for fraction\n");
    return expr->Clone();
}

ast_ptr getDenominator(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getDenominator: the input expr is nullptr!\n");
        return nullptr;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return std::move(binOpPtr->getRHS());
        }
    }
    fprintf(stderr, "\tWARNING: getDenominator: the input expr is not match for fraction\n");
    return expr->Clone();
}

ast_ptr createBinaryExpr(const ast_ptr &expr1, const ast_ptr &expr2, const char op)
{
    if (expr1 == nullptr && expr2 == nullptr)
    {
        fprintf(stderr, "\tWARNING: createBinaryExpr: all the inputs are nullptr!\n");
        return nullptr;
    }
    if (expr1 == nullptr)
    {
        return expr2->Clone();
    }
    if (expr2 == nullptr)
    {
        return expr1->Clone();
    }
    auto lhs = expr1->Clone();
    auto rhs = expr2->Clone();
    return makePtr<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
}

ast_ptr addExpr(const ast_ptr &expr1, const ast_ptr &expr2) { return createBinaryExpr(expr1, expr2, '+'); }

ast_ptr mulExpr(const ast_ptr &expr1, const ast_ptr &expr2) { return createBinaryExpr(expr1, expr2, '*'); }

ast_ptr divExpr(const ast_ptr &expr1, const ast_ptr &expr2) { return createBinaryExpr(expr1, expr2, '/'); }

void mineAppend(vector<ast_ptr> &dest, vector<ast_ptr> &origin)
{
    dest.insert(dest.end(), std::make_move_iterator(origin.begin()), std::make_move_iterator(origin.end()));
}

//===----------------------------------------------------------------------===//
// print information
//===----------------------------------------------------------------------===//

string PrintExpression(const ast_ptr &expr, const size_t PRECISION)
{
    if(expr == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
// const string exprType = expr->type();
#ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
#endif
    string exprStr = "";
    if(expr->type() == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        // std::unique_ptr<NumberExprAST> numberExpr;
        // if (tmp != nullptr)
        // {
        //     expr.release();
        //     numberExpr.reset(tmp);
        // }
        double number = (numberExpr->getNumber());
#ifdef DEBUG
        fprintf(stderr, "number: %f\n", number);
#endif
        string numberString;
        if(number - int(number) == 0) // to avoid situations like 1 / 6 = 0
        {
            numberString = fmt::format("{:.1f}", number);
        }
        else
        {
            numberString = fmt::format("{}", number);
        }
        return numberString;  // better than stringstream && setprecision. For example, return 0.1 as 0.100...001, or return 10 as 10.1
        // std::stringstream ss;
        // if(number - int(number) == 0)
        // {
        //     ss.precision(1);
        //     ss.setf(std::ios::fixed);
        //     ss << number;
        // }
        // else
        // {
        //     ss << std::setprecision(PRECISION) << number;
        // }
        // return ss.str(); // better than to_string: 1 outputs as 1, 1.23456789 outputs as 1.23456789
        // return std::to_string(number); // really not a good choice: 1 outputs as 1.000000, 1.23456789 outputs as 1.234567
    }
    else if(expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());
#ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
#endif

        return variable;
    }
    else if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        string callee = (callExpr->getCallee());
#ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
#endif
        vector<ast_ptr> &args = callExpr->getArgs();

        vector<string> argsStr;
        for(long unsigned int i = 0; i < args.size(); ++i)
        {
            string strTmp = PrintExpression(args.at(i), PRECISION);  // ast_ptr& exprTmp = args.at(i);
            argsStr.push_back(strTmp);
        }
        callee += "(";
        for(long unsigned int i = 0; i < argsStr.size() - 1; ++i)
        {
            callee += argsStr.at(i) + ", ";
        }
        callee += argsStr.back() + ")";
        return callee;
    }
    else if(expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        // std::unique_ptr<BinaryExprAST> binOp = makePtr<BinaryExprAST>(expr); // a old wrong method
        char op = binOp->getOp();
        string opStr(1, op);
#ifdef DEBUG
        fprintf(stderr, "op: %s\n", opStr.c_str());
#endif

        ast_ptr &lhs = binOp->getLHS();
        string lhsStr = PrintExpression(lhs, PRECISION);
        ast_ptr &rhs = binOp->getRHS();
        string rhsStr = PrintExpression(rhs, PRECISION);

        exprStr += "(" + lhsStr + " " + opStr + " " + rhsStr + ")";
    }
    else
    {
        exprStr = "unknown expression";
    }
    return exprStr;
}

void PrintFunction(std::unique_ptr<FunctionAST> &fun)
{
    if(fun == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
    else
    {
        string funcNameStr = fun->getFuncName();
        vector<string> funcArgsStr = fun->getFuncArgs();
        ast_ptr &expr = fun->getFuncBody();

        fprintf(stderr, "funcName: %s\n", funcNameStr.c_str());
        fprintf(stderr, "Args list:\n");
        if(funcArgsStr.size() == 0)
        {
            fprintf(stderr, "\tempty args");
        }
        else
        {
            for(auto it = funcArgsStr.begin(); it != funcArgsStr.end(); ++it)
            {
                fprintf(stderr, "%s ", (*it).c_str());
            }
        }
        fprintf(stderr, "\nFunc Body:\n");
        // expr->printExpr();
        string funcBodyStr = PrintExpression(expr);
        fprintf(stderr, "\t%s\n", funcBodyStr.c_str());
    }
}

void printExpr(const ast_ptr &expr, string prefix, const size_t PRECISION, int index)
{
    if(index == -1)
        fprintf(stderr, "%s%s\n", prefix.c_str(), PrintExpression(expr, PRECISION).c_str());
    else
        fprintf(stderr, "%sNo.%d: %s\n", prefix.c_str(), index, PrintExpression(expr, PRECISION).c_str());
}

void printExprs(const vector<ast_ptr> &exprs, string prefix, bool showTree, const size_t PRECISION)
{
    for(size_t i = 0; i < exprs.size(); i++)
    {
        auto &expr = exprs.at(i);
        fprintf(stderr, "%sNo.%ld: %s\n", prefix.c_str(), i, PrintExpression(expr, PRECISION).c_str());
        if(showTree)
        {
            printAST(expr);
        }
    }
}

const char blankChar(' ');
const string blankStr(2, blankChar);

void updateStr(string &str, const int posit, const int rightest)
{
    if(posit > rightest)
    {
        string tmpStr(posit - rightest, blankChar);
        str.append(tmpStr);
    }
}

int printASTKernel(const ast_ptr &expr, const int posit, vector<string> &treePics, vector<int> &rightests, const size_t PRECISION = DOUBLE_PRECISION)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "ERROR: printASTKernel: this is a nullptr.\n");
        exit(EXIT_FAILURE);
    }
    if (treePics.size() != rightests.size())
    {
        cerr << "ERROR: printASTKernel: size not equal" << endl;
        exit(EXIT_FAILURE);
    }
    
    static int level = -1;
    level++;
    if (int(treePics.size()) < level + 1) // lhs
    {
        if (int(treePics.size()) != level)
        {
            cerr << "ERROR: printASTKernel: size small" << endl;
            exit(EXIT_FAILURE);
        }
        string tmpStr(posit, blankChar);
        int tmpSize = tmpStr.size();
        treePics.push_back(tmpStr);
        rightests.push_back(tmpSize);
    }

    auto currentPic = treePics.at(level);
    int rightest = currentPic.size(); // int rightest = rightests.at(level); // NOTE: can not change 'auto rightest' to 'auto &rightest' !
    const string exprType = expr->type();
    if(exprType == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());

        updateStr(currentPic, posit, rightest);

        std::stringstream ss;
        ss << std::setprecision(PRECISION) << number;
        currentPic.append(ss.str());
        currentPic.append(blankStr);
        rightest = currentPic.size();
    }
    else if(exprType == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());

        updateStr(currentPic, posit, rightest);

        currentPic.append(variable);
        currentPic.append(blankStr);
        rightest = currentPic.size();
    }
    else if(exprType == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        string callee = (callExpr->getCallee());
        vector<ast_ptr> &args = callExpr->getArgs();

        updateStr(currentPic, posit, rightest);

        currentPic.append(callee + "()");
        currentPic.append(blankStr);
        rightest = currentPic.size();

        if(args.size() == 0)
        {
            cerr << "ERROR: printASTKernel: func has no args" << endl;
            exit(EXIT_FAILURE);
        }
        int childRightest = std::max(0, posit);
        for(auto &arg : args)
        {
            childRightest = printASTKernel(arg, childRightest, treePics, rightests, PRECISION);
        }
        rightest = std::max(rightest, childRightest);
    }
    else if(exprType == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();
        string opStr(1, op);
        ast_ptr &lhs = binOp->getLHS();
        ast_ptr &rhs = binOp->getRHS();

        updateStr(currentPic, posit, rightest);
        currentPic.append(opStr);
        currentPic.append(blankStr);
        
        int childRightest = std::max(0, posit);
        childRightest = printASTKernel(lhs, childRightest, treePics, rightests, PRECISION);        
        childRightest = printASTKernel(rhs, childRightest, treePics, rightests, PRECISION);

        rightest = currentPic.size();
        rightest = std::max(rightest, childRightest);
        
        // TODO: draw lines
        // auto positL = drawLeftLine(posit, currentPic);
        // auto positR = drawRightLine(posit, rightest, currentPic);
    }
    else
    {
        cerr << "ERROR: printASTKernel: unknown expression" << endl;
        exit(EXIT_FAILURE);
    }
    rightests.at(level) = rightest;
    treePics.at(level) = currentPic;
    level--;
    return rightest;
}

void printAST(const ast_ptr &expr, const size_t PRECISION)
{
    vector<string> treePics;
    vector<int> rightests;
    printASTKernel(expr, 0, treePics, rightests, PRECISION);
    for (auto &treePic : treePics)
    {
        cout << std::setprecision(PRECISION) << treePic << endl;
    }
}

void printAST(const ast_ptr &expr, string &result, const size_t PRECISION)
{
    vector<string> treePics;
    vector<int> rightests;
    printASTKernel(expr, 0, treePics, rightests, PRECISION);
    for (auto &treePic : treePics)
    {
        result.append(treePic + "\n");
    }
}

string getMpfrParameterNumber(const ast_ptr &expr, size_t &mpfr_variables) {
    if(expr == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
    string exprStr = "";
    if(expr->type() == "Number")
    {
        ++mpfr_variables;
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());
        return std::to_string(number);
    }
    else if(expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());
        ++mpfr_variables;
        return variable;
    }
    else if(expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        string callee = (callExpr->getCallee());
        vector<ast_ptr> &args = callExpr->getArgs();
        vector<string> argsStr;
        for(long unsigned int i = 0; i < args.size(); ++i)
        {
            string strTmp = getMpfrParameterNumber(args.at(i), mpfr_variables);  // ast_ptr& exprTmp = args.at(i);
            argsStr.push_back(strTmp);
        }
        ++mpfr_variables;
        callee += "(";
        for(long unsigned int i = 0; i < argsStr.size() - 1; ++i)
        {
            callee += argsStr.at(i) + ", ";
        }
        callee += argsStr.back() + ")";
        return callee;
    }
    else if(expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();
        string opStr(1, op);
        ast_ptr &lhs = binOp->getLHS();
        string lhsStr = getMpfrParameterNumber(lhs, mpfr_variables);
        ast_ptr &rhs = binOp->getRHS();
        string rhsStr = getMpfrParameterNumber(rhs, mpfr_variables);
        ++mpfr_variables;
        exprStr += "(" + lhsStr + " " + opStr + " " + rhsStr + ")";
    }
    else
    {
        exprStr = "unknown expression";
    }
    return exprStr;
}

string mpfrCodeGenerator(const ast_ptr &expr, size_t &mpfr_variables, const std::map<string, string> &map, ofstream &ofs, string &variable_tmp) {
    string number_str, variable_str, call_str, binary_str, l, r;
    if (expr == nullptr) {
        fprintf(stderr, "this is a nullptr.\n");
    }
    string exprStr = "";
    if (expr->type() == "Number") {
        ++mpfr_variables;
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());
        std::stringstream ss;
        ss << std::setprecision(DOUBLE_PRECISION) << number;
        number_str = "mpfr_set_d(mp" + to_string(mpfr_variables) + ", " + ss.str() + ", MPFR_RNDN);";
        // cout << number_str << endl;
        ofs << "\t" << number_str << endl;
        return "mp" + to_string(mpfr_variables);
    } else if (expr->type() == "Variable") {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        string variable = (variableExpr->getVariable());
        // if (variable_tmp != variable) {
        //     variable_tmp = variable;
        //     ofs << "\tdouble " << variable_tmp << ";" << endl;
        // }
        // ofs << "\tdouble " << variable << ";" << endl;
        ++mpfr_variables;
        variable_str = "mpfr_set_d(mp" + to_string(mpfr_variables) + ", " + variable + ", MPFR_RNDN);";
        // cout << variable_str << endl;
        ofs << "\t" << variable_str << endl;
        return "mp" + to_string(mpfr_variables);
    } else if (expr->type() == "Call") {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        string callee = (callExpr->getCallee());
        vector<ast_ptr> &args = callExpr->getArgs();
        vector<string> argsStr;
        for (long unsigned int i = 0; i < args.size(); ++i) {
            string strTmp = mpfrCodeGenerator(args.at(i), mpfr_variables, map, ofs, variable_tmp);
            argsStr.push_back(strTmp);
        }
        ++mpfr_variables;
        auto it = map.find(callee);
        call_str = it->second;
        string callee_str;
        if (call_str == "mpfr_pow" || call_str == "mpfr_atan2") {
            string str1 = argsStr.at(0), str2 = argsStr.at(1);
            callee_str = call_str + "(mp" + to_string(mpfr_variables) + ", " + str1 + ", " + str2 + ", MPFR_RNDN);";
        } else if (call_str == "mpfr_fma") {
            string str1 = argsStr.at(0), str2 = argsStr.at(1), str3 = argsStr.at(2);
            callee_str = call_str + "(mp" + to_string(mpfr_variables) + ", " + str1 + ", " + str2 + ", " + str3 + ", MPFR_RNDN);";
        } else {
            string str1 = argsStr.at(0);
            callee_str = call_str + "(mp" + to_string(mpfr_variables) + ", " + str1 + ", MPFR_RNDN);";
        }
        // cout << callee_str << endl;
        ofs << "\t" << callee_str << endl;
        return "mp" + to_string(mpfr_variables);
    } else if (expr->type() == "Binary") {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();
        string opStr(1, op);
        auto it = map.find(opStr);
        string map_str = it->second;
        ast_ptr &lhs = binOp->getLHS();
        string lhsStr = mpfrCodeGenerator(lhs, mpfr_variables, map, ofs, variable_tmp);
        l = lhsStr;
        ast_ptr &rhs = binOp->getRHS();
        string rhsStr = mpfrCodeGenerator(rhs, mpfr_variables, map, ofs, variable_tmp);
        r = rhsStr;
        ++mpfr_variables;
        binary_str = map_str + "(mp" + to_string(mpfr_variables) + ", " + l + ", " + r + ", MPFR_RNDN);";
        // cout << binary_str << endl;
        ofs << "\t" << binary_str << endl;
        return "mp" + to_string(mpfr_variables);
    } else {
        exprStr = "unknown expression";
    }
    return exprStr;
}

std::vector<size_t> geneRandom(size_t start, size_t end)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(start, end);
    std::vector<size_t> results;
    
    results.push_back(dis(gen));
    while(true)
    {
        size_t tmp = dis(gen);
        if(tmp != results[0])
        {
            results.push_back(tmp);
            return results;
        }
    }
}

// 从 indexs 集合中选择 num 个元素进行组合并保证返回的组合中没有重复的元素
vector<vector<int>> combination(const int num, const vector<int> &indexs)
{
    vector<vector<int>> set;
    if(num > (int)indexs.size())
        return set;

    vector<int> elements;
    elements.resize(indexs.size());
    for(int i = 0; i < num; i++)
    {
        elements.at(i) = 1;
    }

    do
    {
        vector<int> currentCombination;
        for(size_t i = 0; i < elements.size(); ++i)
        {
            if(elements[i])
            {
                currentCombination.insert(currentCombination.begin(), indexs[i]);
            }
            else
            {
                currentCombination.push_back(indexs[i]);
            }
        }
        set.push_back(currentCombination);
    } while(prev_permutation(elements.begin(), elements.end()));

    return set;
}

inline unsigned int factorial(unsigned int value)
{
    unsigned int local_value = value;
    while(value-- > 1)
    {
        local_value *= value;
    }
    return local_value;
}

size_t combination(size_t k, size_t n)
{
    if(k > n)
    {
        return 0;
    }
    return factorial(n) / (factorial(k) * factorial(n - k));
}

void write_to_file(const string &uniqueLabel, const string &exprOriginBest, const vector<int> &numIntervalsSoloBefore, const vector<int> &numIntervalsSoloAfter, const std::vector<double> thresholds, const std::vector<double> &data, const std::string &filename)
{
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::app);

    if (outputFile.is_open())
    {
        outputFile << uniqueLabel << ", ";
        outputFile << exprOriginBest << ", ";
        outputFile << fmt::format("\"{}\"", numIntervalsSoloBefore) << ", ";
        outputFile << fmt::format("\"{}\"", numIntervalsSoloAfter) << ", ";
        outputFile << fmt::format("\"{}\"", thresholds) << ", ";

        for (const auto &val : data)
        {
            if(int(val) - val == 0)
                outputFile << fmt::format("{}", val) << ", ";
            else
                outputFile << fmt::format("{:.6e}", val) << ", ";
        }
        outputFile << std::endl;
        outputFile.close();

        // std::cout << "Successfully written data to file!" << std::endl;
    }
    // else
    // {
        // std::cout << "Failed to open file!" << std::endl;
    // }
}

// write to file
void write_to_file_wrapper(string uniqueLabel, string exprOriginBest, int dimension, int numIntervalsBefore, double numOfIntervals, const vector<int> &numIntervalsSoloBefore, const vector<int> &numIntervalsSoloAfter, int numOfExprs, vector<double> thresholds, const exprInfo &originExprInfo, const exprInfo &herbieExprInfo, const exprInfo &finalInfo, double originPerformance, double elapsed_seconds, double init_seconds, double matlab_seconds, double regime_seconds, double rewrite_seconds, double final_seconds, double matlabKernelTime)
{
    vector<double> summaryData;
    summaryData.push_back(dimension);
    summaryData.push_back(numIntervalsBefore);
    summaryData.push_back(numOfIntervals);
    summaryData.push_back(double(numOfExprs));
    // if (thresholds.size() == 1)
    // {
    //     summaryData.push_back(thresholds.at(0));
    //     summaryData.push_back(-1);
    //     summaryData.push_back(-1);
    // }
    // else if (thresholds.size() == 2)
    // {
    //     summaryData.push_back(thresholds.at(0));
    //     summaryData.push_back(thresholds.at(1));
    //     summaryData.push_back(-1);
    // }
    // else if (thresholds.size() == 3)
    // {
    //     summaryData.push_back(thresholds.at(0));
    //     summaryData.push_back(thresholds.at(1));
    //     summaryData.push_back(thresholds.at(2));
    // }
    // else if (thresholds.size() == 4)
    // {
    //     summaryData.push_back(thresholds.at(0));
    //     summaryData.push_back(thresholds.at(1));
    //     summaryData.push_back(thresholds.at(2));
    //     summaryData.push_back(thresholds.at(3));
    // }
    // else if (thresholds.size() == 5)
    // {
    //     summaryData.push_back(thresholds.at(0));
    //     summaryData.push_back(thresholds.at(1));
    //     summaryData.push_back(thresholds.at(2));
    //     summaryData.push_back(thresholds.at(3));
    //     summaryData.push_back(thresholds.at(4));
    // }
    // else
    // {
    //     fprintf(stderr, "ERROR: write_to_file_wrapper: we can not support %ld demision now.\n", thresholds.size());
    //     exit(EXIT_FAILURE);
    // }
    summaryData.push_back(originExprInfo.aveError);
    summaryData.push_back(originExprInfo.maxError);
    summaryData.push_back(herbieExprInfo.aveError);
    summaryData.push_back(herbieExprInfo.maxError);
    summaryData.push_back(finalInfo.aveError);
    summaryData.push_back(finalInfo.maxError);
    summaryData.push_back(originPerformance);
    summaryData.push_back(finalInfo.performance);
    summaryData.push_back(elapsed_seconds);
    summaryData.push_back(init_seconds);
    summaryData.push_back(matlab_seconds);
    summaryData.push_back(regime_seconds);
    summaryData.push_back(rewrite_seconds);
    summaryData.push_back(final_seconds);
    summaryData.push_back(matlabKernelTime);
    write_to_file(uniqueLabel, exprOriginBest, numIntervalsSoloBefore, numIntervalsSoloAfter, thresholds, summaryData, "runlog.csv");
}

template <typename T>
std::map<string, T> setVarsValue(const vector<string> &vars, const vector<T> &values)
{
    if (values.size() != vars.size())
    {
        fprintf(stderr, "values.size() != vars.size(), which are %ld and %ld\n", values.size(), vars.size());
        exit(EXIT_FAILURE);
    }
    std::map<string, T> results;
    for (size_t i = 0; i < vars.size(); i++)
    {
        auto &var = vars.at(i);
        auto &value = values.at(i);
        results[var] = value;
    }
    return results;
}

template std::map<string, double> setVarsValue<double>(const vector<string> &vars, const vector<double> &values);
template std::map<string, double *> setVarsValue<double *>(const vector<string> &vars, const vector<double *> &values);
