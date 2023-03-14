from sympy import *
from os import path as os_path

# Simplify expr


def support_char():
    ns = {}
    for i in range(65, 91):
        ns[chr(i)] = Symbol(chr(i))
    # Returns a dictionary of extended characters for use by the locals function in simplif()
    return ns

def parse(t_str):
    # print("parse", t_str)
    expr = sympify(t_str, locals=support_char(), evaluate=False)
    # print("sympify", sstr(expr, min=-11, full_prec=False))
    expr = simplify(expr)
    # print("simplify", expr)
    expr = expand(expr)
    # print("expand", expr)
    expr = cancel(expr)
    # print("cancel", expr)
    expr = together(expr)
    # print("together", expr)
    return sstr(expr, min=-21)



def find_index(expr):
    index_lst = []
    for i in range(len(expr)):
        if expr[i] == '*' and expr[i + 1] == '*':
            index_lst.append(i)
    return index_lst


def find_base(expr, i):
    op_lst = ['+', '-', '*', '/', ',', ' ']
    if expr[i - 1] == ')':
        flag = 0
        j = i - 2
        while j >= 0:
            if expr[j] == ')':
                flag += 1
            if expr[j] == '(' and flag == 0:
                break
            if expr[j] == '(':
                flag -= 1
            j -= 1
        return [expr[j:i], j]
    else:
        j = i - 2
        while j >= 0:
            if expr[j] in op_lst or expr[j] == '(':
                break
            j -= 1
        return [expr[j + 1:i], j + 1]


def find_exp(expr, i):
    op_lst = ['+', '-', '*', '/', ',']
    if expr[i + 2] == '(':
        flag = 0
        j = i + 3
        while j <= len(expr) - 1:
            if expr[j] == '(':
                flag += 1
            if expr[j] == ')' and flag == 0:
                break
            if expr[j] == ')':
                flag -= 1
            j += 1
        # print("j", j)
        return [expr[i + 2:j + 1], j]
    else:
        j = i + 3
        while j <= len(expr) - 1:
            if expr[j] in op_lst or expr[j] == ')':
                break
            j += 1
        return [expr[i + 2:j], j - 1]


def gene_content(temp_dict):
    '''
    {6: [{'x': 5}, {'2': 8}], 11: [{'y': 10}, {'3': 13}]}
    '''
    content_dict = {}
    for key in temp_dict.keys():
        content = ''

        if ('.' in temp_dict[key][1][0]) or ('(' in temp_dict[key][1][0]):
            content = 'pow(' + temp_dict[key][0][0] + \
                ',' + temp_dict[key][1][0] + ')'
            content_dict[key] = content
        else:
            init = temp_dict[key][0][0]
            for i in range(int(temp_dict[key][1][0])):
                if i == int(temp_dict[key][1][0]) - 1:
                    content += init
                else:
                    content += init
                    content += '*'
            content_dict[key] = '(' + content + ')'

    return content_dict


def gene_result(expr, temp_dict, content_dict):
    final_expr = ''
    temp_lst = []
    key_index = 0
    last_key = None
    for key in temp_dict.keys():
        if key_index == 0:
            temp_lst.append(expr[:temp_dict[key][0][1]])
            temp_lst.append(content_dict[key])
            if len(temp_dict.keys()) == 1:
                temp_lst.append(expr[temp_dict[key][1][1] + 1:])
        elif key_index == len(temp_dict.keys()) - 1:
            temp_lst.append(
                expr[(temp_dict[last_key][1][1] + 1):temp_dict[key][0][1]])
            temp_lst.append(content_dict[key])
            temp_lst.append(expr[temp_dict[key][1][1] + 1:])
        else:
            temp_lst.append(
                expr[(temp_dict[last_key][1][1] + 1):temp_dict[key][0][1]])
            temp_lst.append(content_dict[key])
        key_index += 1
        last_key = key
    for char in temp_lst:
        final_expr += char
    return final_expr

# Expand expr,x**n ----> x*x*...*x,x**a.b ----> pow(x,a.b)


def new_expand(expr):
    index_lst = find_index(expr)
    temp_dict = {}
    if index_lst:
        for index in index_lst:
            base_dic = find_base(expr, index)
            exp_dic = find_exp(expr, index)
            temp_dict[index] = [base_dic, exp_dic]
        # print(temp_dict)
        content_dict = gene_content(temp_dict)
        # print(content_dict)
        final_expr = gene_result(expr, temp_dict, content_dict)
        return final_expr

    return expr

# useless
def process(i_filename="pythonBefore.txt", o_filename="pythonAfter.txt"):
    pyfile_path = os_path.dirname(globals()["__file__"])
    # print("process: pyfile_path " + pyfile_path)
    i_filename = pyfile_path + "/" + "pythonBefore.txt"
    o_filename = pyfile_path + "/" + "pythonAfter.txt"
    with open(i_filename, "r", encoding="UTF-8") as f1, \
            open(o_filename, "w", encoding="UTF-8") as f2:
        input_str = f1.read().strip()
        # print("process: input_str " + input_str)
        out_str = str(parse(input_str))
        # print("process: out_str1 " + out_str)
        out_str = new_expand(out_str)
        # print("process: out_str2 " + out_str)
        f2.write(out_str)
    return 1


def processStr(exprStr = "1 + x"):
    # print("processStr: exprStr " + exprStr)
    out_str = str(parse(exprStr))
    # print("processStr: out_str1 " + out_str)
    out_str = new_expand(out_str)
    # print("processStr: out_str2 " + out_str)
    return out_str
