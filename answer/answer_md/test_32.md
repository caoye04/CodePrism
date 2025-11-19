## 关联规范
- R_12_02: The right hand operand of a shift operator shall lie in the range zero to one less than the width in bits of the essential type of the left hand operand
- R_12_03: The comma operator should not be used
- R_12_04: Evaluation of constant expressions should not lead to unsigned integer wrap-around
- R_12_05: The sizeof operator shall not have an operand which is a function parameter declared as "array of type"
- R_13_02: The value of an expression and its persistent side-effects shall be the same under all permitted evaluation orders
- R_13_03: A full expression containing an increment (++) or decrement (--) operator should have no other potential side effects other than that caused by the increment or decrement operator

## 违反说明
- `shift_misuse` 中长期使用 `value << 40`，对 32 位无符号数执行超范围移位，直接违反 R_12_02。
- `order_example` 与 `main` 里的多处 `(builder++, builder + i)` 使用 comma 运算符组合语句，违反 R_12_03。
- `wrap_value = 4000000000U + 4000000000U;` 强制进行超范围的常量求和，触发无符号溢出，违背 R_12_04。
- `sized_parameter` 的 `sizeof(items)` 针对形参声明为数组的参数求大小，本质上测量的是指针，违反 R_12_05。
- `order_example` 把 `param++`、`++param` 和 `global_counter++` 放到同一个求值表达式中，结果依赖求值顺序，违反 R_13_02。
- `global_counter = global_counter++ + sample;` 以及 `result += (param++, param * 3);` 在同一完整表达式内混合副作用和其他运算，违反 R_13_03。
