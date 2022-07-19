#ifndef EXPR
#define EXPR
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <typeinfo>
#include <iostream>

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace
{

    /// ExprAST - Base class for all expression nodes.
    class ExprAST
    {
    public:
        virtual ~ExprAST() = default;

        virtual void printExpr()
        {
            fprintf(stderr, "the base class for expression\n");
        }

        virtual std::string type()
        {
            return "Base";
        }

        virtual std::unique_ptr<ExprAST> Clone() { return std::make_unique<ExprAST>(); }
    };

    /// NumberExprAST - Expression class for numeric literals like "1.0".
    class NumberExprAST : public ExprAST
    {
        double Val;

    public:
        NumberExprAST(double Val) : Val(Val) {}

        void printExpr()
        {
            fprintf(stderr, "num = %f\n", Val);
        }

        std::string type()
        {
            return "Number";
        }

        double getNumber() { return Val; }

        std::unique_ptr<ExprAST> Clone() { return std::make_unique<NumberExprAST>(Val); }
    };

    /// VariableExprAST - Expression class for referencing a variable, like "a".
    class VariableExprAST : public ExprAST
    {
        std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}

        void printExpr()
        {
            fprintf(stderr, "variable = %s\n", Name.c_str());
        }

        std::string type()
        {
            return "Variable";
        }

        std::string getVariable() { return Name; }

        std::unique_ptr<ExprAST> Clone() { return std::make_unique<VariableExprAST>(Name); }
    };

    /// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST
    {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

        void printExpr()
        {
            fprintf(stderr, "op = %c\n", Op);
            LHS->printExpr();
            RHS->printExpr();
        }

        std::string type()
        {
            return "Binary";
        }

        char getOp() { return Op; }
        std::unique_ptr<ExprAST>& getLHS() { return LHS; }
        std::unique_ptr<ExprAST>& getRHS() { return RHS; }

        std::unique_ptr<ExprAST> Clone()
        {
            auto LHSNew = LHS->Clone();
            auto RHSNew = RHS->Clone();
            return std::make_unique<BinaryExprAST>(Op, std::move(LHSNew), std::move(RHSNew));
        }
    };

    /// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST
    {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string &Callee,
                    std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}

        void printExpr()
        {
            fprintf(stderr, "call function name = %s\n", Callee.c_str());
            fprintf(stderr, "call function args =\n");
            for (long unsigned int i = 0; i < Args.size(); ++i)
            {
                (Args[i])->printExpr();
            }
        }

        std::string type()
        {
            return "Call";
        }

        std::string getCallee() { return Callee; }
        std::vector<std::unique_ptr<ExprAST>> &getArgs() { return Args; }

        std::unique_ptr<ExprAST> Clone()
        {
            std::vector<std::unique_ptr<ExprAST>> ArgsNew;
            for (long unsigned int i = 0; i < Args.size(); ++i)
            {
                auto arg = (Args.at(i))->Clone();
                ArgsNew.push_back(std::move(arg));
            }

            return std::make_unique<CallExprAST>(Callee, std::move(ArgsNew));
        }
    };

    /// PrototypeAST - This class represents the "prototype" for a function,
    /// which captures its name, and its argument names (thus implicitly the number
    /// of arguments the function takes).
    class PrototypeAST
    {
        std::string Name;
        std::vector<std::string> Args;

    public:
        PrototypeAST(const std::string &Name, std::vector<std::string> Args)
            : Name(Name), Args(std::move(Args)) {}

        const std::string &getName() const { return Name; }
        const std::vector<std::string> &getArgs() const { return Args; }
    };

    /// FunctionAST - This class represents a function definition itself.
    class FunctionAST
    {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                    std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}

        const std::string &getFuncName() const { return Proto->getName(); }
        const std::vector<std::string> &getFuncArgs() const { return Proto->getArgs(); }
        std::unique_ptr<ExprAST> &getFuncBody() { return Body; }
    };

} // end anonymous namespace



#endif