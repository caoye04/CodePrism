## 关联规范
- R_22_06: The value of a pointer to a FILE shall not be used after the associated stream has been closed
- R_22_07: The macro EOF shall only be compared with the unmodified return value from any Standard Library function capable of returning EOF
- R_22_08: The value of errno shall be set to zero prior to a call to an errno-setting-function

## 违反说明
- `reuse_stream` 在 `fclose` 之后仍向同一个 `FILE *` 写数据，违反 R_22_06。
- `check_eof_wrong` 在比较 EOF 之前篡改了 `fgetc` 的返回值，再判断是否等于 EOF，违反 R_22_07。
- `parse_number` 多次调用 `strtol` 却没有在调用前清零 `errno`，触犯 R_22_08。
