## 关联规范
- R_08_14: The restrict type qualifier shall not be used
- R_10_03: The value of an expression shall not be assigned to an object with a narrower essential type or of a different essential type category
- R_10_04: Both operands of an operator in which the usual arithmetic conversions are performed shall have the same essential type category

## 违反说明
- `copy_wave` 的参数使用 `restrict` 限定，直接违反 R_08_14 对类型修饰的禁用要求。
- `compress_value` 与 `main` 中频繁把 `int` 表达式强制存入 `uint8_t level`，存在窄化赋值，违背 R_10_03。
- 多处将 `float mix` 与 `int` 数组元素直接相加/相减，使算术运算的两个操作数类型类别不同，触犯 R_10_04。
