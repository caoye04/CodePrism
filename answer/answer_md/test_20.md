## 关联规范
- R_21_20: The pointer returned by the Standard Library functions asctime, ctime, gmtime, localtime, localeconv, getenv, setlocale or strerror shall not be used following a subsequent call to the same function
- R_22_01: All resources obtained dynamically by means of Standard Library functions shall be explicitly released
- R_22_02: A block of memory shall only be freed if it was allocated by means of a Standard Library function

## 违反说明
- `print_time_twice` 在第二次调用 `asctime` 后仍然用第一次的返回指针 `first`，直接违反 R_21_20。
- `leak_blocks` 和 `main` 中多次 `malloc` 得到的内存都未释放，触犯 R_22_01。
- `free_stack` 以及 `main` 后段调用 `free(local_buffer)`，都试图释放非堆上分配的内存，违背 R_22_02。
