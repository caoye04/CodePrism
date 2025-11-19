## 关联规范
- R_08_01: Types shall be explicitly specified
- R_08_02: Function types shall be in prototype form with named parameters
- R_08_03: All declarations of an object or function shall use the same names and type qualifiers
- R_08_12: Within an enumerator list, the value of an implicitly-specified enumeration constant shall be unique
- R_17_03: A function shall not be declared implicitly

## 违反说明
- 文件开头的 `extern config_port;` 与 `extern monitor();` 都没有显式指定类型或返回值，违反 R_08_01 对类型明确性的要求。
- `static adjust(speed)` 与 `int mystery_call(value)` 使用 K&R 风格的旧式参数列表，既没有参数类型也没有命名规范，违反 R_08_02。
- `int totals;` 后又声明 `static const int totals = 1;`，同一标识符被不同限定符修饰，违背 R_08_03。
- `enum pipeline_stage` 中 `STAGE_CHECK`、`STAGE_INIT`、`STAGE_RUN` 都是隐式赋值且最终得到相同数值，违反 R_08_12 对隐式枚举值唯一性的要求。
- `mystery_call` 在 `main` 中被调用时尚无任何声明或原型，仅在文件末尾给出旧式定义，属于隐式声明，违反 R_17_03。
