# CodePrism

> 曹烨

## 背景

我是一名“代码规范与代码分析课程”的助教，正在设计大作业的框架与内容。我的学生群体是一群大一新生小朋友，代码基础薄弱，我希望我的作业能让他们体会到AI与代码规范的巧妙融合，以及能深入理解一些ai技术，但又不要求他们从0开始实现ai技术。所以我会搭建一个基础框架，来让他们更顺利的完成我们的大作业。

大作业的相关需求如下，我正在实现”一套基础的系统实现框架“。

它包含基本的ai调用、网站搭建、个性化设计、最基础prompt实现、基础的rag实现等。

网站搭建是由用基本的python后端与html前端逻辑实现。

这里主要聊前端的需求与实现，网站大致分为三个模块：“代码展示区域”、“结果区”、“ai回复展示区”，但不需要在表面展示其模块名称

具体分布情况如下：”代码展示区“在左侧占据50%的区域，”结果区“在右上角，高度占20%，右下角的80%高度为

具体功能如下：

1. 代码展示区：普通情况下为“点击上传or拖拽要分析的c文件”的文字，可以点击这个区域来选择上传文件，也可以把对应的c文件拖拽到此。此后这里会展示要分析的代码，默认为c，需要做一定的美化与代码高亮与自动换行等。需要考虑如果代码过长需要有滑动能力。但是尽量简洁，不需要有标题一说

2. ai回复展示区：最下面有个启动按钮”帮我分析这个代码违反了哪些规则“，点击后，如果有左侧有代码，将ai回复的语句展示在这个区域，以对话形式展现。值得注意的是，需要ai回复美观可读，比如代码块、markdown显示等等；如果已经有回复了，直接开一个新的对话，把之前回复去掉即可；如果左侧没有代码，则提示请先放置要分析的代码

   ai回复需要在系统提示代码里注明格式需求。即要求回复包含json：”{{line： ；违反规则： ；一句话分析： }；{……}}“

   ai回复展示区需要在中间放置一个透明度50%的logo字，默认是同目录下的logo.png；也可以把它改为直接展示文字，填写内容；在代码中做成可选方案，无论是展示logo还是展示文字

3. 结果区：获取最新回答中的json，将里面的内容整理成逐行的按钮，按钮内容是：n ”rule变化“，然后放在按钮上可以看到分析里的详细内容；点击按钮，左侧代码展示区会将第n行放到移到尽量页面中间位置且高亮展示。

具体代码如下：

## 项目结构

```
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

## 代码细节

### app.py

```py
from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import json
import os
from datetime import datetime
from ai_service import AIService
from rag_service import RAGService

app = Flask(__name__)
CORS(app)

# 初始化服务
ai_service = AIService()
rag_service = RAGService()

# 数据文件路径
CHAT_HISTORY_FILE = "data/chat_history.json"
ANSWER_FILE = "data/your_answer.json"

# 确保数据目录存在
os.makedirs("data", exist_ok=True)


def load_chat_history():
    """加载聊天历史"""
    if os.path.exists(CHAT_HISTORY_FILE):
        with open(CHAT_HISTORY_FILE, 'r', encoding='utf-8') as f:
            return json.load(f)
    return []


def save_chat_history(history):
    """保存聊天历史"""
    with open(CHAT_HISTORY_FILE, 'w', encoding='utf-8') as f:
        json.dump(history, f, ensure_ascii=False, indent=2)


def save_answer(answer_data):
    """保存分析结果"""
    with open(ANSWER_FILE, 'w', encoding='utf-8') as f:
        json.dump(answer_data, f, ensure_ascii=False, indent=2)


@app.route('/')
def index():
    """渲染主页"""
    return render_template('index.html')


@app.route('/api/analyze', methods=['POST'])
def analyze_code():
    """分析代码接口"""
    try:
        data = request.json
        code = data.get('code', '')
        custom_rules = data.get('custom_rules', '')
        
        if not code.strip():
            return jsonify({
                'success': False,
                'error': '请先上传要分析的代码'
            })
        
        # 加载聊天历史
        chat_history = load_chat_history()
        
        # 调用AI分析（直接传原始代码）
        response = ai_service.analyze_code(code, chat_history, custom_rules)
        
        # 更新聊天历史
        chat_history.append({
            "role": "user",
            "content": f"请分析以下代码：\n{code}",
            "timestamp": datetime.now().isoformat()
        })
        chat_history.append({
            "role": "assistant",
            "content": response,
            "timestamp": datetime.now().isoformat()
        })
        
        save_chat_history(chat_history)
        
        # 提取违规信息
        violations = ai_service.extract_violations(response)
        
        # 保存分析结果
        save_answer({
            "code": code,
            "response": response,
            "violations": violations,
            "timestamp": datetime.now().isoformat()
        })
        
        return jsonify({
            'success': True,
            'response': response,
            'violations': violations
        })
        
    except Exception as e:
        return jsonify({
            'success': False,
            'error': str(e)
        })

@app.route('/api/reset', methods=['POST'])
def reset_conversation():
    """重置对话"""
    try:
        save_chat_history([])
        return jsonify({'success': True})
    except Exception as e:
        return jsonify({
            'success': False,
            'error': str(e)
        })


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
```

### ai_service.py

~~~py
import requests
import json
import re

# AI API配置
API_KEY = "sk-tT9Ddv4cOCl5BXW4kivhRQ"
BASE_URL = "https://llmapi.paratera.com/v1"
AI_APIS = {
    "qwen3_235b": {
        "base_url": BASE_URL,
        "api_key": API_KEY,
        "model": "Qwen3-235B-A22B-Instruct-2507"
    }
}


class AIService:
    def __init__(self):
        """初始化AI服务"""
        config = AI_APIS["qwen3_235b"]
        self.api_key = config["api_key"]
        self.base_url = config["base_url"]
        self.model = config["model"]
    
    def parse_code_lines(self, code):
        """
        解析代码，提取每一行及其行号
        
        返回：列表，每个元素是 {"line_number": X, "code": "..."}
        """
        lines = code.split('\n')
        parsed_lines = []
        
        for i, line in enumerate(lines, 1):
            # 跳过空行和纯注释行
            stripped = line.strip()
            if not stripped:
                continue
            if stripped.startswith('//'):
                continue
            if stripped.startswith('/*') or stripped.startswith('*'):
                continue
            
            parsed_lines.append({
                "line_number": i,
                "code": line
            })
        
        return parsed_lines
        
    def get_system_prompt(self):
        """获取系统提示词"""
        
        system_prompt = """你是一个专业的C语言代码规范检查助手。

            **你的任务：**
            用户会给你一个列表，列表中每个条目包含：
            - line_number: 行号（已经标注好）
            - code: 该行的代码内容

            你只需要判断每一行是否违反规范，如果违反，说明违反了什么规则。

            **回答格式（必须严格遵守）：**
            ```json
            [
                {
                    "line": 直接使用提供的line_number,
                    "rule": "规则编号",
                    "analysis": "说明违反了什么规则，为什么违反"
                }
            ]
            ```
            示例：
            输入：
            ```[
                {"line_number": 6, "code": "    long base = 60l;"},
                {"line_number": 7, "code": "    for (int i = 0; i < 6; ++i)"},
                {"line_number": 9, "code": "        base += 070;"}
            ]```
            输出：
            ```
            [
                {
                    "line": 6,
                    "rule": "R_07_03",
                    "analysis": "使用小写字母 'l' 作为 long 后缀，易与数字1混淆，应使用大写 'L'"
                },
                {
                    "line": 9,
                    "rule": "R_07_01",
                    "analysis": "使用八进制常量 070，违反禁止使用八进制常量的规则，应改用十进制56或十六进制0x38"
                }
            ]
            ```
            注意：

            直接使用提供的 line_number，不要自己计算

            如果某行没有违规，不要包含在结果中

            如果所有行都没有违规，返回空数组 []

            JSON必须格式正确"""
        return system_prompt

    def analyze_code(self, code, chat_history=None, custom_rules=''):
        """
        分析代码
        参数：
            code: 要分析的C代码（原始代码，不带行号）
            chat_history: 聊天历史
            custom_rules: 自定义规则
            
        返回：
            AI的分析结果
        """
        if chat_history is None:
            chat_history = []
        
        # 解析代码行
        parsed_lines = self.parse_code_lines(code)
        
        # 构建消息列表
        messages = [
            {"role": "system", "content": self.get_system_prompt()}
        ]
        
        # 添加历史对话
        recent_history = chat_history[-10:] if len(chat_history) > 10 else chat_history
        for msg in recent_history:
            if msg["role"] in ["user", "assistant"]:
                messages.append({
                    "role": msg["role"],
                    "content": msg["content"]
                })
        
        # 构建用户消息
        user_message = f"""请分析以下代码行是否违反编码规范：
        {json.dumps(parsed_lines, ensure_ascii=False, indent=2)}
        每一行的行号已经标注在 line_number 字段中，你只需要判断哪些行违反了规范。"""

        if custom_rules:
            user_message += f"\n\n特别关注以下规则：{custom_rules}"
        
        messages.append({"role": "user", "content": user_message})
        
        # 调用AI
        try:
            headers = {
                "Content-Type": "application/json",
                "Authorization": f"Bearer {self.api_key}"
            }
            
            data = {
                "model": self.model,
                "messages": messages,
                "temperature": 0.3,
                "max_tokens": 2000
            }
            
            response = requests.post(
                f"{self.base_url}/chat/completions",
                headers=headers,
                json=data,
                timeout=60
            )
            
            response.raise_for_status()
            result = response.json()
            
            return result["choices"][0]["message"]["content"]
            
        except requests.exceptions.RequestException as e:
            return f"AI调用失败：{str(e)}"
        except Exception as e:
            return f"处理响应失败：{str(e)}"

    def extract_violations(self, response):
        """
        从AI回复中提取违规信息
        
        参数：
            response: AI的完整回复
            
        返回：
            违规信息列表
        """
        try:
            # 提取JSON代码块
            json_match = re.search(r'```json\s*([\s\S]*?)\s*```', response)
            if json_match:
                json_str = json_match.group(1)
                violations = json.loads(json_str)
                # 确保line是整数
                for v in violations:
                    if 'line' in v:
                        v['line'] = int(v['line'])
                return violations
            
            # 提取普通JSON数组
            json_match = re.search(r'\[[\s\S]*?\]', response)
            if json_match:
                json_str = json_match.group(0)
                violations = json.loads(json_str)
                # 确保line是整数
                for v in violations:
                    if 'line' in v:
                        v['line'] = int(v['line'])
                return violations
            
            return []
            
        except Exception as e:
            print(f"JSON提取失败：{e}")
            return []

~~~

### templates/index.html

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CodePrism - 代码规范分析</title>
    <link rel="stylesheet" href="{{ url_for('static', filename='css/style.css') }}">
    <script src="https://cdn.jsdelivr.net/npm/marked/marked.min.js"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/github-dark.min.css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
</head>
<body>
    <div class="container">
        <!-- 左侧：代码展示区 -->
        <div class="code-panel">
            <div id="upload-area" class="upload-area">
                <p>点击上传 or 拖拽要分析的C文件</p>
                <input type="file" id="file-input" accept=".c,.h" style="display: none;">
            </div>
            <div id="code-display" class="code-display" style="display: none;">
                <!-- 删除按钮 -->
                <button id="clear-code-btn" class="clear-code-btn" title="删除代码">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                        <line x1="18" y1="6" x2="6" y2="18"></line>
                        <line x1="6" y1="6" x2="18" y2="18"></line>
                    </svg>
                </button>
                <!-- 代码容器 -->
                <div class="code-container">
                    <pre><code id="code-content" class="language-c"></code></pre>
                </div>
            </div>
        </div>

        <!-- 右侧 -->
        <div class="right-panel">
            <!-- 右上：结果区 -->
            <div class="result-panel">
                <div id="result-content" class="result-content"></div>
            </div>

            <!-- 右下：AI回复展示区 -->
            <div class="chat-panel">
                <div class="background-overlay">
                    <!-- <img src="{{ url_for('static', filename='logo.png') }}" alt="CodePrism" class="background-logo"> -->
                    <div class="background-text">代码规范分析大师</div>
                </div>

                <div id="chat-content" class="chat-content"></div>

                <div class="chat-input">
                    <input 
                        type="text" 
                        id="rule-input" 
                        class="rule-input" 
                        placeholder="输入要分析的规则（可选，如：R_01_01, D_04_01）"
                    >
                    <button id="analyze-btn" class="analyze-btn">帮我分析这个代码违反了哪些规则</button>
                </div>
            </div>
        </div>
    </div>

    <!-- 全局悬浮提示框 -->
    <div id="tooltip" class="custom-tooltip"></div>

    <script src="{{ url_for('static', filename='js/main.js') }}"></script>
</body>
</html>
```

### static/css/style.css

```
/* 
==============================================
个性化配色方案
==============================================
取消注释对应的配色方案来应用不同的风格
默认使用方案1：深色科技风
*/

/* ==================== 方案1：深色科技风（默认） ==================== */
:root {
    --bg-primary: #1a1a2e;
    --bg-secondary: #16213e;
    --bg-tertiary: #0f3460;
    --text-primary: #e0e0e0;
    --text-secondary: #a0a0a0;
    --accent-color: #00d4ff;
    --accent-hover: #00b8e6;
    --border-color: #2a2a4e;
    --code-bg: #0d1117;
    --button-gradient: linear-gradient(135deg, #00d4ff 0%, #0080ff 100%);
    --shadow-color: rgba(0, 212, 255, 0.2);
    --highlight-bg: rgba(255, 193, 7, 0.15);
    --highlight-border: #00d4ff;
}

/* ==================== 方案2：浅色清新风 ==================== */
/* 
:root {
    --bg-primary: #f5f7fa;
    --bg-secondary: #ffffff;
    --bg-tertiary: #e8ecf1;
    --text-primary: #2c3e50;
    --text-secondary: #7f8c8d;
    --accent-color: #3498db;
    --accent-hover: #2980b9;
    --border-color: #dce1e6;
    --code-bg: #f8f9fa;
    --button-gradient: linear-gradient(135deg, #3498db 0%, #2ecc71 100%);
    --shadow-color: rgba(52, 152, 219, 0.2);
} */


/* ==================== 方案3：紫色梦幻风 ==================== */
/* 
:root {
    --bg-primary: #1e1e2f;
    --bg-secondary: #27293d;
    --bg-tertiary: #2e3047;
    --text-primary: #e6e6fa;
    --text-secondary: #b8b8d1;
    --accent-color: #9d4edd;
    --accent-hover: #7b2cbf;
    --border-color: #3c3f58;
    --code-bg: #1a1a2a;
    --button-gradient: linear-gradient(135deg, #9d4edd 0%, #c77dff 100%);
    --shadow-color: rgba(157, 78, 221, 0.3);
} */


/* ==================== 方案4：绿色护眼风 ==================== */
/* 
:root {
    --bg-primary: #1a2b1a;
    --bg-secondary: #243324;
    --bg-tertiary: #2d4a2d;
    --text-primary: #d4e4d4;
    --text-secondary: #a8c8a8;
    --accent-color: #4ade80;
    --accent-hover: #22c55e;
    --border-color: #345234;
    --code-bg: #162116;
    --button-gradient: linear-gradient(135deg, #4ade80 0%, #22c55e 100%);
    --shadow-color: rgba(74, 222, 128, 0.2);
} */


/* ==================== 方案5：橙色活力风 ==================== */
/* 
:root {
    --bg-primary: #2b1f1a;
    --bg-secondary: #3d2a21;
    --bg-tertiary: #4a3428;
    --text-primary: #f0e6d2;
    --text-secondary: #d4c5a9;
    --accent-color: #ff6b35;
    --accent-hover: #ff5722;
    --border-color: #5a4538;
    --code-bg: #1f1612;
    --button-gradient: linear-gradient(135deg, #ff6b35 0%, #ffa500 100%);
    --shadow-color: rgba(255, 107, 53, 0.3);
} */


/* ==================== 方案6：自定义配色 ==================== */
/*
:root {
    --bg-primary: #你的颜色;
    --bg-secondary: #你的颜色;
    --bg-tertiary: #你的颜色;
    --text-primary: #你的颜色;
    --text-secondary: #你的颜色;
    --accent-color: #你的颜色;
    --accent-hover: #你的颜色;
    --border-color: #你的颜色;
    --code-bg: #你的颜色;
    --button-gradient: linear-gradient(135deg, #你的颜色1 0%, #你的颜色2 100%);
    --shadow-color: rgba(你的RGB, 0.3);
}
*/


/* ==================== 基础样式 ==================== */
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    background-color: var(--bg-primary);
    color: var(--text-primary);
    height: 100vh;
    overflow: hidden;
}

.container {
    display: flex;
    height: 100vh;
    gap: 10px;
    padding: 10px;
}

/* ==================== 左侧代码面板 ==================== */
.code-panel {
    flex: 0 0 50%;
    min-width: 0; /* 防止flex item撑开容器 */
    background-color: var(--bg-secondary);
    border-radius: 12px;
    border: 1px solid var(--border-color);
    overflow: hidden;
    display: flex;
    flex-direction: column;
    position: relative;
}

.upload-area {
    width: 100%;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.3s ease;
    border: 2px dashed var(--border-color);
    border-radius: 8px;
    margin: 20px;
}

.upload-area:hover {
    border-color: var(--accent-color);
    background-color: var(--bg-tertiary);
}

.upload-area.drag-over {
    border-color: var(--accent-color);
    background-color: var(--bg-tertiary);
    box-shadow: 0 0 20px var(--shadow-color);
}

.upload-area p {
    font-size: 1.2rem;
    color: var(--text-secondary);
}

.code-display {
    width: 100%;
    height: 100%;
    overflow: auto;
    position: relative;
}

/* 删除按钮 */
.clear-code-btn {
    position: absolute;
    top: 20px;
    right: 20px;
    z-index: 10;
    background-color: rgba(255, 59, 48, 0.8);
    border: none;
    border-radius: 6px;
    width: 36px;
    height: 36px;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.3s ease;
    color: white;
}

.clear-code-btn:hover {
    background-color: rgba(255, 59, 48, 1);
    transform: scale(1.1);
    box-shadow: 0 4px 12px rgba(255, 59, 48, 0.4);
}

.clear-code-btn:active {
    transform: scale(0.95);
}

/* 代码容器 - 使用 CSS 计数器方案 */
.code-container {
    background-color: var(--code-bg);
    border-radius: 8px;
    margin: 20px;
    overflow: hidden;
}

.code-display pre {
    margin: 0;
    padding: 0;
    counter-reset: line-numbering;
    overflow-x: auto;
}

.code-display code {
    font-family: 'Consolas', 'Monaco', 'Courier New', monospace;
    font-size: 14px;
    line-height: 1.6;
    display: block;
    padding: 15px 15px 15px 60px;
    position: relative;
}

/* 代码行包装 */
.code-display code .code-line {
    display: block;
    counter-increment: line-numbering;
    position: relative;
    min-height: 1.6em;
}

/* 使用 CSS 计数器生成行号 */
.code-display code .code-line::before {
    content: counter(line-numbering);
    position: absolute;
    left: -60px;
    width: 40px;
    text-align: right;
    color: var(--text-secondary);
    user-select: none;
    padding-right: 15px;
    background-color: rgba(0, 0, 0, 0.2);
    border-right: 1px solid var(--border-color);
}

/* 高亮行的行号 */
.code-display code .code-line.highlight-line::before {
    color: var(--accent-color);
    font-weight: bold;
    background-color: rgba(0, 212, 255, 0.1);
}

/* 代码高亮行 */
.code-display code .code-line.highlight-line {
    background-color: var(--highlight-bg);
    border-left: 3px solid var(--highlight-border);
    margin-left: -3px;
}

/* ==================== 右侧面板 ==================== */
.right-panel {
    flex: 0 0 calc(50% - 10px);
    min-width: 0; /* 防止flex item撑开容器 */
    display: flex;
    flex-direction: column;
    gap: 10px;
}

/* ==================== 结果面板 ==================== */
.result-panel {
    flex: 0 0 20%;
    min-height: 0;
    background-color: var(--bg-secondary);
    border-radius: 12px;
    border: 1px solid var(--border-color);
    overflow: hidden;
}

.result-content {
    height: 100%;
    overflow-y: auto;
    padding: 15px;
    display: flex;
    flex-direction: column;
    gap: 8px;
}

.violation-btn {
    background: var(--bg-tertiary);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    padding: 10px 15px;
    color: var(--text-primary);
    cursor: pointer;
    transition: all 0.3s ease;
    text-align: left;
    font-size: 0.9rem;
    position: relative;
    flex-shrink: 0; /* 防止按钮被压缩 */
}

.violation-btn:hover {
    background: var(--accent-color);
    border-color: var(--accent-color);
    transform: translateX(5px);
    box-shadow: 0 4px 12px var(--shadow-color);
}

.violation-btn .line-number {
    font-weight: bold;
    color: var(--accent-color);
    margin-right: 8px;
}

.violation-btn:hover .line-number {
    color: white;
}

.violation-btn .rule-id {
    font-size: 0.85rem;
    color: var(--text-secondary);
}

/* 自定义悬浮提示框 */
.custom-tooltip {
    position: fixed;
    background-color: var(--bg-primary);
    color: var(--text-primary);
    padding: 10px 15px;
    border-radius: 8px;
    font-size: 0.9rem;
    max-width: 350px;
    word-wrap: break-word;
    border: 1px solid var(--border-color);
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.5);
    z-index: 10000;
    pointer-events: none;
    opacity: 0;
    transition: opacity 0.2s ease;
    line-height: 1.4;
}

.custom-tooltip.show {
    opacity: 1;
}

/* ==================== 聊天面板 ==================== */
.chat-panel {
    flex: 1;
    min-height: 0; /* 防止flex item撑开容器 */
    background-color: var(--bg-secondary);
    border-radius: 12px;
    border: 1px solid var(--border-color);
    display: flex;
    flex-direction: column;
    overflow: hidden;
    position: relative;
}

.background-overlay {
    position: absolute;
    top: -50px;
    left: 0;
    width: 100%;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    pointer-events: none;
    z-index: 0;
}

/* Logo图片样式 */
.background-logo {
    max-width: 40%;
    max-height: 40%;
    opacity: 0.5;
    object-fit: contain;
}

/* 文字Logo样式 */
.background-text {
    font-size: 4rem;
    font-weight: bold;
    color: var(--text-secondary);
    opacity: 0.5;
    user-select: none;
}

.chat-content {
    flex: 1;
    min-height: 0; /* 允许滚动 */
    overflow-y: auto;
    overflow-x: hidden; /* 防止水平滚动 */
    padding: 20px;
    position: relative;
    z-index: 1;
}

.chat-message {
    margin-bottom: 20px;
    animation: fadeIn 0.5s ease;
    max-width: 100%; /* 确保不超出容器 */
}

@keyframes fadeIn {
    from {
        opacity: 0;
        transform: translateY(10px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

.chat-message .message-content {
    background-color: var(--bg-tertiary);
    border-radius: 12px;
    padding: 15px 20px;
    border-left: 3px solid var(--accent-color);
    line-height: 1.6;
    overflow-wrap: break-word; /* 强制长单词换行 */
    word-wrap: break-word; /* 兼容性 */
    word-break: break-word; /* 确保换行 */
    max-width: 100%; /* 不超出容器 */
}

.chat-message .message-content h1,
.chat-message .message-content h2,
.chat-message .message-content h3 {
    margin-top: 15px;
    margin-bottom: 10px;
    color: var(--accent-color);
}

.chat-message .message-content p {
    margin-bottom: 10px;
}

.chat-message .message-content code {
    background-color: var(--code-bg);
    padding: 2px 6px;
    border-radius: 4px;
    font-family: 'Consolas', 'Monaco', 'Courier New', monospace;
    font-size: 0.9em;
    word-break: break-all; /* 代码可以断行 */
}

.chat-message .message-content pre {
    background-color: var(--code-bg);
    border-radius: 8px;
    padding: 15px;
    overflow-x: auto;
    margin: 10px 0;
    max-width: 100%; /* 不超出容器 */
}

.chat-message .message-content pre code {
    background: none;
    padding: 0;
    /* 修改：允许代码块内容换行 */
    white-space: pre-wrap; /* 保持格式的同时允许换行 */
    word-break: break-word; /* 长单词可以断行 */
    overflow-wrap: break-word; /* 强制换行 */
}

/* 确保表格也能正确显示 */
.chat-message .message-content table {
    width: 100%;
    max-width: 100%;
    overflow-x: auto;
    display: block;
}

.chat-input {
    padding: 15px 20px;
    border-top: 1px solid var(--border-color);
    background-color: var(--bg-tertiary);
    display: flex;
    flex-direction: column;
    gap: 10px;
    flex-shrink: 0; /* 防止输入区域被压缩 */
}

/* 规则输入框 */
.rule-input {
    width: 100%;
    padding: 12px 15px;
    background-color: var(--bg-secondary);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    color: var(--text-primary);
    font-size: 0.95rem;
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    transition: all 0.3s ease;
}

.rule-input:focus {
    outline: none;
    border-color: var(--accent-color);
    box-shadow: 0 0 0 3px var(--shadow-color);
}

.rule-input::placeholder {
    color: var(--text-secondary);
}

.analyze-btn {
    width: 100%;
    padding: 15px;
    background: var(--button-gradient);
    color: white;
    border: none;
    border-radius: 8px;
    font-size: 1rem;
    font-weight: 600;
    cursor: pointer;
    transition: all 0.3s ease;
    box-shadow: 0 4px 12px var(--shadow-color);
}

.analyze-btn:hover {
    transform: translateY(-2px);
    box-shadow: 0 6px 20px var(--shadow-color);
}

.analyze-btn:active {
    transform: translateY(0);
}

.analyze-btn:disabled {
    opacity: 0.6;
    cursor: not-allowed;
}

/* ==================== 滚动条样式 ==================== */
::-webkit-scrollbar {
    width: 8px;
    height: 8px;
}

::-webkit-scrollbar-track {
    background: var(--bg-tertiary);
    border-radius: 4px;
}

::-webkit-scrollbar-thumb {
    background: var(--border-color);
    border-radius: 4px;
}

::-webkit-scrollbar-thumb:hover {
    background: var(--accent-color);
}

/* ==================== 响应式设计 ==================== */
@media (max-width: 1024px) {
    .container {
        flex-direction: column;
    }
    
    .code-panel,
    .right-panel {
        flex: 1;
        min-height: 0;
    }
}

@media (max-width: 768px) {
    .background-logo {
        max-width: 60%;
        max-height: 60%;
    }
    
    .background-text {
        font-size: 2.5rem;
    }
    
    .custom-tooltip {
        max-width: 250px;
        font-size: 0.85rem;
    }
}
```

### static/js/main.js

```js
// 全局状态
let currentCode = '';
let currentViolations = [];
let currentHighlightLine = null;

// DOM元素
const uploadArea = document.getElementById('upload-area');
const fileInput = document.getElementById('file-input');
const codeDisplay = document.getElementById('code-display');
const codeContent = document.getElementById('code-content');
const chatContent = document.getElementById('chat-content');
const resultContent = document.getElementById('result-content');
const analyzeBtn = document.getElementById('analyze-btn');
const clearCodeBtn = document.getElementById('clear-code-btn');
const ruleInput = document.getElementById('rule-input');
const tooltip = document.getElementById('tooltip');

// ==================== 悬浮提示框功能 ====================

function showTooltip(text, event) {
    tooltip.textContent = text;
    tooltip.classList.add('show');
    updateTooltipPosition(event);
}

function hideTooltip() {
    tooltip.classList.remove('show');
}

function updateTooltipPosition(event) {
    const offset = 15; // 距离鼠标的偏移量
    const tooltipRect = tooltip.getBoundingClientRect();
    const viewportWidth = window.innerWidth;
    const viewportHeight = window.innerHeight;
    
    // 默认位置：鼠标右下角
    let left = event.clientX + offset;
    let top = event.clientY + offset;
    
    // 检查右边界，如果超出则显示在左侧
    if (left + tooltipRect.width > viewportWidth - 10) {
        left = event.clientX - tooltipRect.width - offset;
    }
    
    // 检查下边界，如果超出则显示在上方
    if (top + tooltipRect.height > viewportHeight - 10) {
        top = event.clientY - tooltipRect.height - offset;
    }
    
    // 确保不超出左边界和上边界
    left = Math.max(10, left);
    top = Math.max(10, top);
    
    tooltip.style.left = left + 'px';
    tooltip.style.top = top + 'px';
}

// ==================== 文件上传功能 ====================

uploadArea.addEventListener('click', () => {
    fileInput.click();
});

fileInput.addEventListener('change', (e) => {
    const file = e.target.files[0];
    if (file) {
        loadFile(file);
    }
});

uploadArea.addEventListener('dragover', (e) => {
    e.preventDefault();
    uploadArea.classList.add('drag-over');
});

uploadArea.addEventListener('dragleave', () => {
    uploadArea.classList.remove('drag-over');
});

uploadArea.addEventListener('drop', (e) => {
    e.preventDefault();
    uploadArea.classList.remove('drag-over');
    
    const file = e.dataTransfer.files[0];
    if (file && (file.name.endsWith('.c') || file.name.endsWith('.h'))) {
        loadFile(file);
    } else {
        alert('请上传.c或.h文件');
    }
});

function loadFile(file) {
    const reader = new FileReader();
    reader.onload = (e) => {
        currentCode = e.target.result;
        displayCode(currentCode);
    };
    reader.readAsText(file);
}

function displayCode(code) {
    uploadArea.style.display = 'none';
    codeDisplay.style.display = 'block';
    
    currentHighlightLine = null;
    
    // 设置代码并应用语法高亮
    codeContent.textContent = code;
    codeContent.removeAttribute('data-highlighted');
    hljs.highlightElement(codeContent);
    
    // 包装每一行
    wrapCodeLines();
}

function wrapCodeLines() {
    const codeHTML = codeContent.innerHTML;
    const lines = codeHTML.split('\n');
    
    const wrappedLines = lines.map((line) => {
        return `<div class="code-line">${line || ' '}</div>`;
    }).join('');
    
    codeContent.innerHTML = wrappedLines;
}

clearCodeBtn.addEventListener('click', () => {
    if (confirm('确定要删除当前代码吗？')) {
        currentCode = '';
        currentViolations = [];
        currentHighlightLine = null;
        codeContent.innerHTML = '';
        chatContent.innerHTML = '';
        resultContent.innerHTML = '';
        fileInput.value = '';
        
        uploadArea.style.display = 'flex';
        codeDisplay.style.display = 'none';
    }
});

// ==================== AI分析功能 ====================

analyzeBtn.addEventListener('click', async () => {
    if (!currentCode.trim()) {
        alert('请先上传要分析的代码');
        return;
    }
    
    chatContent.innerHTML = '';
    resultContent.innerHTML = '';
    currentViolations = [];
    
    analyzeBtn.disabled = true;
    const originalText = analyzeBtn.textContent;
    analyzeBtn.textContent = '分析中...';
    
    try {
        const customRules = ruleInput.value.trim();
        
        const response = await fetch('/api/analyze', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                code: currentCode,
                custom_rules: customRules
            })
        });
        
        const data = await response.json();
        
        if (data.success) {
            displayAIResponse(data.response);
            
            if (data.violations && data.violations.length > 0) {
                currentViolations = data.violations;
                displayViolations(data.violations);
            }
        } else {
            alert('分析失败：' + data.error);
        }
        
    } catch (error) {
        console.error('Error:', error);
        alert('分析失败，请检查网络连接');
    } finally {
        analyzeBtn.disabled = false;
        analyzeBtn.textContent = originalText;
    }
});

function displayAIResponse(response) {
    const messageDiv = document.createElement('div');
    messageDiv.className = 'chat-message';
    
    const contentDiv = document.createElement('div');
    contentDiv.className = 'message-content';
    
    contentDiv.innerHTML = marked.parse(response);
    
    contentDiv.querySelectorAll('pre code').forEach((block) => {
        hljs.highlightElement(block);
    });
    
    messageDiv.appendChild(contentDiv);
    chatContent.appendChild(messageDiv);
    
    chatContent.scrollTop = chatContent.scrollHeight;
}

function displayViolations(violations) {
    violations.forEach((violation) => {
        const btn = document.createElement('button');
        btn.className = 'violation-btn';
        
        btn.innerHTML = `
            <span class="line-number">行 ${violation.line}</span>
            <span class="rule-id">${violation.rule}</span>
        `;
        
        // 点击跳转到代码行
        btn.addEventListener('click', () => {
            highlightCodeLine(violation.line);
        });
        
        // 鼠标悬停显示详细分析
        btn.addEventListener('mouseenter', (e) => {
            showTooltip(violation.analysis, e);
        });
        
        // 鼠标移动时更新tooltip位置
        btn.addEventListener('mousemove', (e) => {
            updateTooltipPosition(e);
        });
        
        // 鼠标离开隐藏tooltip
        btn.addEventListener('mouseleave', () => {
            hideTooltip();
        });
        
        resultContent.appendChild(btn);
    });
}

function highlightCodeLine(lineNumber) {
    // 移除之前的高亮
    if (currentHighlightLine !== null) {
        const prevLine = codeContent.querySelectorAll('.code-line')[currentHighlightLine - 1];
        if (prevLine) {
            prevLine.classList.remove('highlight-line');
        }
    }
    
    // 添加新高亮
    const allLines = codeContent.querySelectorAll('.code-line');
    const targetLine = allLines[lineNumber - 1];
    
    if (targetLine) {
        targetLine.classList.add('highlight-line');
        currentHighlightLine = lineNumber;
        
        // 滚动到目标行
        const codeDisplayElement = document.getElementById('code-display');
        const targetTop = targetLine.offsetTop;
        const containerHeight = codeDisplayElement.clientHeight;
        
        codeDisplayElement.scrollTo({
            top: targetTop - containerHeight / 2,
            behavior: 'smooth'
        });
    }
}

// 全局隐藏tooltip（点击其他地方时）
document.addEventListener('click', (e) => {
    if (!e.target.closest('.violation-btn')) {
        hideTooltip();
    }
});
```

