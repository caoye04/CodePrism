# backend.py
from flask import Flask, request, jsonify, render_template, send_from_directory
import json
import os
from ai_module import AIAssistant
from rag_module import RAGSystem

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = 'uploads'
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # 16MB max file size

# 确保必要的目录存在
os.makedirs('data', exist_ok=True)
os.makedirs('uploads', exist_ok=True)

# 初始化AI助手和RAG系统
ai_assistant = AIAssistant()
rag_system = RAGSystem()

# 初始化数据文件
def init_data_files():
    """初始化数据文件"""
    if not os.path.exists('data/chat_history.json'):
        with open('data/chat_history.json', 'w', encoding='utf-8') as f:
            json.dump({}, f, ensure_ascii=False, indent=2)
    
    if not os.path.exists('data/your_answer.json'):
        with open('data/your_answer.json', 'w', encoding='utf-8') as f:
            json.dump({}, f, ensure_ascii=False, indent=2)

init_data_files()

@app.route('/')
def index():
    """主页面"""
    return render_template('index.html')

@app.route('/api/upload_code', methods=['POST'])
def upload_code():
    """上传代码文件"""
    try:
        if 'file' not in request.files:
            return jsonify({'error': '没有文件上传'}), 400
        
        file = request.files['file']
        if file.filename == '':
            return jsonify({'error': '文件名为空'}), 400
        
        if not file.filename.endswith('.c'):
            return jsonify({'error': '只支持.c文件'}), 400
        
        # 读取文件内容
        content = file.read().decode('utf-8')
        
        return jsonify({
            'success': True,
            'filename': file.filename,
            'content': content
        })
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/submit_answer', methods=['POST'])
def submit_answer():
    """提交答案"""
    try:
        data = request.json
        filename = data.get('filename', '').replace('.c', '')
        rule_ids = data.get('rule_ids', [])
        
        # 读取现有答案
        with open('data/your_answer.json', 'r', encoding='utf-8') as f:
            answers = json.load(f)
        
        # 更新答案
        answers[filename] = rule_ids
        
        # 保存答案
        with open('data/your_answer.json', 'w', encoding='utf-8') as f:
            json.dump(answers, f, ensure_ascii=False, indent=2)
        
        return jsonify({'success': True, 'message': '答案已保存'})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/chat', methods=['POST'])
def chat():
    """AI对话接口"""
    try:
        data = request.json
        user_message = data.get('message', '')
        chat_id = data.get('chat_id', 'chat1')
        code_content = data.get('code_content', '')  # 可选：当前代码内容
        
        # 读取聊天历史
        with open('data/chat_history.json', 'r', encoding='utf-8') as f:
            chat_history = json.load(f)
        
        # 获取当前对话历史
        if chat_id not in chat_history:
            chat_history[chat_id] = []
        
        current_chat = chat_history[chat_id]
        
        # 调用AI获取回复
        # 这里可以选择是否使用RAG增强
        use_rag = data.get('use_rag', False)
        if use_rag:
            # TODO: 学生可以在这里集成RAG功能
            enhanced_context = rag_system.retrieve_relevant_info(user_message)
            ai_response = ai_assistant.chat(user_message, current_chat, enhanced_context)
        else:
            ai_response = ai_assistant.chat(user_message, current_chat, code_content)
        
        # 更新聊天历史
        current_chat.append({'role': 'user', 'content': user_message})
        current_chat.append({'role': 'assistant', 'content': ai_response})
        
        # 保存聊天历史
        chat_history[chat_id] = current_chat
        with open('data/chat_history.json', 'w', encoding='utf-8') as f:
            json.dump(chat_history, f, ensure_ascii=False, indent=2)
        
        return jsonify({
            'success': True,
            'response': ai_response,
            'chat_id': chat_id
        })
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/new_chat', methods=['POST'])
def new_chat():
    """创建新对话"""
    try:
        with open('data/chat_history.json', 'r', encoding='utf-8') as f:
            chat_history = json.load(f)
        
        # 生成新的chat_id
        chat_count = len(chat_history)
        new_chat_id = f'chat{chat_count + 1}'
        
        chat_history[new_chat_id] = []
        
        with open('data/chat_history.json', 'w', encoding='utf-8') as f:
            json.dump(chat_history, f, ensure_ascii=False, indent=2)
        
        return jsonify({
            'success': True,
            'chat_id': new_chat_id
        })
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/load_chat/<chat_id>', methods=['GET'])
def load_chat(chat_id):
    """加载指定对话历史"""
    try:
        with open('data/chat_history.json', 'r', encoding='utf-8') as f:
            chat_history = json.load(f)
        
        if chat_id not in chat_history:
            return jsonify({'error': '对话不存在'}), 404
        
        return jsonify({
            'success': True,
            'messages': chat_history[chat_id]
        })
    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    print("🚀 CodePrism 代码分析助手启动中...")
    print("📝 访问 http://localhost:5000 开始使用")
    app.run(debug=True, host='0.0.0.0', port=5000)