#include "basic.hpp"
#include "laxerASTLY.hpp"
#include "parserASTLY.hpp"
#include "string.h"

#include "stdlib.h "
#include "stdio.h"


static void HandleParseExpressionFromString()
{
    std::unique_ptr<ExprAST> es = ParseExpressionFromString();
    if(es)
    {
        fprintf(stderr, "Parsed an string\n");
	std::string str = PrintExpression(es);
        fprintf(stderr, "\tstr: %s\n", str.c_str());
    }
}


int main()
{
    installOperatorsForStr();
	
    HandleParseExpressionFromString();
    system("pause");
    return 0;
}
