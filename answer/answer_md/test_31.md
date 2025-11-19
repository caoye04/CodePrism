## 关联规范
- R_11_01: Conversions shall not be performed between a pointer to a function and any other type
- R_11_03: A cast shall not be performed between a pointer to object type and a pointer to a different object type
- R_11_04: A conversion should not be performed between a pointer to object and an integer type
- R_11_06: A cast shall not be performed between pointer to void and an arithmetic type
- R_11_07: A cast shall not be performed between pointer to object and a non-integer arithmetic type
- R_11_08: A cast shall not remove any const or volatile qualification from the type pointed to by a pointer

## 违反说明
- `pointer_misuse` 内将 `compute_step` 的函数指针强制转换为 `void *`、再转换为整数并重新解释为函数指针，直接违反 R_11_01 对函数指针转换的禁止。
- `int32_t *int_ptr = (int32_t *)sample_ptr;` 将 `double *` 重新解释为 `int32_t *`，构成不同对象类型之间的指针转换，违反 R_11_03。
- `uintptr_t addr_number = (uintptr_t)sample_ptr;` 及随后的 `int (*fn_from_number)(int) = (int (*)(int))func_number;` 将对象或函数指针当作整数处理又转回，违反 R_11_04。
- `int plain = (int)generic;` 与 `long direct = (long)void_alias;` 把 `void *` 直接转换为算术类型，违反 R_11_06。
- `double ratio = (double)sample_ptr;` 和 `float alias = (float)probe;` 将对象指针当作浮点数使用，违反 R_11_07。
- `int *mutable = (int *)&immutable;` 去掉了常量限定并写回数据，直接违反 R_11_08。
