import pandas as pd
import itertools

# 读取2个文件，对应表达式中的两个函数。每个文件有2列，对应函数的误差和性能。
def geneData():
    # 读取文件并将数据存储在 DataFrame 中
    func1 = pd.read_csv("func1.csv", delimiter='\t', dtype={0: str})
    func2 = pd.read_csv("func2.csv", delimiter='\t', dtype={0: str})

    # 获取第一列数据
    error1 = func1.iloc[:, 0].tolist()
    error2 = func2.iloc[:, 0].tolist()

    # 转换为浮点数
    error1 = [float(x) for x in error1]
    error2 = [float(x) for x in error2]

    # 获取第二列数据
    perf1 = func1.iloc[:, 1].tolist()
    perf2 = func2.iloc[:, 1].tolist()


    # 对error和perf两个数据列表进行两两组合求和，并存储在新的数组中
    result1 = []
    for pair in itertools.product(error1, error2):
        sum = pair[0] + pair[1]
        result1.append(sum)

    result2 = []
    for pair in itertools.product(perf1, perf2):
        sum = pair[0] + pair[1]
        result2.append(sum)

    result = []
    for error, perf in zip(result1, result2):
        pair = (error, perf)
        result.append(pair)

    # 按科学计数法输出结果
    # for num in result1:
    #     print(f"{num:.2e}", end=' ')
    # print(f"\nLength of 'result2' array: {len(result2)}")
    # for pair in result:
    #     print(f"error: {pair[0]:.2e}, perf: {pair[1]}")

    return result

def pareto_optimal(points):
    pareto_points = []

    for i in range(len(points)):
        is_pareto = True
        x1, y1 = points[i]

        for j in range(len(points)):
            x2, y2 = points[j]
            if x2 < x1 and y2 < y1:
                is_pareto = False
                break

        if is_pareto:
            pareto_points.append((x1, y1, i))

    return pareto_points

# 默认读取2个文件：func1.csv func2.csv，分别存储的是两个函数的误差-开销信息
if __name__ == '__main__':
    # input_points = [(1, 5), (2, 4), (1, 3), (2, 1), (3, 3), (4, 2), (5, 1)]
    input_points = geneData()

    pareto_result = pareto_optimal(input_points)

    # 输出符合帕累托最优化的点集
    print("符合帕累托最优化的点集：")
    for pair in pareto_result:
        print(f"No.{pair[2]}: error: {pair[0]:.2e}, perf: {pair[1]}")
    print(f"\nLength of 'pareto_optimal points': {len(pareto_result)}")
    for pair in pareto_result:
        print(f"{pair[0]:.2e} {pair[1]}")
    # for point in pareto_result:
    #     print(point)