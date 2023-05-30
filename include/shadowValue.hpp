#ifndef _SHADOWVALUE
#define _SHADOWVALUE

#include "basic.hpp"
namespace Shadow {

void shadowValue(ast_ptr &expr, double input);

template <typename T>
vector<string> shadowValue(const ast_ptr &expr, const std::map<string, T> &varsValue, int length = 1, bool ifUnique = false, string uniqueLabel = "", string funcName = "");

}
#endif