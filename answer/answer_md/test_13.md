## 关联规范
- R_08_08: The static storage class specifier shall be used in all declarations of objects and functions that have internal linkage
- R_08_09: An object should be defined at block scope if its identifier only appears in a single function
- R_08_10: An inline function shall be declared with the static storage class

## 违反说明
- `helper_sum` 仅在本文件内使用却未加 `static`，破坏了内部链接应显式声明的约束（R_08_08）。
- 全局数组 `only_main_buffer` 只在 `main` 中使用，却放在文件作用域，违背 R_08_09 要求的“只在单个函数使用的对象应置于块级”。
- `scale_value` 被声明为 `inline` 但缺少 `static`，直接违反 R_08_10。
