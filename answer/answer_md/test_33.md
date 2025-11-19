## 关联规范
- R_14_01: A loop counter shall not have essentially floating type
- R_14_02: A for loop shall be well-formed
- R_14_03: Controlling expressions shall not be invariant
- R_14_04: The controlling expression of an if-statement and the controlling expression of an iteration-statement shall have essentially Boolean type
- R_13_04: The result of an assignment operator should not be used

## 违反说明
- `iterate_states` 与 `main` 中多处 `for (float ...)` 使用浮点作为循环计数器，直接违反 R_14_01。
- `for (counter = 0.0f; counter < 10.0f; )`、`for (int index = 0; index < limit;)` 省略了更新表达式，循环状态需要在体内手工维护，破坏 R_14_02 所定义的规范 for 结构。
- `while (5)` 控制表达式恒定不变，造成不受控制的死循环，违反 R_14_03。
- `if ((counter + 2.0f))`、`if ((status + 3))` 等用整型/浮点表达式直接作为条件，未转换为布尔语义，触犯 R_14_04。
- `if ((status = fetch_value(status)) > 10)` 和 `if ((data = fetch_value(data)))` 依赖赋值表达式的结果进行判断，违反 R_13_04 对赋值结果使用的限制。
