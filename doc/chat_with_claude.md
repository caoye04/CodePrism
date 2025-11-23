# CodePrism项目设计书

> 曹烨 caoye541@gmail.com

[toc]

## 背景

我是一名“代码规范与代码分析课程”的助教，正在设计大作业的框架与内容。我的学生群体是一群大一新生小朋友，代码基础薄弱，我希望我的作业能让他们体会到AI与代码规范的巧妙融合，以及能深入理解一些ai技术，但又不要求他们从0开始实现ai技术。所以我会搭建一个基础框架，来让他们更顺利的完成我们的大作业。

大作业的相关需求如下，我正在实现”一套基础的系统实现框架“。

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

这里主要聊前端的需求与实现，网站大致分为三个模块：“代码展示区域”、“结果区”、“ai回复展示区”，但不需要在表面展示其模块名称

具体分布情况如下：”代码展示区“在左侧占据50%的区域，”结果区“在右上角，高度占20%，右下角的80%高度为

具体功能如下：

1. 代码展示区：普通情况下为“点击上传or拖拽要分析的c文件”的文字，可以点击这个区域来选择上传文件，也可以把对应的c文件拖拽到此。此后这里会展示要分析的代码，默认为c，需要做一定的美化与代码高亮与自动换行等。需要考虑如果代码过长需要有滑动能力。但是尽量简洁，不需要有标题一说

2. ai回复展示区：最下面有个启动按钮”帮我分析这个代码违反了哪些规则“，点击后，如果有左侧有代码，将ai回复的语句展示在这个区域，以对话形式展现。值得注意的是，需要ai回复美观可读，比如代码块、markdown显示等等；如果已经有回复了，直接开一个新的对话，把之前回复去掉即可；如果左侧没有代码，则提示请先放置要分析的代码

   ai回复需要在系统提示代码里注明格式需求。即要求回复包含json：”{{line： ；违反规则： ；一句话分析： }；{……}}“

   ai回复展示区需要在中间放置一个透明度50%的logo字，默认是同目录下的logo.png；也可以把它改为直接展示文字，填写内容；在代码中做成可选方案，无论是展示logo还是展示文字

3. 结果区：获取最新回答中的json，将里面的内容整理成逐行的按钮，按钮内容是：n ”rule变化“，然后放在按钮上可以看到分析里的详细内容；点击按钮，左侧代码展示区会将第n行放到移到尽量页面中间位置且高亮展示。

### 个性化设计

由于我们的课程有多组同学，但他们均没有网站搭建基础，所以我希望在前端网页的视觉选择上能有多个方案，且便于他们在代码中快速切换。

比如我们提供5套美观的配色方案和1套自行填充色数的配色方案，放在代码中用注释的形式。他们可以快速的取消注释等操作来更换整体风格。

以及logo这种图片和文字也可以方便切换

以及一些其他的你认为可能更有便捷个性化的设计可以直接操作。

### 最基础prompt实现

在ai调用时，实现一个最基础的prompt，”帮我分析这里的代码是否违反了以下规范，需要回答包含一个json文件{}“

并且在这里留一个注释：“在这里实现prompt规则”

而我期待学生们会在prompt中加入思维链、前置规范知识指导、提问方法优化、规范回答等一系列方法

### 基础的rag实现

在合适的位置写最基本的rag实现的一个空函数，但是内容是skip，且用注释给出一些指导与思路

具体的思路有两个：

1. 将规范整理为知识库
2. 用规范文档与Example-Suite.rar 里的示例代码作为知识库

### 关于agent实现

单列一个readme_how_to_make_agent.txt之类的文件，将一写基本的思路和如何集成到项目中的指导

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

