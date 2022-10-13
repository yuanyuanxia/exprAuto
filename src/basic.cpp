#include "basic.hpp"

#include <sstream>
#include <iomanip>
#include <string>

#define DOUBLE_PRECISION 17

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

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

string PrintExpression(const ast_ptr &expr)
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
        std::stringstream ss;
        ss << std::setprecision(DOUBLE_PRECISION) << number;
        return ss.str(); // better than to_string: 1 outputs as 1, 1.23456789 outputs as 1.23456789
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
            string strTmp = PrintExpression(args.at(i));  // ast_ptr& exprTmp = args.at(i);
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
        string lhsStr = PrintExpression(lhs);
        ast_ptr &rhs = binOp->getRHS();
        string rhsStr = PrintExpression(rhs);

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

void printExpr(const ast_ptr &expr, string prefix, int index)
{
    if(index == -1)
        fprintf(stderr, "%s%s\n", prefix.c_str(), PrintExpression(expr).c_str());
    else
        fprintf(stderr, "%sNo.%d: %s\n", prefix.c_str(), index, PrintExpression(expr).c_str());
}

void printExprs(const vector<ast_ptr> &exprs, string prefix)
{
    for(size_t i = 0; i < exprs.size(); i++)
    {
        auto &expr = exprs.at(i);
        fprintf(stderr, "%sNo.%ld: %s\n", prefix.c_str(), i, PrintExpression(expr).c_str());
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

int printASTKernel(const ast_ptr &expr, const int posit, vector<string> &treePics, vector<int> &rightests)
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
    
    static size_t level = -1;
    level++;
    if (treePics.size() < level + 1) // lhs
    {
        if (treePics.size() != level)
        {
            cerr << "ERROR: printASTKernel: size small" << endl;
            exit(EXIT_FAILURE);
        }
        string tmpStr(posit, blankChar);
        int tmpSize = tmpStr.size();
        treePics.push_back(tmpStr);
        rightests.push_back(tmpSize);
    }

    auto &currentPic = treePics.at(level);
    int rightest = currentPic.size(); // int rightest = rightests.at(level); // NOTE: can not change 'auto rightest' to 'auto &rightest' !
    const string exprType = expr->type();
    if(exprType == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());

        updateStr(currentPic, posit, rightest);

        std::stringstream ss;
        ss << std::setprecision(DOUBLE_PRECISION) << number;
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
            childRightest = printASTKernel(arg, childRightest, treePics, rightests);
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
        childRightest = printASTKernel(lhs, childRightest, treePics, rightests);        
        childRightest = printASTKernel(rhs, childRightest, treePics, rightests);

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
    level--;
    return rightest;
}

void printAST(const ast_ptr &expr)
{
    vector<string> treePics;
    vector<int> rightests;
    printASTKernel(expr, 0, treePics, rightests);
    for (auto &treePic : treePics)
    {
        cout << treePic << endl;
    }
}
