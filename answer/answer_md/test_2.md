## 关联规范
- R_02_02: There shall be no dead code.
- R_02_07: There should be no unused parameters in functions.

## 违反说明
1. 多处 `if (0)` 块（例如 `sum_prefix`、`scatter`、`main` 中）永远不会执行，构成死代码，违反 R_02_02。
2. 各函数形式参数 `t` 从未参与有效计算，仅被传递或忽略，属于未使用参数，违反 R_02_07。
