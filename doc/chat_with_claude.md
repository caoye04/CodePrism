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

ai调用会用下列的接口与形式：

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

这里主要聊前端的需求与实现，网站大致分为三个模块：“代码展示区域”、“答案记录区”、“ai对话区”

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

### 基础的rag实现

在合适的位置写最基本的rag实现的函数名，但是内容是skip，且用注释给出一些指导与思路

## 目前已有的文件组织形式

```
code/
│
├── code_specifications/
│   ├── code_specifications_list.json    # 编码规范的结构化数据
│   ├── code_specifications_list.txt     # 编码规范的文本版本
│   └── readme.txt                       # 规范使用说明
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
    "R_03_01": "The character sequences /* and // shall not be used within a comment",
    "R_03_02": "Line-splicing shall not be used in // comments",
    "R_04_01": "Octal and hexadecimal escape sequences shall be terminated",
    "R_04_02": "Trigraphs should not be used",
    "R_05_02": "Identifiers declared in the same scope and name space shall be distinct",
    "R_05_03": "An identifier declared in an inner scope shall not hide an identifier declared in an outer scope",
    "R_05_04": "Macro identifiers shall be distinct",
    "R_05_05": "Identifiers shall be distinct from macro names",
    "R_05_06": "A typedef name shall be a unique identifier",
    "R_05_07": "A tag name shall be a unique identifier",
    "R_06_01": "Bit-fields shall only be declared with an appropriate type",
    "R_06_02": "Single-bit named bit fields shall not be of a signed type",
    "R_07_01": "Octal constants shall not be used",
    "R_07_02": "A \"u\" or \"U\" suffix shall be applied to all integer constants that are represented in an unsigned type",
    "R_07_03": "The lowercase character 'l' shall not be used in a literal suffix",
    "R_07_04": "A string literal shall not be assigned to an object unless the object's type is \"pointer to const-qualified char\"",
    "R_08_01": "Types shall be explicitly specified",
    "R_08_02": "Function types shall be in prototype form with named parameters",
    "R_08_03": "All declarations of an object or function shall use the same names and type qualifiers",
    "R_08_04": "A compatible declaration shall be visible when an object or function with external linkage is defined",
    "R_08_08": "The static storage class specifier shall be used in all declarations of objects and functions that have internal linkage",
    "R_08_09": "An object should be defined at block scope if its identifier only appears in a single function",
    "R_08_10": "An inline function shall be declared with the static storage class",
    "R_08_12": "Within an enumerator list, the value of an implicitly-specified enumeration constant shall be unique",
    "R_08_14": "The restrict type qualifier shall not be used",
    "R_09_01": "The value of an object with automatic storage duration shall not be read before it has been set",
    "R_09_02": "The initializer for an aggregate or union shall be enclosed in braces",
    "R_09_03": "Arrays shall not be partially initialized",
    "R_09_04": "An element of an object shall not be initialised more than once",
    "R_09_05": "Where designated initialisers are used to initialize an array object the size of the array shall be specified explicitly",
    "R_10_01": "Operands shall not be of an inappropriate essential type",
    "R_10_02": "Expressions of essentially character type shall not be used inappropriately in addition and subtraction operations",
    "R_10_03": "The value of an expression shall not be assigned to an object with a narrower essential type or of a different essential type category",
    "R_10_04": "Both operands of an operator in which the usual arithmetic conversions are performed shall have the same essential type category",
    "R_10_05": "The value of an expression should not be cast to an inappropriate essential type",
    "R_10_06": "The value of a composite expression shall not be assigned to an object with wider essential type",
    "R_10_07": "If a composite expression is used as one operand of an operator in which the usual arithmetic conversions are performed then the other operand shall not have wider essential type",
    "R_10_08": "The value of a composite expression shall not be cast to a different essential type category or a wider essential type",
    "R_11_01": "Conversions shall not be performed between a pointer to a function and any other type",
    "R_11_02": "Conversions shall not be performed between a pointer to incomplete and any other type",
    "R_11_03": "A cast shall not be performed between a pointer to object type and a pointer to a different object type",
    "R_11_04": "A conversion should not be performed between a pointer to object and an integer type",
    "R_11_05": "A conversion should not be performed from pointer to void into pointer to object",
    "R_11_06": "A cast shall not be performed between pointer to void and an arithmetic type",
    "R_11_07": "A cast shall not be performed between pointer to object and a non-integer arithmetic type",
    "R_11_08": "A cast shall not remove any const or volatile qualification from the type pointed to by a pointer",
    "R_11_09": "The macro NULL shall be the only permitted form of integer null pointer constant",
    "R_12_01": "The precedence of operators within expressions should be made explicit",
    "R_12_02": "The right hand operand of a shift operator shall lie in the range zero to one less than the width in bits of the essential type of the left hand operand",
    "R_12_03": "The comma operator should not be used",
    "R_12_04": "Evaluation of constant expressions should not lead to unsigned integer wrap-around",
    "R_12_05": "The sizeof operator shall not have an operand which is a function parameter declared as \"array of type\"",
    "R_13_02": "The value of an expression and its persistent side-effects shall be the same under all permitted evaluation orders",
    "R_13_03": "A full expression containing an increment (++) or decrement (--) operator should have no other potential side effects other than that caused by the increment or decrement operator",
    "R_13_04": "The result of an assignment operator should not be used",
    "R_13_06": "The operand of the sizeof operator shall not contain any expression which has potential side-effects",
    "R_14_01": "A loop counter shall not have essentially floating type",
    "R_14_02": "A for loop shall be well-formed",
    "R_14_03": "Controlling expressions shall not be invariant",
    "R_14_04": "The controlling expression of an if-statement and the controlling expression of an iteration-statement shall have essentially Boolean type",
    "R_15_01": "The goto statement should not be used",
    "R_15_02": "The goto statement shall jump to a label declared later in the same function",
    "R_15_03": "Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement",
    "R_15_04": "There should be no more than one break or goto statement used to terminate any iteration statement",
    "R_15_05": "A function should have a single point of exit at the end",
    "R_15_06": "The body of an iteration-statement or a selection-statement shall be a compound statement",
    "R_15_07": "All if ... else if constructs shall be terminated with an else statement",
    "R_16_01": "All switch statements shall be well-formed",
    "R_16_02": "A switch label shall only be used when the most closely-enclosing compound statement is the body of a switch statement",
    "R_16_03": "An unconditional break statement shall terminate every switch-clause",
    "R_16_04": "Every switch statement shall have a default label",
    "R_16_05": "A default label shall appear as either the first or the last switch label of a switch statement",
    "R_16_06": "Every switch statement shall have at least two switch-clauses",
    "R_16_07": "A switch-expression shall not have essentially Boolean type",
    "R_17_01": "The features of <stdarg.h> shall not be used",
    "R_17_02": "Functions shall not call themselves, either directly or indirectly",
    "R_17_03": "A function shall not be declared implicitly",
    "R_17_04": "All exit paths from a function with non-void return type shall have an explicit return statement with an expression",
    "R_17_05": "The function argument corresponding to a parameter declared to have an array type shall have an appropriate number of elements",
    "R_17_06": "The declaration of an array parameter shall not contain the static keyword between the [ ]",
    "R_17_07": "The value returned by a function having non-void return type shall be used",
    "R_17_08": "A function parameter should not be modified",
    "R_18_01": "A pointer resulting from arithmetic on a pointer operand shall address an element of the same array as that pointer operand",
    "R_18_02": "Subtraction between pointers shall only be applied to pointers that address elements of the same array",
    "R_18_03": "The relational operators >, >=, < and <= shall not be applied to objects of pointer type except where they point into the same object",
    "R_18_04": "The +, -, += and -= operators should not be applied to an expression of pointer type",
    "R_18_05": "Declarations should contain no more than two levels of pointer nesting",
    "R_18_07": "Flexible array members shall not be declared",
    "R_18_08": "Variable-length array types shall not be used",
    "R_19_01": "An object shall not be assigned or copied to an overlapping object",
    "R_19_02": "The union keyword should not be used",
    "R_20_01": "#include directives should only be preceded by preprocessor directives or comments",
    "R_20_02": "The ', \" or \\ characters and the /* or // character sequences shall not occur in a header file name",
    "R_20_03": "The #include directive shall be followed by either a <filename> or \"filename\" sequence",
    "R_20_04": "A macro shall not be defined with the same name as a keyword",
    "R_20_05": "#undef should not be used",
    "R_20_06": "Tokens that look like a preprocessing directive shall not occur within a macro argument",
    "R_20_07": "Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses",
    "R_20_08": "The controlling expression of a #if or #elif preprocessing directive shall evaluate to 0 or 1",
    "R_20_09": "All identifiers used in the controlling expression of #if or #elif preprocessing directives shall be #define'd before evaluation",
    "R_20_10": "The # and ## preprocessor operators should not be used",
    "R_20_11": "A macro parameter immediately following a # operator shall not immediately be followed by a ## operator",
    "R_20_12": "A macro parameter used as an operand to the # or ## operators, which is itself subject to further macro replacement, shall only be used as an operand to these operators",
    "R_20_13": "A line whose first token is # shall be a valid preprocessing directive",
    "R_20_14": "All #else, #elif and #endif preprocessor directives shall reside in the same file as the #if, #ifdef or #ifndef directive to which they are related",
    "R_21_01": "#define and #undef shall not be used on a reserved identifier or reserved macro name",
    "R_21_02": "A reserved identifier or reserved macro name shall not be declared",
    "R_21_03": "The memory allocation and deallocation functions of <stdlib.h> shall not be used",
    "R_21_04": "The standard header file <setjmp.h> shall not be used",
    "R_21_05": "The standard header file <signal.h> shall not be used",
    "R_21_06": "The Standard Library input/output routines shall not be used",
    "R_21_07": "The Standard Library functions atof, atoi, atol and atoll of <stdlib.h> shall not be used",
    "R_21_08": "The Standard Library functions abort, exit, getenv and system of <stdlib.h> shall not be used",
    "R_21_09": "The library functions bsearch and qsort of <stdlib.h> shall not be used",
    "R_21_10": "The Standard Library time and date routines shall not be used",
    "R_21_11": "The standard header file <tgmath.h> shall not be used",
    "R_21_12": "The exception handling features of <fenv.h> should not be used",
    "R_21_13": "Any value passed to a function in <ctype.h> shall be representable as an unsigned char or be the value EOF",
    "R_21_14": "The Standard Library function memcmp shall not be used to compare null terminated strings",
    "R_21_15": "The pointer arguments to the Standard Library functions memcpy, memmove and memcmp shall be pointers to qualified or unqualified versions of compatible types",
    "R_21_16": "The pointer arguments to the Standard Library function memcmp shall point to either a pointer type, an essentially signed type, an essentially unsigned type, an essentially Boolean type or an essentially enum type",
    "R_21_17": "Use of the string handling functions from <string.h> shall not result in accesses beyond the bounds of the objects referenced by their pointer parameters",
    "R_21_18": "The size_t argument passed to any function in <string.h> shall have an appropriate value",
    "R_21_19": "The pointers returned by the Standard Library functions localeconv, getenv, setlocale or, strerror shall only be used as if they have pointer to const-qualified type",
    "R_21_20": "The pointer returned by the Standard Library functions asctime, ctime, gmtime, localtime, localeconv, getenv, setlocale or strerror shall not be used following a subsequent call to the same function",
    "R_22_01": "All resources obtained dynamically by means of Standard Library functions shall be explicitly released",
    "R_22_02": "A block of memory shall only be freed if it was allocated by means of a Standard Library function",
    "R_22_03": "The same file shall not be open for read and write access at the same time on different streams",
    "R_22_04": "There shall be no attempt to write to a stream which has been opened as read-only",
    "R_22_05": "A pointer to a FILE object shall not be dereferenced",
    "R_22_06": "The value of a pointer to a FILE shall not be used after the associated stream has been closed",
    "R_22_07": "The macro EOF shall only be compared with the unmodified return value from any Standard Library function capable of returning EOF",
    "R_22_08": "The value of errno shall be set to zero prior to a call to an errno-setting-function",
    "R_22_09": "The value of errno shall be tested against zero after calling an errno-setting-function",
    "R_22_10": "The value of errno shall only be tested when the last function to be called was an errno-setting-function"
  }
}
```

