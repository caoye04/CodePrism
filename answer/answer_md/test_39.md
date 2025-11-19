## 关联规范
- R_10_05: The value of an expression should not be cast to an inappropriate essential type
- R_10_06: The value of a composite expression shall not be assigned to an object with wider essential type
- R_10_07: If a composite expression is used as one operand of an operator in which the usual arithmetic conversions are performed then the other operand shall not have wider essential type
- R_10_08: The value of a composite expression shall not be cast to a different essential type category or a wider essential type
- R_13_06: The operand of the sizeof operator shall not contain any expression which has potential side-effects

## 违反说明
- `flag = (bool)(u16a + u32b);` 以及循环中的同类语句把无符号求和直接转换成布尔值，表达式所属的整数类型与布尔类型类别不同，违反 R_10_05。
- `uint64_t wide = u16a + u16b;` 与循环中 `wide = u16a + u16b;` 都把 16 位相加的复合表达式隐式赋给 64 位结果，违反 R_10_06。
- `u32a = u32b * (u16a + u16b);` 将 `u16a + u16b` 这个复合表达式与 32 位乘法操作组合，另一操作数更宽，违反 R_10_07。
- `narrow = (uint16_t)(s32a + s32b);` 把两个 32 位有符号数的结果直接压缩成无符号 16 位，并且循环中重复出现，违反 R_10_08 对复合表达式转换的限制。
- `size = sizeof(counter++);` 以及 `sizeof(u16a++)` 在 `sizeof` 运算符里包含了自增表达式，具有副作用，违背 R_13_06。
