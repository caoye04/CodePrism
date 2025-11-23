## 关联规范
- D_04_05: Identifiers in the same namespace with overlapping visibility should be typographically unambiguous.
- R_05_03: An identifier declared in an inner scope shall not hide an identifier declared in an outer scope.

## 违反说明
1. `sensorValue`, `sensor_value`, `l0_counter`, `I0_counter`, `IO_counter`, `lO_counter` 等名字仅大小写或字符相近，且在同一命名空间可见，造成易混淆，违反 D_04_05。
2. `collect_series` 和 `main` 中重新声明局部 `sensorValue`、`I0_counter` 等，遮蔽了全局同名对象，违反 R_05_03。
