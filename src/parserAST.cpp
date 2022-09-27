#include "basic.hpp"
#include "laxerAST.hpp"

using std::string;
using std::vector;

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

ast_ptr ParseExpression();

/// numberexpr ::= number
ast_ptr ParseNumberExpr()
{
    auto Result = makePtr<NumberExprAST>(NumVal);
    getNextToken();  // consume the number
    return Result;
}

/// parenexpr ::= '(' expression ')'
ast_ptr ParseParenExpr()
{
    getNextToken();  // eat (.
    auto V = ParseExpression();
    if(!V)
        return nullptr;

    if(CurTok != ')')
        return LogError("expected ')'");
    getNextToken();  // eat ).
    return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
ast_ptr ParseIdentifierExpr()
{
    string IdName = IdentifierStr;

    getNextToken();  // eat identifier.

    if(CurTok != '(')  // Simple variable ref.
        return makePtr<VariableExprAST>(IdName);

    // Call.
    getNextToken();  // eat (
    vector<ast_ptr> Args;
    if(CurTok != ')')
    {
        while(true)
        {
            if(auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if(CurTok == ')')
                break;

            if(CurTok != ',')
                return LogError("ParseIdentifierExpr: Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return makePtr<CallExprAST>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
ast_ptr ParsePrimary()
{
    switch(CurTok)
    {
        default:
            return LogError("ParsePrimary: unknown token when expecting an expression");
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
ast_ptr ParseBinOpRHS(int ExprPrec, ast_ptr LHS)
{
    // If this is a binop, find its precedence.
    while(true)
    {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if(TokPrec < ExprPrec)
            return LHS;

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if(!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = GetTokPrecedence();
        if(TokPrec < NextPrec)
        {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if(!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        if(BinOp == '`'){
            BinOp = '*';
        }
        LHS = makePtr<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

/// expression
///   ::= primary binoprhs
///
ast_ptr ParseExpression()
{
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> ParsePrototype()
{
    if(CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    string FnName = IdentifierStr;
    getNextToken();

    if(CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    vector<string> ArgNames;
    while(getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if(CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success.
    getNextToken();  // eat ')'.

    return makePtr<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> ParseDefinition()
{
    getNextToken();  // eat def.
    auto Proto = ParsePrototype();
    if(!Proto)
        return nullptr;

    if(auto E = ParseExpression())
        return makePtr<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

/// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> ParseTopLevelExpr()
{
    if(auto E = ParseExpression())
    {
        // Make an anonymous proto.
        auto Proto = makePtr<PrototypeAST>("__anon_expr", vector<string>());
        return makePtr<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> ParseExtern()
{
    getNextToken();  // eat extern.
    return ParsePrototype();
}
