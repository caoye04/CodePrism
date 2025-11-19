## 关联规范
- R_09_02: The initializer for an aggregate or union shall be enclosed in braces
- R_09_04: An element of an object shall not be initialised more than once
- R_09_05: Where designated initialisers are used to initialize an array object the size of the array shall be specified explicitly

## 违反说明
- `matrix` 是二维数组却只用一层大括号初始化，违背 R_09_02 要求的聚合初始化风格。
- `values` 同时为索引 0 重复赋值 10 和 12，出现同一元素多次初始化，违反 R_09_04。
- `design` 使用指定初始化 `[3] = 20` 等，但未声明数组长度，触犯 R_09_05。
