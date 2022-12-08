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
        {"pow", "mpfr_pow"},
        {"sqrt", "mpfr_sqrt"},
        {"sin", "mpfr_sin"},
        {"log", "mpfr_log"},
        {"cos", "mpfr_cos"},
        {"atan", "mpfr_atan"},
        {"tan", "mpfr_tan"}};
    
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
    fout << "#include <math.h>" << std::endl;
    fout << "double " << funcName << "(";
    for (size_t i = 0; i < vars.size(); ++i)
    {
        if (i != vars.size() - 1)
            fout << "double" << " " << vars.at(i) << ", ";
        else
            fout << "double" << " " << vars.at(i);
    }
    fout << ") {\n";
    fout << "    double result;" << std::endl;

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
        fout << ") {" << endl;
        fout << "        result = " << exprStr << ";" << endl;
    }
    fout << "    } else {" << std::endl;
    fout << "        result = " << exprStr << ";" << std::endl;
    fout << "    }" << std::endl;
    fout << "    return result;" << std::endl;
    fout << "}" << std::endl;

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