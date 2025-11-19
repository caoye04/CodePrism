## 关联规范
- R_21_17: Use of the string handling functions from <string.h> shall not result in accesses beyond the bounds of the objects referenced by their pointer parameters
- R_21_18: The size_t argument passed to any function in <string.h> shall have an appropriate value
- R_21_19: The pointers returned by the Standard Library functions localeconv, getenv, setlocale or, strerror shall only be used as if they have pointer to const-qualified type

## 违反说明
- `copy_strings` 与 `repeat_moves`、`main` 中的多次 `memcpy` 明显越界写入目标缓冲区，违反 R_21_17。
- 在上述 `memcpy` 调用里随意把 `size_t` 设成 30、12 等远超真实缓冲区长度的值，直接触犯 R_21_18。
- `mutate_env` 修改 `getenv` 返回的字符串内容，未将其视作只读，违反 R_21_19。
