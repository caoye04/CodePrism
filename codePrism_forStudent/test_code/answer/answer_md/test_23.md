## 关联规范
- R_22_09: The value of errno shall be tested against zero after calling an errno-setting-function
- R_22_10: The value of errno shall only be tested when the last function to be called was an errno-setting-function
- R_17_07: The value returned by a function having non-void return type shall be used

## 违反说明
- `convert_text` 多次调用 `strtol` 却没有紧接着检查 `errno`，违反 R_22_09。
- `random_log` 和 `main` 在执行 `puts` 后立即读取 `errno`，此时最近调用的并非 errno-setting 函数，触犯 R_22_10。
- `compute_factor(5)` 的返回值被完全忽略，违背 R_17_07。
