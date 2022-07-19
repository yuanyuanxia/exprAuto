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
// #define DEBUG

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token
{
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok()
{
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar))
    { // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.')
    { // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#')
    {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

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

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() { return CurTok = gettok(); }

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence()
{
    if (!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str)
{
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
    LogError(Str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // consume the number
    return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr()
{
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ).
    return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
    std::string IdName = IdentifierStr;

    getNextToken(); // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return std::make_unique<VariableExprAST>(IdName);

    // Call.
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        while (true)
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary()
{
    switch (CurTok)
    {
    default:
        return LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}

/// binoprhs
///   ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS)
{
    // If this is a binop, find its precedence.
    while (true)
    {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec)
        {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        LHS =
            std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

/// expression
///   ::= primary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression()
{
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype()
{
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success.
    getNextToken(); // eat ')'.

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition()
{
    getNextToken(); // eat def.
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

/// toplevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr()
{
    if (auto E = ParseExpression())
    {
        // Make an anonymous proto.
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                    std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern()
{
    getNextToken(); // eat extern.
    return ParsePrototype();
}


//===----------------------------------------------------------------------===//
// print information
//===----------------------------------------------------------------------===//

std::string PrintExpression(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
    // const std::string exprType = expr->type();
    #ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
    #endif
    std::string exprStr = "";
    if (expr->type() == "Number")
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

        return std::to_string(number);
    }
    else if (expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        std::string variable = (variableExpr->getVariable());
        #ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
        #endif

        return variable;
    }
    else if (expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
        #ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
        #endif
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();

        std::vector<std::string> argsStr;
        for (long unsigned int i = 0; i < args.size(); ++i)
        {
            std::string strTmp = PrintExpression(args.at(i)); // std::unique_ptr<ExprAST>& exprTmp = args.at(i);
            argsStr.push_back(strTmp);
        }
        callee += "(";
        for (long unsigned int i = 0; i < argsStr.size() - 1; ++i)
        {
            callee += argsStr.at(i) + ", ";
        }
        callee += argsStr.back() + ")";
        return callee;
    }
    else if (expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        // std::unique_ptr<BinaryExprAST> binOp = std::make_unique<BinaryExprAST>(expr); // a old wrong method
        char op = binOp->getOp();
        std::string opStr(1, op);
        #ifdef DEBUG
        fprintf(stderr, "op: %s\n", opStr.c_str());
        #endif

        std::unique_ptr<ExprAST>& lhs = binOp->getLHS();
        std::string lhsStr = PrintExpression(lhs);
        std::unique_ptr<ExprAST>& rhs = binOp->getRHS();
        std::string rhsStr = PrintExpression(rhs);

        exprStr += "(" + lhsStr + opStr + rhsStr + ")";
    }
    else
    {
        exprStr = "unknown expression";
    }
    return exprStr;
}

static void PrintFunction(std::unique_ptr<FunctionAST> &fun)
{
    if (fun == nullptr)
    {
        fprintf(stderr, "this is a nullptr.\n");
    }
    else
    {
        std::string funcNameStr = fun->getFuncName();
        std::vector<std::string> funcArgsStr = fun->getFuncArgs();
        std::unique_ptr<ExprAST> &expr = fun->getFuncBody();

        fprintf(stderr, "funcName: %s\n", funcNameStr.c_str());
        fprintf(stderr, "Args list:\n");
        if (funcArgsStr.size() == 0)
        {
            fprintf(stderr, "\tempty args");
        }
        else
        {
            for (auto it = funcArgsStr.begin(); it != funcArgsStr.end(); ++it)
            {
                fprintf(stderr, "%s ", (*it).c_str());
            }
        }
        fprintf(stderr, "\nFunc Body:\n");
        // expr->printExpr();
        std::string funcBodyStr = PrintExpression(expr);
        fprintf(stderr, "\t%s\n", funcBodyStr.c_str());
    }
}


//===----------------------------------------------------------------------===//
// Change exprAST equally
//===----------------------------------------------------------------------===//
std::unique_ptr<ExprAST> changeExpression(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr) {
        fprintf(stderr, "empty\n");
    }
    const std::string exprType = expr->type();
    #ifdef DEBUG
    fprintf(stderr, "expr type: %s;\t", exprType.c_str());
    #endif

    if (expr->type() == "Number")
    {
        NumberExprAST *numberExpr = dynamic_cast<NumberExprAST *>(expr.get());
        double number = (numberExpr->getNumber());
        #ifdef DEBUG
        fprintf(stderr, "number: %f\n", number);
        #endif

        return std::make_unique<NumberExprAST>(number);
    }
    else if (expr->type() == "Variable")
    {
        VariableExprAST *variableExpr = dynamic_cast<VariableExprAST *>(expr.get());
        std::string variable = (variableExpr->getVariable());
        #ifdef DEBUG
        fprintf(stderr, "variable: %s\n", variable.c_str());
        #endif
        return std::make_unique<VariableExprAST>(variable);
    }
    else if (expr->type() == "Call")
    {
        CallExprAST *callExpr = dynamic_cast<CallExprAST *>(expr.get());
        std::string callee = (callExpr->getCallee());
        #ifdef DEBUG
        fprintf(stderr, "call: %s\n", callee.c_str());
        #endif
        std::vector<std::unique_ptr<ExprAST>> &args = callExpr->getArgs();

        std::vector<std::unique_ptr<ExprAST>> argsNew;
        for (long unsigned int i = 0; i < args.size(); ++i)
        {
            auto arg = changeExpression(args.at(i));
            argsNew.push_back(std::move(arg));
        }

        return std::make_unique<CallExprAST>(callee, std::move(argsNew));
    }
    else if (expr->type() == "Binary")
    {
        BinaryExprAST *binOp = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOp->getOp();
        std::string opStr(1, op);
        #ifdef DEBUG
        fprintf(stderr, "op: %s\n", opStr.c_str());
        #endif

        std::unique_ptr<ExprAST>& lhs = binOp->getLHS();
        std::unique_ptr<ExprAST>& rhs = binOp->getRHS();
        
        const std::string exprTypeLHS = lhs->type();
        const std::string exprTypeRHS = rhs->type();

        if (op == '*')
        {
            if ((exprTypeLHS == "Binary") && (exprTypeRHS == "Binary")) // LHS = RHS = "Binary" && opL = opR = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opL = binOpL->getOp();
                std::string opLStr(1, opL);
                char opR = binOpR->getOp();
                std::string opRStr(1, opR);
                if ((opL == '+') && (opR == '+'))
                {
                    // ready for left
                    std::unique_ptr<ExprAST>& lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST>& rhsL = binOpL->getRHS();
                    std::unique_ptr<ExprAST> lhsLNew = changeExpression(lhsL);
                    auto lhsLNewCopy = lhsLNew->Clone();
                    std::unique_ptr<ExprAST> rhsLNew = changeExpression(rhsL);
                    auto rhsLNewCopy = rhsLNew->Clone();

                    // ready for right
                    std::unique_ptr<ExprAST>& lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST>& rhsR = binOpR->getRHS();
                    std::unique_ptr<ExprAST> lhsRNew = changeExpression(lhsR);
                    auto lhsRNewCopy = lhsRNew->Clone();
                    std::unique_ptr<ExprAST> rhsRNew = changeExpression(rhsR);
                    auto rhsRNewCopy = rhsRNew->Clone();

                    // combine left
                    std::unique_ptr<ExprAST> lhsLTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNew), std::move(lhsRNew));
                    std::unique_ptr<ExprAST> lhsLFinal = changeExpression(lhsLTmp);
                    std::unique_ptr<ExprAST> rhsLTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNewCopy), std::move(rhsRNew));
                    std::unique_ptr<ExprAST> rhsLFinal = changeExpression(rhsLTmp);
                    std::unique_ptr<ExprAST> exprLFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsLFinal), std::move(rhsLFinal));
                    // combine right
                    std::unique_ptr<ExprAST> lhsRTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNew), std::move(lhsRNewCopy));
                    std::unique_ptr<ExprAST> lhsRFinal = changeExpression(lhsRTmp);
                    std::unique_ptr<ExprAST> rhsRTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNewCopy), std::move(rhsRNewCopy));
                    std::unique_ptr<ExprAST> rhsRFinal = changeExpression(rhsRTmp);
                    std::unique_ptr<ExprAST> exprRFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsRFinal), std::move(rhsRFinal));
                    // combine together
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(exprLFinal), std::move(exprRFinal));

                    return exprFinal;
                }
            }
            else if (exprTypeLHS == "Binary") // LHS = "Binary" && RHS != "Binary" && opL = "+"
            {
                BinaryExprAST *binOpL = dynamic_cast<BinaryExprAST *>(lhs.get());

                char opL = binOpL->getOp();
                std::string opLStr(1, opL);

                if (opLStr == "+") {
                    std::unique_ptr<ExprAST>& lhsL = binOpL->getLHS();
                    std::unique_ptr<ExprAST>& rhsL = binOpL->getRHS();
                    
                    std::unique_ptr<ExprAST> lhsLNew = changeExpression(lhsL);
                    std::unique_ptr<ExprAST> rhsLNew = changeExpression(rhsL);
                    std::unique_ptr<ExprAST> rhsNew = changeExpression(rhs);
                    auto rhsNewCopy = rhsNew->Clone();

                    std::unique_ptr<ExprAST> lhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsLNew), std::move(rhsNew));
                    std::unique_ptr<ExprAST> lhsFinal = changeExpression(lhsTmp);
                    std::unique_ptr<ExprAST> rhsTmp = std::make_unique<BinaryExprAST>('*', std::move(rhsLNew), std::move(rhsNewCopy));
                    std::unique_ptr<ExprAST> rhsFinal = changeExpression(rhsTmp);
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));
                    
                    return exprFinal;
                }
            }
            else if (exprTypeRHS == "Binary") // LHS != "Binary" && RHS = "Binary" && opR = "+"
            {
                BinaryExprAST *binOpR = dynamic_cast<BinaryExprAST *>(rhs.get());

                char opR = binOpR->getOp();
                std::string opRStr(1, opR);
                
                if (opRStr == "+") {
                    std::unique_ptr<ExprAST>& lhsR = binOpR->getLHS();
                    std::unique_ptr<ExprAST>& rhsR = binOpR->getRHS();
                    
                    std::unique_ptr<ExprAST> lhsNew = changeExpression(lhs);
                    auto lhsNewCopy = lhsNew->Clone();
                    std::unique_ptr<ExprAST> lhsRNew = changeExpression(lhsR);
                    std::unique_ptr<ExprAST> rhsRNew = changeExpression(rhsR);

                    std::unique_ptr<ExprAST> lhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsNew), std::move(lhsRNew));
                    std::unique_ptr<ExprAST> lhsFinal = changeExpression(lhsTmp);
                    std::unique_ptr<ExprAST> rhsTmp = std::make_unique<BinaryExprAST>('*', std::move(lhsNewCopy), std::move(rhsRNew));
                    std::unique_ptr<ExprAST> rhsFinal = changeExpression(rhsTmp);
                    std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>('+', std::move(lhsFinal), std::move(rhsFinal));
                    return exprFinal;
                }
            }
        }
        // other situations
        std::unique_ptr<ExprAST> lhsNew = changeExpression(lhs);
        std::unique_ptr<ExprAST> rhsNew = changeExpression(rhs);
        std::unique_ptr<ExprAST> exprFinal = std::make_unique<BinaryExprAST>(op, std::move(lhsNew), std::move(rhsNew));
        return exprFinal;
    }
    else // TODO: Error handling is to be supported
    {
        std::string exprStr = "unknown expression";
        return std::make_unique<VariableExprAST>(exprStr);
    }
}

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition()
{
    if (ParseDefinition())
    {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern()
{
    if (ParseExtern())
    {
        fprintf(stderr, "Parsed an extern\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression()
{
    std::unique_ptr<FunctionAST> fun = ParseTopLevelExpr();
    // Evaluate a top-level expression into an anonymous function.
    if (fun)
    {
        fprintf(stderr, "Parsed a top-level expr\n");
        PrintFunction(fun);
        
        fprintf(stderr, "After change\n");
        
        std::unique_ptr<ExprAST> &exprOrigin = fun->getFuncBody();
        std::unique_ptr<ExprAST> exprNew = changeExpression(exprOrigin);

        std::string funcBodyStr = PrintExpression(exprNew);
        fprintf(stderr, "\t%s\n", funcBodyStr.c_str());
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop()
{
    while (true)
    {
        fprintf(stderr, "ready> ");
        switch (CurTok)
        {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            getNextToken();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
    // Install standard binary operators.
    // 1 is lowest precedence.
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // highest.

    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();

    // Run the main "interpreter loop" now.
    MainLoop();

    return 0;
}