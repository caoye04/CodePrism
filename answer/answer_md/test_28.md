## 关联规范
- R_16_05: A default label shall appear as either the first or the last switch label of a switch statement
- R_16_06: Every switch statement shall have at least two switch-clauses
- R_16_07: A switch-expression shall not have essentially Boolean type

## 违反说明
- `dispatch` 把 `default` 放在中间，既不是首个也不是最后一个标签，违反 R_16_05。
- `single_case` 的 switch 只有一个 default 分支，违背 R_16_06 对分支数量的要求。
- `dispatch` 以布尔变量作为 switch 表达式，违反 R_16_07。
