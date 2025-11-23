# CodePrism - 代码规范分析系统

[toc]

## 项目简介

CodePrism是一个基于AI的C语言代码规范分析系统，帮助开发者检查代码是否符合MISRA C编码规范。

## 功能特点

- 🚀 智能代码分析：基于大语言模型的深度代码分析
- 📝 可视化展示：美观的代码高亮和违规标注
- 🎨 个性化主题：5种预设配色方案 + 自定义配色
- 💬 上下文记忆：AI记住历史对话内容
- 🔍 精准定位：点击违规项直接跳转到对应代码行

## 快速开始

### 1. 安装依赖

```bash
pip install -r requirements.txt
```

### 2. 运行服务器

```python
python app.py
```

### 3. 访问网页

打开浏览器访问：<http://localhost:5000>

## 网站使用说明

1. 上传代码：点击或拖拽C文件到左侧代码区
2. 分析代码：点击"帮我分析这个代码违反了哪些规则"按钮
3. 查看结果

## 个性化定制方案

### 1. 更换配色方案

在static/css/style.css文件中，找到配色方案部分，取消注释想要使用的方案：

```css
/* 方案1：深色科技风（默认） */
/* 方案2：浅色清新风 */
/* 方案3：紫色梦幻风 */
/* 方案4：绿色护眼风 */
/* 方案5：橙色活力风 */
/* 方案6：自定义配色 */
```

### 2. 更换logo

在templates/index.html中选择显示Logo图片或文字：

```html
<!-- 选项1：显示Logo图片 -->
<img src="{{ url_for('static', filename='logo.png') }}" alt="CodePrism">

<!-- 选项2：显示文字 -->
<!-- <div class="background-text">CodePrism</div> -->
```

## 高级功能

### 1. 实现RAG（检索增强生成）

在`rag_service.py`中实现：

1. 构建规范知识库
2. 实现相似度检索
3. 增强AI分析能力

详见文件中的TODO注释和实现思路。

### 2. 实现Agent

参考`readme_how_to_make_agent.txt`文档，实现：

1. ReAct模式Agent
2. LangChain Agent
3. 多Agent协作系统

## 项目结构

```axapta
forStudent/
├── app.py                      # Flask后端
├── ai_service.py               # AI调用服务
├── rag_service.py              # RAG服务框架
├── templates/
│   └── index.html              # 前端页面
├── static/
│   ├── css/style.css           # 样式文件
│   ├── js/main.js              # 前端逻辑
│   └── logo.png                # Logo图片
├── code_specifications/        # 规范文档
├── data/                       # 数据存储
├── knowledge_base/             # 知识库
└── test_code/                  # 测试代码
```

## 技术栈

- **后端**：Flask + Python
- **前端**：HTML + CSS + JavaScript
- **AI**：OpenAI API (Qwen3模型)
- **代码高亮**：Highlight.js
- **Markdown渲染**：Marked.js

## 开发建议

1. **优化Prompt**

在`ai_service.py`的`get_system_prompt()`方法中添加：

- 思维链提示
- Few-shot示例
- 规范详细说明

2. **实现RAG**

在`rag_service.py`中实现知识库检索，提高分析准确性。

3. **添加更多功能**

- 批量分析多个文件
- 导出分析报告
- 代码修复建议
- 实时分析（边写边检查）

## 常见问题

**Q: AI分析速度慢怎么办？**
 A: 可以调整`max_tokens`参数，或实现缓存机制。

**Q: 如何添加新的编码规范？**
 A: 修改`code_specifications/code_specifications_list.json`文件。

**Q: 如何切换AI模型？**
 A: 修改`ai_service.py`中的API配置。

