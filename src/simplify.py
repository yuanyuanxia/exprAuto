from sympy import *
from os import path as os_path

# Simplify expr
def parse(t_str):
    expr = sympify(t_str)
    exprFinal = expand(simplify(expr))
    return exprFinal

# Expand expr,x**n ----> x*x*...*x,x**a.b ----> pow(x,a.b)
def new_expand(expr):
    """
    index_lst:[]    Position of the first character of the power monomial to be replaced
    nextop_dic:{}   The position where the next operator of the power monomial is stored
    max_op:x        The position of the last operator in the polynomial
    coefficient_dic:{}  Power of storing power monomial
    content_dic:{}  Store the contents after power monomial replacement

    """
    result = ""

    

    # Add the index of the character to be replaced to the list
    index_lst = []
    for i in range(len(expr)):
        if expr[i] == '*' and expr[i + 1] == '*':
            index_lst.append(i - 1)
        
    if index_lst:
        operator_lst = ['+', '-', '*', '/']
        nextop_dic = {}
        # Get the position of the next operator of the power
        for k in index_lst:
            for i in range(k + 3, len(expr)):
                if expr[i] in operator_lst:
                    nextop_dic[k] = i
                    break
                else:
                    nextop_dic[k] = None


        # Get the position of the last operator
        max_op = 0
        flag = 0
        while flag < len(expr):
            if expr[flag] in operator_lst:
                if expr[flag] == '*':
                    if expr[flag + 1] == '*':  # If it is * *, jump to the first digit after * *
                        flag += 3
                        continue
                    else:
                        max_op = flag
                else:
                    max_op = flag
            flag += 1

        # If max_op is 0 only to power
        # If the index position to be replaced is greater than max_op, get to the end directly


        coefficient_dic = {}
        for index in index_lst:
            # max_op == 0 It means that there is only one power monomial
            # index > max_o It means that this power monomial is at the end

            # is_one is used to judge whether the expression is only a power expression
            # When max_op==0 and index==0, there will be a conflict, so you need to determine whether the index is 0
            is_one = True
            if index == 0:
                for i in range(index + 3, len(expr)):
                    if expr[i] in operator_lst:
                        is_one = False
                if is_one:
                    coefficient = expr[index + 3:]
                    coefficient_dic[index] = coefficient
                else:
                    for i in range(index + 3, len(expr)):
                        if expr[i] in operator_lst:
                            coefficient = expr[index + 3:i]
                            coefficient_dic[index] = coefficient
                            break
            elif index > max_op:  # If the index is greater than the position of the maximum operator, the following characters are added
                coefficient = expr[index + 3:]
                coefficient_dic[index] = coefficient
            else:  #
                for i in range(index + 3, len(expr)):
                    if expr[i] in operator_lst:
                        coefficient = expr[index + 3:i]
                        coefficient_dic[index] = coefficient
                        break

        # Traverse the index of the character to get the content to be replaced
        content_dic = {}
        for i in index_lst:
            new = ""  # Store replaced content
            if '.' in coefficient_dic[i]:  # Power is not an integer
                new = "pow(" + expr[i] + "," + coefficient_dic[i] + ")"
            else:  # Power is an integer
                char = expr[i]
                new += char
                # pass
                for j in range(int(coefficient_dic[i]) - 1):  # Determine how many times to expand
                    new += "*"
                    new += char
            content_dic[i] = new


        # Replace and generate expression
        dic_keys = list(content_dic.keys())
        result += expr[:dic_keys[0]]  # First add the content before the first power expression to be replaced

        for i in range(len(dic_keys)):
            if i == len(dic_keys) - 1:  # If it is the last power expression to be replaced
                result += content_dic[dic_keys[i]]  # Add the power expression to be replaced first
                if nextop_dic:  # If there is a nextop_dic is a polynomial
                    if nextop_dic[dic_keys[i]]:  # If the monomial is not the last polynomial
                        result += expr[nextop_dic[dic_keys[i]]:]
            else:
                result += content_dic[dic_keys[i]]  # Add the content to be replaced first
                result += expr[nextop_dic[dic_keys[i]]:dic_keys[i+1]]  # Plus the content sandwiched between the original two power monomials

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
        # print("process: input_str " + input_str)
        out_str = str(parse(input_str))
        # print("process: out_str1 " + out_str)
        out_str = new_expand(out_str)
        # print("process: out_str2 " + out_str)
        f2.write(out_str)
    return 1
