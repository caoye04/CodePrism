## 关联规范
- R_07_02: A "u" or "U" suffix shall be applied to all integer constants that are represented in an unsigned type
- R_07_04: A string literal shall not be assigned to an object unless the object's type is pointer to const-qualified char
- R_10_02: Expressions of essentially character type shall not be used inappropriately in addition and subtraction operations

## 违反说明
- 代码中 `limit`, `storage`, `flags` 等无符号变量都由未带 U 后缀的整数字面量初始化，违反 R_07_02。
- `status` 被声明为 `char *` 并直接指向字符串常量，缺少 `const` 限定，违反 R_07_04。
- 在循环中持续对 `char code` 做加减法并与 `buffer` 元素相加，依赖字符类型参与算术运算，触犯 R_10_02。
