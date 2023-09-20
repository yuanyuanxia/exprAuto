import pandas as pd
import glob

# 查找所有带有 "Sorted.csv" 后缀的文件
csv_files = glob.glob('*Sorted.csv')

# 创建一个 Excel Writer 对象，用于写入 Excel 文件
excel_writer = pd.ExcelWriter('output.xlsx', engine='openpyxl')

# 定义空白行数和起始列数
num_blank_rows = 3
start_rows = num_blank_rows
start_col = 3


# 遍历 CSV 文件列表
for csv_file in csv_files:
    # 读取 CSV 文件
    df = pd.read_csv(csv_file, sep='\t')
    
    # 提取文件名作为工作表名称
    sheet_name = csv_file[:-24]  # 去除文件扩展名和后缀
    
    # 在左侧的一个单元格内写入文件名
    df.insert(0, sheet_name, '')

    # 将当前文件的数据写入 Excel 表格，追加到同一个工作表中，并添加空白行和起始列
    df.to_excel(excel_writer, sheet_name='Combined', startrow=start_rows, startcol=start_col, index=False, header=True)

    # 计算下一个文件的起始行数
    start_rows += df.shape[0] + 4  # 添加间隔行


# 遍历 CSV 文件列表
for csv_file in csv_files:
    # 读取 CSV 文件
    df = pd.read_csv(csv_file, sep='\t')
    
    # 提取文件名作为工作表名称
    sheet_name = csv_file[:-24]  # 去除文件扩展名和后缀
    
    # 将当前文件的数据写入 Excel 表格，每个文件写入一个新的工作表
    df.to_excel(excel_writer, sheet_name=sheet_name, startrow=num_blank_rows, startcol=start_col, index=False, header=True)


# 关闭 Excel Writer 对象
excel_writer.save()
