## 关联规范
- R_21_08: The Standard Library functions abort, exit, getenv and system of <stdlib.h> shall not be used
- R_21_09: The library functions bsearch and qsort of <stdlib.h> shall not be used
- R_21_10: The Standard Library time and date routines shall not be used

## 违反说明
- 主函数直接调用 `system("dir > NUL")`，触犯 R_21_08 禁用的系统调用。
- 数据排序完全依赖 `qsort`，与 R_21_09 相冲突。
- 程序多次调用 `time` 和 `localtime` 获取时间信息，违反 R_21_10。
