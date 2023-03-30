#include "basic.hpp"
#include "geneCode.hpp"
#include "parserASTLY.hpp"

#include <algorithm>
#include <cmath>
#include <fmt/ranges.h>

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

string geneExprCodeKernel(string exprStr, vector<string> vars, string uniqueLabel, string tail)
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

    return exprStr;
}

string geneExprCode(string exprStr, string uniqueLabel, string tail)
{
    auto originExpr = ParseExpressionFromString(exprStr);

    vector<string> vars;
    getVariablesFromExpr(originExpr, vars);
    geneExprCodeKernel(exprStr, vars, uniqueLabel, tail);

    return exprStr;
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

// TODO: not real implementation
string geneHerbieCode(string uniqueLabel)
{
    map<string, string> benchmarkHerbie = {
        {"Bsplines3", "-pow(x, 3.0) /6.0"}, // warning, the origin is -x*x*x/6
        {"exp1x", "expm1(x) / x"},
        {"exp1x_log", "expm1(x) / x"},
        {"intro_example", "(x / (1.0 + pow(x, 3.0))) * fma(x, x, (1.0 - x))"},
        {"logexp", "log1p(exp(x))"},
        {"NMSEexample31", "(x + (1.0 - x)) / fma(pow(x, 0.25), pow(x, 0.25), sqrt(x + 1.0))"},
        {"NMSEexample310", "log1p(-x) / log1p(x)"},
        {"NMSEexample34", "tan((x / 2.0))"},
        {"NMSEexample35", "atan2((x + (1.0- x)), (1.0+fma(sqrt(x), sqrt(x), (x * x))))"},
        {"NMSEexample36", "1.0 / ((x * (pow(x, -0.5) + pow((1.0 + x), -0.5))) + (x * (x * fma(pow(x, -0.25), pow(x, -0.25), pow((1.0 + x), -0.5)))))"},
        {"NMSEexample37", "expm1(x)"},
        {"NMSEexample38", "fma(x, log((1.0 + (1.0 / x))), log1p(x)) + -1.0"},
        {"NMSEexample39", ""}, // herbie is if-else, the origin is 1/x - 1/tan(x)
        {"NMSEproblem331", "-1.0 / fma(x, x, x)"}, // warning, the origin is 1/(x+1) - 1/x
        {"NMSEproblem333", "((1.0 / (x + -1.0)) + (1.0 / (1.0 + x))) + (-2.0 / x)"}, // warning, the origin is 1/(1+x) - 2/x + 1/(x - 1)
        {"NMSEproblem334", "((x + 1.0) - x) / (pow(cbrt((x + 1.0)), 2.0) + (fma((x + 1.0), cbrt(x), (x * cbrt(x))) / (pow(cbrt((x + 1.0)), 2.0) + (cbrt(x) * (cbrt(x) - cbrt((x + 1.0)))))))"},
        {"NMSEproblem336", "log1p((1.0 / x))"},
        {"NMSEproblem337", ""}, // herbie is if-else, the origin is exp(x) + (-2) + exp(-x)
        {"NMSEproblem341", "(sin(x) / x) * (tan((x / 2.0)) / x)"},
        {"NMSEproblem343", "log1p(-x) - log1p(x)"},
        {"NMSEproblem344", "sqrt((1.0 + exp(x)))"},
        {"NMSEproblem345", ""}, // if else
        {"NMSEsection311", "(1.0 + expm1(x)) / expm1(x)"},
        {"predatorPrey", "x * ((x * 4.0) * exp(-log1p(sqrt(pow((x * 0.9009009009009009), 4.0)))))"}, // warning, the origin is ((4.0 * x) * x) / (1.0 + ((x / 1.11) * (x / 1.11)))
        {"sine", "x - fma(0.0001984126984126984, pow(x, 7.0), fma(0.16666666666666666, pow(x, 3.0), (-0.008333333333333333 * pow(x, 5.0))))"}, // warning, the origin is x - (1.0/6.0)*x*x*x+(1.0/120.0)*x*x*x*x*x - (1.0/5040.0)*x*x*x*x*x*x*x
        {"sineorder3", "fma(x, 0.954929658551372, (pow(x, 3.0) * -0.12900613773279798))"}, // warning, the origin is (238732414637843.0/250000000000000.0)*x - (6450306886639899.0/50000000000000000.0)*x*x*x
        {"sqroot", "fma(x, (0.5 + (x * fma(x, fma(x, -0.0390625, 0.0625), -0.125))), 1.0)"}, // warning, the origin is 1.0 + 0.5*x - 0.125*x*x + 0.0625*x*x*x - 0.0390625*x*x*x*x
        {"sqrt_add", "1.0 / (sqrt((1.0 + x)) + sqrt(x))"},
        {"test05_nonlin1_r4", "exp(-log1p(x))"}, // warning, the origin is (x - 1)/(x*x - 1)
        {"test05_nonlin1_test2", "exp(-log1p(x))"}, // warning, the origin is 1.0/(1+x)
        {"verhulst", "pow(log1p(expm1((64.0 * pow((x / fma(x, 0.9009009009009009, 1.0)), 3.0)))), 0.3333333333333333)"}, // warning, the origin is (4*x)/(1+x/1.11)
        {"ComplexSinCos", "sin(r) * ((pow(i, 3.0) * -0.16666666666666666) - i)"},
        {"ComplexSquareRoot", "0.5 * sqrt((2.0 * (x1 + hypot(x1, x2))))"}, 
        {"doppler1", "(fma(T, -0.6, -331.4) / pow((331.4 + fma(0.6, T, u)), 2.0)) * v"}, // warning, the origin is ((-(1657.0/5.0+3.0/5.0*T))*v)/(((1657.0/5.0+3.0/5.0*T)+u)*((1657.0/5.0+3.0/5.0*T)+u))
        {"doppler2", "v * (fma(T, -0.6, -331.4) / fma(u, (u + fma(1.2, T, 662.8)), pow(fma(T, 0.6, 331.4), 2.0)));"}, // warning, the origin is ((-(1657.0/5.0+3.0/5.0*T))*v)/(((1657.0/5.0+3.0/5.0*T)+u)*((1657.0/5.0+3.0/5.0*T)+u))
        {"doppler3", "(fma(T, -0.6, -331.4) / pow((331.4 + fma(0.6, T, u)), 2.0)) * v"}, // warning, the origin is ((-(1657.0/5.0+3.0/5.0*T))*v)/(((1657.0/5.0+3.0/5.0*T)+u)*((1657.0/5.0+3.0/5.0*T)+u))
        {"hypot32", "hypotf(x1, x2)"}, // single precision
        {"i4", "sqrtf(fmaf(f2, f2, f1))"}, // single precision
        {"i6", "sinf((f1 * f2))"}, // single precision
        {"NMSEexample33", "fma(x, fma((x * -0.5), sin(eps), (cos(eps) + -1.0)), sin(eps))"},
        {"NMSEproblem332", "fma(sin(eps) / cos(eps), (x * (x + (pow(sin(eps), 2.0) / (pow(cos(eps), 2.0) / x)))), fma((pow(sin(eps), 2.0) / pow(cos(eps), 2.0)), x, sin(eps) / cos(eps)))"}, // double t_0 = sin(x2) / cos(x2); double t_1 = pow(sin(x2), 2.0); double t_2 = pow(cos(x2), 2.0); *resultPtr = fma(t_0, (x1 * (x1 + (t_1 / (t_2 / x1)))), fma((t_1 / t_2), x1, t_0));
        {"NMSEproblem335", "-2.0 * (sin(((eps + (x - x)) * 0.5)) * sin((0.5 * fma(x, 2.0, eps))))"},
        {"NMSEproblem346", "expm1(log1p((exp((log1p(x) / n)) - pow(x, (1.0 / n)))))"},
        {"NMSEsection35", "expm1((a * x))"},
        {"polarToCarthesianX", "x1 * cos(log((pow(sqrt(exp(0.017453292519944444)), x2) * fma(0.5, (pow(log(sqrt(exp(0.017453292519944444))), 2.0) * (x2 * x2)), fma(log(sqrt(exp(0.017453292519944444))), x2, fma(pow(x2, 3.0), (0.16666666666666666 * pow(log(sqrt(exp(0.017453292519944444))), 3.0)), 1.0))))))"}, // double t_1 = sqrt(exp(0.017453292519944444)); double t_2 = log(t_1); *resultPtr = x1 * cos(log((pow(t_1, x2) * fma(0.5, (pow(t_2, 2.0) * (x2 * x2)), fma(t_2, x2, fma(pow(x2, 3.0), (0.16666666666666666 * pow(t_2, 3.0)), 1.0))))));
        {"polarToCarthesianY", "exp((((log(0.017453292519944444) * log(0.017453292519944444)) + (0.0 - pow(pow(log((x1 * x2)), 6.0), 0.3333333333333333))) / (log(0.017453292519944444) - log((x1 * x2)))))"}, // double t_1 = log((x1 * x2)); *resultPtr = exp((((log(0.017453292519944444) * log(0.017453292519944444)) + (0.0 - pow(pow(t_1, 6.0), 0.3333333333333333))) / (log(0.017453292519944444) - t_1)));
        {"sec4example", "pow(fma(x1, x2, 1.0), -1.0)"}, // warning, the origin is ((x1*x2) - 1)/((x1*x2)*(x1*x2) - 1)
        {"test03_nonlin2", "(x + y)/(x - y)"}, // for test03_nonlin2, herbie is the same to origin
        {"theta", "(pow(pow(cbrt(cbrt(cbrt(cbrt((atan((x2 / x1)) * 57.29577951307855))))), 2.0), 18.0) * pow(cbrt(cbrt(cbrt(cbrt((atan((x2 / x1)) * 57.29577951307855))))), 18.0)) * pow(cbrt(cbrt(cbrt((atan((x2 / x1)) * 57.29577951307855)))), 9.0)"}, // double t_0 = cbrt(cbrt(cbrt((atan((x2 / x1)) * 57.29577951307855)))); double t_1 = cbrt(t_0); *resultPtr = (pow(pow(t_1, 2.0), 18.0) * pow(t_1, 18.0)) * pow(t_0, 9.0)
        {"turbine1", "cbrt(pow((2.0 * pow(x2, -2.0)), 3.0)) - fma(fma(x0, -0.25, 0.375), ((x2 / (1.0 - x0)) * (x2 * pow(x1, 2.0))), 1.5)"}, // warning, the origin is (2.0/(r*r)+3.0) - ((3.0 - 2.0*v)*(1.0/8.0)*((w*w)*r*r))/(1.0 - v) - 9.0/2.0
    };

    auto pos = benchmarkHerbie.find(uniqueLabel);
    if (pos != benchmarkHerbie.end())
    {
        string herbieExpr = pos->second;
        if(herbieExpr != "")
        {
            geneExprCode(herbieExpr, uniqueLabel, "herbie");
        }
        else
        {
            fprintf(stderr, "ERROR: geneHerbieCode: herbie's rewrite result for %s can not be used.\n", uniqueLabel.c_str());
            // TODO: just write the if-else result to file.
        }
        return herbieExpr;
    }
    fprintf(stderr, "ERROR: geneHerbieCode: we can not support %s now\n", uniqueLabel.c_str());
    exit(EXIT_FAILURE);
}

string geneDaisyCode(string uniqueLabel)
{
    map<string, string> benchmarkDaisy = {
        {"Bsplines3", ""},
        {"exp1x", ""},
        {"exp1x_log", ""},
        {"intro_example", ""},
        {"logexp", ""},
        {"NMSEexample31", ""},
        {"NMSEexample310", ""},
        {"NMSEexample34", ""},
        {"NMSEexample35", ""},
        {"NMSEexample36", ""},
        {"NMSEexample37", ""},
        {"NMSEexample38", "(((log((1.0 + x)) * x) + (log((x + 1.0)) - 1.0)) - (x * log(x)))"},
        {"NMSEexample39", ""},
        {"NMSEproblem331", ""},
        {"NMSEproblem333", ""},
        {"NMSEproblem334", ""},
        {"NMSEproblem336", ""},
        {"NMSEproblem337", ""},
        {"NMSEproblem341", ""},
        {"NMSEproblem343", ""},
        {"NMSEproblem344", ""},
        {"NMSEproblem345", ""},
        {"NMSEsection311", ""},
        {"predatorPrey", ""},
        {"sine", ""},
        {"sineorder3", ""},
        {"sqroot", ""},
        {"sqrt_add", ""},
        {"test05_nonlin1_r4", ""},
        {"test05_nonlin1_test2", ""},
        {"verhulst", ""},
        {"ComplexSinCos", ""},
        {"ComplexSquareRoot", ""},
        {"doppler1", ""},
        {"doppler2", ""},
        {"doppler3", ""},
        {"hypot32", ""},
        {"i4", ""},
        {"i6", ""},
        {"NMSEexample33", ""},
        {"NMSEproblem332", ""},
        {"NMSEproblem335", ""},
        {"NMSEproblem346", ""},
        {"NMSEsection35", ""},
        {"polarToCarthesianX", "radius * cos(theta * 0.017453292519944444)"},
        {"polarToCarthesianY", "radius * sin(theta * 0.017453292519944444)"},
        {"sec4example", ""},
        {"test03_nonlin2", ""},
        {"theta", "atan((x2 / x1)) * 57.29577951307855"},
        {"turbine1", ""},
    };

    auto pos = benchmarkDaisy.find(uniqueLabel);
    if (pos != benchmarkDaisy.end())
    {
        string daisyExpr = pos->second;
        if(daisyExpr != "")
        {
            geneExprCode(daisyExpr, uniqueLabel, "daisy");
        }
        else
        {
            fprintf(stderr, "ERROR: geneDaisyCode: daisy's rewrite result for %s can not be used.\n", uniqueLabel.c_str());
            // TODO: just write the if-else result to file.
        }
        return daisyExpr;
    }
    fprintf(stderr, "ERROR: geneDaisyCode: we can not support %s now\n", uniqueLabel.c_str());
    exit(EXIT_FAILURE);
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
        {"log1p", "mpfr_log1p"},
        {"pow", "mpfr_pow"},
        {"sqrt", "mpfr_sqrt"},
        {"sqrtf", "mpfr_sqrt"},
        {"cbrt", "mpfr_cbrt"},
        {"sin", "mpfr_sin"},
        {"sinf", "mpfr_sin"},
        {"cos", "mpfr_cos"},
        {"tan", "mpfr_tan"},
        {"asin", "mpfr_asin"},
        {"acos", "mpfr_acos"},
        {"atan", "mpfr_atan"},
        {"atan2", "mpfr_atan2"},
        {"sinh", "mpfr_sinh"},
        {"cosh", "mpfr_cosh"},
        {"tanh", "mpfr_tanh"},
        {"fma", "mpfr_fma"},
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
        auto &exprInfoTmp = exprInfoVector.at(i);
        fmt::print("Interval NO.{}: {}\n", i, exprInfoTmp.intervals);
        cout << "\trewriteID: " << exprInfoTmp.rewriteID << endl;
        cout << "\taveError: " << exprInfoTmp.aveError << "\tmaxError: " << exprInfoTmp.maxError << endl;
        cout << "\texpr: " << exprInfoTmp.exprStr << endl;
        // cout << "Interval: [" << exprInfoTmp.start << "," << exprInfoTmp.end << "]" << endl;
    }

    // FLAG to if mpfr
    bool flagMPFR = false;

    // generate code to file
    string directory = "srcTest/" + uniqueLabel + "/";
    string funcName = "expr_" + uniqueLabel + "_final";
    string fileName = directory + funcName + ".c";
    std::ofstream fout;
    fout.open(fileName.c_str());

    // function declearation
    fout << "#include \"mine.h\"\n";
    stringstream ss, ss1;
    for (size_t i = 0; i < vars.size(); ++i)
    {
        if (i != vars.size() - 1)
        {
            ss << "double" << " " << vars.at(i) << ", ";
            ss1 << vars.at(i) << ", ";
        }
        else
        {
            ss << "double" << " " << vars.at(i);
            ss1 << vars.at(i);
        }
    }
    const string &paramListWithType = ss.str();
    const string &paramListNoType = ss1.str();

    // function main body
    fout << "double expr_" << uniqueLabel << "_final(" << paramListWithType << ") {\n";
    fout << "    double result;\n";

    // function body
    if (exprInfoVector.size() == 0)
    {
        fout << "    result = " << exprStr << ";" << "\n";
        fout << "    return result;" << "\n";
        fout << "}" << "\n";
    }
    else
    {
        for (size_t i = 0; i < exprInfoVector.size(); i++)
        {
            std::vector<double> interval = exprInfoVector.at(i).intervals;

            string exprStr;
            if(exprInfoVector.at(i).exprStr == "mpfr")
            {
                exprStr = "expr_" + uniqueLabel + "_mpfr1(" + paramListNoType + ")";
                flagMPFR = true;
            }
            else
            {
                exprStr = exprInfoVector.at(i).exprStr;
            }
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
    }

    fout << std::flush;
    fout.close();
    cout << "generate file: " << fileName << endl;

    // write "mine.h"
    fileName = "includeTEST/mine.h";
    fout.open(fileName.c_str());
    if(!fout.is_open())
    {
        string tmp = "file open error " + fileName;
        perror(tmp.c_str());
        exit(EXIT_FAILURE);
    }
    fout << "#include <math.h>\n";
    // reference to mpfr version
    if(flagMPFR)
    {
        fout << "#include <mpfr.h>\n";
        fout << "int expr_" << uniqueLabel << "_mpfr(" << paramListWithType << ", mpfr_t mpfrResult);\n";
        // write expr_uniqueLable_mpfr1
        fout << "double expr_" << uniqueLabel << "_mpfr1(" << paramListWithType << ")\n";
        fout << "{\n";
        fout << "    mpfr_t mpfrResult;\n";
        fout << "    mpfr_init2(mpfrResult, 128);\n";
        fout << "    expr_" << uniqueLabel << "_mpfr(" << paramListNoType << ", mpfrResult);\n";
        fout << "    double result = mpfr_get_d(mpfrResult, MPFR_RNDN);\n";
        fout << "    return result;\n";
        fout << "}\n";
    }
    fout << std::flush;
    fout.close();

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

// set opTypes for different tree nodes by opTypes
void setType(ast_ptr &expr, vector<string> opTypes)
{
    auto type = expr->type();
    if(type == "Number" || type == "Variable")
    {
        return;
    }
    
    auto order = expr->getOrder();
    auto opType = opTypes.at(order);
    // cout << "setType: NO." << order << ", type = " << type << ", opType = " << opType << "\n";
    expr->setOpType(opType);
    if(type == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callExpr->getArgs();
        for(auto& arg : args)
        {
            setType(arg, opTypes);
        }
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        setType(lhs, opTypes);
        setType(rhs, opTypes);
    }
    else
    {
        fprintf(stderr, "ERROR: unknown type %s", type.c_str());
        exit(EXIT_FAILURE);
    }
}

// set opTypes for different tree nodes by opTypes
void setType(ast_ptr &expr, map<int, string> opTypes)
{
    auto type = expr->type();
    if(type == "Number" || type == "Variable")
    {
        return;
    }
    
    auto order = expr->getOrder();
    auto opType = opTypes.at(order);
    // cout << "setType: NO." << order << ", type = " << type << ", opType = " << opType << "\n";
    expr->setOpType(opType);
    if(type == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callExpr->getArgs();
        for(auto& arg : args)
        {
            setType(arg, opTypes);
        }
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        setType(lhs, opTypes);
        setType(rhs, opTypes);
    }
    else
    {
        fprintf(stderr, "ERROR: unknown type %s", type.c_str());
        exit(EXIT_FAILURE);
    }
}

void setOrdersKernel(ast_ptr &expr, int &orderNow, vector<int> &opOrder)
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
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        auto &args = callPtr->getArgs();
        vector<int> paramOrders;
        for(auto& arg : args)
        {
            setOrdersKernel(arg, orderNow, opOrder);
        }
        expr->setOrder(orderNow);
        opOrder.push_back(orderNow);
        orderNow++;
    }
    else if(type == "Binary")
    {
        BinaryExprAST *binPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        ast_ptr &lhs = binPtr->getLHS();
        ast_ptr &rhs = binPtr->getRHS();
        setOrdersKernel(lhs, orderNow, opOrder);
        setOrdersKernel(rhs, orderNow, opOrder);
        expr->setOrder(orderNow);
        opOrder.push_back(orderNow);
        orderNow++;
    }
    else
    {
        fprintf(stderr, "ERROR: unknowntype %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

// set orders for nodes in expr and return the operation nodes' order (aka binary & call)
vector<int> setOrder(ast_ptr &expr)
{
    int order = 0;
    vector<int> opOrder;
    setOrdersKernel(expr, order, opOrder);
    return opOrder;
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
        else if(opType == "ld")
        {
            ofs << "\t" << "long double tmp" << order << " = " << num << ";\n";
        }
        else if(opType == "DD")
        {
            ofs << "\t" << "double tmp" << order << "[2];\n";
            ofs << "\t" << "tmp" << order << "[0] = " << num << ";\n";
            ofs << "\t" << "tmp" << order << "[1] = 0.0;\n";
        }
        else
        {
            fprintf(stderr, "opType %s of number %lf is error\n", opType.c_str(), num);
            exit(EXIT_FAILURE);
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
        else if(opType == "ld")
        {
            ofs << "\t" << "long double tmp" << order << " = " << var << ";\n";
        }
        else if(opType == "DD")
        {
            ofs << "\t" << "double tmp" << order << "[2];\n";
            ofs << "\t" << "tmp" << order << "[0] = " << var << ";\n";
            ofs << "\t" << "tmp" << order << "[1] = 0.0;\n";
        }
        else
        {
            fprintf(stderr, "opType %s of variable %s is error\n", opType.c_str(), var.c_str());
            exit(EXIT_FAILURE);
        }
        return order;
    }
    else if(type == "Call") // the parameters' type should be the same to the return value's type
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
                else if(paramOpType == "ld") // ld to double is straight.
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
        else if(opType == "ld")
        {
            for(size_t i = 0; i < paramOpTypes.size(); i++)
            {
                auto &paramOpType = paramOpTypes.at(i);
                if(paramOpType == "double")
                {
                    ofs << "\t" << "long double p" << paramOrders.at(i) << " = tmp" << paramOrders.at(i) << ";\n";
                }
                else if(paramOpType == "ld")
                {
                    ofs << "\t" << "long double p" << paramOrders.at(i) << " = tmp" << paramOrders.at(i) << ";\n";
                }
                else if(paramOpType == "DD")
                {
                    ofs << "\t" << "long double p" << paramOrders.at(i) << " = tmp" << paramOrders.at(i) << "[0];\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << " += tmp" << paramOrders.at(i) << "[1];\n";
                }
                else
                {
                    fprintf(stderr, "ERROR: Unknown paramOpType %s\n", paramOpType.c_str());
                    exit(EXIT_FAILURE);
                }
            }
            ofs << "\t" << "long double tmp" << order << " = " << callee << "(";
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
                else if(paramOpType == "ld")
                {
                    ofs << "\t" << "double p" << paramOrders.at(i) << "[2];\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << "[0] = tmp" << paramOrders.at(i) << ";\n";
                    ofs << "\t" << "p" << paramOrders.at(i) << "[1] = tmp" << paramOrders.at(i) << " - p" << paramOrders.at(i) << "[0];\n";
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
        else
        {
            fprintf(stderr, "ERROR: Unknown opType %s\n", opType.c_str());
            exit(EXIT_FAILURE);
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
        std::map<string, string> opType2funcType = {
            {"double", "d"},
            {"ld", "ld"},
            {"DD", "dd"},
        };
        auto funcType = opType2funcType.at(opType);
        auto funcTypeL = opType2funcType.at(opTypeL);
        auto funcTypeR = opType2funcType.at(opTypeR);
        ofs << "\t" << opStr << "_" << funcTypeL << "_" << funcTypeR << "_" << funcType << "(tmp" << orderL << ", tmp" << orderR << ", tmp" << order << ");\n";

        return order;
    }
    else
    {
        fprintf(stderr, "ERROR: unsupported type: type = %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }
}

void codegen(ast_ptr &expr, vector<string> &vars, const string funcName, ofstream &ofs)
{
    auto opType = expr->getOpType();
    auto order = expr->getOrder();
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
    if(opType == "double" || opType == "ld")
    {
        ofs << "\t" << "double result = tmp" << order << ";\n"; 
    }
    else if(opType == "DD")
    {
        ofs << "\t" << "double result = tmp" << order << "[0];\n"; 
    }
    else
    {
        fprintf(stderr, "ERROR: unsupported opType: opType = %s\n", opType.c_str());
        exit(EXIT_FAILURE);
    }
    ofs << "\t" << "return result;\n";
    ofs << "}" << endl;
}

// Note: Each operator node has two states, double or double-double. if there are n node in that expression, there are 2^n states. 
// Generate double-double implementations in all states and return the number of generated codes
int codegenWrapper(ast_ptr &expr, vector<string> &vars, const string uniqueLabel, string tail)
{
    // AST init
    auto opOrder = setOrder(expr);
    auto lenOp = opOrder.size();
    
    // set opTypes: method NO.1
    // int depth = 0;
    // getDepth(expr, depth);
    // cout << "codegen: depth: " << depth << endl;
    // int middle = (depth - 3) > 1 ? (depth - 3) : 1; // !!!
    // int middle = depth - 1;
    // setType(expr, depth, middle);
    // set opTypes: method NO.2
    // auto order = expr->getOrder();
    // vector<string> opTypes;
    // for(int j = 0; j <= order; j++)
    // {
    //     auto tmp = currentNum % 2;
    //     if(tmp == 0)
    //     {
    //         opTypes.push_back("DD");
    //     }
    //     else
    //     {
    //         opTypes.push_back("double");
    //     }
    //     currentNum = currentNum >> 1;
    // }
    
    vector<string> dataTypes = {"DD", "ld", "double"};
    int lenDataTypes = dataTypes.size();
    int maxNum = pow(lenDataTypes, lenOp); // for dataTypes = {"ld", "double"}, maxNum = 1 << lenOp;
    for(int num = 0; num < maxNum; num++)
    {
        // set opTypes: method NO.3
        auto currentNum = num;
        std::map<int, string> opTypes;
        for(size_t i = 0; i < lenOp; i++)
        {
            auto id = opOrder[i];
            auto tmp = currentNum % lenDataTypes;
            opTypes[id] = dataTypes[tmp];
            currentNum = currentNum / lenDataTypes;
        }
        setType(expr, opTypes);

        // init to generate code
        string directory = "srcTest/" + uniqueLabel + "/";
        string funcName = "expr_" + uniqueLabel + "_" + tail + "_" + to_string(num);
        string fileName = directory + funcName + ".c";
        cout << "fileName: " << fileName << "\topTypes: ";
        for(map<int, string>::iterator it = opTypes.begin(); it != opTypes.end(); it++)
        {
            cout << it->first << " " << it->second << ", ";
        }
        cout << "\n";
        ofstream file_clean(fileName, ios_base::out);
        ofstream ofs(fileName, ios::app);        

        // call codegen to generate code
        codegen(expr, vars, funcName, ofs);
    }

    return maxNum;
}
