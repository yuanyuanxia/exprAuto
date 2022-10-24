#include "simplifyExpr.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"

#include <iostream>
#include <Python.h>
#include <unistd.h>
#include <fstream>

using std::string;

void simplifyInit(const string &exprStr)
{
    // Get the path of pythonBefore.txt
    string filename;
    char buf[128] = {0};
    getcwd(buf, sizeof(buf));
    string bufStr = buf;
    filename = bufStr + "/src/" + "pythonBefore.txt";
    // std::cout << "exprStr : " << exprStr << std::endl;
    // std::cout << "bufStr  : " << bufStr << std::endl;
    // std::cout << "filename: " << filename << std::endl;
    
    // Write the expression to pythonBefore.txt
    std::ofstream fout;
    fout.open(filename, std::ios::trunc);
    fout << exprStr;
    fout.close();
}

void initPython()
{
    // Initialize Python interface
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        std::cout << "python init fail" << std::endl;
    }

    // Initialize Python system file path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("from sys import path as sys_path");
    PyRun_SimpleString("from os import path as os_path");
    PyRun_SimpleString("path = os_path.abspath('./src')");
    // PyRun_SimpleString("print(path)");
    PyRun_SimpleString("sys_path.append(path)");
}

void handlePython()
{
    // Initialize variables
    PyObject *pModule = nullptr;
    PyObject *pFunc = nullptr;

    // Call simplify.py
    pModule = PyImport_ImportModule("simplify");
    if (!pModule)
    {
        std::cout << "pModule is nullptr" << std::endl;
    }

    // Call the process function in the simplify.py
    pFunc = PyObject_GetAttrString(pModule, "process");
    if (!pFunc)
    {
        std::cout << "pFunc is nullptr" << std::endl;
    }
    PyObject *pArgs = PyTuple_New(0);
    PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);
    
    int nResult;
    PyArg_Parse(pReturn, "i", &nResult);
    // std::cout << "return result is " << nResult << std::endl;
}

void endPython()
{
    // End Python interpreter and release resources
    Py_Finalize();
}

ast_ptr simplifyExpr(const string &exprStr)
{
    // Write expr string into pythonBefore.txt
    simplifyInit(exprStr);
    
    // Call a python script named simplify.py
    // Simplify.py:
    // 1. extract expression from pythonBefore.txt;
    // 2. simplify the expression;
    // 3. write the simplify result into pythonAfter.txt
    handlePython();
    
    // Convert the string in pythonAfter.txt to ExprAST
    return ParseExpressionFromString();
}

ast_ptr simplifyExprFromStr(const string &exprStr)
{
    // Call a python script named simplify.py
    // 1. transfer the exprStr by c++&python interface;
    // 2. simplify the expression by sympy;
    // 3. return the simplify result
    
    // Initialize variables
    PyObject *pModule = nullptr;
    PyObject *pFunc = nullptr;

    // Call simplify.py
    pModule = PyImport_ImportModule("simplify");
    if (!pModule)
    {
        std::cout << "pModule is nullptr" << std::endl;
    }

    // Call the process function in the simplify.py
    pFunc = PyObject_GetAttrString(pModule, "processStr");
    if (!pFunc)
    {
        std::cout << "pFunc is nullptr" << std::endl;
    }

    char *exprChar;
    exprChar = (char *)calloc(exprStr.length(), sizeof(char));
    strcpy(exprChar, exprStr.c_str());
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", exprChar));
    PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);
    
    char *resultChar;
    PyArg_Parse(pReturn, "s", &resultChar);
    string result = resultChar;
    // std::cout << "return result is " << result << std::endl;

    return ParseExpressionFromString(result);
}

ast_ptr simplifyExpr(const ast_ptr &expr)
{
    string exprStr = PrintExpression(expr);

    return simplifyExprFromStr(exprStr);
}
