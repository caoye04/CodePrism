## 关联规范
- R_05_04: Macro identifiers shall be distinct
- R_05_06: A typedef name shall be a unique identifier
- R_05_07: A tag name shall be a unique identifier
- R_02_06: A function should not contain unused label declarations
- R_03_02: Line-splicing shall not be used in // comments

## 违反说明
- `meas_value_reading` 与 `meas_value_readlng` 仅在个别字符上区别极小，属于字体形近的宏命名冲突，违反 R_05_04。
- `typedef_blocks` 中先后在不同作用域重复声明 `typedef unsigned char data8;`，破坏类型别名唯一性，违反 R_05_06。
- 函数 `tag_reuse` 以及 `main` 再次定义相同的 `struct tracker`、`struct deer` 标签，导致标签名复用，违反 R_05_07。
- `main` 里的 `unused_label:` 从未被 goto 或跳转使用，违反 R_02_06。
- `log_series` 顶部使用 `// running trace \` 形式的行连接注释，直接违反 R_03_02 对 // 注释不允许行拼接的要求。
