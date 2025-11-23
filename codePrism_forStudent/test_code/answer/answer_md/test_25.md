## 关联规范
- R_15_01: The goto statement should not be used
- R_15_02: The goto statement shall jump to a label declared later in the same function
- R_15_04: There should be no more than one break or goto statement used to terminate any iteration statement

## 违反说明
- `run_loop` 与 `main` 中大量使用 `goto` 控制流程，本身即与 R_15_01 不符。
- `goto START` 跳向函数前部的标签，明显违反 R_15_02 只能跳向后续标签的要求。
- 单个 `for` 循环里布置了多个 `goto` 和 `break` 用于提前退出，超出了 R_15_04 限制的“至多一个终止语句”。
