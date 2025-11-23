## 关联规范
- R_07_01: Octal constants shall not be used.
- R_07_03: The lowercase character 'l' shall not be used in a literal suffix.

## 违反说明
1. 代码中多次使用 `010`, `0123`, `0777` 等八进制字面量，直接违反 R_07_01。
2. `60l`, `100l`, `250l`, `900l` 等常量都采用小写 `l` 后缀，违反 R_07_03 的约束。
