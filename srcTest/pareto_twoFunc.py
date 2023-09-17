import pandas as pd

def geneData(file1):
    data = pd.read_csv(file1, sep='\t')
    return data.values.tolist()

def pareto_optimal(points):
    pareto_points = []
    is_pareto = [True] * len(points)
    for i in range(len(points)):
        # if is_pareto[i] == False:
            # continue

        err1 = points[i][4]
        cost1 = points[i][5]

        for j in range(len(points)):
            # if is_pareto[j] == False:
            #     continue
            if j == i:
                continue
            err2 = points[j][4]
            cost2 = points[j][5]
            if (err2 <= err1) and (cost2 <= cost1):
                is_pareto[i] = False
                break

    for i in range(len(points)):
        if is_pareto[i]:
            pareto_points.append(points[i])

    return pareto_points

def writeToFile(data, file_path, func1, func2):
    with open(file_path, 'w') as file:
        file.write(f"{func1}_regime1\tdegree1\t{func2}_regime2\tdegree2\terror\tperformance\n")
        for item in data:
            file.write(f"{int(item[0])}\t{int(item[1])}\t{int(item[2])}\t{int(item[3])}\t{item[4]:.7e}\t{int(item[5])}\n")
    print("output file is %s"%(str(file_path)))

# 默认读取1个文件：func1.csv，存储的是函数的误差-开销信息
if __name__ == '__main__':
    expr = "NMSEproblem345"
    inputFile = expr + "_errPerf.csv"
    outputFile = expr + "_errPerfPareto.csv"
    func1 = "sin"
    func2 = "tan"

    input_points = geneData(inputFile)
    pareto_result = pareto_optimal(input_points)

    # 输出符合帕累托最优化的点集
    print(f"\nLength of 'pareto_optimal points': {len(pareto_result)}")
    writeToFile(pareto_result, outputFile, func1, func2)