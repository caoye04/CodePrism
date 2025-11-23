# rag_module.py
"""
RAG (Retrieval-Augmented Generation) 检索增强生成模块

这个模块提供了RAG的基础框架，学生需要完成具体实现。
RAG的核心思想是：在回答问题前，先从知识库中检索相关信息，
然后将检索到的信息作为上下文提供给大模型，提高回答的准确性。

这里推荐把自己的思路和项目框架给 大模型，问问大模型建议

参考实现思路：
思路1：基于规范文档的RAG
    1. 将code_specifications_list.json中的规范构建为向量数据库
    2. 用户提问时，检索最相关的规范条目
    3. 将检索到的规范作为上下文注入到prompt中

思路2：基于示例代码的RAG
    1. 将Example-Suite.rar中的示例代码解压并索引
    2. 对每个示例提取特征（违反的规范、代码模式等）
    3. 检索时找到相似的示例，作为Few-shot学习的样本

推荐的技术栈：
- 向量数据库：FAISS、Chroma、Pinecone等
- 文本嵌入模型：sentence-transformers、OpenAI embeddings等
- 文档处理：langchain、llamaindex等框架
"""

import json
import os

class RAGSystem:
    """RAG系统基础框架"""
    
    def __init__(self):
        """初始化RAG系统"""
        self.knowledge_base_path = "knowledge_base"
        self.specifications_path = "code_specifications/code_specifications_list.json"
        
        # TODO: 学生需要在这里初始化向量数据库
        # 示例代码（需要安装相应库）：
        # from langchain.vectorstores import FAISS
        # from langchain.embeddings import OpenAIEmbeddings
        # self.vectorstore = FAISS(embedding_function=OpenAIEmbeddings())
        
        self.vectorstore = None  # 暂时为空，等待学生实现
        
        # 加载规范数据
        self.specifications = self._load_specifications()
    
    def _load_specifications(self):
        """
        加载编码规范数据
        
        Returns:
            dict: 规范数据字典
        """
        try:
            with open(self.specifications_path, 'r', encoding='utf-8') as f:
                return json.load(f)
        except Exception as e:
            print(f"加载规范文件失败: {e}")
            return {}
    
    def build_knowledge_base(self):
        """
        构建知识库
        
        这个方法需要学生实现，主要步骤：
        1. 读取规范文档和示例代码
        2. 对文本进行分块（Chunking）
        3. 生成文本向量（Embedding）
        4. 存储到向量数据库
        
        实现提示：
        - 规范文档可以按照每条规则为单位进行分块
        - 示例代码可以提取代码片段+违反的规则+说明作为文档
        - 使用适当的embedding模型（如sentence-transformers）
        """
        # TODO: 学生实现
        # 示例伪代码：
        # documents = []
        # 
        # # 1. 处理规范文档
        # for rule_type in self.specifications:
        #     for rule_id, rule_desc in self.specifications[rule_type].items():
        #         doc = {
        #             'id': rule_id,
        #             'type': rule_type,
        #             'content': rule_desc,
        #             'metadata': {'source': 'specifications'}
        #         }
        #         documents.append(doc)
        # 
        # # 2. 处理示例代码
        # example_files = self._load_examples()
        # for example in example_files:
        #     doc = {
        #         'content': example['code'],
        #         'metadata': {
        #             'source': 'examples',
        #             'violations': example['violations']
        #         }
        #     }
        #     documents.append(doc)
        # 
        # # 3. 构建向量数据库
        # self.vectorstore = create_vectorstore(documents)
        
        pass  # 学生实现
    
    def retrieve_relevant_info(self, query, top_k=3):
        """
        根据查询检索相关信息
        
        Args:
            query: 查询文本（如用户的问题或代码片段）
            top_k: 返回最相关的top_k个结果
        
        Returns:
            str: 检索到的相关信息，格式化后的文本
        
        实现提示：
        - 将query转换为向量
        - 在向量数据库中进行相似度搜索
        - 返回最相关的文档内容
        """
        # TODO: 学生实现
        # 示例伪代码：
        # if self.vectorstore is None:
        #     return "RAG系统未初始化，请先构建知识库"
        # 
        # # 检索相关文档
        # results = self.vectorstore.similarity_search(query, k=top_k)
        # 
        # # 格式化输出
        # context = "【检索到的相关规范】\n"
        # for i, doc in enumerate(results):
        #     context += f"\n{i+1}. {doc.metadata.get('id', 'Unknown')}: {doc.page_content}\n"
        # 
        # return context
        
        # 当前返回空字符串，表示RAG功能未启用
        return ""
    
    def _load_examples(self):
        """
        加载示例代码
        
        Returns:
            list: 示例列表
        
        实现提示：
        - 需要先解压Example-Suite.rar
        - 解析每个示例文件，提取代码和违反的规则
        - 可以使用正则表达式或AST解析
        """
        # TODO: 学生实现
        pass
    
    def search_by_rule_id(self, rule_id):
        """
        根据规则ID精确查找规范内容
        
        这是一个简单的辅助方法，不需要向量检索
        
        Args:
            rule_id: 规则ID，如 "D_04_05"
        
        Returns:
            str: 规范描述
        """
        for rule_type in self.specifications:
            if rule_id in self.specifications[rule_type]:
                return self.specifications[rule_type][rule_id]
        return None
    
    def get_all_rules(self):
        """
        获取所有规范列表
        
        Returns:
            dict: 所有规范
        """
        return self.specifications

# 使用示例
if __name__ == "__main__":
    # 测试RAG模块
    rag = RAGSystem()
    
    # 测试精确查找
    rule_desc = rag.search_by_rule_id("D_04_05")
    print(f"D_04_05规范：{rule_desc}")
    
    # 测试检索（需要学生实现后才能工作）
    context = rag.retrieve_relevant_info("变量命名规范")
    print(f"\n检索结果：{context}")