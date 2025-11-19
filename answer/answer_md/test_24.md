## 关联规范
- R_17_01: The features of <stdarg.h> shall not be used
- R_17_02: Functions shall not call themselves, either directly or indirectly
- R_17_05: The function argument corresponding to a parameter declared to have an array type shall have an appropriate number of elements

## 违反说明
- `log_values` 使用 `va_list`/`va_arg` 处理可变参数，违反 R_17_01。
- `recursive_sum` 直接递归调用自身，同时在 `main` 中多次触发，违背 R_17_02。
- `consume_buffer` 形参声明为拥有 8 个元素的数组，却只传入 4 个元素的 `local`，违反 R_17_05。
