import pandas as pd
import sys

condNumSin = 8.943225
condNumTan = 1.07E+00

# 读取1个文件，对应表达式中的函数。每个文件有若干列，但只需要 regime 和 degree 这2列，对应函数的误差和性能。
def geneData(file1, regimeStr):
    # 读取文件并将数据存储在 DataFrame 中
    csvReader = pd.read_csv(file1, sep='\t') # sin

    # regime
    regime1 = csvReader[regimeStr]

    # degree
    degree1 = csvReader["degree"]

    # 平均误差
    error1 = csvReader["averageErr"]

    # 将平均误差的字符串数据转换为浮点数
    error1 = [float(x) for x in error1]

    # 性能节拍
    perf1 = csvReader["performance"]

    # 两两组合
    result = []
    for i in range(len(error1)):
            # errorSum = error1[i] + error2[j] # !!! 这里是关键，关键在于要不要从error变成epsilon。
            errorSum = error1[i]
            perfSum = perf1[i]
            pair = (regime1[i], degree1[i], errorSum, perfSum)
            result.append(pair)
    return result

def pareto_optimal(points):
    pareto_points = []
    is_pareto = [True] * len(points)
    for i in range(len(points)):
        # if is_pareto[i] == False:
            # continue

        err1 = points[i][2]
        cost1 = points[i][3]

        for j in range(len(points)):
            # if is_pareto[j] == False:
            #     continue
            if j == i:
                continue
            err2 = points[j][2]
            cost2 = points[j][3]
            if (err2 < err1) and (cost2 <= cost1):
                is_pareto[i] = False
                break

    for i in range(len(points)):
        if is_pareto[i]:
            pareto_points.append(points[i])

    return pareto_points

def writeToFile(data, file_path):
    # file_path = 'output.csv'

    with open(file_path, 'w') as file:
        file.write(f"regime\tdegree\taverageErr\tperformance\n")
        for item in data:
            file.write(f"{item[0]}\t{item[1]}\t{item[2]:.6e}\t{item[3]}\n")

# 默认读取1个文件：func1.csv，存储的是函数的误差-开销信息
if __name__ == '__main__':
    # 读取命令行参数
    arguments = sys.argv

    func = "tan"
    funcInputFile = func + "Input.csv"
    funcOutputFile = func + "Output.csv"
    regimeStr="regime"
    # 检查是否传递了参数
    if len(arguments) > 3:
        print("please input only one paremeter!")
        sys.exit(1)
    elif len(arguments) == 3: # 表达式中只有一个函数。用于实测性能误差后的对表达式的帕累托优化
        funcInputFile = arguments[1] + "_errPerf.csv" # arguments[1] 为 exp1x, 即优化表达式
        funcOutputFile = arguments[1] + "_errPerfPareto.csv"
        func = arguments[2] # arguments[1] 为 exp， 即优化函数
        regimeStr = func + "_regime"
    elif len(arguments) == 2: # 用于一开始针对单个函数的帕累托优化
        func = arguments[1] # arguments[1] 为 exp， 即优化函数
        funcInputFile = func + "Input.csv"
        funcOutputFile = func + "Output.csv"

    input_points = geneData(funcInputFile, regimeStr)

    pareto_result = pareto_optimal(input_points)

    # 输出符合帕累托最优化的点集
    print(f"\nLength of 'pareto_optimal points': {len(pareto_result)}")
    writeToFile(pareto_result, funcOutputFile)
    print("Input file is %s"%(str(funcInputFile)))
    print("Output file is %s"%(str(funcOutputFile)))