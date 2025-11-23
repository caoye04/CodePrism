## 关联规范
- R_18_03: The relational operators >, >=, < and <= shall not be applied to objects of pointer type except where they point into the same object
- R_18_04: The +, -, += and -= operators should not be applied to an expression of pointer type
- R_18_05: Declarations should contain no more than two levels of pointer nesting
- R_18_07: Flexible array members shall not be declared
- R_19_01: An object shall not be assigned or copied to an overlapping object
- R_19_02: The union keyword should not be used

## 违反说明
- `pointer_relations` 与 `main` 中将 `pa`、`pb`、`ptr_a`、`ptr_b` 等不同数组的指针互相比大小，违背 R_18_03 关于仅比较同一对象内指针的限制。
- 代码多处执行 `pa += 5;`、`pb -= 2;`、`ptr_a += ptr_b - ptr_a;` 等指针算术，直接违反 R_18_04。
- `int ***triple = (int ***)&pa;` 使用了三级指针，超过 R_18_05 允许的两层限制。
- `struct FlexBuffer` 定义了柔性数组成员 `int payload[];`，违反 R_18_07。
- `move_overlap` 在同一个数组上从前向后覆盖赋值，属于对自身重叠区域的复制，违反 R_19_01。
- `union RawView` 用 union 复用存储，触犯 R_19_02 对 union 的禁用建议。
