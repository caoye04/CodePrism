## 关联规范
- R_22_03: The same file shall not be open for read and write access at the same time on different streams
- R_22_04: There shall be no attempt to write to a stream which has been opened as read-only
- R_22_05: A pointer to a FILE object shall not be dereferenced

## 违反说明
- `duplicate_streams` 同时以读取和写入模式打开同一 `log.txt`，违背 R_22_03。
- `write_readonly` 以及 `main` 中对只读打开的文件执行 `fprintf`/`fputs`，违反 R_22_04。
- `FILE shadow = *fp;` 等语句直接解引用 `FILE *` 指针，触犯 R_22_05。
