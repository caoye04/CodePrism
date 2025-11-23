## 关联规范
- D_04_07: If a function returns error information, then that error information shall be tested.
- R_10_01: Operands shall not be of an inappropriate essential type.

## 违反说明
1. `transmit` 的返回值用于传递错误，但在 `write_frame`、`drive_values`、`main` 中被忽略，未做任何检查，违反 D_04_07。
2. `void *raw` 被直接与整数运算并赋给 `int alias`，随后又赋给 `status`，混用了指针与整数，违反 R_10_01 关于基本类型适配的要求。
