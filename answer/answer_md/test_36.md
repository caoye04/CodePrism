## 关联规范
- R_21_01: #define and #undef shall not be used on a reserved identifier or reserved macro name
- R_21_02: A reserved identifier or reserved macro name shall not be declared
- R_21_04: The standard header file <setjmp.h> shall not be used
- R_21_05: The standard header file <signal.h> shall not be used
- R_01_02: Language extensions should not be used

## 违反说明
- 文件顶部直接通过 `#define errno 12` 与 `#define EOF 99` 重定义了受保护的库标识符，违反 R_21_01。
- 自行实现 `_Exit` 函数属于重新声明保留名称，触犯 R_21_02。
- 程序包含 `<setjmp.h>` 并使用 `setjmp/longjmp`，违反 R_21_04。
- 代码包含 `<signal.h>` 并设置 `signal` 处理函数，违反 R_21_05。
- `asm("nop")` 的 GNU 扩展语法贯穿全文件，属于使用实现特有的语言扩展，违反 R_01_02。
