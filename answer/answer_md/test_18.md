## 关联规范
- R_21_14: The Standard Library function memcmp shall not be used to compare null terminated strings
- R_21_15: The pointer arguments to the Standard Library functions memcpy, memmove and memcmp shall be pointers to qualified or unqualified versions of compatible types
- R_21_16: The pointer arguments to the Standard Library function memcmp shall point to either a pointer type, an essentially signed type, an essentially unsigned type, an essentially Boolean type or an essentially enum type

## 违反说明
- `compare_names` 与 `main` 中多次用 `memcmp` 比较以 `\0` 结尾的字符串，直接违反 R_21_14。
- `move_numbers` 把 `double` 数组复制到 `int` 数组，源和目标指针类型不兼容，触犯 R_21_15。
- `compare_records` 让 `memcmp` 接收 `Record *` 数组与结构体地址，对应的指针既不是基本整数也不是指针类型本身，违反 R_21_16。
