import requests
import json
import re

# AI API配置
API_KEY = "yourapikey"
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

        # system_prompt += "加入你设计的新prompt"
        # system_prompt += "加入你设计的新prompt"
        # system_prompt += "加入你设计的新prompt"
        # system_prompt += "加入你设计的新prompt"
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
