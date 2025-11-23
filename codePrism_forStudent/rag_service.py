### 4. rag_service.py - RAG服务框架

"""
RAG (Retrieval-Augmented Generation) 服务

这个文件提供了RAG的基础框架，学生可以在此基础上实现完整的RAG功能。

RAG的基本思路：
1. 将编码规范和示例代码构建成知识库
2. 当需要分析代码时，从知识库中检索相关的规范和示例
3. 将检索到的内容作为上下文提供给AI，帮助AI做出更准确的判断

实现方式建议：
方案一：基于规范的RAG
    - 将code_specifications_list.json中的规范条目向量化
    - 使用embeddings将用户代码转换为向量
    - 检索最相关的规范条目
    - 将相关规范作为prompt的一部分

方案二：基于示例的RAG
    - 将Example-Suite中的示例代码和对应的规范向量化
    - 检索与用户代码相似的示例
    - 以few-shot的方式提供给AI

工具推荐：
    - 向量数据库：FAISS, Chroma, Qdrant
    - Embedding模型：OpenAI embeddings, sentence-transformers
    - 文档处理：langchain, llama-index
"""

import json
import os


class RAGService:
    def __init__(self):
        """初始化RAG服务"""
        self.knowledge_base_path = "knowledge_base"
        self.specifications_path = "code_specifications/code_specifications_list.json"
        
        # TODO: 学生需要在这里初始化向量数据库
        # 例如：
        # self.vector_store = self.build_vector_store()
        pass
    
    def build_vector_store(self):
        """
        构建向量数据库
        
        实现思路：
        1. 读取code_specifications_list.json
        2. 为每条规范生成embedding向量
        3. 可选：读取Example-Suite中的示例代码
        4. 将向量存储到向量数据库中
        
        返回：
            向量数据库实例
        """
        # TODO: 学生实现
        # 示例代码框架：
        """
        from langchain.vectorstores import FAISS
        from langchain.embeddings import OpenAIEmbeddings
        
        # 读取规范
        with open(self.specifications_path, 'r', encoding='utf-8') as f:
            specs = json.load(f)
        
        # 构建文档列表
        documents = []
        for category in ['directive', 'rule']:
            for rule_id, description in specs[category].items():
                documents.append({
                    'id': rule_id,
                    'text': f"{rule_id}: {description}",
                    'category': category
                })
        
        # 创建向量存储
        embeddings = OpenAIEmbeddings()
        texts = [doc['text'] for doc in documents]
        vector_store = FAISS.from_texts(texts, embeddings)
        
        return vector_store
        """
        pass
    
    def retrieve_relevant_rules(self, code, top_k=5):
        """
        检索与代码相关的规范
        
        参数：
            code: 用户提交的C代码
            top_k: 返回最相关的k条规范
            
        返回：
            相关规范列表
            
        实现思路：
        1. 将用户代码转换为embedding向量
        2. 在向量数据库中进行相似度搜索
        3. 返回最相关的top_k条规范
        """
        # TODO: 学生实现
        # 示例代码框架：
        """
        # 在向量数据库中搜索
        results = self.vector_store.similarity_search(code, k=top_k)
        
        # 提取规范信息
        relevant_rules = []
        for result in results:
            relevant_rules.append({
                'rule_id': result.metadata['id'],
                'description': result.page_content,
                'score': result.metadata.get('score', 0)
            })
        
        return relevant_rules
        """
        pass
    
    def build_context(self, relevant_rules):
        """
        根据检索到的规范构建提示词上下文
        
        参数：
            relevant_rules: 相关规范列表
            
        返回：
            格式化的上下文字符串
            
        实现思路：
        将检索到的规范整理成易读的格式，添加到prompt中
        """
        # TODO: 学生实现
        # 示例代码框架：
        """
        if not relevant_rules:
            return ""
        
        context = "以下是可能相关的编码规范：\n\n"
        for i, rule in enumerate(relevant_rules, 1):
            context += f"{i}. {rule['rule_id']}: {rule['description']}\n"
        
        context += "\n请重点检查代码是否违反了以上规范。\n"
        
        return context
        """
        pass
    
    def load_examples_from_suite(self):
        """
        从Example-Suite中加载示例代码
        
        实现思路：
        1. 解压Example-Suite.rar
        2. 读取每个示例代码文件
        3. 提取文件名中的规则编号
        4. 构建示例数据结构
        
        返回：
            示例代码字典 {rule_id: [examples]}
        """
        # TODO: 学生实现
        # 提示：需要先解压Example-Suite.rar
        # 可以使用rarfile或patool库
        pass
    
    def find_similar_examples(self, code, top_k=3):
        """
        查找与用户代码相似的示例
        
        参数：
            code: 用户代码
            top_k: 返回最相似的k个示例
            
        返回：
            相似示例列表
            
        实现思路：
        1. 将所有示例代码向量化
        2. 计算用户代码与示例的相似度
        3. 返回最相似的示例及其对应的规范说明
        """
        # TODO: 学生实现
        pass


# ============================================
# 高级功能提示
# ============================================
"""
如果学生想要实现更高级的RAG功能，可以考虑：

1. 混合检索策略
   - 结合关键词检索和向量检索
   - 使用重排序(reranking)提高检索质量

2. 动态上下文构建
   - 根据代码长度和复杂度动态调整检索的规范数量
   - 优先检索与代码语法特征最相关的规范

3. 示例增强
   - 使用正例和反例(违规/不违规示例)
   - Few-shot learning提高AI判断准确性

4. 知识图谱
   - 构建规范之间的关联关系
   - 如果违反了某条规范，提示相关的其他规范

5. 缓存机制
   - 缓存常见代码模式的分析结果
   - 提高响应速度
"""