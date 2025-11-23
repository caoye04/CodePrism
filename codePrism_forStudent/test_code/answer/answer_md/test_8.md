## 关联规范
- D_04_04: Sections of code should not be 'commented out'.
- R_03_01: The character sequences /* and // shall not be used within a comment.

## 违反说明
1. 文件顶部及 `main` 中出现大段“已移除计算”的注释块，实际内容是被注释掉的代码，违反 D_04_04。
2. 同一注释中混入 `//` 及再次嵌套 `/* ... */`，破坏注释结构，违反 R_03_01。
