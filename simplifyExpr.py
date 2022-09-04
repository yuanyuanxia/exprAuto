from sympy import *

#对数据进行化简
def parse(t_str):
    expr = simplify(sympify(t_str))
    return expr

# 展开表达式,将x**n展开为x*x*...*x
def expand_expr(expr):
    result = ""

    # 将需要替换字符的索引加入列表
    i_lst = []
    for i in range(len(expr)):
        if expr[i] == "*" and expr[i + 1] == "*":
            i_lst.append(i)

    if i_lst:
        # dic的key为开始替换的位置,value为替换的内容
        dic = {}

        # 遍历字符的索引,获取要替换后的内容
        for i in i_lst:
            new = ""  # 存放替换的内容
            for j in range(int(expr[i + 2]) - 1):  # 判断要展开几次
                new += "*"
                new += expr[i - 1]
            dic[i + 1] = new

        dic_keys = list(dic.keys())

        result += expr[:(dic_keys[0] - 1)]

        for i in range(len(dic_keys)):
            # 如果是最后一个替换的内容,替换完成之后,直接加到末尾
            if i == len(dic_keys) - 1:
                result += dic[dic_keys[i]]
                result += expr[dic_keys[i] + 2:]
            else:
                # 先加替换的内容
                result += dic[dic_keys[i]]
                # 再加上两个替换内容之间夹着的内容
                result += expr[dic_keys[i] + 2:dic_keys[i + 1] - 1]
        return result
    return expr

# 将数据导出
def process(i_filename="pythonBefore.txt", o_filename="pythonAfter.txt"):
    """
    :param i_filename:  输入数据文件的绝对路径,默认路径为.\pythonBefore.txt
    :param o_filename:  输出数据文件的绝对路径,默认路径为.\pythonAfter.txt
    :return: None
    """
    with open(i_filename, "r", encoding="UTF-8") as f1, \
            open(o_filename, "w", encoding="UTF-8") as f2:
        input_str = f1.read().strip()
        out_str = str(parse(input_str))
        print(out_str)
        out_str = "".join(out_str.split(" "))
        out_str = expand_expr(out_str)
        f2.write(out_str)


# 生成测试数据
def gene_data(filename):
    with open(filename, "w", encoding="UTF-8") as f:
        f.write("(x**2-1)/(x-1)")
        #f.write("x**4+y**3")


if __name__ == "__main__":
    #gene_data("pythonBefore.txt")
    #process()

