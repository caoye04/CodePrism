import os
import json
import re
from pathlib import Path

def extract_rule_ids(md_content):
    """从Markdown内容中提取关联规范的编号"""
    rule_ids = []
    
    # 查找"## 关联规范"部分
    pattern = r'## 关联规范\s*\n(.*?)(?=\n##|\Z)'
    match = re.search(pattern, md_content, re.DOTALL)
    
    if match:
        rules_section = match.group(1)
        # 提取所有形如 "D_04_05" 或 "R_05_03" 的编号
        id_pattern = r'-\s*([A-Z]_\d+_\d+):'
        ids = re.findall(id_pattern, rules_section)
        rule_ids.extend(ids)
    
    return rule_ids

def process_md_files(directory):
    """处理目录中的所有test_*.md文件"""
    result = {}
    
    # 按1-40的顺序处理
    for i in range(1, 41):
        file_name = f'test_{i}.md'
        md_file = Path(directory) / file_name
        
        if md_file.exists():
            try:
                # 读取文件内容
                with open(md_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                # 提取规范编号
                rule_ids = extract_rule_ids(content)
                
                # 使用 test1, test2 等作为key
                file_key = f'test{i}'
                result[file_key] = rule_ids
                
                print(f"处理 {file_name}: 找到 {len(rule_ids)} 个规范编号")
                
            except Exception as e:
                print(f"处理 {file_name} 时出错: {e}")
        else:
            print(f"文件 {file_name} 不存在，跳过")
    
    return result

def save_json_one_line_per_entry(data, output_file):
    """保存JSON，每个条目占一行"""
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write('{\n')
        items = list(data.items())
        for i, (key, value) in enumerate(items):
            # 将value转为紧凑的JSON字符串
            value_str = json.dumps(value, ensure_ascii=False)
            line = f'  "{key}": {value_str}'
            if i < len(items) - 1:
                line += ','
            f.write(line + '\n')
        f.write('}\n')

def main():
    # 设置目录路径
    directory = r'C:\Users\caoye04\Desktop\CodePrism\answer\answer_md'
    
    # 处理所有Markdown文件
    result = process_md_files(directory)
    
    # 保存为JSON文件
    output_file = 'rule_ids.json'
    save_json_one_line_per_entry(result, output_file)
    
    print(f"\n结果已保存到 {output_file}")
    print(f"总共处理了 {len(result)} 个文件")
    
    # 打印预览
    print("\n结果预览:")
    for key, values in list(result.items())[:5]:
        print(f'  "{key}": {json.dumps(values, ensure_ascii=False)}')

if __name__ == '__main__':
    main()