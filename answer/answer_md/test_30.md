## 关联规范
- R_18_01: A pointer resulting from arithmetic on a pointer operand shall address an element of the same array as that pointer operand
- R_18_02: Subtraction between pointers shall only be applied to pointers that address elements of the same array
- R_18_08: Variable-length array types shall not be used

## 违反说明
- `pointer_math` 和 `main` 中通过 `values + 12`、`ptr + 6` 得到的指针越过原数组范围，违反 R_18_01。
- `other - values` 对来自不同数组的指针求差，触犯 R_18_02。
- `process_vla` 使用 `int buffer[length]` 创建可变长度数组，直接违反 R_18_08。
