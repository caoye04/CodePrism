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