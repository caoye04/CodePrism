## 关联规范
- R_21_11: The standard header file <tgmath.h> shall not be used
- R_21_12: The exception handling features of <fenv.h> should not be used
- R_21_13: Any value passed to a function in <ctype.h> shall be representable as an unsigned char or be the value EOF

## 违反说明
- 源码显式包含并调用 `<tgmath.h>` 中的 `tgamma`，违背 R_21_11。
- `adjust_round` 通过 `fesetround` 操作浮点环境，违反 R_21_12。
- `transform_text` 将 -5 到 4 直接传给 `tolower`，输入值不在无符号 char 范围内，违反 R_21_13。
