## 关联规范
- R_16_01: All switch statements shall be well-formed
- R_16_03: An unconditional break statement shall terminate every switch-clause
- R_16_04: Every switch statement shall have a default label

## 违反说明
- `handle_status` 内的 `switch` 缺少 default，且多个分支互相跌落，整体结构不合规，违反 R_16_01。
- case 0、case 2、`main` 中的 case 6 等都没有 `break`，违背 R_16_03。
- 两个 switch 都未提供 `default` 分支，与 R_16_04 要求不符。
