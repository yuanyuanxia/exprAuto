from sympy import *
from os import path as os_path

# Simplify expr
def parse(t_str):
    expr = sympify(t_str, locals=support_char())
    expr = simplify(expr)
    expr = expand(expr)
    expr = cancel(expr)
    return expr


def support_char():
    ns = {}
    for i in range(65, 91):
        ns[chr(i)] = Symbol(chr(i))
    # Returns a dictionary of extended characters for use by the locals function in simplif()
    return ns

# Get ')'
def get_next(expr, index):
    j = index + 1
    while j >= 0:
        if expr[j] == '(':
            break
        j -= 1
    return j


# Expand expr,x**n ----> x*x*...*x,x**a.b ----> pow(x,a.b)
def new_expand(expr):
    """
    index_lst:[]    存放要替换的幂次单项式第一个字符所在的位置
    nextop_dic:{}   存放幂次单项式下一个操作符所在的位置
    max_op:x        多项式中最后一个运算符所在的位置
    coefficient_dic:{}  存放幂次单项式的幂
    content_dic:{}  存放幂次单项式替换后的内容

    """
    result = ""

    # 将需要替换字符的索引加入列表
    index_lst = []
    for i in range(len(expr)):
        if expr[i] == '*' and expr[i + 1] == '*':
            index_lst.append(i - 1)

    if index_lst:
        operator_lst = ['+', '-', '*', '/']

        nextop_dic = {}
        # 获取幂次式下一个操作符所在的位置
        for k in index_lst:
            nextop_dic[k] = None
            # 如果系数带括号
            if expr[k + 3] == '(':
                right_bracket = 0
                # 先找到')'所在的位置
                for p in range(k + 4, len(expr)):
                    if expr[p] == ')':
                        right_bracket = p
                        break
                # 从')'往后开始找下一个运算符
                for q in range(right_bracket + 1, len(expr)):
                    if expr[q] in operator_lst:
                        nextop_dic[k] = q
                        break
            else:
                for i in range(k + 3, len(expr)):
                    if expr[i] in operator_lst:
                        nextop_dic[k] = i
                        break


        # 获取最后一个运算符所在的位置
        max_op = 0
        flag = 0
        while flag < len(expr):
            if expr[flag] in operator_lst:
                if expr[flag] == '*':
                    if expr[flag + 1] == '*':  # 如果是**,直接跳到**后的第一个数字
                        flag += 3
                        continue
                    else:
                        max_op = flag
                else:
                    max_op = flag
            flag += 1

        # 如果max_op为0只有幂次
        # 如果要替换的索引位置大于max_op,直接获取到末尾

        # print("max_op:", max_op)

        coefficient_dic = {}
        for index in index_lst:
            # max_op == 0说明只有一个幂次
            # index > max_op说明在最后

            # is_one用来判断表达式是否只是一个幂次表达式
            # 当max_op==0且index==0时就会冲突
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
            elif index > max_op:  # 如果大于最大运算符所在的位置,后边的都加上
                coefficient = expr[index + 3:]
                coefficient_dic[index] = coefficient
            else:
                # 如果幂次带括号
                if expr[index + 3] == '(':
                    for i in range(index + 3, len(expr)):
                        if expr[i] == ')':
                            coefficient = expr[index + 3:i + 1]
                            coefficient_dic[index] = coefficient
                            break
                else:
                    # 幂次不带括号
                    for i in range(index + 3, len(expr)):
                        if expr[i] in operator_lst:
                            coefficient = expr[index + 3:i]
                            coefficient_dic[index] = coefficient
                            break


        # 遍历字符的索引,获取要替换后的内容
        content_dic = {}
        for i in index_lst:
            new = ""  # 存放替换的内容
            if expr[i] == ')':
                j = get_next(expr, i)
                char = expr[j:i + 1]

            else:
                char = expr[i]

            if ('.' in coefficient_dic[i]) or ('(' in coefficient_dic[i]):  # 幂次为非整数
                new = "pow(" + char + "," + coefficient_dic[i] + ")"
            else:  # 幂次为整数
                new += char
                for j in range(int(coefficient_dic[i]) - 1):  # 判断要展开几次
                    new += "*"
                    new += char
            content_dic[i] = "(" + new + ")"

        # 进行替换,生成表达式
        dic_keys = list(content_dic.keys())

        # 如果第一个要替换的内容带括号
        if expr[dic_keys[0]] == ')':
            j = get_next(expr, dic_keys[0])
            result += expr[:j]
        else:
            result += expr[:dic_keys[0]]  # 先加上第一个要替换内容前面的内容

        for i in range(len(dic_keys)):
            if i == len(dic_keys) - 1:  # 如果是最后一个要替换的内容
                result += content_dic[dic_keys[i]]  # 先加上要替换的内容
                if nextop_dic:  # 如果有nextop_dic 说明是多项式
                    if nextop_dic[dic_keys[i]]:  # 如果该单项式不是多项式中的最后一个
                        result += expr[nextop_dic[dic_keys[i]]:]
            else:
                result += content_dic[dic_keys[i]]  # 先加上要替换的内容
                if expr[dic_keys[i + 1]] == ')':
                    j = get_next(expr, dic_keys[i + 1])
                    result += expr[nextop_dic[dic_keys[i]]:j]
                else:
                    result += expr[nextop_dic[dic_keys[i]]:dic_keys[i + 1]]  # 再加上原来两个幂次单项式之间夹的内容
        return result
    else:
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
