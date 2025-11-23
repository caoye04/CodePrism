## 关联规范
- R_09_01: The value of an object with automatic storage duration shall not be read before it has been set.
- R_09_03: Arrays shall not be partially initialized.

## 违反说明
1. `temperature`、`marker` 等局部变量在读取（累加到 `overall`、`report`）之前未赋值，直接违反 R_09_01。
2. `history` 与 `partial_buffer` 仅初始化部分元素，其余保持未定义，构成对数组的部分初始化，违反 R_09_03。
