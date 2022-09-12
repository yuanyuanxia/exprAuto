from sympy import *
from os import path as os_path

# Simplify expr
def parse(t_str):
    expr = expand(simplify(sympify(t_str)))
    return expr

# Expand expr,x**n ----> x*x*...*x
def expand_expr(expr):
    result = ""

    i_lst = []
    for i in range(len(expr)):
        if expr[i] == "*" and expr[i + 1] == "*":
            i_lst.append(i)

    if i_lst:
        dic = {}

        for i in i_lst:
            new = ""  
            for j in range(int(expr[i + 2]) - 1): 
                new += "*"
                new += expr[i - 1]
            dic[i + 1] = new

        dic_keys = list(dic.keys())

        result += expr[:(dic_keys[0] - 1)]

        for i in range(len(dic_keys)):
            if i == len(dic_keys) - 1:
                result += dic[dic_keys[i]]
                result += expr[dic_keys[i] + 2:]
            else:
                result += dic[dic_keys[i]]
                result += expr[dic_keys[i] + 2:dic_keys[i + 1] - 1]
        return result
    return expr


def process(i_filename="pythonBefore.txt", o_filename="pythonAfter.txt"):
    pyfile_path = os_path.dirname(globals()["__file__"])
    # print("process: pyfile_path " + pyfile_path)
    i_filename = pyfile_path + "/" + "pythonBefore.txt"
    o_filename = pyfile_path + "/" + "pythonAfter.txt"
    with open(i_filename, "r", encoding="UTF-8") as f1, \
            open(o_filename, "w", encoding="UTF-8") as f2:
        input_str = f1.read().strip()
        print("process: input_str " + input_str)
        out_str = str(parse(input_str))
        print("process: out_str1 " + out_str)
        out_str = expand_expr(out_str)
        print("process: out_str2 " + out_str)
        f2.write(out_str)
    return 1
