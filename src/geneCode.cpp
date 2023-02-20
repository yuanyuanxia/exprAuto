#include "basic.hpp"
#include "geneCode.hpp"
#include "parserASTLY.hpp"

#include <algorithm>

using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::ofstream;
using std::string;
using std::to_string;

// 0.000287413s
vector<string> getVariablesFromExpr(const ast_ptr &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: the input is nullptr");
        exit(EXIT_FAILURE);
    }
    auto type = expr->type();
    if (type == "Number")
    {
        return {};
    }
    else if (type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        vector<string> vars{var};
        return vars;
    }
    else if (type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        vector<string> vars;
        for (auto &arg : args)
        {
            auto varsTmp = getVariablesFromExpr(arg);
            vars.insert(vars.end(), varsTmp.begin(), varsTmp.end());
        }
        std::sort(vars.begin(), vars.end());
        vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
        return vars;
    }
    if (type != "Binary")
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
    vars.insert(vars.end(), varsL.begin(), varsL.end());
    vars.insert(vars.end(), varsR.begin(), varsR.end());
    std::sort(vars.begin(), vars.end());
    vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
    return vars;
}

// this version is more faster. 0.000232973s
bool getVariablesFromExpr(const ast_ptr &expr, vector<string> &vars)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: the input is nullptr\n");
        exit(EXIT_FAILURE);
    }
    auto type = expr->type();
    if (type == "Number")
    {
        return true;
    }
    else if (type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        vars.push_back(var);
        return true;
    }
    else if (type == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        for (auto &arg : args)
        {
            if (!getVariablesFromExpr(arg, vars))
            {
                fprintf(stderr, "ERROR: getVariablesFromExpr: run failed\n");
                exit(EXIT_FAILURE);
            }
        }
        std::sort(vars.begin(), vars.end()); // sort to unique the parameters queue
        vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
        return true;
    }
    if (type != "Binary")
    {
        fprintf(stderr, "ERROR: getVariablesFromExpr: type %s is wrong\n", type.c_str());
        exit(EXIT_FAILURE);
    }
    BinaryExprAST *binaryPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    auto &lhs = binaryPtr->getLHS();
    auto &rhs = binaryPtr->getRHS();
    getVariablesFromExpr(lhs, vars);
    getVariablesFromExpr(rhs, vars);
    std::sort(vars.begin(), vars.end()); // sort to unique the parameters queue
    vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
    return true;
}

string geneOriginCodeKernel(string exprStr, vector<string> vars, string uniqueLabel, string tail)
{
    // print input info
    // cout << "expression    : " << exprStr << endl;
    // cout << "variables list:";
    // for (auto &var : vars)
    // {
    //     cout << " " << var;
    // }
    // cout << endl;

    std::ofstream fout;
    // expr_uniquelabel_origin.c
    string directory = "srcTest/" + uniqueLabel + "/";
    string funcName = "expr_" + uniqueLabel + "_" + tail;
    string fileName = directory + funcName + ".c";
    // string fileName = "./outputs/" + funcName + ".c";
    fout.open(fileName);
    fout << "#include <math.h>\n";
    fout << "double " << funcName << "(";
    for (size_t i = 0; i < vars.size(); ++i)
    {
        if (i != vars.size() - 1)
            fout << "double" << " " << vars.at(i) << ", ";
        else
            fout << "double" << " " << vars.at(i);
    }
    fout << ")\n";
    fout << "{\n";
    fout << "\t" << "double result = " << exprStr << ";\n";
    fout << "\t" << "return result;\n";
    fout << "}\n";
    fout.close();

    return funcName;
}

string geneOriginCode(string exprStr, string uniqueLabel, string tail)
{
    auto originExpr = ParseExpressionFromString(exprStr);

    vector<string> vars;
    getVariablesFromExpr(originExpr, vars);
    auto funcName = geneOriginCodeKernel(exprStr, vars, uniqueLabel, tail);

    return funcName;
}

string geneTGenCode(string exprStr, vector<string> vars, string uniqueLabel, string tail)
{
    // replace func to func_gen
    string exprNewStr(exprStr);
    vector<string> supportFuncList = {"sin", "cos", "tan", "exp", "log", "sinh", "cosh", "tanh"};
    for(auto replaceFrom : supportFuncList)
    {
        string replaceTo(replaceFrom);
        replaceTo.append("_gen(");
        replaceFrom += "(";
        int pos = exprNewStr.find(replaceFrom);
        while(pos != -1)
        {
            exprNewStr.replace(pos, replaceFrom.length(), replaceTo);
            pos = exprNewStr.find(replaceFrom);
        }
    }
    // generate TGen code
    std::ofstream fout;
    string directory = "srcTest/" + uniqueLabel + "/";
    string funcName = "expr_" + uniqueLabel + "_" + tail;
    string fileName = directory + funcName + ".c";
    fout.open(fileName);
    fout << "#include <math.h>\n";
    fout << "#include \"tgen.h\"\n"; // this one is important
    fout << "double " << funcName << "(";
    for (size_t i = 0; i < vars.size(); ++i)
    {
        if (i != vars.size() - 1)
            fout << "double" << " " << vars.at(i) << ", ";
        else
            fout << "double" << " " << vars.at(i);
    }
    fout << ")\n";
    fout << "{\n";
    fout << "\t" << "double result = " << exprNewStr << ";\n";
    fout << "\t" << "return result;\n";
    fout << "}\n";
    fout.close();

    return funcName;
}

void geneHerbieCode(string exprstr, vector<string> cs, string exprname, double v[], double u[])
{
    std::ofstream fout;
    fout.open("./herbie.info"); 
    fout << "(FPCore" << endl;
    fout << "(";
    for(size_t i = 0; i<(cs.size()-1); i++)//输出参数
    {
        fout<<cs[i]<<" ";
    }
    fout << cs[cs.size()-1]; 
    fout << ")" << endl;
    fout << ":name" << endl;//输出表达式名字
    fout << "\"" << exprname << "\"" << endl;
    fout << ":pre" << endl;
    fout << "(and ";
    for(size_t j = 0; j < (cs.size()-1); j++)//输出范围
    {
        fout << "\t(<= " << v[j] << " " << cs[j] << " " << u[j] << ")" << endl;
    }
    fout << "\t" << "(<= " << v[cs.size()-1] << " " << cs[cs.size()-1] << " " << u[cs.size()-1] << "))" << endl;
    fout << "(" << endl;
    fout << exprstr << ")" << endl;
    fout.close();
}

void geneDaisyCode(string exprStr)
{
    cout << exprStr << endl;
}

string geneMpfrCode(const ast_ptr &exprAst, const string uniqueLabel, vector<string> vars)
{
    string directory = "srcTest/" + uniqueLabel + "/";
    string funcName = "expr_" + uniqueLabel + "_mpfr";
    string fileName = directory + funcName + ".c";
    ofstream file_clean(fileName, ios_base::out);
    ofstream ofs(fileName, ios::app);

    std::map<string, string> mpfr_map = {
        {"+", "mpfr_add"},
        {"-", "mpfr_sub"},
        {"*", "mpfr_mul"},
        {"/", "mpfr_div"},
        {"exp", "mpfr_exp"},
        {"log", "mpfr_log"},
        {"pow", "mpfr_pow"},
        {"sqrt", "mpfr_sqrt"},
        {"sin", "mpfr_sin"},
        {"cos", "mpfr_cos"},
        {"tan", "mpfr_tan"},
        {"asin", "mpfr_asin"},
        {"acos", "mpfr_acos"},
        {"atan", "mpfr_atan"},
        {"sinh", "mpfr_sinh"},
        {"cosh", "mpfr_cosh"},
        {"tanh", "mpfr_tanh"},
    };
    // vector<string> vars;
    // getVariablesFromExpr(exprAst, vars);
    size_t mpfr_variables = 0;
    getMpfrParameterNumber(exprAst, mpfr_variables);

    ofs << "#include <stdio.h>\n"
        << "#include <gmp.h>\n"
        << "#include <math.h>\n"
        << "#include <mpfr.h>\n"
        << "int " << funcName << "(";
    for (auto &var : vars)
    {
        ofs << "double" << " " << var << ", ";
    }
    ofs << "mpfr_t mpfrResult) {\n"
        << "\tint status = 0;\n"
        << "\tmpfr_t ";
    for (size_t i = 0; i < mpfr_variables; ++i)
    {
        if (i != mpfr_variables - 1)
            ofs << "mp" + to_string(i + 1) + ", ";
        else
            ofs << "mp" + to_string(i + 1) + ";\n";
    }
    for (size_t i = 0; i < mpfr_variables; ++i)
    {
        ofs << "\tmpfr_init2(mp" + to_string(i + 1) + ", 128);\n";
    }
    mpfr_variables = 0;
    string variable_tmp = "";
    mpfrCodeGenerator(exprAst, mpfr_variables, mpfr_map, ofs, variable_tmp);
    ofs << "\n\tmpfr_set(mpfrResult, mp" << mpfr_variables << ", MPFR_RNDN);\n";
    for (size_t i = 0; i < mpfr_variables; ++i)
    {
        ofs << "\tmpfr_clear(mp" + to_string(i + 1) + ");\n";
    }
    ofs << "\treturn status;\n"
        << "}";

    return funcName;
}

string geneMpfrCode(const string exprStr, const string uniqueLabel, vector<string> vars)
{
    const auto exprAst = ParseExpressionFromString(exprStr);
    auto funcName = geneMpfrCode(exprAst, uniqueLabel, vars);
    return funcName;
}

// generate final code
string geneFinalCodeKernel(string exprStr, string uniqueLabel, std::vector<exprInfo> exprInfoVector, vector<string> vars)
{
    cout << "\n&&&&&&&&&&&&&&&&&&&&&&& geneFinalCode &&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;

    cout << "size of exprInfoVector:" << exprInfoVector.size() << endl;
    cout << "the general information" << endl;
    for (size_t i = 0; i < exprInfoVector.size(); i++)
    {
        cout << "Interval NO." << i << endl;
        cout << "\trewriteID: " << exprInfoVector.at(i).rewriteID << endl;
        cout << "\taveError: " << exprInfoVector.at(i).aveError << "\tmaxError: " << exprInfoVector.at(i).maxError << endl;
        cout << "\texpr: " << exprInfoVector.at(i).exprStr << endl;
        // cout << "Interval: [" << exprInfoVector.at(i).start << "," << exprInfoVector.at(i).end << "]" << endl;
    }

    // generate code to file
    string directory = "srcTest/" + uniqueLabel + "/";
    string funcName = "expr_" + uniqueLabel + "_final";
    string fileName = directory + funcName + ".c";
    std::ofstream fout;
    fout.open(fileName.c_str());

    // function declearation
    fout << "#include <math.h>\n";
    fout << "double " << funcName << "(";
    for (size_t i = 0; i < vars.size(); ++i)
    {
        if (i != vars.size() - 1)
            fout << "double" << " " << vars.at(i) << ", ";
        else
            fout << "double" << " " << vars.at(i);
    }
    fout << ") {\n";
    fout << "    double result;\n";

    // function body
    for (size_t i = 0; i < exprInfoVector.size(); i++)
    {
        std::vector<double> interval = exprInfoVector.at(i).intervals;

        string &exprStr = exprInfoVector.at(i).exprStr;
        // generate if statement
        if (i == 0)
        {
            fout << "    if(";
        }
        else
        {
            fout << "    } else if(";
        }
        for(size_t j = 0; j < interval.size(); j +=2)
        {
            if(j < interval.size() - 2)
            {
                fout << "(" << interval.at(j) << " < " << vars.at(j / 2) << " && " << vars.at(j / 2) << " < " << interval.at(j + 1) << ") && ";
            }
            else
            {
                fout << "(" << interval.at(j) << " < " << vars.at(j / 2) << " && " << vars.at(j / 2) << " < " << interval.at(j + 1) << ")";
            }
        }
        fout << ") {" << "\n";
        fout << "        result = " << exprStr << ";" << "\n";
    }
    fout << "    } else {" << "\n";
    fout << "        result = " << exprStr << ";" << "\n";
    fout << "    }" << "\n";
    fout << "    return result;" << "\n";
    fout << "}" << "\n";

    fout << std::flush;
    fout.close();
    cout << "generate file: " << fileName << endl;

    cout << "&&&&&&&&&&&&&&&&&&&&&&& geneFinalCode &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;

    return funcName;
}

string geneFinalCode(string exprStr, string uniqueLabel, vector<exprInfo> exprInfoVector)
{
    auto originExpr = ParseExpressionFromString(exprStr);

    vector<string> vars;
    getVariablesFromExpr(originExpr, vars);
    auto funcName = geneFinalCodeKernel(exprStr, uniqueLabel, exprInfoVector, vars);

    return funcName;
}

void getDepth(ast_ptr &expr, int &depth)
{
    static int depthNow = 0;
    depthNow++;
    auto type = expr->type();
    if(type == "Number" || type == "Variable")
    {
        // nothing
    }
    else if(type == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callExpr->getArgs();
        for(auto& arg : args)
        {
            getDepth(arg, depth);
        }
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        getDepth(lhs, depth);
        getDepth(rhs, depth);
    }
    else
    {
        fprintf(stderr, "ERROR: unknown type %s", type.c_str());
        exit(EXIT_FAILURE);
    }
    if(depthNow > depth)
    {
        depth = depthNow;
    }
    depthNow--;
}

void setTypeKernel(ast_ptr &expr, const vector<string> &opTypes)
{
    static size_t depthNow = 0;
    auto type = expr->type();
    auto opType = opTypes.at(depthNow);
    depthNow++;
    if(type == "Number" || type == "Variable")
    {
        expr->setOpType(opType);
    }
    else if(type == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callExpr->getArgs();
        for(auto& arg : args)
        {
            setTypeKernel(arg, opTypes);
        }
        expr->setOpType(opType);
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        setTypeKernel(lhs, opTypes);
        setTypeKernel(rhs, opTypes);
        expr->setOpType(opType);
    }
    else
    {
        fprintf(stderr, "ERROR: unknown type %s", type.c_str());
        exit(EXIT_FAILURE);
    }
    depthNow--;
}

// set opTypes for different tree levels by middle
void setType(ast_ptr &expr, int depth, int middle)
{
    vector<string> opTypes;
    for(int i = 0; i < middle; i++)
    {
        opTypes.push_back("DD");
    }
    for(int i = middle; i < depth; i++)
    {
        opTypes.push_back("double");
    }

    // start setting the operators' type by vector opTypes
    setTypeKernel(expr, opTypes);
}

void setOrdersKernel(ast_ptr &expr, int &orderNow)
{
    auto type = expr->type();
    if(type == "Number")
    {
        expr->setOrder(orderNow);
        orderNow++;
    }
    else if(type == "Variable")
    {
        expr->setOrder(orderNow);
        orderNow++;
    }
    else if(type == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callExpr->getArgs();
        vector<int> paramOrders;
        for(auto& arg : args)
        {
            setOrdersKernel(arg, orderNow);
        }
        expr->setOrder(orderNow);
        orderNow++;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        setOrdersKernel(lhs, orderNow);
        setOrdersKernel(rhs, orderNow);
        expr->setOrder(orderNow);
        orderNow++;
    }
    else
    {
        fprintf(stderr, "ERROR: unknowntype %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

int setOrder(ast_ptr &expr)
{
    int order = 0;
    setOrdersKernel(expr, order);
    return order + 1;
}

static std::map<char, string> opMap = {
    {'+', "add"},
    {'-', "sub"},
    {'*', "mul"},
    {'/', "div"}};

static std::map<string, string> callMap = {
    {"sin", "c_dd_sin"},
    {"cos", "c_dd_cos"},
    {"tan", "c_dd_tan"},
    {"exp", "c_dd_exp"},
    // {"exp2", "c_dd_exp2"},
    // {"exp10", "c_dd_exp10"},
    {"log", "c_dd_log"},
    // {"log2", "c_dd_log2"},
    {"log10", "c_dd_log10"},
    {"asin", "c_dd_asin"},
    {"acos", "c_dd_acos"},
    {"atan", "c_dd_atan"},
    {"sinh", "c_dd_sinh"},
    {"cosh", "c_dd_cosh"},
    {"tanh", "c_dd_tanh"},
    {"asinh", "c_dd_asinh"},
    {"acosh", "c_dd_acosh"},
    {"atanh", "c_dd_atanh"},
    {"sqrt", "c_dd_sqrt"},
    {"pow", "c_dd_pow_mine"}
};

int codegenKernel(ofstream &ofs, const ast_ptr &expr)
{
    auto type = expr->type();
    auto opType = expr->getOpType();
    auto order = expr->getOrder();
    if(type == "Number")
    {
        NumberExprAST *numPtr = dynamic_cast<NumberExprAST *>(expr.get());
        auto num = numPtr->getNumber();
        if(opType == "double")
        {
            ofs << "\t" << "double tmp" << order << " = " << num << ";\n";
        }
        else if(opType == "DD")
        {
            ofs << "\t" << "double tmp" << order << "[2];\n";
            ofs << "\t" << "tmp" << order << "[0] = " << num << ";\n";
            ofs << "\t" << "tmp" << order << "[1] = 0.0;\n";
        }
        return order;
    }
    else if(type == "Variable")
    {
        VariableExprAST *varPtr = dynamic_cast<VariableExprAST *>(expr.get());
        auto var = varPtr->getVariable();
        if(opType == "double")
        {
            ofs << "\t" << "double tmp" << order << " = " << var << ";\n";
        }
        else if(opType == "DD")
        {
            ofs << "\t" << "double tmp" << order << "[2];\n";
            ofs << "\t" << "tmp" << order << "[0] = " << var << ";\n";
            ofs << "\t" << "tmp" << order << "[1] = 0.0;\n";
        }
        return order;
    }
    else if(type == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        auto callee = (callExpr->getCallee());
        auto &args = callExpr->getArgs();
        vector<string> paramOpTypes;
        vector<int> paramOrders;
        for(const auto& arg : args)
        {
            auto opType = arg->getOpType();
            paramOpTypes.push_back(opType);
            auto paramOrder = codegenKernel(ofs, arg);
            paramOrders.push_back(paramOrder);
        }

        if(opType == "double")
        {
            for(size_t i = 0; i < paramOpTypes.size(); i++)
            {
                auto &paramOpType = paramOpTypes.at(i);
                if(paramOpType == "double")
                {
                    ofs << "\t" << "double p" << paramOrders.at(i) << " = tmp" << paramOrders.at(i) << ";\n";
                }
                else if(paramOpType == "DD")
                {
                    ofs << "\t" << "double p" << paramOrders.at(i) << " = tmp" << paramOrders.at(i) << "[0];\n";
                }
                else
                {
                    fprintf(stderr, "ERROR: Unknown paramOpType %s\n", paramOpType.c_str());
                    exit(EXIT_FAILURE);
                }
            }
            ofs << "\t" << "double tmp" << order << " = " << callee << "(";
            for(size_t i = 0; i < paramOrders.size() - 1; i++)
            {
                ofs << "p" << paramOrders.at(i) << ", ";
            }
            ofs << "p" << paramOrders.back();
            ofs << ");\n";
        }
        else if(opType == "DD")
        {
            for(size_t i = 0; i < paramOpTypes.size(); i++)
            {
                auto &paramOpType = paramOpTypes.at(i);
                if(paramOpType == "double")
                {
                    ofs << "\t" << "double p" << paramOrders.at(i) << "[2];\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << "[0] = tmp" << paramOrders.at(i) << ";\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << "[1] = 0.0;\n";
                }
                else if(paramOpType == "DD")
                {
                    ofs << "\t" << "double p" << paramOrders.at(i) << "[2];\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << "[0] = tmp" << paramOrders.at(i) << "[0];\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << "[1] = tmp" << paramOrders.at(i) << "[1];\n";
                }
                else
                {
                    fprintf(stderr, "ERROR: Unknown paramOpType %s\n", paramOpType.c_str());
                    exit(EXIT_FAILURE);
                }
            }
            ofs << "\t" << "double tmp" << order << "[2];\n";
            auto it = callMap.find(callee);
            auto callStr = it->second;
            ofs << "\t" << callStr << "(";
            for(size_t i = 0; i < paramOrders.size(); i++)
            {
                ofs << "p" << paramOrders.at(i) << ", ";
            }
            ofs << "tmp" << order;
            ofs << ");\n";
        }

        return order;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binPtr->getOp();
        auto it = opMap.find(op);
        auto opStr = it->second;
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        auto orderL = codegenKernel(ofs, lhs);
        auto orderR = codegenKernel(ofs, rhs);
        auto opTypeL = lhs->getOpType();
        auto opTypeR = rhs->getOpType();
        if(opType == "double")
        {
            if(opTypeL == "double" && opTypeR == "double")
            {
                ofs << "\t" << opStr << "_d_d_d(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else if(opTypeL == "double" && opTypeR == "DD")
            {
                ofs << "\t" << opStr << "_d_dd_d(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else if(opTypeL == "DD" && opTypeR == "double")
            {
                ofs << "\t" << opStr << "_dd_d_d(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else if(opTypeL == "DD" && opTypeR == "DD")
            {
                ofs << "\t" << opStr << "_dd_dd_d(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else
            {
                fprintf(stderr, "ERROR: unsupported type: opTypeL = %s, opTypeR = %s\n", opTypeL.c_str(), opTypeR.c_str());
                exit(EXIT_FAILURE);
            }
        }
        else if(opType == "DD")
        {
            if(opTypeL == "double" && opTypeR == "double")
            {
                ofs << "\t" << opStr << "_d_d_dd(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else if(opTypeL == "double" && opTypeR == "DD")
            {
                ofs << "\t" << opStr << "_d_dd_dd(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else if(opTypeL == "DD" && opTypeR == "double")
            {
                ofs << "\t" << opStr << "_dd_d_dd(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else if(opTypeL == "DD" && opTypeR == "DD")
            {
                ofs << "\t" << opStr << "_dd_dd_dd(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";
            }
            else
            {
                fprintf(stderr, "ERROR: unsupported type: opTypeL = %s, opTypeR = %s\n", opTypeL.c_str(), opTypeR.c_str());
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            fprintf(stderr, "ERROR: unsupported type: opType = %s\n", opType.c_str());
            exit(EXIT_FAILURE);
        }
        return order;
    }
    else
    {
        fprintf(stderr, "ERROR: unsupported type: type = %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

void codegen(ast_ptr &expr, vector<string> &vars, const string uniqueLabel, string tail)
{
    // AST init
    setOrder(expr);
    auto order = expr->getOrder();
    int depth = 0;
    getDepth(expr, depth);
    cout << "depth: " << depth << endl;
    // int middle = (depth - 3) > 1 ? (depth - 3) : 1; // !!!
    int middle = depth - 1;
    setType(expr, depth, middle);
    auto opType = expr->getOpType();

    // generate init
    string directory = "srcTest/" + uniqueLabel + "/";
    // string funcName = "expr_" + uniqueLabel + "_" + tail + "_" + to_string(middle);
    string funcName = "expr_" + uniqueLabel + "_" + tail;
    string fileName = directory + funcName + ".c";
    cout << "fileName: " << fileName << endl;
    ofstream file_clean(fileName, ios_base::out);
    ofstream ofs(fileName, ios::app);

    // generate
    ofs << "#include <math.h>\n";
    ofs << "#include <qd/c_dd.h>\n";
    ofs << "#include \"dd.h\"\n";
    ofs << "#include \"inline.h\"\n";
    ofs << "double " << funcName << "(";
    for (size_t i = 0; i < vars.size() - 1; i++)
    {
        ofs << "double" << " " << vars.at(i) << ", ";
    }
    ofs << "double" << " " << vars.back();
    ofs << ") {\n";
    codegenKernel(ofs, expr);
    ofs << "\t" << "double result;\n";
    if(opType == "double")
    {
        ofs << "\t" << "result = tmp" << order << ";\n"; 
    }
    else
    {
        ofs << "\t" << "result = tmp" << order << "[0];\n"; 
    }
    ofs << "\t" << "return result;\n";
    ofs << "}" << endl;
}