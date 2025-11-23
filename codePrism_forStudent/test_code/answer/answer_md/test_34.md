## 关联规范
- R_20_01: #include directives should only be preceded by preprocessor directives or comments
- R_20_04: A macro shall not be defined with the same name as a keyword
- R_20_05: #undef should not be used
- R_20_07: Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses
- R_20_08: The controlling expression of a #if or #elif preprocessing directive shall evaluate to 0 or 1
- R_20_09: All identifiers used in the controlling expression of #if or #elif preprocessing directives shall be #define'd before evaluation

## 违反说明
- 文件首行的 `int pre_header = 0;` 出现在 `#include` 之前，违反 R_20_01 要求的“前面只能是预处理指令或注释”。
- 通过 `#define goto LABEL_ALIAS` 重新定义关键字名称作为宏，直接触犯 R_20_04。
- `#undef SCALE` 主动取消宏定义，与 R_20_05 “不应使用 #undef” 的要求冲突。
- `ADD_VALUES(a, b)` 展开后缺少括号，导致表达式直接拼接，违反 R_20_07。
- `#if 5`、`#if 3` 使用的控制表达式不是 0 或 1，违反 R_20_08。
- `#if FLAG_TOGGLE` 与 `#if MISSING_SYMBOL` 中引用的标识符未事先 `#define`，违反 R_20_09。
