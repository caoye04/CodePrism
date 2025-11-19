## 关联规范
- R_17_04: All exit paths from a function with non-void return type shall have an explicit return statement with an expression
- R_17_05: The function argument corresponding to a parameter declared to have an array type shall have an appropriate number of elements
- R_17_08: A function parameter should not be modified

## 违反说明
- `compute_sum` 只有当 `total > 50` 时返回，其余路径直接掉到函数末尾无返回值，违反 R_17_04。
- `process_matrix` 的数组参数声明需要 10 个元素，却在 `call_with_small_buffer` 中传入 4 个元素的数组，违背 R_17_05。
- `process_matrix` 直接修改 `rows`、`cols` 两个形参，违反 R_17_08 对形参只读的建议。
