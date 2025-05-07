# 将当前文件夹中的名称为*.JPG文件和名称为*.NEF的文件分别放在名称为JPG的文件夹和名称为NEF的文件夹中，如果没有文件夹则创建

import os
import shutil

# 获取当前脚本所在的目录
current_dir = os.path.dirname(os.path.abspath(__file__))

# 定义目标文件夹名称
jpg_folder = os.path.join(current_dir, "JPG")
nef_folder = os.path.join(current_dir, "NEF")

# 如果目标文件夹不存在，则创建它们
os.makedirs(jpg_folder, exist_ok=True)
os.makedirs(nef_folder, exist_ok=True)

# 遍历当前目录中的所有文件
for filename in os.listdir(current_dir):
    # 构建文件的完整路径
    source_path = os.path.join(current_dir, filename)

    # 检查是否是文件以及文件扩展名
    if os.path.isfile(source_path):
        # 检查是否是 JPG 文件（不区分大小写）
        if filename.lower().endswith(".jpg"):
            destination_path = os.path.join(jpg_folder, filename)
            print(f"正在移动 {filename} 到 {jpg_folder}")
            shutil.move(source_path, destination_path)
        # 检查是否是 NEF 文件（不区分大小写）
        elif filename.lower().endswith(".nef"):
            destination_path = os.path.join(nef_folder, filename)
            print(f"正在移动 {filename} 到 {nef_folder}")
            shutil.move(source_path, destination_path)

print("文件整理完成！")