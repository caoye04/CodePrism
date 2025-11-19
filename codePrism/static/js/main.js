// main.js

// 全局状态
let currentChatId = 'chat1';
let currentFilename = '';
let currentCode = '';

// 初始化
document.addEventListener('DOMContentLoaded', () => {
    initEventListeners();
    hljs.highlightAll();
});

// ==================== 事件监听器 ====================

function initEventListeners() {
    // 文件上传相关
    const uploadArea = document.getElementById('uploadArea');
    const fileInput = document.getElementById('fileInput');
    const clearButton = document.getElementById('clearCode');

    uploadArea.addEventListener('click', () => fileInput.click());
    fileInput.addEventListener('change', handleFileSelect);
    clearButton.addEventListener('click', clearCode);

    // 拖拽上传
    uploadArea.addEventListener('dragover', (e) => {
        e.preventDefault();
        uploadArea.classList.add('dragover');
    });

    uploadArea.addEventListener('dragleave', () => {
        uploadArea.classList.remove('dragover');
    });

    uploadArea.addEventListener('drop', (e) => {
        e.preventDefault();
        uploadArea.classList.remove('dragover');
        const files = e.dataTransfer.files;
        if (files.length > 0) {
            handleFile(files[0]);
        }
    });

    // 答案提交
    document.getElementById('submitAnswer').addEventListener('click', submitAnswer);

    // AI对话相关
    document.getElementById('sendMessage').addEventListener('click', sendMessage);
    document.getElementById('newChat').addEventListener('click', createNewChat);

    // 输入框快捷键
    const chatInput = document.getElementById('chatInput');
    chatInput.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && !e.shiftKey) {
            e.preventDefault();
            sendMessage();
        }
    });
}

// ==================== 文件处理 ====================

function handleFileSelect(event) {
    const file = event.target.files[0];
    if (file) {
        handleFile(file);
    }
}

async function handleFile(file) {
    if (!file.name.endsWith('.c')) {
        showNotification('请上传 .c 文件', 'error');
        return;
    }

    const formData = new FormData();
    formData.append('file', file);

    try {
        const response = await fetch('/api/upload_code', {
            method: 'POST',
            body: formData
        });

        const data = await response.json();

        if (data.success) {
            currentFilename = data.filename;
            currentCode = data.content;
            displayCode(data.filename, data.content);
            document.getElementById('answerFilename').value = data.filename.replace('.c', '');
            showNotification('文件上传成功', 'success');
        } else {
            showNotification(data.error || '上传失败', 'error');
        }
    } catch (error) {
        console.error('上传错误:', error);
        showNotification('上传失败: ' + error.message, 'error');
    }
}

function displayCode(filename, content) {
    document.getElementById('uploadArea').style.display = 'none';
    document.getElementById('codeDisplay').style.display = 'flex';
    document.getElementById('codeFilename').textContent = filename;
    const codeElement = document.getElementById('codeContent');
    codeElement.textContent = content;
    hljs.highlightElement(codeElement);
}

function clearCode() {
    document.getElementById('uploadArea').style.display = 'flex';
    document.getElementById('codeDisplay').style.display = 'none';
    document.getElementById('fileInput').value = '';
    document.getElementById('answerFilename').value = '';
    currentFilename = '';
    currentCode = '';
    showNotification('已清除代码', 'success');
}

// ==================== 答案提交 ====================

async function submitAnswer() {
    const filename = document.getElementById('answerFilename').value;
    const ruleIdsText = document.getElementById('ruleIds').value;

    if (!filename) {
        showNotification('请先上传代码文件', 'warning');
        return;
    }

    if (!ruleIdsText.trim()) {
        showNotification('请输入违反的规则ID', 'warning');
        return;
    }

    // 解析规则ID（去除空格，按逗号分割）
    const ruleIds = ruleIdsText.split(',').map(id => id.trim()).filter(id => id);

    try {
        const response = await fetch('/api/submit_answer', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                filename: filename,
                rule_ids: ruleIds
            })
        });

        const data = await response.json();

        if (data.success) {
            showNotification('答案已保存', 'success');
            document.getElementById('ruleIds').value = '';
        } else {
            showNotification(data.error || '保存失败', 'error');
        }
    } catch (error) {
        console.error('提交错误:', error);
        showNotification('提交失败: ' + error.message, 'error');
    }
}

// ==================== AI对话 ====================

async function sendMessage() {
    const input = document.getElementById('chatInput');
    const message = input.value.trim();

    if (!message) return;

    // 显示用户消息
    addMessage('user', message);
    input.value = '';

    // 显示加载动画
    const loadingId = showLoading();

    try {
        const response = await fetch('/api/chat', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                message: message,
                chat_id: currentChatId,
                code_content: currentCode,  // 传递当前代码内容
                use_rag: false  // 学生可以修改这里来启用RAG
            })
        });

        const data = await response.json();

        // 移除加载动画
        removeLoading(loadingId);

        if (data.success) {
            // 显示AI回复
            addMessage('ai', data.response);
        } else {
            showNotification(data.error || 'AI回复失败', 'error');
            addMessage('ai', '抱歉，我遇到了一些问题，请稍后再试。');
        }
    } catch (error) {
        console.error('对话错误:', error);
        removeLoading(loadingId);
        showNotification('发送失败: ' + error.message, 'error');
        addMessage('ai', '抱歉，网络连接出现问题，请检查后重试。');
    }
}

function addMessage(role, content) {
    const messagesContainer = document.getElementById('chatMessages');
    
    // 移除欢迎消息
    const welcomeMessage = messagesContainer.querySelector('.welcome-message');
    if (welcomeMessage) {
        welcomeMessage.remove();
    }

    const messageDiv = document.createElement('div');
    messageDiv.className = `message ${role}`;

    const avatar = document.createElement('div');
    avatar.className = 'message-avatar';
    avatar.textContent = role === 'user' ? '我' : 'AI';

    const messageContent = document.createElement('div');
    messageContent.className = 'message-content';

    // 如果是AI回复，渲染Markdown
    if (role === 'ai') {
        messageContent.innerHTML = marked.parse(content);
        // 高亮代码块
        messageContent.querySelectorAll('pre code').forEach((block) => {
            hljs.highlightElement(block);
        });
    } else {
        messageContent.textContent = content;
    }

    messageDiv.appendChild(avatar);
    messageDiv.appendChild(messageContent);
    messagesContainer.appendChild(messageDiv);

    // 滚动到底部
    messagesContainer.scrollTop = messagesContainer.scrollHeight;
}

function showLoading() {
    const messagesContainer = document.getElementById('chatMessages');
    const loadingDiv = document.createElement('div');
    loadingDiv.className = 'message ai';
    loadingDiv.id = 'loading-message';

    const avatar = document.createElement('div');
    avatar.className = 'message-avatar';
    avatar.textContent = 'AI';

    const loadingContent = document.createElement('div');
    loadingContent.className = 'message-content loading';
    loadingContent.innerHTML = `
        <div class="loading-dot"></div>
        <div class="loading-dot"></div>
        <div class="loading-dot"></div>
    `;

    loadingDiv.appendChild(avatar);
    loadingDiv.appendChild(loadingContent);
    messagesContainer.appendChild(loadingDiv);

    messagesContainer.scrollTop = messagesContainer.scrollHeight;

    return 'loading-message';
}

function removeLoading(loadingId) {
    const loadingElement = document.getElementById(loadingId);
    if (loadingElement) {
        loadingElement.remove();
    }
}

async function createNewChat() {
    try {
        const response = await fetch('/api/new_chat', {
            method: 'POST'
        });

        const data = await response.json();

        if (data.success) {
            currentChatId = data.chat_id;
            
            // 清空对话区
            const messagesContainer = document.getElementById('chatMessages');
            messagesContainer.innerHTML = `
                <div class="welcome-message">
                    <h3>👋 欢迎使用代码分析助手</h3>
                    <p>我可以帮助你：</p>
                    <ul>
                        <li>分析C代码的规范问题</li>
                        <li>解释编码规范的含义</li>
                        <li>提供代码改进建议</li>
                    </ul>
                    <p class="hint-text">上传代码后，可以直接询问我相关问题</p>
                </div>
            `;

            showNotification('新对话已创建', 'success');
        } else {
            showNotification(data.error || '创建失败', 'error');
        }
    } catch (error) {
        console.error('创建新对话错误:', error);
        showNotification('创建失败: ' + error.message, 'error');
    }
}

// ==================== 通知系统 ====================

function showNotification(message, type = 'info') {
    // 创建通知元素
    const notification = document.createElement('div');
    notification.className = `notification notification-${type}`;
    notification.textContent = message;
    
    // 添加样式
    notification.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        padding: 1rem 1.5rem;
        background-color: ${type === 'success' ? 'var(--success-color)' : 
                          type === 'error' ? 'var(--error-color)' : 
                          type === 'warning' ? 'var(--warning-color)' : 
                          'var(--primary-color)'};
        color: white;
        border-radius: 8px;
        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
        z-index: 1000;
        animation: slideIn 0.3s ease-out;
    `;

    document.body.appendChild(notification);

    // 3秒后移除
    setTimeout(() => {
        notification.style.animation = 'slideOut 0.3s ease-in';
        setTimeout(() => {
            notification.remove();
        }, 300);
    }, 3000);
}

// 添加动画样式
const style = document.createElement('style');
style.textContent = `
    @keyframes slideIn {
        from {
            transform: translateX(400px);
            opacity: 0;
        }
        to {
            transform: translateX(0);
            opacity: 1;
        }
    }

    @keyframes slideOut {
        from {
            transform: translateX(0);
            opacity: 1;
        }
        to {
            transform: translateX(400px);
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);