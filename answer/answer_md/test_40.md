## 关联规范
- R_17_06: The declaration of an array parameter shall not contain the static keyword between the [ ]
- R_17_08: A function parameter should not be modified
- R_15_03: Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement
- R_01_03: There shall be no occurrence of undefined or critical unspecified behaviour
- D_04_13: Functions which are designed to provide operations on a resource should be called in an appropriate sequence

## 违反说明
- `write_data(int values[static 8])` 在形参上使用了 `[static 8]`，违反 R_17_06 对数组参数声明的限制。
- `adjust_counter` 直接修改了形参 `count`，与 R_17_08 “不应修改形参”的建议相悖。
- `jump_example` 在 `goto inner_only;` 处跳向定义在 if 块内部的标签，该块并不包围 goto 语句，违反 R_15_03。
- `int crash = 10 / zero;` 将零作为除数，构成未定义行为，违反 R_01_03。
- 资源操作顺序 `close_channel(); open_channel(); close_channel(); close_channel();` 先关闭未打开的通道，结束后又重复关闭，没有遵循获取/释放的配对顺序，违反 D_04_13。
