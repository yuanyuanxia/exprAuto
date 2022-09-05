#include "laxerAST.hpp"

std::string IdentifierStr;  // Filled in if tok_identifier
double NumVal;              // Filled in if tok_number
int CurTok;                 // CurTok is the current token the parser is looking at.
// BinopPrecedence - This holds the precedence for each binary operator that is defined.
std::map<char, int> BinopPrecedence;

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

/// gettok - Return the next token from standard input.
int gettok()
{
    static int LastChar = ' ';
    bool minusFlag = false;

    // Skip any whitespace.
    while(isspace(LastChar))
        LastChar = getchar();

    if(isalpha(LastChar))
    {  // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while(isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if(IdentifierStr == "string")    //add the option of stringToExpr
			return tok_string;
        if(IdentifierStr == "def")
            return tok_def;
        if(IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    if(LastChar == '-')
    {  // judge if there is a minus sign
        LastChar = getchar();
        if(isspace(LastChar))
        {  // if the character after '-' is a space, just return '-'
            LastChar = getchar();
            return '-';
        }
        minusFlag = true;
    }

    if(isdigit(LastChar) || LastChar == '.')
    {  // Number: [0-9.]+
        std::string NumStr;
        if(minusFlag)
        {
            NumStr += '-';
            minusFlag = false;
        }
        do
        {
            NumStr += LastChar;
            LastChar = getchar();
        } while(isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if(LastChar == '#')
    {
        // Comment until end of line.
        do
            LastChar = getchar();
        while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if(LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
int getNextToken() { return CurTok = gettok(); }

// Install standard binary operators.
void installOperators()
{
    // 1 is lowest precedence.
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;  // highest.
    BinopPrecedence['/'] = 40;  // highest.
}

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
int GetTokPrecedence()
{
    if(!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    if(TokPrec <= 0)
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
