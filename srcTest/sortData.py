import csv
import sys

def sort_csv_by_column(csv_file, output_file, sort_column, func1, func2):
    # 读取CSV文件数据
    data = []
    with open(csv_file, 'r') as file:
        lines = file.readlines()
        
        # 跳过第一行
        lines = lines[1:]

        for line in lines:
            row = line.strip().split('\t')
            data.append(row)

    # 按照指定列进行排序
    sorted_data = sorted(data, key=lambda row: float(row[sort_column]))

    # 将排序后的结果写入新文件
    with open(output_file, 'w', newline='') as file:
        csv_writer = csv.writer(file, delimiter = "\t")
        header = [f"{func1}_regime1", "degree1", f"{func2}_regime2", "degree2", "averageErr", "performance", ]
        csv_writer.writerow(header)
        csv_writer.writerows(sorted_data)
    print("Input file: %s"%(csv_file))
    print("Output file: %s"%(output_file))


def sort_csv_by_column_1(csv_file, output_file, sort_column, func1):
    # 读取CSV文件数据
    data = []
    with open(csv_file, 'r') as file:
        lines = file.readlines()
        
        # 跳过第一行
        lines = lines[1:]

        for line in lines:
            row = line.strip().split('\t')
            data.append(row)

    # 按照指定列进行排序
    sorted_data = sorted(data, key=lambda row: float(row[sort_column]))

    # 将排序后的结果写入新文件
    with open(output_file, 'w', newline='') as file:
        csv_writer = csv.writer(file, delimiter = "\t")
        header = [f"{func1}_regime", "degree", "averageErr", "performance", ]
        csv_writer.writerow(header)
        csv_writer.writerows(sorted_data)
    print("Input file: %s"%(csv_file))
    print("Output file: %s"%(output_file))


# 按照第 sort_column 列数据对 inputFile 文件进行排序，结果写入 outputFile 文件
if __name__ == '__main__':
    # 读取命令行参数, 并检查是否传递了参数
    arguments = sys.argv
    if len(arguments) > 4:
        print("please input only 2 or 3 paremeters or none paremeter!")
        sys.exit(1)
    elif len(arguments) == 4: # 表达式中有 2 个函数。
        uniqueLabel = arguments[1] # arguments[2] 为 NMSEexample34, 即优化表达式

        inputFile = uniqueLabel + "_errPerfPareto.csv"
        outputFile = uniqueLabel + "_errPerfParetoSorted.csv"
        sort_column = int(4) # 说明表达式中有2个函数，故第5列才是平均误差
        func1 = arguments[2]
        func2 = arguments[3]

        sort_csv_by_column(inputFile, outputFile, sort_column, func1, func2)
    elif len(arguments) == 3: # 表达式中只有 1 个函数。
        uniqueLabel = arguments[1] # arguments[2] 为 exp1x, 即优化表达式

        inputFile = uniqueLabel + "_errPerfPareto.csv"
        outputFile = uniqueLabel + "_errPerfParetoSorted.csv"
        sort_column = int(2) # 说明表达式中有1个函数，故第3列才是平均误差
        func1 = arguments[2]

        sort_csv_by_column_1(inputFile, outputFile, sort_column, func1)
    elif len(arguments) == 1: # 没有输入，对应的是之前的默认情况，即 NMSEproblem345
        uniqueLabel = "NMSEproblem345"

        inputFile = uniqueLabel + "_errPerfPareto.csv"
        outputFile = uniqueLabel + "_errPerfParetoSorted.csv"
        sort_column = int(4) # 说明表达式中有2个函数，故第5列才是平均误差
        func1 = "sin"
        func2 = "tan"

        sort_csv_by_column(inputFile, outputFile, sort_column, func1, func2)