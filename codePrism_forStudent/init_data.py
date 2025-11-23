# 创建data目录下的初始文件
import json
import os

# 创建data目录
os.makedirs("data", exist_ok=True)

# 创建空的chat_history.json
with open("data/chat_history.json", "w", encoding="utf-8") as f:
    json.dump([], f)

# 创建空的your_answer.json  
with open("data/your_answer.json", "w", encoding="utf-8") as f:
    json.dump({}, f)

print("数据文件创建完成！")