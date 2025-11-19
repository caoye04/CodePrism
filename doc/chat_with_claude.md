# CodePrism项目设计书

> 曹烨 caoye541@gmail.com

[toc]

## 背景

我是一名“代码规范与代码分析课程”的助教，正在设计大作业的框架与内容。我的学生群体是一群大一新生小朋友，代码基础薄弱，我希望我的作业能让他们体会到AI与代码规范的巧妙融合，以及能深入理解一些ai技术，但又不要求他们从0开始实现ai技术。所以我会搭建一个基础框架，来让他们更顺利的完成我们的大作业。

大作业的相关需求如下，我正在实现”一套基础的系统实现框架“。

```
# 系统工程探究实践项目任务书
> 实践项目：代码规范检查智能助手
良好的代码规范是团队协作与软件维护的基础，它对代码风格、结构与安全等方面提出了要求。代码规范中通常包含多条规则，要识别程序员提交的代码是否符合全部规则通常非常复杂，人工检查容易出现遗漏或者误判，因此实际生产中通常使用代码规范分析工具（如：BSSLint、Cppcheck等）来检查代码是否违反相应规范。此类工具应用广泛，但是要求事先针对每条规则实现特有的检查器（Checker），因此无法快速适用于用户自定义的规则，适应性受限。
本实践项目围绕代码规范的自动检查开展研究，为解决适应性的问题，拟以大语言模型为推理引擎，在给定代码规范（一组规则）与待检查代码（或代码库）的前提下，通过理解需求、提示词工程、检索增强生成或者智能体等技术，完成规范检查智能助手的设计与实现，尝试检查出违背代码规范的代码片段。
1)   实践内容及要求：
主要的实践内容在于实现代码规范检查器，根据技术方案的复杂度以及投入工作量的不同，分成以下几个递进的要求：
（1）面向自定义规则的代码规范检查器
给定自然语言描述的规则与待检查的代码库，进行提示词工程，利用大语言模型的推理能力识别出违背规则的代码，并按照用户友好的方式反馈给用户。需要注意的是，代码库中可能会有多个文件。
（2）基于检索增强的代码规范检查器
在前一步的基础上，结合规范文档库和检索增强生成（RAG）技术，在执行检查时，首先从规范文档中查询与待检测规范相关的描述、示例等信息，作为外部知识注入大模型，提升检查的精度。
（3）基于智能体的代码规范检查器
在前面的基础上，设计并实现智能体流程，围绕检测出来的代码问题，进一步尝试提升检查精度或者提供更丰富的分析、修复建议，可自由发挥想象。
2)   实验室资源：
（1）代码规范和相关的知识库；
（2）待检测的代码样本；
（3）一套基础的系统实现框架
```

## 需求与细节

我希望你能帮助我来实现这样一个基础系统。

它包含基本的ai调用、网站搭建、个性化设计、最基础prompt实现、基础的rag实现等。接下来我会简单聊聊细节。

### ai调用规则

ai调用会用下列的接口与形式，完全依照这个裸露key的形式即可

```
API_KEY = "sk-tT9Ddv4cOCl5BXW4kivhRQ"
BASE_URL = "https://llmapi.paratera.com/v1"
AI_APIS = {
    "qwen3_235b": {
        "base_url": BASE_URL,
        "api_key": API_KEY,
        "model": "Qwen3-235B-A22B-Instruct-2507"
    }
}
```

之后需要实现ai对话功能。

基本的要求是：需要做到上下文记忆

### 网站搭建

用基本的python后端与html前端逻辑实现。

这里主要聊前端的需求与实现，网站大致分为三个模块：“代码展示区域”、“答案记录区”、“ai对话区”，但不需要在表面展示其模块名称

在分布上具体如下：“代码展示区域”与“答案记录区”在左侧，大约30%；“ai对话区”在右侧，占70%。代码展示区相对更长，约80%；答案记录区大概20%。

具体功能如下：

1. 代码展示区：普通情况下为“点击上传or拖拽要分析的c文件”的文字，可以点击这个区域来选择上传文件，也可以把对应的c文件拖拽到此。此后这里会展示要分析的代码，默认为c，需要做一定的美化与代码高亮等。需要考虑如果代码过长需要有滑动能力

2. 答案记录区：这里有三个组件。第一个是目前分析的代码文件名称，会自动识别代码展示区的代码名称；第二个是需要输入其违反的规则id，如D_04_05、D_04_12等，值得注意一个case可能对应多个规则id；第三个是提交按钮，会自动将上面的信息整合到同文件夹中的"data/your_answer.json"，以下列形式存储：

   ```json
   {
     "test1": ["D_04_05", "R_05_03"],
     "test2": ["R_02_02", "R_02_07"],
     "test3": ["D_04_12", "R_21_03", "R_22_2"],
     "test4": ["R_09_01", "R_09_03"],
     "test5": ["R_21_06", "R_21_07", "D_04_14"],
     "test6": ["D_04_07", "R_10_01"],
     "test7": ["R_07_01", "R_07_03"],
   ```

3. ai对话区：这里需要做一个项目名称展示和对话框和聊天记录展示；项目名称在这个区域的最顶部，默认为“代码分析大师”；对话框在最底部上一点点，可以发送；聊天记录展示需要做一些基本的ai对话的功能，比如换行、代码框等基本的对api返回语句做适当美化的能力。以及右上角放一个新建对话按钮。所有的聊天记录以以下形式存储在data/chat_history.json中：

   ```json
   {
     "chat1": {
     	"user":"...",
     	"ai":"...",
     	"user":""
     },
     "chat2": {
     	"user":"...",
     	"ai":"...",
     	"user":"",
     	"ai":"..."
     },
   ```

### 个性化设计

由于我们的课程有多组同学，但他们均没有网站搭建基础，所以我希望在前端网页的视觉选择上能有多个方案，且便于他们在代码中快速切换。

比如我们提供5套美观的配色方案和1套自行填充色数的配色方案，放在代码中用注释的形式。他们可以快速的取消注释等操作来更换整体风格。

以及一些其他的你认为可能更有便捷个性化的设计可以直接操作。

### 最基础prompt实现

在ai调用时，实现一个最基础的prompt：“你是一个很厉害的代码规范分析大师。”

并且在这里留一个注释：“可以在这里加一些prompt，也可以在网页版对话框中实现prompt的要求”

而我期待学生们会在prompt中加入思维链、前置规范知识指导、提问方法优化、规范回答等一系列方法

### 基础的rag实现

在合适的位置写最基本的rag实现的一个空函数，但是内容是skip，且用注释给出一些指导与思路

具体的思路有两个：

1. 将规范整理为知识库
2. 用规范文档与Example-Suite.rar 里的示例代码作为知识库

### 关于agent实现

单列一个readme_how_to_make_agent之类的文件，将一写基本的思路和如何集成到项目中的指导

## 目前已有的文件组织形式

```
forStudent/
│
├── code_specifications/
│   ├── code_specifications_list.json    # 编码规范的结构化数据
│   ├── code_specifications_list.txt     # 编码规范的文本版本
│   └── readme.txt                       # 规范使用说明
│
├── data/
│   ├── chat_history.json 
│   └── your_answer.json
│
├── knowledge_base/                      # RAG知识库目录
│   ├── Example-Suite.rar               # 原始MISRA示例压缩包
│   └── readme.txt                      # 知识库使用说明
│
└── test_code/                          # 测试代码目录
    ├── test_1.c
    ├── test_2.c
    ├── ...
    └── test_40.c
```

### code_specifications_list.json

```json
{
  "directive": {
    "D_01_01": "Any implementation-defined behaviour on which the output of the program depends shall be documented and understood",
    "D_02_01": "All source files shall compile without any compilation errors",
    "D_03_01": "All code shall be traceable to documented requirements",
    "D_04_01": "Run-time failures shall be minimized",
    "D_04_02": "All usage of assembly language should be documented",
    "D_04_03": "Assembly language shall be encapsulated and isolated",
    "D_04_04": "Sections of code should not be 'commented out'",
    "D_04_05": "Identifiers in the same namespace with overlapping visibility should be typographically unambiguous",
    "D_04_06": "typedefs that indicate size and signedness should be used in place of the basic numerical types",
    "D_04_07": "If a function returns error information, then that error information shall be tested",
    "D_04_09": "A function should be used in preference to a function-like macro where they are interchangeable",
    "D_04_10": "Precautions shall be taken in order to prevent the contents of a header file being included more than once",
    "D_04_11": "The validity of values passed to library functions shall be checked",
    "D_04_12": "Dynamic memory allocation shall not be used",
    "D_04_13": "Functions which are designed to provide operations on a resource should be called in an appropriate sequence",
    "D_04_14": "The validity of values received from external sources shall be checked"
  },
  "rule": {
    "R_01_01": "The program shall contain no violations of the standard C syntax and constraints, and shall not exceed the implementation's translation limits",
    "R_01_02": "Language extensions should not be used",
    "R_01_03": "There shall be no occurrence of undefined or critical unspecified behaviour",
    "R_02_02": "There shall be no dead code",
    "R_02_03": "A project should not contain unused type declarations",
    "R_02_04": "A project should not contain unused tag declarations",
    "R_02_05": "A project should not contain unused macro declarations",
    "R_02_06": "A function should not contain unused label declarations",
    "R_02_07": "There should be no unused parameters in functions",
    "R_03_01": "The 
```

