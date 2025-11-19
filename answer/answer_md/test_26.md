## 关联规范
- R_15_05: A function should have a single point of exit at the end
- R_15_06: The body of an iteration-statement or a selection-statement shall be a compound statement
- R_15_07: All if ... else if constructs shall be terminated with an else statement

## 违反说明
- `evaluate` 在不同条件下多次 `return`，没有将出口集中在末尾，违背 R_15_05。
- 多个 `if`/`else` 分支（如 `mode == 5` 的判断）缺少花括号，违反 R_15_06。
- `evaluate` 和 `main` 中的 `if ... else if` 链都没有最终的 `else` 分支，触犯 R_15_07。
