#ifndef _SHADOWVALUE
#define _SHADOWVALUE

#include "basic.hpp"

void shadowValue(ast_ptr &expr, double input);

void shadowValue(const ast_ptr &expr, const std::map<string, double> &varsValue);

#endif