import pandas as pd
import itertools
import math

condNumSin = 8.943225
condNumTan = 1.07E+00

# 读取2个文件，对应表达式中的两个函数。每个文件有2列，对应函数的误差和性能。
def geneData(file1, file2):
    # 读取文件并将数据存储在 DataFrame 中
    func1 = pd.read_csv(file1, sep='\t') # sin
    func2 = pd.read_csv(file2, sep='\t') # tan

    # regime
    regime1 = func1["regime"]
    regime2 = func2["regime"]

    # degree
    degree1 = func1["degree"]
    degree2 = func2["degree"]

    # 平均误差
    error1 = func1["averageErr"]
    error2 = func2["averageErr"]

    # 将平均误差的字符串数据转换为浮点数
    error1 = [float(x) for x in error1]
    error2 = [float(x) for x in error2]

    # 性能节拍
    perf1 = func1["performance"]
    perf2 = func2["performance"]

    # 两两组合
    result = []
    for i in range(len(error1)):
        for j in range(len(error2)):
            # errorSum = error1[i] + error2[j] # !!! 这里是关键，关键在于要不要从error变成epsilon。
            errorSum = error1[i] * condNumSin + error2[j] * condNumTan # !!! 这里是关键，关键在于要不要从error变成epsilon。
            perfSum = perf1[i] + perf2[j]
            pair = (regime1[i], degree1[i], degree1[j], degree2[j], errorSum, perfSum)
            result.append(pair)
    return result

def pareto_optimal(points):
    pareto_points = []
    is_pareto = [True] * len(points)
    for i in range(len(points)):
        # if is_pareto[i] == False:
            # continue

        regime1, degree1, regime2, degree2, err, cost = points[i]

        for j in range(len(points)):
            # if is_pareto[j] == False:
            #     continue
            if j == i:
                continue
            err2 = points[j][4]
            cost2 = points[j][5]
            if (err2 < err) and (cost2 <= cost):
                is_pareto[i] = False
                break

    for i in range(len(points)):
        if is_pareto[i]:
            pareto_points.append(points[i])

    return pareto_points

def writeToFile(data, file_path):
    # file_path = 'output.csv'

    with open(file_path, 'w') as file:
        file.write(f"regime1\tdegree1\tregime2\tdegree2\tepsilon\tcost\n")
        for item in data:
            # file.write(f"{item[2]} {item[0]:.6e} {item[1]}\n")
            file.write(f"{item[0]}\t{item[1]}\t{item[2]}\t{item[3]}\t{item[4]:.6e}\t{item[5]}\n")
    print("output file is %s"%(str(file_path)))

# 默认读取2个文件：func1.csv func2.csv，分别存储的是两个函数的误差-开销信息
if __name__ == '__main__':
    input_points = geneData("sinInput.csv", "tanInput.csv")

    pareto_result = pareto_optimal(input_points)
    print(f"\nLength of 'pareto_optimal points': {len(pareto_result)}")
    writeToFile(pareto_result, "NMSEproblem345ParetoOutput.csv")