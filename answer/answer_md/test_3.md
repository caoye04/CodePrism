## 关联规范
- D_04_12: Dynamic memory allocation shall not be used.
- R_21_03: The memory allocation and deallocation functions of <stdlib.h> shall not be used.
- R_22_2: A block of memory shall only be freed if it was allocated by means of a Standard Library function.

## 违反说明
1. `build_buffer`、`stretch`、`main` 中通过 `malloc`、`realloc` 分配内存，直接违反禁止使用动态内存的 D_04_12。
2. 同样的 `malloc`/`realloc` 调用也触犯了 R_21_03 对 `<stdlib.h>` 动态内存接口的禁用要求。
3. `free(allocated);` 违反R_22_2对内存释放的要求
