## 关联规范
- D_04_09: A function should be used in preference to a function-like macro where they are interchangeable.
- R_20_10: The # and ## preprocessor operators should not be used.

## 违反说明
1. 代码定义了 `COMPUTE_PRODUCT`、`DECLARE_VAR` 等函数式宏并在主流程中多次使用，尽管已有 `multiply_values` 这样的函数可替代，违反 D_04_09。
2. 宏中大量使用 `#` 与 `##`（例如 `TO_STRING`, `COMBINE`），直接触犯 R_20_10 对这些运算符的限制。
