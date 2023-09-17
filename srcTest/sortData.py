import csv

def sort_csv_by_column(csv_file, sort_column, output_file, func1, func2):
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
        header = [f"{func1}_regime1", "degree1", f"{func2}_regime2", "degree2", "error", "performance", ]
        csv_writer.writerow(header)
        csv_writer.writerows(sorted_data)
    print("input file: %s"%(csv_file))
    print("output file: %s"%(output_file))

# 按照第 sort_column 列数据对 inputFile 文件进行排序，结果写入 outputFile 文件
if __name__ == '__main__':
    uniqueLabel = "NMSEproblem345"
    inputFile = uniqueLabel + "_errPerfPareto.csv"
    outputFile = uniqueLabel + "_errPerfParetoSorted.csv"
    sort_column = int(4) # 说明表达式中有2个函数，故第5列才是平均误差
    func1 = "sin"
    func2 = "tan"
    sort_csv_by_column(inputFile, sort_column, outputFile, func1, func2)