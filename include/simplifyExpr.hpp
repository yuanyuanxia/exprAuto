#include <iostream>
#include <Python.h>
#include <unistd.h>
std::unique_ptr<ExprAST> simplifyExpr(std::unique_ptr<ExprAST> &expr)
{
    std::string exprStr = PrintExpression(expr);

    //Get file path
    std::string filename;
    char buf[128] = {0};
    getcwd(buf, sizeof(buf));
    std::cout << buf << std::endl;
    for (size_t i = 0; i < sizeof(buf) / sizeof(buf[0]); i++)
    {
        if (buf[i])
        {
            filename += buf[i];
        }
    }
    filename += "/pythonBefore.txt";

    //Write file
    std::ofstream fout;

    fout.open(filename, std::ios::trunc);
    fout << exprStr;
    fout.close();

    //Handle by python

    //Initialize Python interface
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        std::cout << "python init fail" << std::endl;
    }
    //Initialize variables
    PyObject *pModule = nullptr;
    PyObject *pFunc = nullptr;

    //Initialize Python system file path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("path = os.path.abspath('.')");
    PyRun_SimpleString("sys.path.append(path)");

    //Call simplify.py
    pModule = PyImport_ImportModule("simplify");
    if (!pModule)
    {
        std::cout << "pModule is nullptr" << std::endl;
    }

    //Call the process function in the simplify.py
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

    //End Python interpreter and release resources
    Py_Finalize();

    //Convert the string in pythonAfter.txt to ExprAST
    std::unique_ptr<ExprAST> finalExpr = ParseExpressionFromString();

    return finalExpr->Clone();
}
