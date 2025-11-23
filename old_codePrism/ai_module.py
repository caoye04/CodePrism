# ai_module.py (备选版本 - 使用 requests)
import requests
import json

API_KEY = "sk-tT9Ddv4cOCl5BXW4kivhRQ"
BASE_URL = "https://llmapi.paratera.com/v1"
MODEL_NAME = "Qwen3-235B-A22B-Instruct-2507"

class AIAssistant:
    """AI助手类，负责与大语言模型交互"""
    
    def __init__(self):
        """初始化AI助手"""
        self.api_key = API_KEY
        self.base_url = BASE_URL.rstrip('/')
        self.model = MODEL_NAME

        # TODO: 可以在这里修改基础系统提示词
        # ============ 基础系统提示词 ============
        # 学生可以在这里扩展提示词，添加：
        # 1. 思维链引导（如：请一步步思考）
        # 2. 回答格式要求（如：按照特定格式输出检查结果）
        # 3. 角色设定细化（如：你是一个严谨的代码审查专家）
        # 4. Few-shot示例（如：提供一些检查示例）        
        self.system_prompt = "你是一个很厉害的代码规范分析大师。你需要以下面的格式输出，这是最重要的需求，需要严格遵守"

        # TODO: 可以在下方添加更多提示词内容
        # 示例：
        # self.system_prompt += "\n\n在分析代码时，请按照以下步骤进行：\n"
        # self.system_prompt += "1. 仔细阅读代码，理解其功能\n"
        # self.system_prompt += "2. 对照每一条规范逐一检查\n"
        # self.system_prompt += "3. 列出所有违反的规范及其原因\n"
        # self.system_prompt += "4. 提供修改建议\n"
    
    def chat(self, user_message, chat_history=None, context=""):
        """与AI进行对话"""
        # 构建消息列表
        messages = [{"role": "system", "content": self.system_prompt}]
        
        if chat_history:
            messages.extend(chat_history)
        
        if context:
            user_message = f"【上下文信息】\n{context}\n\n【用户问题】\n{user_message}"
        
        messages.append({"role": "user", "content": user_message})
        
        try:
            # 使用 requests 调用 API
            response = requests.post(
                f"{self.base_url}/chat/completions",
                headers={
                    "Authorization": f"Bearer {self.api_key}",
                    "Content-Type": "application/json"
                },
                json={
                    "model": self.model,
                    "messages": messages,
                    "temperature": 0.7,
                    "max_tokens": 2000
                },
                timeout=60
            )
            
            response.raise_for_status()
            result = response.json()
            
            return result['choices'][0]['message']['content']
        
        except Exception as e:
            return f"AI调用出错：{str(e)}\n请检查网络连接或API配置。"

if __name__ == "__main__":
    print("正在测试AI模块（requests版本）...")
    assistant = AIAssistant()
    response = assistant.chat("你好")
    print("AI回复：", response)