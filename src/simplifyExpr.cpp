#include <iostream>
#include <Python.h>
#include <unistd.h>
#include <fstream>
#include "simplifyExpr.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"

void simplifyInit(std::unique_ptr<ExprAST> &expr)
{
    std::string exprStr = PrintExpression(expr);

    // Get the path of pythonBefore.txt
    std::string filename;
    char buf[128] = {0};
    getcwd(buf, sizeof(buf));
    std::string bufStr = buf;
    bufStr += "/src/";
    filename = bufStr + "pythonBefore.txt";
    // std::cout << "exprStr : " << exprStr << std::endl;
    // std::cout << "bufStr  : " << bufStr << std::endl;
    // std::cout << "filename: " << filename << std::endl;
    
    // Write the expression to pythonBefore.txt
    std::ofstream fout;
    fout.open(filename, std::ios::trunc);
    fout << exprStr;
    fout.close();
}

void handlePython()
{
    // Initialize Python interface
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        std::cout << "python init fail" << std::endl;
    }
    // Initialize variables
    PyObject *pModule = nullptr;
    PyObject *pFunc = nullptr;

    // Initialize Python system file path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("path = os.path.abspath('.')");
    PyRun_SimpleString("path = path + '/src/'");
    // PyRun_SimpleString("print(path)");
    PyRun_SimpleString("sys.path.append(path)");

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
    std::cout << "return result is " << nResult << std::endl;

    // End Python interpreter and release resources
    Py_Finalize();
}

std::unique_ptr<ExprAST> simplifyExpr(std::unique_ptr<ExprAST> &expr)
{
    // Write expr string into pythonBefore.txt
    simplifyInit(expr);
    
    // Call a python script named simplify.py
    // Simplify.py:
    // 1. extract expression from pythonBefore.txt;
    // 2. simplify the expression;
    // 3. write the simplify result into pythonAfter.txt
    handlePython();
    
    // Convert the string in pythonAfter.txt to ExprAST
    std::unique_ptr<ExprAST> finalExpr = ParseExpressionFromString();

    return finalExpr->Clone();
}
