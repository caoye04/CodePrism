## 关联规范
- R_21_06: The Standard Library input/output routines shall not be used.
- R_21_07: The Standard Library functions atof, atoi, atol and atoll of <stdlib.h> shall not be used.
- D_04_14: The validity of values received from external sources shall be checked.

## 违反说明
1. 程序大量调用 `printf`、`fprintf`, `fopen`、`fclose` 等标准 I/O 例程，违反 R_21_06。
2. `mirror_inputs` 与 `convert_value` 中直接使用 `atof`，违反 R_21_07。
3. 命令行参数 `argv` 未做任何合法性检查即被解析、输出，违反 D_04_14 对外部输入的验证要求。
